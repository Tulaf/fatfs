/**
  * Copyright (c) 2019 Nuclei Limited. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
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
#include "ns.h"

#ifndef _NS_I3C_H__
#define _NS_I3C_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
  * \brief  I3C Pad Output Mode Define
  */
typedef enum {
    I3C_PAD_OPEN_DRAIN_MODE             = 0,/*!< open drain mode */
    I3C_PAD_PUSH_PULL_MODE              = 1,/*!< push pull mode */
} I3C_PadOutModeTypedef;

/**
  * \brief  I3C Parity Bit Define
  */
typedef enum {
    I3C_PARITY_8BIT_MODE                = 0,/*!< the rx_data[8] indicate parity_bit */
    I3C_PARITY_0BIT_MODE                = 1,/*!< the rx_data[0] indicate parity_bit */
} I3C_ParityModeTypedef;

/**
  * \brief  I3C Data Direction Define
  */
typedef enum {
    I3C_DIR_WRITE                       = 0,/*!< I3C Write Flag */
    I3C_DIR_READ                        = 1,/*!< I3C Read Flag */
} I3C_DIRTypedef;

/**
  * \brief  I3C Mode Define
  */
typedef enum {
    I3C_MASTER_MODE                     = 0,/*!< I3C Master mode */
    I3C_SLAVE_MODE                      = 1,/*!< I3C Slave mode */
} I3C_ModeTypedef;

/**
  * \brief  I3C Dma Config Define
  */
typedef enum {
    I3C_DMA_CONTINUE             = 0x1 << 0,/*!< I3C DMA Continue Config */
    I3C_DMA_ENABLE               = 0x1 << 1,/*!< I3C DMA Enable Config */
    I3C_DMA_PENDING              = 0x1 << 2,/*!< I3C DMA Pending Config */
    I3C_DMA_CLEAR                = 0x1 << 3,/*!< I3C DMA Clear Config */
} I3C_DmaTypedef;
/**
  * \brief  I3C Dma Data Size Define
  */
typedef enum {
    I3C_DMA_DATA_SIZE_BYTE       = 0x0,/*!< I3C DMA Data Size Byte */
    I3C_DMA_DATA_SIZE_WORD       = 0x2,/*!< I3C DMA Data Size Word */
} I3C_DmaDataSizeTypedef;

/**
  * \brief  I3C Duty Define
  */
typedef enum {
    I3C_30_DUTY                     = 0,/*!< I3C %30 duty */
    I3C_40_DUTY                     = 1,/*!< I3C %40 duty */
    I3C_50_DUTY                     = 2,/*!< I3C %50 duty */
} I3C_DutyTypedef;

/**
  * \brief  I3C Init Struct Define
  */
typedef struct
{
    uint32_t clk;                           /*!< clock div number config */
    uint16_t slave_addr;                    /*!< Slave addr config */
    I3C_DutyTypedef duty;                   /*!< SCL duty config */
    I3C_ModeTypedef mode;                   /*!< I3C mode config */
    ControlStatus scl_pull;                 /*!< SCL line pull up */
    ControlStatus sda_pull;                 /*!< SDA line pull up */
    I3C_ParityModeTypedef i3c_parity_mode;  /*!< I3C parity bit config */
    ControlStatus dma_mode;                 /*!< dma mode config */
    I3C_DmaDataSizeTypedef dma_txdata_size; /*!< dma txdata size */
    I3C_DmaDataSizeTypedef dma_rxdata_size; /*!< dma rxdata size */
} I3C_InitTypeDef;

#define I3C_CMD_SLV_ADDR            (0x7E)
#define I3C_CMD_START               (0x00000000)                        /*!< I3C DMA CMD:I3C Start of Transfer command */
#define I3C_CMD_STOP                (0x10000000)                        /*!< I3C DMA CMD:I3C End of Transfer command */
#define I3C_CMD_RD_ACK              (0x20000000)                        /*!< I3C DMA CMD:I3C receive data and acknowledge command */
#define I3C_CMD_RD_NACK             (0x30000000)                        /*!< I3C DMA CMD:I3C receive data and not acknowledge command */
#define I3C_CMD_WR                  (0x40000000)                        /*!< I3C DMA CMD:I3C send data and wait acknowledge command */
#define I3C_CMD_WAIT(icb_clk_num)   (0x50000000 | (icb_clk_num & 0xFF)) /*!< I3C DMA CMD:I3C wait dummy cycles command */
#define I3C_CMD_RPT(rpeat_num)      (0x60000000 | (rpeat_num & 0xFF))   /*!< I3C DMA CMD:I3C next command repeat command */
#define I3C_CMD_CFG(clk_div)        (0x70000000 | (clk_div & 0xFFFF))   /*!< I3C DMA CMD:I3C configuration command */
#define I3C_CMD_WAIT_EV(event)      (0x08000000 | (event & 0x3))        /*!< I3C DMA CMD:I3C wait uDMA external event command */
#define I3C_CMD_WRB(data)           (0x38000000 | (data & 0xFF))        /*!< I3C DMA CMD:I3C write byte command */
#define I3C_CMD_EOT                 (0x48000000)                        /*!< I3C DMA CMD:Signal end of transfer */
#define I3C_CMD_SETUP_UCA(uca)      (0x18000000 | (uca) & 0x41FFFFF)    /*!< I3C DMA CMD:Setup RX or TX channel start address */
#define I3C_CMD_SETUP_UCS(ucs)      (0x28000000 | (ucs) & 0x40FFFFF)    /*!< I3C DMA CMD:Setup RX or TX channel transfer size+enable channel */
#define I3C_CMD_WR_9BIT             (0x68000000)                        /*!< I3C DMA CMD:I3C write byte command + T */
#define I3C_CMD_RD_8BIT             (0x78000000)                        /*!< I3C DMA CMD:I3C Read byte continuous */
#define I3C_CMD_RD_9BIT             (0x80000000)                        /*!< I3C DMA CMD:I3C Read 9bit contiuous */

#define I3C_RX_SADDR_OFFSET                                           0x0 /*!< RX buffer base address  */
#define I3C_RX_SIZE_OFFSET                                            0x4 /*!< RX buffer size bitfield in bytes */
#define I3C_RX_CFG_OFFSET                                             0x8 /*!< RX channel configuration  */
#define I3C_TX_SADDR_OFFSET                                           0xc /*!< TX buffer base address  */
#define I3C_TX_SIZE_OFFSET                                            0x10 /*!< TX buffer size bitfield in bytes */
#define I3C_TX_CFG_OFFSET                                             0x14 /*!< TX channel configuration field */
#define I3C_CMD_SADDR_OFFSET                                          0x18 /*!< CMD buffer base address bitfield */
#define I3C_CMD_SIZE_OFFSET                                           0x1c /*!< CMD buffer size bitfield in bytes */
#define I3C_CMD_CFG_OFFSET                                            0x20 /*!< CMD channel configuration field */
#define I3C_STATUS_OFFSET                                             0x24 /*!< I3C status register */
#define I3C_DIV_OFFSET                                                0x28 /*!< I3C clock divider */
#define I3C_SETUP_OFFSET                                              0x2c /*!< I3C setup configure register */
#define I3C_TXDATA_OFFSET                                             0x30 /*!< Transmit data register */
#define I3C_RXDATA_OFFSET                                             0x34 /*!< Receive data register */
#define I3C_INT_IE_OFFSET                                             0x38 /*!< Interrupt enable register */
#define I3C_SLAVE_ADDRESS_OFFSET                                      0x3c /*!< Slave address */
#define I3C_INT_CLR_OFFSET                                            0x40 /*!< Interrupt clear */
#define I3C_TXE_TIME_OFFSET                                           0x44 /*!< The time between txe_clr and SCL release for first bit
Note: this bit is used in slave mode */
#define I3C_TIME_OUT_OFFSET                                           0x48 /*!< SCL time out number */
#define I3C_PID_LOW_OFFSET                                            0x4c /*!< Slave's provisional ID[31:0] */
#define I3C_PID_HIGH_OFFSET                                           0x50 /*!< Slave's provisional ID[47:32] */
#define I3C_MASTER_BCR_OFFSET                                         0x54 /*!< I3C Master Bus Characteristics Register */
#define I3C_MASTER_DCR_OFFSET                                         0x58 /*!< I3C Master Device Characteristics Register */
#define I3C_TSCO_OFFSET                                               0x5c /*!< I3C set up timing */
#define I3C_DMA_SLV_TX_TSU_OFFSET                                     0x60 /*!< The time of slave drive sda line after scl falling edge */
#define I3C_DMA_RX_DATASIZE_OFFSET                                    0x64 /*!< The datasize of dma rx channel */
#define I3C_DMA_TX_DATASIZE_OFFSET                                    0x68 /*!< The datasize of dma tx channel */
#define I3C_DEBUG_OFFSET                                              0x98 /*!< The I2C internal signal for systerm debug */
#define I3C_IP_VERSION_OFFSET                                         0x9c /*!< The IP version of I2C-Controller */

 /* ===== I3C RX_SADDR Register definition ===== */
