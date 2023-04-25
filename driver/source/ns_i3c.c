/**
  * Copyright (c) 2019 Nuclei Limited. All rights reserved.
  *
  * SPDX-License-Identifier Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */
#include "ns_i3c.h"

#include "ns_p2m_udma.h"
#include <stdlib.h>

/**
  * \brief I3C Get Data Partity Bit
  * \param data data need to get parity bit
  * \return parity bit
  */
static inline uint8_t I3C_Get_DataParityBit(uint8_t data)
{
    int count = 0;
    while (data != 0) {
        count += data & 1;
        data = data >> 1;
    }
    return !(count % 2);
}

/**
  * \brief I3C Clk div config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param div clk div number
  */
static inline void I3C_Clk_Cfg(I3C_TypeDef* I3Cx, uint32_t div)
{
    I3Cx->DIV = div;
}

/**
  * \brief I3C core mode enable/disable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I3C_Control_En(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_ENABLE;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_ENABLE);
    }
}

/**
  * \brief I3C write enable
  * \param I3Cx where x can be to select the I3C peripheral.
  */
static inline void I3C_WriteEnable(I3C_TypeDef* I3Cx)
{
    I3Cx->SETUP |= I3C_SETUP_WRITE;
}

/**
  * \brief I3C read enable
  * \param I3Cx where x can be to select the I3C peripheral.
  */
static inline void I3C_ReadEnable(I3C_TypeDef* I3Cx)
{
    I3Cx->SETUP |= I3C_SETUP_READ;
}

/**
  * \brief I3C timeout enable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I3C_TimeOut_En(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_TIME_OUT;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_TIME_OUT);
    }
}

/**
  * \brief I3C 10bit addr mode enable
  *
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I3C_10BitAddr_Cfg(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_10BIT_ADDR_ENABLE;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_10BIT_ADDR_ENABLE);
    }
}

/**
  * \brief I3C 64bit data mode enable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I3C_64BitData_Cfg(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_RD_64BIT;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_RD_64BIT);
    }
}

/**
  * \brief I3C slave mode
  * \param I3Cx where x can be to select the I3C peripheral.
  */
static inline void I3C_Slave_Cfg(I3C_TypeDef* I3Cx)
{
    I3Cx->SETUP |= I3C_SETUP_ROLE_MODE_SLAVE;
}

/**
  * \brief I3C master mode
  * \param I3Cx where x can be to select the I3C peripheral.
  */
static inline void I3C_Master_Cfg(I3C_TypeDef* I3Cx)
{
    I3Cx->SETUP |= I3C_SETUP_ROLE_MODE_MASTER;
}

/**
  * \brief I3C mode enable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I3C_Mode_En(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_MODE_ENABLE;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_MODE_ENABLE);
    }
}

/**
  * \brief I3C pad out mode
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param mode i3c putout mode
  */
static inline void I3C_PadOutMode_Cfg(I3C_TypeDef* I3Cx, I3C_PadOutModeTypedef mode)
{
    if (I3C_PAD_PUSH_PULL_MODE == mode) {
        I3Cx->SETUP |= I3C_SETUP_PUSH_PULL_MODE_PP;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_PUSH_PULL_MODE_PP);
    }
}

/**
  * \brief I3C SCL io pull enable/disable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I3C_SCLPull_En(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_SCL_PULLUP_ENABLE;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_SCL_PULLUP_ENABLE);
    }
}

/**
  * \brief I3C SDA io pull enable/disable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I3C_SDAPull_En(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_SDA_PULLUP_ENABLE;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_SDA_PULLUP_ENABLE);
    }
}

/**
  * \brief I3C parity bit enable/disable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param mode i3c output mode
  */
static inline void I3C_ParityMode_En(I3C_TypeDef* I3Cx, I3C_ParityModeTypedef mode)
{
    if (I3C_PARITY_0BIT_MODE == mode) {
        I3Cx->SETUP |= I3C_SETUP_PARITY_MODE_ENABLE;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_PARITY_MODE_ENABLE);
    }
}