#define I3C_RX_SADDR_RX_SADDR_MASK                BITS(0,31)                                   /*!< I3C RX SADDR: RX_SADDR Bit Mask */  
#define I3C_RX_SADDR_RX_SADDR_OFS                 0U                                          /*!< I3C RX SADDR: RX_SADDR Bit Offset */
#define I3C_RX_SADDR_RX_SADDR(regval)             (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I3C RX SADDR: RX_SADDR Bit Value */  
 
 /* ===== I3C RX_SIZE Register definition ===== */
#define I3C_RX_SIZE_RX_SIZE_MASK                 BITS(0,19)                                   /*!< I3C RX SIZE: RX_SIZE Bit Mask */  
#define I3C_RX_SIZE_RX_SIZE_OFS                  0U                                          /*!< I3C RX SIZE: RX_SIZE Bit Offset */
#define I3C_RX_SIZE_RX_SIZE(regval)              (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< I3C RX SIZE: RX_SIZE Bit Value */  
 
 /* ===== I3C RX_CFG Register definition ===== */
#define I3C_RX_CFG_RX_CONTINUOUS                BIT(0)                                      /*!< RX channel continuous mode bitfield -1'b0: disable -1'b1: enable */
#define I3C_RX_CFG_RX_CONTINUOUS_OFS            0U                                          /*!< I3C RX CFG: RX_CONTINUOUS Bit Offset */
#define I3C_RX_CFG_RX_CONTINUOUS_VAL(regval)        (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I3C RX CFG: RX_CONTINUOUS Bit Value */  
#define I3C_RX_CFG_RX_CONTINUOUS_DISABLE             0x0UL                                              /*!< DISABLE */
#define I3C_RX_CFG_RX_CONTINUOUS_ENABLE              BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_rx_cfg rx_continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_continuous bit.
  * \retval 1 This is a valid rx_continuous bit.
  */
#define IS_I3C_I2C_RX_CFG_RX_CONTINUOUS(regval)          (\
                                    ((regval) == I3C_RX_CFG_RX_CONTINUOUS_DISABLE             ) || \
                                    ((regval) == I3C_RX_CFG_RX_CONTINUOUS_ENABLE              )  \
                                                 )

#define I3C_RX_CFG_RX_EN                        BIT(1)                                      /*!< RX channel enable and start transfer bitfield: -1'b0: disable -1'b1: enable */
#define I3C_RX_CFG_RX_EN_OFS                    1U                                          /*!< I3C RX CFG: RX_EN Bit Offset */
#define I3C_RX_CFG_RX_EN_VAL(regval)                (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I3C RX CFG: RX_EN Bit Value */  
#define I3C_RX_CFG_RX_EN_DISABLE                     0x0UL                                              /*!< DISABLE */
#define I3C_RX_CFG_RX_EN_ENABLE                      BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_rx_cfg rx_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_en bit.
  * \retval 1 This is a valid rx_en bit.
  */
#define IS_I3C_I2C_RX_CFG_RX_EN(regval)                  (\
                                            ((regval) == I3C_RX_CFG_RX_EN_DISABLE             ) || \
                                            ((regval) == I3C_RX_CFG_RX_EN_ENABLE              )  \
                                                 )

#define I3C_RX_CFG_RX_PENDING                   BIT(2)                                      /*!< RX transfer pending in queue status flag: -1'b0: no_pending: no pending transfer in the queue -1'b1: pending transfer in the queue */
#define I3C_RX_CFG_RX_PENDING_NO_PENDING             ((uint32_t)(0) << 2)                                                /*!< NO_PENDING */
#define I3C_RX_CFG_RX_PENDING_PENDING                ((uint32_t)(1) << 2)                                                /*!< PENDING */

/**
  * \brief Check the I3C i2c_rx_cfg rx_pending bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_pending bit.
  * \retval 1 This is a valid rx_pending bit.
  */
#define IS_I3C_I2C_RX_CFG_RX_PENDING(regval)             (\
                                       ((regval) == I3C_RX_CFG_RX_PENDING_NO_PENDING          ) || \
                                       ((regval) == I3C_RX_CFG_RX_PENDING_PENDING             )  \
                                                 )

#define I3C_RX_CFG_RX_CLR                       BIT(3)                                      /*!< RX channel clear and stop transfer: -1'b0: disable -1'b1: enable stop and clear the on-going transfer */
#define I3C_RX_CFG_RX_CLR_OFS                   3U                                          /*!< I3C RX CFG: RX_CLR Bit Offset */
#define I3C_RX_CFG_RX_CLR_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I3C RX CFG: RX_CLR Bit Value */  
#define I3C_RX_CFG_RX_CLR_DISABLE                    0x0UL                                              /*!< DISABLE */
#define I3C_RX_CFG_RX_CLR_ENABLE                     BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_rx_cfg rx_clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_clr bit.
  * \retval 1 This is a valid rx_clr bit.
  */
#define IS_I3C_I2C_RX_CFG_RX_CLR(regval)                 (\
                                           ((regval) == I3C_RX_CFG_RX_CLR_DISABLE             ) || \
                                           ((regval) == I3C_RX_CFG_RX_CLR_ENABLE              )  \
                                                 )

 /* ===== I3C TX_SADDR Register definition ===== */
#define I3C_TX_SADDR_TX_SADDR_MASK                BITS(0,31)                                   /*!< I3C TX SADDR: TX_SADDR Bit Mask */  
#define I3C_TX_SADDR_TX_SADDR_OFS                 0U                                          /*!< I3C TX SADDR: TX_SADDR Bit Offset */
#define I3C_TX_SADDR_TX_SADDR(regval)             (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I3C TX SADDR: TX_SADDR Bit Value */  
 
 /* ===== I3C TX_SIZE Register definition ===== */
#define I3C_TX_SIZE_TX_SIZE_MASK                 BITS(0,19)                                   /*!< I3C TX SIZE: TX_SIZE Bit Mask */  
#define I3C_TX_SIZE_TX_SIZE_OFS                  0U                                          /*!< I3C TX SIZE: TX_SIZE Bit Offset */
#define I3C_TX_SIZE_TX_SIZE(regval)              (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< I3C TX SIZE: TX_SIZE Bit Value */  
 
 /* ===== I3C TX_CFG Register definition ===== */
#define I3C_TX_CFG_TX_CONTINUOUS                BIT(0)                                      /*!< TX channel continuous mode bitfield -1'b0: disable -1'b1: enable */
#define I3C_TX_CFG_TX_CONTINUOUS_OFS            0U                                          /*!< I3C TX CFG: TX_CONTINUOUS Bit Offset */
#define I3C_TX_CFG_TX_CONTINUOUS_VAL(regval)        (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I3C TX CFG: TX_CONTINUOUS Bit Value */  
#define I3C_TX_CFG_TX_CONTINUOUS_DISABLE             0x0UL                                              /*!< DISABLE */
#define I3C_TX_CFG_TX_CONTINUOUS_ENABLE              BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_tx_cfg tx_continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_continuous bit.
  * \retval 1 This is a valid tx_continuous bit.
  */
#define IS_I3C_I2C_TX_CFG_TX_CONTINUOUS(regval)          (\
                                    ((regval) == I3C_TX_CFG_TX_CONTINUOUS_DISABLE             ) || \
                                    ((regval) == I3C_TX_CFG_TX_CONTINUOUS_ENABLE              )  \
                                                 )

#define I3C_TX_CFG_TX_EN                        BIT(1)                                      /*!< TX channel enable and start transfer bitfield: -1'b0: disable -1'b1: enable */
#define I3C_TX_CFG_TX_EN_OFS                    1U                                          /*!< I3C TX CFG: TX_EN Bit Offset */
#define I3C_TX_CFG_TX_EN_VAL(regval)                (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I3C TX CFG: TX_EN Bit Value */  
#define I3C_TX_CFG_TX_EN_DISABLE                     0x0UL                                              /*!< DISABLE */
#define I3C_TX_CFG_TX_EN_ENABLE                      BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_tx_cfg tx_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_en bit.
  * \retval 1 This is a valid tx_en bit.
  */
#define IS_I3C_I2C_TX_CFG_TX_EN(regval)                  (\
                                            ((regval) == I3C_TX_CFG_TX_EN_DISABLE             ) || \
                                            ((regval) == I3C_TX_CFG_TX_EN_ENABLE              )  \
                                                 )

#define I3C_TX_CFG_TX_PENDING                   BIT(2)                                      /*!< RX transfer pending in queue status flag: -1'b0: no_pending no pending transfer in the queue -1'b1: pending transfer in the queue */
#define I3C_TX_CFG_TX_PENDING_NO_PENDING             ((uint32_t)(0) << 2)                                                /*!< NO_PENDING */
#define I3C_TX_CFG_TX_PENDING_PENDING                ((uint32_t)(1) << 2)                                                /*!< PENDING */

/**
  * \brief Check the I3C i2c_tx_cfg tx_pending bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_pending bit.
  * \retval 1 This is a valid tx_pending bit.
  */
#define IS_I3C_I2C_TX_CFG_TX_PENDING(regval)             (\
                                       ((regval) == I3C_TX_CFG_TX_PENDING_NO_PENDING          ) || \
                                       ((regval) == I3C_TX_CFG_TX_PENDING_PENDING             )  \
                                                 )

#define I3C_TX_CFG_TX_CLR                       BIT(3)                                      /*!< TX channel clear and stop transfer: -1'b0: disable -1'b1: enable stop and clear the on-going transfer */
#define I3C_TX_CFG_TX_CLR_OFS                   3U                                          /*!< I3C TX CFG: TX_CLR Bit Offset */
#define I3C_TX_CFG_TX_CLR_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I3C TX CFG: TX_CLR Bit Value */  
#define I3C_TX_CFG_TX_CLR_DISABLE                    0x0UL                                              /*!< DISABLE */
#define I3C_TX_CFG_TX_CLR_ENABLE                     BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_tx_cfg tx_clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_clr bit.
  * \retval 1 This is a valid tx_clr bit.
  */
#define IS_I3C_I2C_TX_CFG_TX_CLR(regval)                 (\
                                           ((regval) == I3C_TX_CFG_TX_CLR_DISABLE             ) || \
                                           ((regval) == I3C_TX_CFG_TX_CLR_ENABLE              )  \
                                                 )

 /* ===== I3C CMD_SADDR Register definition ===== */
#define I3C_CMD_SADDR_CMD_SADDR_MASK               BITS(0,31)                                   /*!< I3C CMD SADDR: CMD_SADDR Bit Mask */  
#define I3C_CMD_SADDR_CMD_SADDR_OFS                0U                                          /*!< I3C CMD SADDR: CMD_SADDR Bit Offset */
#define I3C_CMD_SADDR_CMD_SADDR(regval)            (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I3C CMD SADDR: CMD_SADDR Bit Value */  
 
 /* ===== I3C CMD_SIZE Register definition ===== */
#define I3C_CMD_SIZE_CMD_SIZE_MASK                BITS(0,19)                                   /*!< I3C CMD SIZE: CMD_SIZE Bit Mask */  
#define I3C_CMD_SIZE_CMD_SIZE_OFS                 0U                                          /*!< I3C CMD SIZE: CMD_SIZE Bit Offset */
#define I3C_CMD_SIZE_CMD_SIZE(regval)             (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< I3C CMD SIZE: CMD_SIZE Bit Value */  
 
 /* ===== I3C CMD_CFG Register definition ===== */
#define I3C_CMD_CFG_CMD_CONTINUOUS               BIT(0)                                      /*!< CMD channel continuous mode bitfield -1'b0: disable -1'b1: enable */
#define I3C_CMD_CFG_CMD_CONTINUOUS_OFS           0U                                          /*!< I3C CMD CFG: CMD_CONTINUOUS Bit Offset */
#define I3C_CMD_CFG_CMD_CONTINUOUS_VAL(regval)       (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I3C CMD CFG: CMD_CONTINUOUS Bit Value */  
#define I3C_CMD_CFG_CMD_CONTINUOUS_DISABLE             0x0UL                                             /*!< DISABLE */
#define I3C_CMD_CFG_CMD_CONTINUOUS_ENABLE             BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_cmd_cfg cmd_continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cmd_continuous bit.
  * \retval 1 This is a valid cmd_continuous bit.
  */
#define IS_I3C_I2C_CMD_CFG_CMD_CONTINUOUS(regval)         (\
                                   ((regval) == I3C_CMD_CFG_CMD_CONTINUOUS_DISABLE             ) || \
                                   ((regval) == I3C_CMD_CFG_CMD_CONTINUOUS_ENABLE              )  \
                                                 )

#define I3C_CMD_CFG_CMD_EN                       BIT(1)                                      /*!< CMD channel enable and start transfer bitfield: -1'b0: disable -1'b1: enable */
#define I3C_CMD_CFG_CMD_EN_OFS                   1U                                          /*!< I3C CMD CFG: CMD_EN Bit Offset */
#define I3C_CMD_CFG_CMD_EN_VAL(regval)               (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I3C CMD CFG: CMD_EN Bit Value */  
#define I3C_CMD_CFG_CMD_EN_DISABLE                    0x0UL                                             /*!< DISABLE */
#define I3C_CMD_CFG_CMD_EN_ENABLE                     BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_cmd_cfg cmd_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cmd_en bit.
  * \retval 1 This is a valid cmd_en bit.
  */
#define IS_I3C_I2C_CMD_CFG_CMD_EN(regval)                 (\
                                           ((regval) == I3C_CMD_CFG_CMD_EN_DISABLE             ) || \
                                           ((regval) == I3C_CMD_CFG_CMD_EN_ENABLE              )  \
                                                 )

#define I3C_CMD_CFG_CMD_PENDING                  BIT(2)                                      /*!< CMD transfer pending in queue status flag: -1'b0: disable no pending transfer in the queue -1'b1: enable pending transfer in the queue */
#define I3C_CMD_CFG_CMD_PENDING_DISABLE               ((uint32_t)(0) << 2)                                               /*!< DISABLE */
#define I3C_CMD_CFG_CMD_PENDING_ENABLE                ((uint32_t)(1) << 2)                                               /*!< ENABLE */

/**
  * \brief Check the I3C i2c_cmd_cfg cmd_pending bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cmd_pending bit.
  * \retval 1 This is a valid cmd_pending bit.
  */
#define IS_I3C_I2C_CMD_CFG_CMD_PENDING(regval)            (\
                                      ((regval) == I3C_CMD_CFG_CMD_PENDING_DISABLE             ) || \
                                      ((regval) == I3C_CMD_CFG_CMD_PENDING_ENABLE              )  \
                                                 )

#define I3C_CMD_CFG_CMD_CLR                      BIT(3)                                      /*!< CMD channel clear and stop transfer: -1'b0: disable -1'b1: ebable stop and clear the on-going transfer */
#define I3C_CMD_CFG_CMD_CLR_OFS                  3U                                          /*!< I3C CMD CFG: CMD_CLR Bit Offset */
#define I3C_CMD_CFG_CMD_CLR_VAL(regval)              (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I3C CMD CFG: CMD_CLR Bit Value */  
#define I3C_CMD_CFG_CMD_CLR_DISABLE                   0x0UL                                             /*!< DISABLE */
#define I3C_CMD_CFG_CMD_CLR_EBABLE                    BIT(3)                                                   /*!< EBABLE */

/**
  * \brief Check the I3C i2c_cmd_cfg cmd_clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cmd_clr bit.
  * \retval 1 This is a valid cmd_clr bit.
  */
#define IS_I3C_I2C_CMD_CFG_CMD_CLR(regval)                (\
                                          ((regval) == I3C_CMD_CFG_CMD_CLR_DISABLE             ) || \
                                          ((regval) == I3C_CMD_CFG_CMD_CLR_EBABLE              )  \
                                                 )

 /* ===== I3C STATUS Register definition ===== */
#define I3C_STATUS_BUSY                         BIT(0)                                      /*!< The i3c bus busy status flag This bit assert when I3C-Bus data-transfer is on progress */
#define I3C_STATUS_ARB_LOST                     BIT(1)                                      /*!< I3C arbitration lost status flag This bit assert when arbitration lost error */
#define I3C_STATUS_ACK                          BIT(2)                                      /*!< The i3c ack status from I3C Slave Peripheral */
#define I3C_STATUS_EOT                          BIT(3)                                      /*!< The end of transfer in dma-transfer */
#define I3C_STATUS_EOT_OFS                      3U                                          /*!< I3C STATUS: EOT Bit Offset */
#define I3C_STATUS_EOT_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I3C STATUS: EOT Bit Value */  
#define I3C_STATUS_BYTE_ON_GOING                BIT(4)                                      /*!< A data byte finish on master receive or master transmit mode This bit assert when a data byte receive/transmit is on process */
#define I3C_STATUS_TIME_OUT                     BIT(5)                                      /*!< This bit is assert when the length of low level in SCL line is bigger than config number */
#define I3C_STATUS_SIR_IRQ                      BIT(6)                                      /*!< I3C Slave Interrupt Request interrupt */
#define I3C_STATUS_HJR_IRQ                      BIT(7)                                      /*!< I3C Hot Join Request interrupt */
#define I3C_STATUS_SMR_IRQ                      BIT(8)                                      /*!< I3C Secondary Master Request interrupt */
#define I3C_STATUS_SLAVE_RECEIVE                BIT(9)                                      /*!< The slave receive mode This bit assert when master send write operate to slave, so slave will read from master soon Note:this bit is only used in I3C slave mode */
#define I3C_STATUS_SLAVE_TRANSMIT               BIT(10)                                      /*!< The slave transmit mode This bit assert when master send read operate to slave, so slave will write to master soon Note:this bit is only used in I3C slave mode */
#define I3C_STATUS_SLAVE_TXE                    BIT(11)                                      /*!< Data register empty (slave in transmitter mode) This bit assert when when soft-ware TxData register empty Note: this bit is used in I3C slave cpu mode only */
#define I3C_STATUS_TXFIFO_EMPTY                 BIT(12)                                      /*!< This bit assert when tx-fifo is empty, which used in dma mode only */
#define I3C_STATUS_TXFIFO_FULL                  BIT(13)                                      /*!< This bit assert when tx-fifo is full, which used in dma mode only */
#define I3C_STATUS_SLAVE_RXF                    BIT(14)                                      /*!< Data register full (slave receiver mode) This bit assert when when soft-ware TxData register empty Note: this bit is used in I3C slave cpu mode only. */
#define I3C_STATUS_RXFIFO_EMPTY                 BIT(15)                                      /*!< This bit assert when rx-fifo is empty, which used in dma mode only */
#define I3C_STATUS_RXFIFO_FULL                  BIT(16)                                      /*!< This bit assert when rx-fifo is full, which used in dma mode only */
#define I3C_STATUS_BUS_CONFLICT                 BIT(17)                                      /*!< The I3C-BUS conflict when multi master drive bus */
#define I3C_STATUS_CFG_CMD_EN                   BIT(18)                                      /*!< The cmd channel enable hand shaked signal, which come from DMA-Core */
#define I3C_STATUS_CFG_TX_EN                    BIT(19)                                      /*!< The tx channel enable hand shaked signal, which come from DMA-Core */
#define I3C_STATUS_CFG_RX_EN                    BIT(20)                                      /*!< The rx channel enable hand shaked signal, which come from DMA-Core */
#define I3C_STATUS_DMA_MODE_STATUS              BIT(21)                                      /*!< Indicate the i3c is worked in dma mode This bit assert when there is data transfer or receive with DMA */
#define I3C_STATUS_DMA_CMD_LOAD_STATUS          BIT(22)                                      /*!< Indicate the cmd shadow setting is load to main register successful */
#define I3C_STATUS_DMA_RX_LOAD_STATUS           BIT(23)                                      /*!< Indicate the rx shadow setting is load to main register successful */
#define I3C_STATUS_DMA_TX_LOAD_STATUS           BIT(24)                                      /*!< Indicate the tx shadow setting is load to main register successful */
 
 /* ===== I3C DIV Register definition ===== */