/**
  * \brief I3C interrupt enable config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param interrupt:
  *     \arg I3C_INT_IE_EOT_ENABLE
  *     \arg I3C_INT_IE_ARBITRATION_LOST_ENABLE
  *     \arg I3C_INT_IE_SLAVE_TX_EMPTY_ENABLE
  *     \arg I3C_INT_IE_SLAVE_RX_FULL_ENABLE
  *     \arg I3C_INT_IE_TIME_OUT_ENABLE
  *     \arg I3C_INT_IE_INT_ENABLE
  *     \arg I3C_INT_IE_DMA_CMD_LOAD_ENABLE
  *     \arg I3C_INT_IE_DMA_RX_LOAD_ENABLE
  *     \arg I3C_INT_IE_DMA_TX_LOAD_ENABLE
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I3C_Interrupt_En(I3C_TypeDef* I3Cx, uint32_t interrupt, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->INT_IE |= interrupt;
    } else {
        I3Cx->INT_IE &= ~(interrupt);
    }
}

/**
  * \brief I3C slave addr config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param addr i3c config slave addr while in slave mode
  */
static inline void I3C_SlaveAddr_Cfg(I3C_TypeDef* I3Cx, uint16_t addr)
{
    I3Cx->SLAVE_ADDRESS &= ~I3C_SLAVE_ADDRESS_SLAVE_ADDRESS_MASK;
    I3Cx->SLAVE_ADDRESS |= I3C_SLAVE_ADDRESS_SLAVE_ADDRESS(addr);
}

/**
  * \brief I3C interrupt clear
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param interrupt:
  *     \arg I3C_INT_CLR_EOT
  *     \arg I3C_INT_CLR_ARBITRATION_LOST
  *     \arg I3C_INT_CLR_SLAVE_TX_EMPTY
  *     \arg I3C_INT_CLR_SLAVE_RX_FULL
  *     \arg I3C_INT_CLR_TIME_OUT
  *     \arg I3C_INT_CLR_SIR_IBI_IRQ
  *     \arg I3C_INT_CLR_DMA_CMD_LOAD
  *     \arg I3C_INT_CLR_DMA_RX_LOAD
  *     \arg I3C_INT_CLR_DMA_TX_LOAD
  */
void I3C_Interrupt_Clear(I3C_TypeDef* I3Cx, uint32_t interrupt)
{
    I3Cx->INT_CLR |= interrupt;
}

/**
  * \brief I3C Interrupt status
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param interrupt
  *     \arg I3C_INT_IE_EOT_ENABLE
  *     \arg I3C_INT_IE_ARBITRATION_LOST_ENABLE
  *     \arg I3C_INT_IE_SLAVE_TX_EMPTY_ENABLE
  *     \arg I3C_INT_IE_SLAVE_RX_FULL_ENABLE
  *     \arg I3C_INT_IE_TIME_OUT_ENABLE
  *     \arg I3C_INT_IE_INT_ENABLE
  *     \arg I3C_INT_IE_DMA_CMD_LOAD_ENABLE
  *     \arg I3C_INT_IE_DMA_RX_LOAD_ENABLE
  *     \arg I3C_INT_IE_DMA_TX_LOAD_ENABLE
  * \return FlagStatus
  */