#define I3C_DIV_NUM_MASK                     BITS(0,31)                                   /*!< I3C DIV: NUM Bit Mask */  
#define I3C_DIV_NUM_OFS                      0U                                          /*!< I3C DIV: NUM Bit Offset */
#define I3C_DIV_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I3C DIV: NUM Bit Value */  
 
 /* ===== I3C SETUP Register definition ===== */
#define I3C_SETUP_WORK_MODE                    BIT(0)                                      /*!< I3C-Core work mode selected -1'b0: cpu mode -1'b1: dma mode */
#define I3C_SETUP_WORK_MODE_OFS                0U                                          /*!< I3C SETUP: WORK_MODE Bit Offset */
#define I3C_SETUP_WORK_MODE_VAL(regval)            (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I3C SETUP: WORK_MODE Bit Value */  
#define I3C_SETUP_WORK_MODE_CPU                     0x0UL                                               /*!< CPU */
#define I3C_SETUP_WORK_MODE_DMA                     BIT(0)                                                   /*!< DMA */

/**
  * \brief Check the I3C i2c_setup work_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid work_mode bit.
  * \retval 1 This is a valid work_mode bit.
  */
#define IS_I3C_I2C_SETUP_WORK_MODE(regval)              (\
                                        ((regval) == I3C_SETUP_WORK_MODE_CPU                 ) || \
                                        ((regval) == I3C_SETUP_WORK_MODE_DMA                 )  \
                                                 )

#define I3C_SETUP_ENABLE                       BIT(1)                                      /*!< I3C-Core enable in cpu mode This bit assert to enable I3C-Controller */
#define I3C_SETUP_ENABLE_OFS                   1U                                          /*!< I3C SETUP: ENABLE Bit Offset */
#define I3C_SETUP_ENABLE_VAL(regval)               (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I3C SETUP: ENABLE Bit Value */  
#define I3C_SETUP_START                        BIT(2)                                      /*!< Send start condition in I3C-bus This bit assert to send start condition in I2C-Bus */
#define I3C_SETUP_START_OFS                    2U                                          /*!< I3C SETUP: START Bit Offset */
#define I3C_SETUP_START_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< I3C SETUP: START Bit Value */  
#define I3C_SETUP_STOP                         BIT(3)                                      /*!< Send stop condition in I3C-bus This bit assert to send stop condition in I2C-Bus */
#define I3C_SETUP_STOP_OFS                     3U                                          /*!< I3C SETUP: STOP Bit Offset */
#define I3C_SETUP_STOP_VAL(regval)                 (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I3C SETUP: STOP Bit Value */  
#define I3C_SETUP_WRITE                        BIT(4)                                      /*!< The master transmit mode */
#define I3C_SETUP_WRITE_OFS                    4U                                          /*!< I3C SETUP: WRITE Bit Offset */
#define I3C_SETUP_WRITE_VAL(regval)                (BIT(4) & ((uint32_t)(regval) << 4))        /*!< I3C SETUP: WRITE Bit Value */  
#define I3C_SETUP_READ                         BIT(5)                                      /*!< The master receiver mode */
#define I3C_SETUP_READ_OFS                     5U                                          /*!< I3C SETUP: READ Bit Offset */
#define I3C_SETUP_READ_VAL(regval)                 (BIT(5) & ((uint32_t)(regval) << 5))        /*!< I3C SETUP: READ Bit Value */  
#define I3C_SETUP_ACK                          BIT(6)                                      /*!< The ack bit for hand shake which used in master or slave mode This bit assert when not return a response */
#define I3C_SETUP_ACK_OFS                      6U                                          /*!< I3C SETUP: ACK Bit Offset */
#define I3C_SETUP_ACK_VAL(regval)                  (BIT(6) & ((uint32_t)(regval) << 6))        /*!< I3C SETUP: ACK Bit Value */  
#define I3C_SETUP_SOFT_RESET                   BIT(7)                                      /*!< The soft-reset command to I3C-Core logic -1'b0: disable soft-reset de-assert -1'b1: enable soft-reset assert */
#define I3C_SETUP_SOFT_RESET_OFS               7U                                          /*!< I3C SETUP: SOFT_RESET Bit Offset */
#define I3C_SETUP_SOFT_RESET_VAL(regval)           (BIT(7) & ((uint32_t)(regval) << 7))        /*!< I3C SETUP: SOFT_RESET Bit Value */  
#define I3C_SETUP_SOFT_RESET_DISABLE                0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_SOFT_RESET_ENABLE                 BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup soft_reset bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid soft_reset bit.
  * \retval 1 This is a valid soft_reset bit.
  */
#define IS_I3C_I2C_SETUP_SOFT_RESET(regval)             (\
                                       ((regval) == I3C_SETUP_SOFT_RESET_DISABLE             ) || \
                                       ((regval) == I3C_SETUP_SOFT_RESET_ENABLE              )  \
                                                 )

#define I3C_SETUP_TIME_OUT                     BIT(8)                                      /*!< The SCL line timeout enable -1'b0: disable SCL timeout -1'b1: enable SCL timeout */
#define I3C_SETUP_TIME_OUT_OFS                 8U                                          /*!< I3C SETUP: TIME_OUT Bit Offset */
#define I3C_SETUP_TIME_OUT_VAL(regval)             (BIT(8) & ((uint32_t)(regval) << 8))        /*!< I3C SETUP: TIME_OUT Bit Value */  
#define I3C_SETUP_TIME_OUT_DISABLE                  0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_TIME_OUT_ENABLE                   BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup time_out bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid time_out bit.
  * \retval 1 This is a valid time_out bit.
  */
#define IS_I3C_I2C_SETUP_TIME_OUT(regval)               (\
                                         ((regval) == I3C_SETUP_TIME_OUT_DISABLE             ) || \
                                         ((regval) == I3C_SETUP_TIME_OUT_ENABLE              )  \
                                                 )

#define I3C_SETUP_10BIT_ADDR                   BIT(9)                                      /*!< The 10-bit address mode enable -1'b0: disable 10-bit address mode, and enable 7-bit mode -1'b1: enable 10-bit address mode */
#define I3C_SETUP_10BIT_ADDR_OFS               9U                                          /*!< I3C SETUP: 10BIT_ADDR Bit Offset */
#define I3C_SETUP_10BIT_ADDR_VAL(regval)           (BIT(9) & ((uint32_t)(regval) << 9))        /*!< I3C SETUP: 10BIT_ADDR Bit Value */  
#define I3C_SETUP_10BIT_ADDR_DISABLE                0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_10BIT_ADDR_ENABLE                 BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup 10bit_addr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid 10bit_addr bit.
  * \retval 1 This is a valid 10bit_addr bit.
  */
#define IS_I3C_I2C_SETUP_10BIT_ADDR(regval)             (\
                                       ((regval) == I3C_SETUP_10BIT_ADDR_DISABLE             ) || \
                                       ((regval) == I3C_SETUP_10BIT_ADDR_ENABLE              )  \
                                                 )

#define I3C_SETUP_RD_64BIT                     BIT(10)                                      /*!< This bit only used in i3c mode, when set this bit, the design will  Send 64 clk in SCL bus,and keep SDA line high -1'b0: Disable I3C-Master Read 8 bytes from slave -1'b1: Enable I3C-Master Read 8 bytes from slave */
#define I3C_SETUP_RD_64BIT_OFS                 10U                                          /*!< I3C SETUP: RD_64BIT Bit Offset */
#define I3C_SETUP_RD_64BIT_VAL(regval)             (BIT(10) & ((uint32_t)(regval) << 10))        /*!< I3C SETUP: RD_64BIT Bit Value */  
#define I3C_SETUP_RD_64BIT_DISABLE                  0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_RD_64BIT_ENABLE                   BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup rd_64bit bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rd_64bit bit.
  * \retval 1 This is a valid rd_64bit bit.
  */
#define IS_I3C_I2C_SETUP_RD_64BIT(regval)               (\
                                         ((regval) == I3C_SETUP_RD_64BIT_DISABLE             ) || \
                                         ((regval) == I3C_SETUP_RD_64BIT_ENABLE              )  \
                                                 )

#define I3C_SETUP_ROLE_MODE                    BIT(11)                                      /*!< The I3C-Controller is worked in Slave or Master mode -1'b0: Master mode -1'b1: Slave mode */
#define I3C_SETUP_ROLE_MODE_OFS                11U                                          /*!< I3C SETUP: ROLE_MODE Bit Offset */
#define I3C_SETUP_ROLE_MODE_VAL(regval)            (BIT(11) & ((uint32_t)(regval) << 11))        /*!< I3C SETUP: ROLE_MODE Bit Value */  
#define I3C_SETUP_ROLE_MODE_MASTER                  0x0UL                                               /*!< MASTER */
#define I3C_SETUP_ROLE_MODE_SLAVE                   BIT(11)                                                  /*!< SLAVE */

/**
  * \brief Check the I3C i2c_setup role_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid role_mode bit.
  * \retval 1 This is a valid role_mode bit.
  */
#define IS_I3C_I2C_SETUP_ROLE_MODE(regval)              (\
                                        ((regval) == I3C_SETUP_ROLE_MODE_MASTER              ) || \
                                        ((regval) == I3C_SETUP_ROLE_MODE_SLAVE               )  \
                                                 )

#define I3C_SETUP_MODE                         BIT(12)                                      /*!< Enable IP is working in the MIPI_I3C mode or I2C mode only. -1'b0: Disable the mipi_i3c controller. -1'b1: Enable the mipi_i3c controller. */
#define I3C_SETUP_MODE_OFS                     12U                                          /*!< I3C SETUP: MODE Bit Offset */
#define I3C_SETUP_MODE_VAL(regval)                 (BIT(12) & ((uint32_t)(regval) << 12))        /*!< I3C SETUP: MODE Bit Value */  
#define I3C_SETUP_MODE_DISABLE                      0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_MODE_ENABLE                       BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid mode bit.
  * \retval 1 This is a valid mode bit.
  */
#define IS_I3C_I2C_SETUP_MODE(regval)                   (\
                                             ((regval) == I3C_SETUP_MODE_DISABLE             ) || \
                                             ((regval) == I3C_SETUP_MODE_ENABLE              )  \
                                                 )

#define I3C_SETUP_PUSH_PULL_MODE               BIT(13)                                      /*!< The SDA line push-pull enable -1'b0: OD: the SDA line is work in open-drain mode -1'b1: PP: the SDA line is work in push-pull mode */
#define I3C_SETUP_PUSH_PULL_MODE_OFS           13U                                          /*!< I3C SETUP: PUSH_PULL_MODE Bit Offset */
#define I3C_SETUP_PUSH_PULL_MODE_VAL(regval)       (BIT(13) & ((uint32_t)(regval) << 13))        /*!< I3C SETUP: PUSH_PULL_MODE Bit Value */  
#define I3C_SETUP_PUSH_PULL_MODE_OD                 0x0UL                                               /*!< OD */
#define I3C_SETUP_PUSH_PULL_MODE_PP                 BIT(13)                                                  /*!< PP */

/**
  * \brief Check the I3C i2c_setup push_pull_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid push_pull_mode bit.
  * \retval 1 This is a valid push_pull_mode bit.
  */
#define IS_I3C_I2C_SETUP_PUSH_PULL_MODE(regval)         (\
                                   ((regval) == I3C_SETUP_PUSH_PULL_MODE_OD                  ) || \
                                   ((regval) == I3C_SETUP_PUSH_PULL_MODE_PP                  )  \
                                                 )

#define I3C_SETUP_SCL_PULLUP                   BIT(14)                                      /*!< The SCL line pull-up enable -1'b0: disable pullup resistor in scl bus -1'b1: enable pullup resistor in scl bus */
#define I3C_SETUP_SCL_PULLUP_OFS               14U                                          /*!< I3C SETUP: SCL_PULLUP Bit Offset */
#define I3C_SETUP_SCL_PULLUP_VAL(regval)           (BIT(14) & ((uint32_t)(regval) << 14))        /*!< I3C SETUP: SCL_PULLUP Bit Value */  
#define I3C_SETUP_SCL_PULLUP_DISABLE                0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_SCL_PULLUP_ENABLE                 BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup scl_pullup bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid scl_pullup bit.
  * \retval 1 This is a valid scl_pullup bit.
  */
#define IS_I3C_I2C_SETUP_SCL_PULLUP(regval)             (\
                                       ((regval) == I3C_SETUP_SCL_PULLUP_DISABLE             ) || \
                                       ((regval) == I3C_SETUP_SCL_PULLUP_ENABLE              )  \
                                                 )

#define I3C_SETUP_SDA_PULLUP                   BIT(15)                                      /*!< The SDA line pull-up enable -1'b0: disable pullup resistor in sda bus -1'b1: enable pullup resistor in sda bus */
#define I3C_SETUP_SDA_PULLUP_OFS               15U                                          /*!< I3C SETUP: SDA_PULLUP Bit Offset */
#define I3C_SETUP_SDA_PULLUP_VAL(regval)           (BIT(15) & ((uint32_t)(regval) << 15))        /*!< I3C SETUP: SDA_PULLUP Bit Value */  
#define I3C_SETUP_SDA_PULLUP_DISABLE                0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_SDA_PULLUP_ENABLE                 BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup sda_pullup bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid sda_pullup bit.
  * \retval 1 This is a valid sda_pullup bit.
  */
#define IS_I3C_I2C_SETUP_SDA_PULLUP(regval)             (\
                                       ((regval) == I3C_SETUP_SDA_PULLUP_DISABLE             ) || \
                                       ((regval) == I3C_SETUP_SDA_PULLUP_ENABLE              )  \
                                                 )

#define I3C_SETUP_PARITY_MODE                  BIT(16)                                      /*!< Parity-bit on the SDA line -1'b0: DISABLE: Not send/receive parity-bit after last bit of byte -1'b1: ENABLE: Send/Receive parity-bit after last bit of Byte */
#define I3C_SETUP_PARITY_MODE_OFS              16U                                          /*!< I3C SETUP: PARITY_MODE Bit Offset */
#define I3C_SETUP_PARITY_MODE_VAL(regval)          (BIT(16) & ((uint32_t)(regval) << 16))        /*!< I3C SETUP: PARITY_MODE Bit Value */  
#define I3C_SETUP_PARITY_MODE_DISABLE               0x0UL                                               /*!< DISABLE */
#define I3C_SETUP_PARITY_MODE_ENABLE                BIT(16)                                                  /*!< ENABLE */

/**
  * \brief Check the I3C i2c_setup parity_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid parity_mode bit.
  * \retval 1 This is a valid parity_mode bit.
  */
#define IS_I3C_I2C_SETUP_PARITY_MODE(regval)            (\
                                      ((regval) == I3C_SETUP_PARITY_MODE_DISABLE             ) || \
                                      ((regval) == I3C_SETUP_PARITY_MODE_ENABLE              )  \
                                                 )

#define I3C_SETUP_REORDER_PARITY_BIT           BIT(17)                                      /*!< When receive data is include Parity-bit(9-bit mode), Re-order Parity-bit of receive data -1'b0: MSB: Parity-bit is the MSB of receive data -1'b1: LSB: Parity-bit is the LSB of receive data Note: When recieve data is not include ParityBit(8-bit mode), This bit should be setted, which means RXDATA[7:0] is receive data from I3C-Slave. */
#define I3C_SETUP_REORDER_PARITY_BIT_OFS       17U                                          /*!< I3C SETUP: REORDER_PARITY_BIT Bit Offset */
#define I3C_SETUP_REORDER_PARITY_BIT_VAL(regval)   (BIT(17) & ((uint32_t)(regval) << 17))        /*!< I3C SETUP: REORDER_PARITY_BIT Bit Value */  
#define I3C_SETUP_REORDER_PARITY_BIT_MSB             0x0UL                                               /*!< MSB */
#define I3C_SETUP_REORDER_PARITY_BIT_LSB             BIT(17)                                                  /*!< LSB */

/**
  * \brief Check the I3C i2c_setup reorder_parity_bit bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid reorder_parity_bit bit.
  * \retval 1 This is a valid reorder_parity_bit bit.
  */
#define IS_I3C_I2C_SETUP_REORDER_PARITY_BIT(regval)     (\
                               ((regval) == I3C_SETUP_REORDER_PARITY_BIT_MSB                 ) || \
                               ((regval) == I3C_SETUP_REORDER_PARITY_BIT_LSB                 )  \
                                                 )