FlagStatus I3C_Get_IntStatus(I3C_TypeDef* I3Cx, uint32_t interrupt)
{
    if (interrupt == ((I3Cx->INT_IE) & interrupt)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief I3C TXE time config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param time the time between txe_clr and SCL release for first bit
  */
static inline void I3C_TxeTime_Cfg(I3C_TypeDef* I3Cx, uint8_t time)
{
    I3Cx->TXE_TIME = time;
}

/**
  * \brief I3C time out num config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param num time out number
  */
void I3C_TimeOut_Cfg(I3C_TypeDef* I3Cx, uint16_t num)
{
    I3Cx->TIME_OUT = num;
}

/**
  * \brief I3C Read Response0
  * \param I3Cx where x can be to select the I3C peripheral.
  * \return Slave Response0 in DAA procedure when Master send ENTDAA cmd
  */
static inline uint32_t I3C_Read_Response0(I3C_TypeDef* I3Cx)
{
    return I3Cx->PID_LOW;
}

/**
  * \brief I3C Read Response1
  * \param I3Cx where x can be to select the I3C peripheral.
  * \return Slave Response1 in DAA procedure when Master send ENTDAA cmd
  */
static inline uint32_t I3C_Read_Response1(I3C_TypeDef* I3Cx)
{
    return I3Cx->PID_HIGH;
}

/**
  * \brief I3C TSCO config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param time the time of master drive sda line after scl rising (unit:the cycle of I3C_clk)
  */
static inline void I3C_Tsco_Cfg(I3C_TypeDef* I3Cx, uint8_t time)
{
    I3Cx->TSCO = time;
}

/**
  * \brief I3C software reset
  * \param I3Cx where x can be to select the I3C peripheral.
  */
void I3C_SoftReset_En(I3C_TypeDef* I3Cx)
{
    I3Cx->SETUP |= I3C_SETUP_SOFT_RESET_ENABLE;
    I3Cx->SETUP &= ~I3C_SETUP_SOFT_RESET_ENABLE;
}

/**
  * \brief I3C DMA RX Channel data size config
  *
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param size the datasize of rx dma channel
  *     \arg I3C_DMA_RX_DATASIZE_RX_DATASIZE_BYTE
  *     \arg I3C_DMA_RX_DATASIZE_RX_DATASIZE_WORD
  */
void I3C_DmaRxDataSize_Cfg(I3C_TypeDef* I3Cx, uint8_t size)
{
    I3Cx->DMA_RX_DATASIZE &= ~I3C_DMA_RX_DATASIZE_RX_DATASIZE_MASK;
    I3Cx->DMA_RX_DATASIZE |= size;
}

/**
  * \brief I3C dma mode enable/disable
  *
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I3C_Dma_Cfg(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_WORK_MODE_DMA;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_WORK_MODE_DMA);
    }
}

/**
  * \brief I3C get status
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
        \arg I3C_STATUS_BUSY
        \arg I3C_STATUS_ARB_LOST
        \arg I3C_STATUS_ACK                         
        \arg I3C_STATUS_EOT                         
        \arg I3C_STATUS_BYTE_ON_GOING
        \arg I3C_STATUS_TIME_OUT
        \arg I3C_STATUS_SIR_IRQ
        \arg I3C_STATUS_HJR_IRQ
        \arg I3C_STATUS_SMR_IRQ
        \arg I3C_STATUS_SLAVE_RECEIVE
        \arg I3C_STATUS_SLAVE_TRANSMIT
        \arg I3C_STATUS_SLAVE_TXE
        \arg I3C_STATUS_TXFIFO_EMPTY
        \arg I3C_STATUS_TXFIFO_FULL
        \arg I3C_STATUS_SLAVE_RXF
        \arg I3C_STATUS_RXFIFO_EMPTY
        \arg I3C_STATUS_RXFIFO_FULL
        \arg I3C_STATUS_BUS_CONFLICT
        \arg I3C_STATUS_CFG_CMD_EN
        \arg I3C_STATUS_CFG_TX_EN
        \arg I3C_STATUS_CFG_RX_EN
        \arg I3C_STATUS_DMA_MODE_STATUS
        \arg I3C_STATUS_DMA_CMD_LOAD_STATUS
        \arg I3C_STATUS_DMA_RX_LOAD_STATUS
        \arg I3C_STATUS_DMA_TX_LOAD_STATUS
  * \return FlagStatus
  */
FlagStatus I3C_Get_Status(I3C_TypeDef* I3Cx, uint32_t status)
{
    if (status == ((I3Cx->STATUS) & status)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief I3C send start commond
  * \param I3Cx where x can be to select the I3C peripheral.
  */
void I3C_Start(I3C_TypeDef* I3Cx)
{
    while (I3C_Get_Status(I3Cx, I3C_STATUS_BUSY)) {}
    I3Cx->SETUP |= I3C_SETUP_START;
}

/**
  * \brief I3C send restart commond
  * \param I3Cx where x can be to select the I3C peripheral.
  */
void I3C_Restart(I3C_TypeDef* I3Cx)
{
    I3Cx->SETUP |= I3C_SETUP_START;
}

/**
  * \brief I3C send stop commond
  * \param I3Cx where x can be to select the I3C peripheral.
  */
void I3C_Stop(I3C_TypeDef* I3Cx)
{
    I3Cx->SETUP |= I3C_SETUP_STOP;
}

/**
  * \brief I3C ACK ENABLE
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I3C_ACK(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP &= ~(I3C_SETUP_ACK);
    } else {
        I3Cx->SETUP |= I3C_SETUP_ACK;
    }
}

/**
  * \brief I3C parity bit enable/disable
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param status:
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I3C_Parity(I3C_TypeDef* I3Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I3Cx->SETUP |= I3C_SETUP_PARITY_MODE_ENABLE;
    } else {
        I3Cx->SETUP &= ~(I3C_SETUP_PARITY_MODE_ENABLE);
    }
}

/**
  * \brief DMA tx data size config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param size tx data size
  */
void I3C_DmaTxDateSize_Cfg(I3C_TypeDef* I3Cx, I3C_DmaDataSizeTypedef size)
{
    I3Cx->DMA_TX_DATASIZE = size;
}

/**
  * \brief DMA rx data size config
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param size rx data size
  */
void I3C_DmaRxDateSize_Cfg(I3C_TypeDef* I3Cx, I3C_DmaDataSizeTypedef size)
{
    I3Cx->DMA_RX_DATASIZE = size;
}

/**
  * \brief I3C set SCL duty
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param duty after scl rise time
  *     \arg I3C_SETUP_SCL_DUTY_CYCLE_SEL_THIRTY
  *     \arg I3C_SETUP_SCL_DUTY_CYCLE_SEL_FORTY
  *     \arg I3C_SETUP_SCL_DUTY_CYCLE_SEL_FIFTY
  */
void I3C_Set_SclDuty(I3C_TypeDef* I3Cx, uint32_t duty)
{
    I3Cx->SETUP &= ~(I3C_SETUP_SCL_DUTY_CYCLE_SEL_MASK);
    I3Cx->SETUP |= duty;
}

/**
  * \brief  Initializes the I3Cx peripheral according to the specified
  *   parameters in the I3C_InitStruct.
  * \param  I3Cx where x can be to select the I3C peripheral.
  * \param  init pointer to a I3C_InitTypeDef structure that
  *         contains the configuration information for the specified I3C peripheral.
  */
void I3C_Init(I3C_TypeDef* I3Cx, I3C_InitTypeDef* init)
{
    UDMA_PeriphCfg_TypeDef* UDMA_PCFG = (UDMA_PeriphCfg_TypeDef*)I3Cx;
    uint32_t clk_div = 0;
    uint32_t dma_cmd[2] = {0};
    I3C_Dma_Cfg(I3Cx, DISABLE);
    I3C_Control_En(I3Cx, DISABLE);
    I3C_SoftReset_En(I3Cx);
    I3C_Set_SclDuty(I3Cx, init->duty);
    switch (init->duty) {
        case I3C_30_DUTY:
        case I3C_50_DUTY:
            clk_div = ((SystemCoreClock / init->clk) / 6) - 1;
            break;
        case I3C_40_DUTY:
            clk_div = ((SystemCoreClock / init->clk) / 5) - 1;
            break;
        default:
            break;
    }
    switch (init->mode) {
        case I3C_MASTER_MODE:
            I3C_Master_Cfg(I3Cx);
            break;
        case I3C_SLAVE_MODE:
            I3C_Slave_Cfg(I3Cx);
    }
    I3C_Clk_Cfg(I3Cx, clk_div);
    I3C_Mode_En(I3Cx, DISABLE);
    I3C_Master_Cfg(I3Cx);
    I3C_SlaveAddr_Cfg(I3Cx, init->slave_addr);
    I3C_SCLPull_En(I3Cx, init->scl_pull);
    I3C_SDAPull_En(I3Cx, init->sda_pull);
    I3C_ParityMode_En(I3Cx, init->i3c_parity_mode);
    I3C_Control_En(I3Cx, ENABLE);
    I3C_DmaTxDateSize_Cfg(I3Cx, init->dma_txdata_size);
    I3C_DmaRxDateSize_Cfg(I3Cx, init->dma_rxdata_size);
    if (ENABLE == init->dma_mode) {
        /* config dma div num */
        I3C_Dma_Cfg(I3Cx, ENABLE);
        dma_cmd[0] = I3C_CMD_CFG(clk_div);
        dma_cmd[1] = I3C_CMD_EOT;
        UDMA_EnqueueChannel(UDMA_PCFG, (uint32_t)&dma_cmd, sizeof(dma_cmd), I3C_DMA_ENABLE, COMMAND_CHANNEL);
        while (0 == I3C_Get_Status(I3Cx, I3C_STATUS_DMA_CMD_LOAD_STATUS));
        I3C_Interrupt_Clear(I3Cx, I3C_INT_CLR_DMA_CMD_LOAD);
        while (I3C_Get_Status(I3Cx, I3C_STATUS_DMA_MODE_STATUS)) {}
    }
}

/**
  * \brief I3C send slave addr
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param addr slave addr without r/w bit
  * \param dir I3C_DIR_WRITE or I3C_DIR_READ
  * \param timeout wait status times
  * \retval 0 ok
  * \retval -1 send slave addr timeout
  */
int8_t I3C_SendSlaveAddr(I3C_TypeDef* I3Cx, uint8_t addr, I3C_DIRTypedef dir, uint32_t timeout)
{
    addr <<= 1;
    addr |= dir;
    I3Cx->TXDATA = addr;
    I3C_WriteEnable(I3Cx);
    while (I3C_Get_Status(I3Cx, I3C_STATUS_BYTE_ON_GOING)) {
        if (!timeout--) {
            return -1;
        }
    }
    return 0;
}

/**
  * \brief I3C write data to slave
  *
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param data write data
  * \param timeout wait status times
  * \retval 0 ok
  * \retval -1 write data timeout
  */
int8_t I3C_Write(I3C_TypeDef* I3Cx, uint8_t data, uint32_t timeout)
{
    I3C_Parity(I3Cx, ENABLE);
    I3Cx->TXDATA = data;
    I3C_WriteEnable(I3Cx);
    while (I3C_Get_Status(I3Cx, I3C_STATUS_BYTE_ON_GOING)) {
        if (!timeout--) {
            I3C_Parity(I3Cx, DISABLE);
            return -1;
        }
    }
    I3C_Parity(I3Cx, DISABLE);
    return 0;
}

/**
  * \brief I3C read data from slave
  * \param I3Cx where x can be to select the I3C peripheral.
  * \param data read data
  * \param timeout wait status times
  * \retval 0 read data ok
  * \retval -1 read data timeout
  */
int8_t I3C_Read(I3C_TypeDef* I3Cx, uint8_t* data, uint32_t timeout)
{
    uint8_t temp_parity = 0;
    uint8_t temp_data = 0;
    I3C_Parity(I3Cx, ENABLE);
    I3C_ReadEnable(I3Cx);
    while (I3C_Get_Status(I3Cx, I3C_STATUS_BYTE_ON_GOING)) {
        if (!timeout--) {
            I3C_Parity(I3Cx, DISABLE);
            return -1;
        }
    }
    if (I3Cx->SETUP & I3C_SETUP_PARITY_MODE_ENABLE) {
        *data = (I3Cx->RXDATA & 0x1FF) >> 1;
    } else {
        *data = (I3Cx->RXDATA & 0x1FF) & 0xFF;
    }
    I3C_Parity(I3Cx, DISABLE);
    return 0;
}

/*==== The following API is I3C usage example ====*/
/**
  * \brief I3C master write
  *
  * \param I3Cx I3Cx(x=0,1)
  * \param slave_addr slave address
  * \param data write data
  * \param bytes write data bytes
  */
void I3C_MasterWriteBytes(I3C_TypeDef* I3Cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes)
{
    uint32_t timeout = 0xFFFF;
    /* config acknowledge */
    I3C_ACK(I3Cx, DISABLE);
    I3C_Start(I3Cx);
    /* send slave address to I3C bus */
    I3C_SendSlaveAddr(I3Cx, I3C_CMD_SLV_ADDR, I3C_DIR_WRITE, timeout);
    I3C_Restart(I3Cx);
    /* send slave address to I3C bus */
    I3C_SendSlaveAddr(I3Cx, slave_addr, I3C_DIR_WRITE, timeout);
    for (int i = 0; i < bytes; i++) {
        I3C_Write(I3Cx, data[i], timeout);
    }
    I3C_Stop(I3Cx);
}

/**
  * \brief I3C master read
  * \param I3Cx I3Cx(x=0,1)
  * \param slave_addr slave address
  * \param data read data
  * \param bytes read data bytes
  */
void I3C_MasterReadBytes(I3C_TypeDef* I3Cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes)
{
    uint32_t timeout = 0xFFFF;
    /* config acknowledge */
    I3C_ACK(I3Cx, DISABLE);
    I3C_Start(I3Cx);
    /* send slave address to I3C bus */
    I3C_SendSlaveAddr(I3Cx, I3C_CMD_SLV_ADDR, I3C_DIR_READ, timeout);
    I3C_Restart(I3Cx);
    /* send slave address to I3C bus */
    I3C_SendSlaveAddr(I3Cx, slave_addr, I3C_DIR_READ, timeout);
    /* config acknowledge */
    I3C_ACK(I3Cx, ENABLE);
    for (int i = 0; i < bytes; i++) {
        if (i == (bytes - 1)) {
            I3C_ACK(I3Cx, DISABLE);
        }
        I3C_Read(I3Cx, &data[i], timeout);
    }
    I3C_Stop(I3Cx);
}

/**
  * \brief I3C master write DMA
  * \param I3Cx I3Cx(x=0,1)
  * \param slave_addr slave address
  * \param data write data
  * \param bytes write data bytes
  */
void I3C_MasterWriteBytesDMA(I3C_TypeDef* I3Cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes)
{
    UDMA_PeriphCfg_TypeDef* UDMA_PCFG = (UDMA_PeriphCfg_TypeDef*)I3Cx;
    uint32_t dma_cmd[8] = {0};
    dma_cmd[0] = I3C_CMD_START;
    dma_cmd[1] = I3C_CMD_WRB((I3C_CMD_SLV_ADDR << 1) | I3C_DIR_WRITE);
    dma_cmd[2] = I3C_CMD_START;
    dma_cmd[3] = I3C_CMD_WRB((slave_addr << 1) | I3C_DIR_WRITE);
    dma_cmd[4] = I3C_CMD_RPT(bytes);
    dma_cmd[5] = I3C_CMD_WR_9BIT;
    dma_cmd[6] = I3C_CMD_STOP;
    dma_cmd[7] = I3C_CMD_EOT;
    UDMA_EnqueueChannel(UDMA_PCFG, (uint32_t)data, bytes, I3C_DMA_ENABLE, TX_CHANNEL);
    UDMA_EnqueueChannel(UDMA_PCFG, (uint32_t)dma_cmd, sizeof(dma_cmd), I3C_DMA_ENABLE, COMMAND_CHANNEL);
    while (0 == I3C_Get_Status(I3Cx, I3C_STATUS_DMA_CMD_LOAD_STATUS));
    I3C_Interrupt_Clear(I3Cx, I3C_INT_CLR_DMA_CMD_LOAD);
    while (0 == I3C_Get_Status(I3Cx, I3C_STATUS_DMA_TX_LOAD_STATUS));
    I3C_Interrupt_Clear(I3Cx, I3C_INT_CLR_DMA_TX_LOAD);
    while (I3C_Get_Status(I3Cx, I3C_STATUS_DMA_MODE_STATUS)) {}
}

/**
  * \brief I3C master read DMA
  *
  * \param I3Cx I3Cx(x=0,1)
  * \param slave_addr slave address
  * \param data read data
  * \param bytes read data bytes
  */
void I3C_MasterReadBytesDMA(I3C_TypeDef* I3Cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes)
{
    UDMA_PeriphCfg_TypeDef* UDMA_PCFG = (UDMA_PeriphCfg_TypeDef*)I3Cx;
    uint32_t dma_cmd[9] = {0};
    uint32_t dma_cmd_len;
    dma_cmd[0] = I3C_CMD_START;
    dma_cmd[1] = I3C_CMD_WRB((I3C_CMD_SLV_ADDR << 1) | I3C_DIR_WRITE);
    dma_cmd[2] = I3C_CMD_START;
    dma_cmd[3] = I3C_CMD_WRB((slave_addr << 1) | I3C_DIR_READ);
    if (bytes > 1) {
        dma_cmd[4] = I3C_CMD_RPT(bytes - 1);
        dma_cmd[5] = I3C_CMD_RD_NACK;
        dma_cmd[6] = I3C_CMD_RD_ACK;
        dma_cmd[7] = I3C_CMD_STOP;
        dma_cmd[8] = I3C_CMD_EOT;
        dma_cmd_len = 9;
    } else {
        dma_cmd[4] = I3C_CMD_RD_ACK;
        dma_cmd[5] = I3C_CMD_STOP;
        dma_cmd[6] = I3C_CMD_EOT;
        dma_cmd_len = 7;
    }
    UDMA_EnqueueChannel(UDMA_PCFG, (uint32_t)data, bytes, I3C_DMA_ENABLE, RX_CHANNEL);
    UDMA_EnqueueChannel(UDMA_PCFG, (uint32_t)dma_cmd, sizeof(uint32_t) * dma_cmd_len, I3C_DMA_ENABLE, COMMAND_CHANNEL);
    while (RESET == I3C_Get_Status(I3Cx, I3C_STATUS_DMA_CMD_LOAD_STATUS));
    I3C_Interrupt_Clear(I3Cx, I3C_INT_CLR_DMA_CMD_LOAD);
    while (RESET == I3C_Get_Status(I3Cx, I3C_STATUS_DMA_RX_LOAD_STATUS));
    I3C_Interrupt_Clear(I3Cx, I3C_INT_CLR_DMA_RX_LOAD);
    while (I3C_Get_Status(I3Cx, I3C_STATUS_DMA_MODE_STATUS)) {}
}

/**
  * \brief  Clears the status of I3C channel in udma mode.
  * \param  I3Cx where x can be 0, 1 to select the I3C peripheral.
  * \param  Channel Channel of the DMA Request sources.
  *                  This parameter can be COMMAND_CHANNEL,TX_CHANNEL or RX_CHANNEL.
  */
void I3C_UDMAClearChannel(I3C_TypeDef* I3Cx, uint32_t Channel)
{
    switch (Channel) {
        case COMMAND_CHANNEL:
            I3Cx->CMD_CFG = I3C_DMA_CLEAR;
            break;
        case RX_CHANNEL:
            I3Cx->RX_CFG = I3C_DMA_CLEAR;
            break;
        case TX_CHANNEL:
            I3Cx->TX_CFG = I3C_DMA_CLEAR;
            break;
        default :
            break;
    }
}