#define I3C_SETUP_SCL_DUTY_CYCLE_SEL_MASK      BITS(18,19)                                   /*!< I3C SETUP: SCL_DUTY_CYCLE_SEL Bit Mask */  
#define I3C_SETUP_SCL_DUTY_CYCLE_SEL_OFS       18U                                          /*!< I3C SETUP: SCL_DUTY_CYCLE_SEL Bit Offset */
#define I3C_SETUP_SCL_DUTY_CYCLE_SEL(regval)   (BITS(18,19) & ((uint32_t)(regval) << 18))        /*!< I3C SETUP: SCL_DUTY_CYCLE_SEL Bit Value */  
#define I3C_SETUP_SCL_DUTY_CYCLE_SEL_30             I3C_SETUP_SCL_DUTY_CYCLE_SEL(0)                                                 /*!< 30 */
#define I3C_SETUP_SCL_DUTY_CYCLE_SEL_40             I3C_SETUP_SCL_DUTY_CYCLE_SEL(1)                                                 /*!< 40 */
#define I3C_SETUP_SCL_DUTY_CYCLE_SEL_50             I3C_SETUP_SCL_DUTY_CYCLE_SEL(2)                                                 /*!< 50 */

/**
  * \brief Check the I3C i2c_setup scl_duty_cycle_sel bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid scl_duty_cycle_sel bits.
  * \retval 1 This is a valid scl_duty_cycle_sel bits.
  */
#define IS_I3C_I2C_SETUP_SCL_DUTY_CYCLE_SEL(regval)     (\
                               ((regval) == I3C_SETUP_SCL_DUTY_CYCLE_SEL_30                  ) || \
                               ((regval) == I3C_SETUP_SCL_DUTY_CYCLE_SEL_40                  ) || \
                               ((regval) == I3C_SETUP_SCL_DUTY_CYCLE_SEL_50                  )  \
                                                 )

#define I3C_SETUP_AUTO_END                     BIT(20)                                      /*!< The STOP condition auto send control bit. -1'b0: Enable I2C-Master send stop condition automatically when I2C-Master has detected nack phase. -1'b1: Disable I2C-Master send stop condition automatically when I2C-Master has detected nack phase. Note: this bit is used only in the Master Mode. */
#define I3C_SETUP_AUTO_END_OFS                 20U                                          /*!< I3C SETUP: AUTO_END Bit Offset */
#define I3C_SETUP_AUTO_END_VAL(regval)             (BIT(20) & ((uint32_t)(regval) << 20))        /*!< I3C SETUP: AUTO_END Bit Value */  
#define I3C_SETUP_AUTO_END_ENABLE                   0x0UL                                               /*!< ENABLE */
#define I3C_SETUP_AUTO_END_DISABLE                  BIT(20)                                                  /*!< DISABLE */

/**
  * \brief Check the I3C i2c_setup auto_end bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid auto_end bit.
  * \retval 1 This is a valid auto_end bit.
  */
#define IS_I3C_I2C_SETUP_AUTO_END(regval)               (\
                                         ((regval) == I3C_SETUP_AUTO_END_ENABLE              ) || \
                                         ((regval) == I3C_SETUP_AUTO_END_DISABLE             )  \
                                                 )

 /* ===== I3C TXDATA Register definition ===== */
#define I3C_TXDATA_TXDATA_MASK                  BITS(0,7)                                   /*!< I3C TXDATA: TXDATA Bit Mask */  
#define I3C_TXDATA_TXDATA_OFS                   0U                                          /*!< I3C TXDATA: TXDATA Bit Offset */
#define I3C_TXDATA_TXDATA(regval)               (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< I3C TXDATA: TXDATA Bit Value */  
 
 /* ===== I3C RXDATA Register definition ===== */
#define I3C_RXDATA_RXDATA                       BITS(0,7)                
 
 /* ===== I3C INT_IE Register definition ===== */
#define I3C_INT_IE_EOT                          BIT(0)                                      /*!< The last transfer interrupt enable -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_EOT_OFS                      0U                                          /*!< I3C INT IE: EOT Bit Offset */
#define I3C_INT_IE_EOT_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I3C INT IE: EOT Bit Value */  
#define I3C_INT_IE_EOT_DISABLE                       0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_EOT_ENABLE                        BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie eot bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid eot bit.
  * \retval 1 This is a valid eot bit.
  */
#define IS_I3C_INT_IE_EOT(regval)                    (\
                                              ((regval) == I3C_INT_IE_EOT_DISABLE             ) || \
                                              ((regval) == I3C_INT_IE_EOT_ENABLE              )  \
                                                 )

#define I3C_INT_IE_ARBITRATION_LOST             BIT(1)                                      /*!< Atribtion lost interrupt enable -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_ARBITRATION_LOST_OFS         1U                                          /*!< I3C INT IE: ARBITRATION_LOST Bit Offset */
#define I3C_INT_IE_ARBITRATION_LOST_VAL(regval)     (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I3C INT IE: ARBITRATION_LOST Bit Value */  
#define I3C_INT_IE_ARBITRATION_LOST_DISABLE             0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_ARBITRATION_LOST_ENABLE             BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie arbitration_lost bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid arbitration_lost bit.
  * \retval 1 This is a valid arbitration_lost bit.
  */
#define IS_I3C_INT_IE_ARBITRATION_LOST(regval)       (\
                                 ((regval) == I3C_INT_IE_ARBITRATION_LOST_DISABLE             ) || \
                                 ((regval) == I3C_INT_IE_ARBITRATION_LOST_ENABLE              )  \
                                                 )

#define I3C_INT_IE_SLAVE_TX_EMPTY               BIT(2)                                      /*!< I3c slave tx empty int enable in cpu mode -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_SLAVE_TX_EMPTY_OFS           2U                                          /*!< I3C INT IE: SLAVE_TX_EMPTY Bit Offset */
#define I3C_INT_IE_SLAVE_TX_EMPTY_VAL(regval)       (BIT(2) & ((uint32_t)(regval) << 2))        /*!< I3C INT IE: SLAVE_TX_EMPTY Bit Value */  
#define I3C_INT_IE_SLAVE_TX_EMPTY_DISABLE             0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_SLAVE_TX_EMPTY_ENABLE             BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie slave_tx_empty bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid slave_tx_empty bit.
  * \retval 1 This is a valid slave_tx_empty bit.
  */
#define IS_I3C_INT_IE_SLAVE_TX_EMPTY(regval)         (\
                                   ((regval) == I3C_INT_IE_SLAVE_TX_EMPTY_DISABLE             ) || \
                                   ((regval) == I3C_INT_IE_SLAVE_TX_EMPTY_ENABLE              )  \
                                                 )

#define I3C_INT_IE_SLAVE_RX_FULL                BIT(3)                                      /*!< I3c slave rx full int enable in cpu mode -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_SLAVE_RX_FULL_OFS            3U                                          /*!< I3C INT IE: SLAVE_RX_FULL Bit Offset */
#define I3C_INT_IE_SLAVE_RX_FULL_VAL(regval)        (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I3C INT IE: SLAVE_RX_FULL Bit Value */  
#define I3C_INT_IE_SLAVE_RX_FULL_DISABLE             0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_SLAVE_RX_FULL_ENABLE              BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie slave_rx_full bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid slave_rx_full bit.
  * \retval 1 This is a valid slave_rx_full bit.
  */
#define IS_I3C_INT_IE_SLAVE_RX_FULL(regval)          (\
                                    ((regval) == I3C_INT_IE_SLAVE_RX_FULL_DISABLE             ) || \
                                    ((regval) == I3C_INT_IE_SLAVE_RX_FULL_ENABLE              )  \
                                                 )

#define I3C_INT_IE_TIME_OUT                     BIT(4)                                      /*!< SCL line time out interrupt enable -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_TIME_OUT_OFS                 4U                                          /*!< I3C INT IE: TIME_OUT Bit Offset */
#define I3C_INT_IE_TIME_OUT_VAL(regval)             (BIT(4) & ((uint32_t)(regval) << 4))        /*!< I3C INT IE: TIME_OUT Bit Value */  
#define I3C_INT_IE_TIME_OUT_DISABLE                  0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_TIME_OUT_ENABLE                   BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie time_out bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid time_out bit.
  * \retval 1 This is a valid time_out bit.
  */
#define IS_I3C_INT_IE_TIME_OUT(regval)               (\
                                         ((regval) == I3C_INT_IE_TIME_OUT_DISABLE             ) || \
                                         ((regval) == I3C_INT_IE_TIME_OUT_ENABLE              )  \
                                                 )

#define I3C_INT_IE_INT                          BIT(5)                                      /*!< The I3C interrupt enable -1'b0: disable -1'b1: enable Which include: 1.Hot-Join-Request 2.Slave-Interrupt-Request 3.Secondary-Master-Request */
#define I3C_INT_IE_INT_OFS                      5U                                          /*!< I3C INT IE: INT Bit Offset */
#define I3C_INT_IE_INT_VAL(regval)                  (BIT(5) & ((uint32_t)(regval) << 5))        /*!< I3C INT IE: INT Bit Value */  
#define I3C_INT_IE_INT_DISABLE                       0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_INT_ENABLE                        BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie int bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid int bit.
  * \retval 1 This is a valid int bit.
  */
#define IS_I3C_INT_IE_INT(regval)                    (\
                                              ((regval) == I3C_INT_IE_INT_DISABLE             ) || \
                                              ((regval) == I3C_INT_IE_INT_ENABLE              )  \
                                                 )

#define I3C_INT_IE_DMA_CMD_LOAD                 BIT(6)                                      /*!< DMA command channel load status interrupt enable -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_DMA_CMD_LOAD_OFS             6U                                          /*!< I3C INT IE: DMA_CMD_LOAD Bit Offset */
#define I3C_INT_IE_DMA_CMD_LOAD_VAL(regval)         (BIT(6) & ((uint32_t)(regval) << 6))        /*!< I3C INT IE: DMA_CMD_LOAD Bit Value */  
#define I3C_INT_IE_DMA_CMD_LOAD_DISABLE              0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_DMA_CMD_LOAD_ENABLE               BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie dma_cmd_load bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_cmd_load bit.
  * \retval 1 This is a valid dma_cmd_load bit.
  */
#define IS_I3C_INT_IE_DMA_CMD_LOAD(regval)           (\
                                     ((regval) == I3C_INT_IE_DMA_CMD_LOAD_DISABLE             ) || \
                                     ((regval) == I3C_INT_IE_DMA_CMD_LOAD_ENABLE              )  \
                                                 )

#define I3C_INT_IE_DMA_RX_LOAD                  BIT(7)                                      /*!< DMA rx channel load status interrupt enable -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_DMA_RX_LOAD_OFS              7U                                          /*!< I3C INT IE: DMA_RX_LOAD Bit Offset */
#define I3C_INT_IE_DMA_RX_LOAD_VAL(regval)          (BIT(7) & ((uint32_t)(regval) << 7))        /*!< I3C INT IE: DMA_RX_LOAD Bit Value */  
#define I3C_INT_IE_DMA_RX_LOAD_DISABLE               0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_DMA_RX_LOAD_ENABLE                BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie dma_rx_load bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_rx_load bit.
  * \retval 1 This is a valid dma_rx_load bit.
  */
#define IS_I3C_INT_IE_DMA_RX_LOAD(regval)            (\
                                      ((regval) == I3C_INT_IE_DMA_RX_LOAD_DISABLE             ) || \
                                      ((regval) == I3C_INT_IE_DMA_RX_LOAD_ENABLE              )  \
                                                 )

#define I3C_INT_IE_DMA_TX_LOAD                  BIT(8)                                      /*!< DMA tx channel load status interrupt enable -1'b0: disable -1'b1: enable */
#define I3C_INT_IE_DMA_TX_LOAD_OFS              8U                                          /*!< I3C INT IE: DMA_TX_LOAD Bit Offset */
#define I3C_INT_IE_DMA_TX_LOAD_VAL(regval)          (BIT(8) & ((uint32_t)(regval) << 8))        /*!< I3C INT IE: DMA_TX_LOAD Bit Value */  
#define I3C_INT_IE_DMA_TX_LOAD_DISABLE               0x0UL                                              /*!< DISABLE */
#define I3C_INT_IE_DMA_TX_LOAD_ENABLE                BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the I3C int_ie dma_tx_load bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_tx_load bit.
  * \retval 1 This is a valid dma_tx_load bit.
  */
#define IS_I3C_INT_IE_DMA_TX_LOAD(regval)            (\
                                      ((regval) == I3C_INT_IE_DMA_TX_LOAD_DISABLE             ) || \
                                      ((regval) == I3C_INT_IE_DMA_TX_LOAD_ENABLE              )  \
                                                 )

 /* ===== I3C SLAVE_ADDRESS Register definition ===== */
#define I3C_SLAVE_ADDRESS_SLAVE_ADDRESS_MASK           BITS(0,9)                                   /*!< I3C SLAVE ADDRESS: SLAVE_ADDRESS Bit Mask */  
#define I3C_SLAVE_ADDRESS_SLAVE_ADDRESS_OFS            0U                                          /*!< I3C SLAVE ADDRESS: SLAVE_ADDRESS Bit Offset */
#define I3C_SLAVE_ADDRESS_SLAVE_ADDRESS(regval)        (BITS(0,9) & ((uint32_t)(regval) << 0))        /*!< I3C SLAVE ADDRESS: SLAVE_ADDRESS Bit Value */  
 
 /* ===== I3C INT_CLR Register definition ===== */
#define I3C_INT_CLR_EOT                          BIT(0)                                      /*!< Clear last transfer interrupt */
#define I3C_INT_CLR_EOT_OFS                      0U                                          /*!< I3C INT CLR: EOT Bit Offset */
#define I3C_INT_CLR_EOT_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I3C INT CLR: EOT Bit Value */  
#define I3C_INT_CLR_ARBITRATION_LOST             BIT(1)                                      /*!< Clear arbitration lost interrupt */
#define I3C_INT_CLR_ARBITRATION_LOST_OFS         1U                                          /*!< I3C INT CLR: ARBITRATION_LOST Bit Offset */
#define I3C_INT_CLR_ARBITRATION_LOST_VAL(regval)     (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I3C INT CLR: ARBITRATION_LOST Bit Value */  
#define I3C_INT_CLR_SLAVE_TX_EMPTY               BIT(2)                                      /*!< Clear i3c slave tx empty int enable in cpu Mode */
#define I3C_INT_CLR_SLAVE_TX_EMPTY_OFS           2U                                          /*!< I3C INT CLR: SLAVE_TX_EMPTY Bit Offset */
#define I3C_INT_CLR_SLAVE_TX_EMPTY_VAL(regval)       (BIT(2) & ((uint32_t)(regval) << 2))        /*!< I3C INT CLR: SLAVE_TX_EMPTY Bit Value */  
#define I3C_INT_CLR_SLAVE_RX_FULL                BIT(3)                                      /*!< Clear i3c slave rx full interrupt */
#define I3C_INT_CLR_SLAVE_RX_FULL_OFS            3U                                          /*!< I3C INT CLR: SLAVE_RX_FULL Bit Offset */
#define I3C_INT_CLR_SLAVE_RX_FULL_VAL(regval)        (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I3C INT CLR: SLAVE_RX_FULL Bit Value */  
#define I3C_INT_CLR_TIME_OUT                     BIT(4)                                      /*!< Clear SCL line time out interrupt */
#define I3C_INT_CLR_TIME_OUT_OFS                 4U                                          /*!< I3C INT CLR: TIME_OUT Bit Offset */
#define I3C_INT_CLR_TIME_OUT_VAL(regval)             (BIT(4) & ((uint32_t)(regval) << 4))        /*!< I3C INT CLR: TIME_OUT Bit Value */  
#define I3C_INT_CLR_SIR_IBI_IRQ                  BIT(5)                                      /*!< Clear i3c-bus interrupt in i3c mode */
#define I3C_INT_CLR_SIR_IBI_IRQ_OFS              5U                                          /*!< I3C INT CLR: SIR_IBI_IRQ Bit Offset */
#define I3C_INT_CLR_SIR_IBI_IRQ_VAL(regval)          (BIT(5) & ((uint32_t)(regval) << 5))        /*!< I3C INT CLR: SIR_IBI_IRQ Bit Value */  
#define I3C_INT_CLR_DMA_CMD_LOAD                 BIT(6)                                      /*!< Clear dma command channel load status Interrupt */
#define I3C_INT_CLR_DMA_CMD_LOAD_OFS             6U                                          /*!< I3C INT CLR: DMA_CMD_LOAD Bit Offset */
#define I3C_INT_CLR_DMA_CMD_LOAD_VAL(regval)         (BIT(6) & ((uint32_t)(regval) << 6))        /*!< I3C INT CLR: DMA_CMD_LOAD Bit Value */  
#define I3C_INT_CLR_DMA_RX_LOAD                  BIT(7)                                      /*!< Clear dma rx channel load status interrupt */
#define I3C_INT_CLR_DMA_RX_LOAD_OFS              7U                                          /*!< I3C INT CLR: DMA_RX_LOAD Bit Offset */
#define I3C_INT_CLR_DMA_RX_LOAD_VAL(regval)          (BIT(7) & ((uint32_t)(regval) << 7))        /*!< I3C INT CLR: DMA_RX_LOAD Bit Value */  
#define I3C_INT_CLR_DMA_TX_LOAD                  BIT(8)                                      /*!< Clear dma tx channel load status interrupt */
#define I3C_INT_CLR_DMA_TX_LOAD_OFS              8U                                          /*!< I3C INT CLR: DMA_TX_LOAD Bit Offset */
#define I3C_INT_CLR_DMA_TX_LOAD_VAL(regval)          (BIT(8) & ((uint32_t)(regval) << 8))        /*!< I3C INT CLR: DMA_TX_LOAD Bit Value */  
 
 /* ===== I3C TXE_TIME Register definition ===== */
#define I3C_TXE_TIME_NUM_MASK                     BITS(0,4)                                   /*!< I3C TXE TIME: NUM Bit Mask */  
#define I3C_TXE_TIME_NUM_OFS                      0U                                          /*!< I3C TXE TIME: NUM Bit Offset */
#define I3C_TXE_TIME_NUM(regval)                  (BITS(0,4) & ((uint32_t)(regval) << 0))        /*!< I3C TXE TIME: NUM Bit Value */  
 
 /* ===== I3C TIME_OUT Register definition ===== */
#define I3C_TIME_OUT_NUM_MASK                     BITS(0,15)                                   /*!< I3C TIME OUT: NUM Bit Mask */  
#define I3C_TIME_OUT_NUM_OFS                      0U                                          /*!< I3C TIME OUT: NUM Bit Offset */
#define I3C_TIME_OUT_NUM(regval)                  (BITS(0,15) & ((uint32_t)(regval) << 0))        /*!< I3C TIME OUT: NUM Bit Value */  
 
 /* ===== I3C PID_LOW Register definition ===== */
#define I3C_PID_LOW_VALUE                        BITS(0,31)                
 
 /* ===== I3C PID_HIGH Register definition ===== */
#define I3C_PID_HIGH_VALUE                        BITS(0,31)                
 
 /* ===== I3C MASTER_BCR Register definition ===== */
#define I3C_MASTER_BCR_VALUE_MASK                   BITS(0,7)                                   /*!< I3C MASTER BCR: VALUE Bit Mask */  
#define I3C_MASTER_BCR_VALUE_OFS                    0U                                          /*!< I3C MASTER BCR: VALUE Bit Offset */
#define I3C_MASTER_BCR_VALUE(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< I3C MASTER BCR: VALUE Bit Value */  
 
 /* ===== I3C MASTER_DCR Register definition ===== */
#define I3C_MASTER_DCR_VALUE_MASK                   BITS(0,7)                                   /*!< I3C MASTER DCR: VALUE Bit Mask */  
#define I3C_MASTER_DCR_VALUE_OFS                    0U                                          /*!< I3C MASTER DCR: VALUE Bit Offset */
#define I3C_MASTER_DCR_VALUE(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< I3C MASTER DCR: VALUE Bit Value */  
 
 /* ===== I3C TSCO Register definition ===== */
#define I3C_TSCO_TSCO_MASK                    BITS(0,31)                                   /*!< I3C TSCO: TSCO Bit Mask */  
#define I3C_TSCO_TSCO_OFS                     0U                                          /*!< I3C TSCO: TSCO Bit Offset */
#define I3C_TSCO_TSCO(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I3C TSCO: TSCO Bit Value */  
 
 /* ===== I3C DMA_SLV_TX_TSU Register definition ===== */
#define I3C_DMA_SLV_TX_TSU_DMA_SLV_TX_TSU_MASK          BITS(0,10)                                   /*!< I3C DMA SLV TX TSU: DMA_SLV_TX_TSU Bit Mask */  
#define I3C_DMA_SLV_TX_TSU_DMA_SLV_TX_TSU_OFS           0U                                          /*!< I3C DMA SLV TX TSU: DMA_SLV_TX_TSU Bit Offset */
#define I3C_DMA_SLV_TX_TSU_DMA_SLV_TX_TSU(regval)       (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< I3C DMA SLV TX TSU: DMA_SLV_TX_TSU Bit Value */  
 
 /* ===== I3C DMA_RX_DATASIZE Register definition ===== */
#define I3C_DMA_RX_DATASIZE_RX_DATASIZE_MASK             BITS(0,1)                                   /*!< I3C DMA RX DATASIZE: RX_DATASIZE Bit Mask */  
#define I3C_DMA_RX_DATASIZE_RX_DATASIZE_OFS              0U                                          /*!< I3C DMA RX DATASIZE: RX_DATASIZE Bit Offset */
#define I3C_DMA_RX_DATASIZE_RX_DATASIZE(regval)          (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< I3C DMA RX DATASIZE: RX_DATASIZE Bit Value */  
#define I3C_DMA_RX_DATASIZE_RX_DATASIZE_BYTE                  I3C_DMA_RX_DATASIZE_RX_DATASIZE(0)                                       /*!< BYTE */
#define I3C_DMA_RX_DATASIZE_RX_DATASIZE_WORD                  I3C_DMA_RX_DATASIZE_RX_DATASIZE(2)                                       /*!< WORD */

/**
  * \brief Check the I3C dma_rx_datasize rx_datasize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid rx_datasize bits.
  * \retval 1 This is a valid rx_datasize bits.
  */
#define IS_I3C_DMA_RX_DATASIZE_RX_DATASIZE(regval)            (\
                                      ((regval) == I3C_DMA_RX_DATASIZE_RX_DATASIZE_BYTE                ) || \
                                      ((regval) == I3C_DMA_RX_DATASIZE_RX_DATASIZE_WORD                )  \
                                                 )

 /* ===== I3C DMA_TX_DATASIZE Register definition ===== */
#define I3C_DMA_TX_DATASIZE_TX_DATASIZE_MASK             BITS(0,1)                                   /*!< I3C DMA TX DATASIZE: TX_DATASIZE Bit Mask */  
#define I3C_DMA_TX_DATASIZE_TX_DATASIZE_OFS              0U                                          /*!< I3C DMA TX DATASIZE: TX_DATASIZE Bit Offset */
#define I3C_DMA_TX_DATASIZE_TX_DATASIZE(regval)          (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< I3C DMA TX DATASIZE: TX_DATASIZE Bit Value */  
#define I3C_DMA_TX_DATASIZE_TX_DATASIZE_BYTE                  I3C_DMA_TX_DATASIZE_TX_DATASIZE(0)                                       /*!< BYTE */
#define I3C_DMA_TX_DATASIZE_TX_DATASIZE_WORD                  I3C_DMA_TX_DATASIZE_TX_DATASIZE(2)                                       /*!< WORD */

/**
  * \brief Check the I3C dma_tx_datasize tx_datasize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid tx_datasize bits.
  * \retval 1 This is a valid tx_datasize bits.
  */
#define IS_I3C_DMA_TX_DATASIZE_TX_DATASIZE(regval)            (\
                                      ((regval) == I3C_DMA_TX_DATASIZE_TX_DATASIZE_BYTE                ) || \
                                      ((regval) == I3C_DMA_TX_DATASIZE_TX_DATASIZE_WORD                )  \
                                                 )

 /* ===== I3C DEBUG Register definition ===== */
#define I3C_DEBUG_STATUS                       BITS(0,14)                
 
 /* ===== I3C IP_VERSION Register definition ===== */
#define I3C_IP_VERSION_VERSION_NUM                  BITS(0,31)                

void I3C_Mode_En(I3C_TypeDef* i3cx, ControlStatus status);
void I3C_TimeOut_En(I3C_TypeDef* i3cx, ControlStatus status);
void I3C_TimeOut_Cfg(I3C_TypeDef* i3cx, uint16_t num);
void I3C_Interrupt_En(I3C_TypeDef* i3cx, uint32_t interrupt, ControlStatus status);
void I3C_Interrupt_Clear(I3C_TypeDef* i3cx, uint32_t interrupt);
FlagStatus I3C_Get_IntStatus(I3C_TypeDef* i3cx, uint32_t interrupt);
void I3C_SoftReset_En(I3C_TypeDef* i3cx);
void I3C_DmaRxDataSize_Cfg(I3C_TypeDef* i3cx, uint8_t size);
void I3C_Dma_Cfg(I3C_TypeDef* i3cx, ControlStatus status);
FlagStatus I3C_Get_Status(I3C_TypeDef* i3cx, uint32_t status);
void I3C_Start(I3C_TypeDef* i3cx);
void I3C_Restart(I3C_TypeDef* i3cx);
void I3C_Stop(I3C_TypeDef* i3cx);
void I3C_ACK(I3C_TypeDef* i3cx, ControlStatus status);
void I3C_Parity(I3C_TypeDef* i3cx, ControlStatus status);
void I3C_Init(I3C_TypeDef* i3cx, I3C_InitTypeDef* I3C_InitStruct);
int8_t I3C_SendSlaveAddr(I3C_TypeDef* i3cx, uint8_t addr, I3C_DIRTypedef dir, uint32_t timeout);
int8_t I3C_Write(I3C_TypeDef* i3cx, uint8_t data, uint32_t timeout);
int8_t I3C_Read(I3C_TypeDef* i3cx, uint8_t* data, uint32_t timeout);
/*==== The following API is I3C usage example ====*/
void I3C_MasterWriteBytes(I3C_TypeDef* i3cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes);
void I3C_MasterReadBytes(I3C_TypeDef* i3cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes);
void I3C_DmaTxDateSize_Cfg(I3C_TypeDef* i3cx, I3C_DmaDataSizeTypedef size);
void I3C_DmaRxDateSize_Cfg(I3C_TypeDef* i3cx, I3C_DmaDataSizeTypedef size);
void I3C_MasterWriteBytesDMA(I3C_TypeDef* i3cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes);
void I3C_MasterReadBytesDMA(I3C_TypeDef* i3cx, uint8_t slave_addr, uint8_t* data, uint32_t bytes);
void I3C_UDMAClearChannel(I3C_TypeDef* i3cx, uint32_t Channel);

#ifdef __cplusplus
}
#endif

#endif /* _NS_I3C_H */
