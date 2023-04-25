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

/**
  * \file ns_sdio.c
  * \brief source file for the SDIO
  */

/* Includes ------------------------------------------------------------------*/
#include "ns.h"
#include "ns_sdio.h"

/**
 * \brief  SDIO dma enable control.
 * \param  SDIOx select the SDIO peripheral.
 * \param  control dma enable control status
 *         This parameter can be ENABLE or DISABLE.
 */
void SDIO_DmaEn(SDIO_TypeDef *SDIOx, ControlStatus control)
{
    if (ENABLE == control) {
        SDIOx->CR |= SDIO_CR_DMA_ENABLE;
    } else {
        SDIOx->CR &= ~(SDIO_CR_DMA_ENABLE);
    }
}

/**
  * \brief  SDIO dma config SDIO_DmaCfgStruct member with its default value.
  * \param  SDIO_DmaCfgStruct pointer to an SDIO_DmaCfgTypeDef structure which will be initialized.
  */
void SDIO_DmaCfgStructInit(SDIO_DmaCfgTypeDef *SDIO_DmaCfgStruct)
{
    SDIO_DmaCfgStruct->Dma_en = 0;
    SDIO_DmaCfgStruct->Rx_addr = 0;
    SDIO_DmaCfgStruct->Rx_size = 0;
    SDIO_DmaCfgStruct->Rx_en = 0;
    SDIO_DmaCfgStruct->Rx_datasize = 2;

    SDIO_DmaCfgStruct->Tx_addr = 0;
    SDIO_DmaCfgStruct->Tx_size = 0;
    SDIO_DmaCfgStruct->Tx_en = 0;
    SDIO_DmaCfgStruct->Tx_datasize = 2;
}

/**
  * \brief  Initializes the SDIO dma config according to the specified 
  *         parameters in the SDIO_DmaCfgStruct.
  * \param  SDIOx select the SDIO peripheral.
  * \param  SDIO_DmaCfgStruct pointer to a SDIO_DmaCfgTypeDef structure 
  *         that contains the configuration information for the SDIO dma.
  */
void SDIO_DMA_Config(SDIO_TypeDef *SDIOx, SDIO_DmaCfgTypeDef *SDIO_DmaCfgStruct)
{
    uint32_t rxdma_cfg = 0;
    uint32_t txdma_cfg = 0;
    if ((SDIO_DmaCfgStruct->Rx_en) & SDIO_RX_CFG_EN_ENABLE)
    {
        SDIOx->RX_SADDR = ADDR32(SDIO_DmaCfgStruct->Rx_addr);
        SDIOx->RX_SIZE = SDIO_DmaCfgStruct->Rx_size;

        rxdma_cfg = SDIOx->RX_CFG;
        rxdma_cfg |= (SDIO_DmaCfgStruct->Rx_en | SDIO_DmaCfgStruct->Rx_datasize);
        SDIOx->RX_CFG = rxdma_cfg;
    } else if ((SDIO_DmaCfgStruct->Tx_en) & SDIO_TX_CFG_EN_ENABLE) {
        SDIOx->TX_SADDR = ADDR32(SDIO_DmaCfgStruct->Tx_addr);
        SDIOx->TX_SIZE = SDIO_DmaCfgStruct->Tx_size;

        txdma_cfg = SDIOx->TX_CFG;
        txdma_cfg |= (SDIO_DmaCfgStruct->Tx_en | SDIO_DmaCfgStruct->Tx_datasize);
        SDIOx->TX_CFG = txdma_cfg;
    }

    SDIO_DmaEn(SDIOx, SDIO_DmaCfgStruct->Dma_en);
}

/**
  * \brief  Checks whether the specified SDIO flag is set or not.
  * \param  SDIOx select the SDIO peripheral.
  * \param  status specifies the flag to check. 
  *   \arg  SDIO_STATUS_EOT  Transmission end flag
  *   \arg  SDIO_STATUS_ERR  Transmission error flag
  *   \arg  SDIO_STATUS_TXUDR_ERR  tx udr error flag
  *   \arg  SDIO_STATUS_TXOVF_ERR  tx ovf error flag
  *   \arg  SDIO_STATUS_RXUDR_ERR  rx udr error flag
  *   \arg  SDIO_STATUS_RXOVF_ERR  rx udr error flag
  *   \arg  SDIO_STATUS_BUSY  sdio busy flag
  *   \arg  SDIO_STATUS_IRQ_CHECK  sdio interrupt check
  *   \arg  SDIO_STATUS_BLOCK_DONE  sdio block done
  *   \arg  SDIO_STATUS_CMD_ERR_NO  No error
  *   \arg  SDIO_STATUS_CMD_ERR_TIMEOUT  cmd timeout error
  *   \arg  SDIO_STATUS_CMD_ERR_DIR  cmd DIR error
  *   \arg  SDIO_STATUS_CMD_ERR_BUSY cmd BUSY
  *   \arg  SDIO_STATUS_CMD_ERR_CRCERR  cmd CRC error
  *   \arg  SDIO_STATUS_DATA_ERR_NO  No error
  *   \arg  SDIO_STATUS_DATA_ERR_TIMEOUT data TIMEOUT error
  *   \arg  SDIO_STATUS_DATA_ERR_BUSY  data BUSY
  *   \arg  SDIO_STATUS_DATA_ERR_CRCTIMEOUT data CRC TIMEOUT
  *   \arg  SDIO_STATUS_DATA_ERR_CRCERR  data CRC ERR
  * \retval The state of SDIO_STATUS.
  */
uint32_t SDIO_GetFlagStatus(SDIO_TypeDef *SDIOx, uint32_t status)
{
    return SDIOx->STATUS & status;
}

/**
  * \brief  Clears the SDIO's pending flags.
  * \param  SDIOx select the SDIO peripheral.
  * \param  status specifies the flag to clear.
  *   \arg  SDIO_STATUS_EOT  Transmission end flag
  *   \arg  SDIO_STATUS_ERR  Transmission error flag
  *   \arg  SDIO_STATUS_TXUDR_ERR  tx udr error flag
  *   \arg  SDIO_STATUS_TXOVF_ERR  tx ovf error flag
  *   \arg  SDIO_STATUS_RXUDR_ERR  rx udr error flag
  *   \arg  SDIO_STATUS_RXOVF_ERR  rx udr error flag
  */
void SDIO_ClearFlag(SDIO_TypeDef *SDIOx, uint32_t status)
{
    SDIOx->STATUS = status ;
}

/**
  * \brief  Checks whether the specified SDIO interrupt has occurred or not.
  * \param  SDIOx select the SDIO peripheral.
  * \param  status specifies the SDIO interrupt source to check.
  *   \arg  SDIO_IP_TX_IRQ Transmit watermark enable
  *   \arg  SDIO_IP_RX_IRQ Receive watermark enable
  *   \arg  SDIO_IP_RX_EMPTY receive fifo empt
  *   \arg  SDIO_IP_TX_FULL send fifo full
  *   \arg  SDIO_IP_TX_EMPTY send fifo empt
  *   \arg  SDIO_IP_RX_FULL receive fifo full
  * \retval The new state of SDIO_IT (SET or RESET).
  */
uint32_t SDIO_GetIPStatus(SDIO_TypeDef *SDIOx, uint32_t status)
{
    return SDIOx->IP & status;
}

/**
  * \brief  Fills each SDIO_CmdInitStruct member with its default value.
  * \param  SDIOx select the SDIO peripheral.
  * \param  SDIO_CmdInitStruct pointer to an SDIO_CmdInitTypeDef 
  *         structure which will be initialized.
  */
void SDIO_CmdInitStructInit(SDIO_TypeDef *SDIOx, SDIO_CmdInitTypeDef *SDIO_CmdInitStruct)
{
    SDIO_CmdInitStruct->SDIO_CmdIndex = 0;
    SDIO_CmdInitStruct->SDIO_No_Rsp = 0;
    SDIO_CmdInitStruct->SDIO_No_Rsp_Len = 0;
    SDIO_CmdInitStruct->SDIO_Busy = 0;
    SDIO_CmdInitStruct->SDIO_CrcEn = 0;
    SDIO_CmdInitStruct->SDIO_Power_Up = 0;
    SDIO_CmdInitStruct->SDIO_Argument= 0;
}

/**
  * \brief  Fills each SDIO_DataInitStruct member with its default value.
  * \param  SDIOx select the SDIO peripheral.
  * \param  SDIO_DataSetupStruct pointer to an SDIO_DataInitTypeDef structure 
  *         which will be initialized.
  */
void SDIO_DataSetupStructInit(SDIO_TypeDef *SDIOx, SDIO_DataSetupTypeDef *SDIO_DataSetupStruct)
{
    SDIO_DataSetupStruct->Data_en = 0;
    SDIO_DataSetupStruct->Data_rwn = 0;
    SDIO_DataSetupStruct->Data_mode = 0;
    SDIO_DataSetupStruct->Block_num = 0;
    SDIO_DataSetupStruct->Block_size = 0;
}

/**
  * \brief  Initializes the SDIO data setup according to the specified 
  *         parameters in the SDIO_DataSetupStruct.
  * \param  SDIOx select the SDIO peripheral.
  * \param  SDIO_DataSetupStruct pointer to a SDIO_DataInitTypeDef structure 
  *         that contains the configuration information for the SDIO command.
  */
void SDIO_DataSetup(SDIO_TypeDef *SDIOx, SDIO_DataSetupTypeDef *SDIO_DataSetupStruct)
{
    uint32_t tmp = 0;
    tmp = SDIO_DataSetupStruct->Data_en 
        | SDIO_DataSetupStruct->Data_rwn
        | SDIO_DataSetupStruct->Data_mode
        | SDIO_DataSetupStruct->Block_num
        | SDIO_DataSetupStruct->Block_size;
    SDIOx->DATA_SETUP = tmp;
}

/**
  * \brief  Clear the SDIO data setup .
  * \param  SDIOx select the SDIO peripheral.
  */
void SDIO_ClearDataSetup(SDIO_TypeDef *SDIOx)
{
    uint32_t tmp = 0;
    SDIOx->DATA_SETUP = tmp;
}

/**
  * \brief  This bit field represents the SDIO write data timeout counter.
  * \param  SDIOx select the SDIO peripheral.
  * \param  timeout 32-bit data word to write.
  */
void SDIO_SetDateTimeout(SDIO_TypeDef *SDIOx, uint32_t timeout)
{
    SDIOx->DATA_TIMEOUT = timeout;
}

/**
  * \brief  Initializes the SDIO Command according to the specified parameters
  *         in the SDIO_CmdInitStruct and send the command.
  * \param  SDIOx select the SDIO peripheral.
  * \param  SDIO_CmdInitStruct  pointer to a SDIO_CmdInitTypeDef structure that
  *         contains the configuration information for the SDIO command.
  */
void SDIO_SendCommand(SDIO_TypeDef *SDIOx, SDIO_CmdInitTypeDef *SDIO_CmdInitStruct)
{
    uint32_t tmpreg = 0;

/*---------------------------- SDIO ARG Configuration ------------------------*/
    /* Set the SDIO Argument value */
    SDIOx->CMD_ARG = SDIO_CmdInitStruct->SDIO_Argument;

/*---------------------------- SDIO CMD Configuration ------------------------*/
    /* Get the SDIO CMD value */
    tmpreg = SDIOx->CMD_OP;
    tmpreg &= ~(BITS(0,4) | BITS(8,13));
    tmpreg = SDIO_CmdInitStruct->SDIO_No_Rsp
            | SDIO_CmdInitStruct->SDIO_No_Rsp_Len
            | SDIO_CmdInitStruct->SDIO_CrcEn
            | SDIO_CmdInitStruct->SDIO_Busy
            | SDIO_CmdInitStruct->SDIO_Power_Up
            | SDIO_CmdInitStruct->SDIO_CmdIndex;

    /* Write to SDIO CMD */
    SDIOx->CMD_OP = tmpreg;

    /* Start transmission */
    SDIOx->START = SDIO_START_ENABLE;
}

/**
  * \brief  Setup sdio clock divider.
  * \param  SDIOx select the SDIO peripheral.
  * \param  clkdiv clock div number.
  */
void SDIO_Clock_Set( SDIO_TypeDef *SDIOx, uint32_t clkdiv)
{
	uint32_t tmpreg = SDIOx->CLK_DIV;
    tmpreg &= 0x0;
    tmpreg |= clkdiv;
	SDIOx->CLK_DIV = tmpreg;
}

/**
  * \brief  Write one data word to Tx FIFO.
  * \param  SDIOx select the SDIO peripheral.
  * \param  data 32-bit data word to write.
  */
void SDIO_SendData(SDIO_TypeDef *SDIOx, uint32_t data)
{
    SDIOx->TX_DATA = data;
}

/**
  * \brief  Read one data word from Rx FIFO.
  * \param  SDIOx select the SDIO peripheral.
  * \retval Data sdio received
  */
uint32_t SDIO_ReadData(SDIO_TypeDef *SDIOx)
{
    return SDIOx->RX_DATA;
}

/**
  * \brief  Config Tx FIFO watermark.
  * \param  SDIOx select the SDIO peripheral.
  * \param  depth Tx watermark
  */
uint32_t SDIO_SetTxMark(SDIO_TypeDef *SDIOx, uint32_t depth)
{
    SDIOx->TX_MARK = depth;
}

/**
  * \brief  Config Rx FIFO watermark.
  * \param  SDIOx select the SDIO peripheral.
  * \param  depth Rx watermark
  */
uint32_t SDIO_SetRxMark(SDIO_TypeDef *SDIOx, uint32_t depth)
{
    SDIOx->RX_MARK = depth;
}

/**
  * \brief  Config interrupt enable.
  * \param  SDIOx select the SDIO peripheral.
  * \param  interrupt sdio interrupt enable
  *   \arg  SDIO_IE_TX_IRQ_EN  Transmit watermark enable
  *   \arg  SDIO_IE_RX_IRQ_EN  Receive watermark enable
  *   \arg  SDIO_IE_EOT_IRQ_EN  end interrupt enable
  *   \arg  SDIO_IE_ERR_IRQ_EN  error interrupt enable
  *   \arg  SDIO_IE_TXUDR_ERR_EN  tx udr error interrupt enable
  *   \arg  SDIO_IE_TXOVF_ERR_EN  tx ovf error interrupt enable
  *   \arg  SDIO_IE_RXUDR_ERR_EN  rx udr error interrupt enable
  *   \arg  SDIO_IE_RXOVF_ERR_EN  rx ovf error interrupt enable
  *   \arg  SDIO_IE_IRQ_CHECK_EN  sdio check interrupt enable
  *   \arg  SDIO_IE_BLOCK_DONE_EN   sdio blcok done
  * \param  control interrupt enable control status
  *                 This parameter can be ENABLE or DISABLE.
  */
uint32_t SDIO_InterruptEn(SDIO_TypeDef *SDIOx, SDIO_IntTypedef interrupt, ControlStatus control)
{
    if (ENABLE == control) {
        SDIOx->IE |= interrupt;
    } else {
        SDIOx->IE &= ~(interrupt);
    }
}

/**
  * \brief  Config Dma interrupt enable.
  * \param  SDIO_DMA select the SDIO DMA peripheral.
  * \param  interrupt select the SDIO DMA interrupt.
  *   \arg  PA2M_FTRANS_IRQ_EN  interrupt enable for Channel0 full transfer
  *   \arg  PA2M_HTRANS_IRQ_EN  interrupt enable for Channel0 half transfer
  *   \arg  PA2M_RSP_ERR_IRQ_EN interrupt enable for Channel0 DMA access error
  * \param  control interrupt enable control status
  *                 This parameter can be ENABLE or DISABLE.
  */
uint32_t SDIO_DmaInterruptEn(UDMA_P2M_CHx_Irq_TypeDef *SDIO_DMA, SDIO_DmaIntTypedef interrupt, ControlStatus control)
{
    if (ENABLE == control) {
        SDIO_DMA->CHX_IRQ_EN |= interrupt;
    } else {
        SDIO_DMA->CHX_IRQ_EN &= ~(interrupt);
    }
}

/**
  * \brief  Clear Dma interrupt enable.
  * \param  SDIO_DMA select the SDIO DMA peripheral.
  * \param  interrupt_clr select the SDIO DMA interrupt_clr flag.
  *   \arg  PA2M_FTRANS_IRQ_CLEAR_STAT   clear irq status flag for Channel0 full transfer 
  *   \arg  PA2M_HTRANS_IRQ_CLEAR_STAT   clear irq status flag for Channel0 half transfer 
  *   \arg  PA2M_RSP_ERR_IRQ_CLEAR_STAT  clear irq status flag for Channel0 DMA access error
  */
uint32_t SDIO_DmaInterruptClr(UDMA_P2M_CHx_Irq_TypeDef *SDIO_DMA, SDIO_DmaIntClrTypedef interrupt_clr)
{
    SDIO_DMA->CHX_IRQ_CLR = interrupt_clr;
}

/**
  * \brief  Get sdio Dma interrupt status.
  * \param  SDIO_DMA select the SDIO DMA peripheral.
  * \param  status select the SDIO DMA interrupt status.
  *   \arg  PA2M_FTRANS_IRQ_STAT  interrupt irq status flag for Channel0 full transfer
  *   \arg  PA2M_HTRANS_IRQ_STAT  interrupt irq status flag for Channel0 full transfer
  *   \arg  PA2M_RSP_ERR_IRQ_STAT  interrupt irq status flag for Channel0 DMA access error
  * \retval SDIO DMA interrupt status
  */
uint32_t SDIO_DmaGetIntStat(UDMA_P2M_CHx_Irq_TypeDef *SDIO_DMA, SDIO_DmaIntStatTypedef status)
{
    return (SDIO_DMA->CHX_IRQ_STAT & status);
}

/**
  * \brief  Config sdio clk_stop enable.
  * \param  SDIOx select the SDIO peripheral.
  * \param  control clk_stop enable control status
  *                 This parameter can be ENABLE or DISABLE.
  */
void SDIO_StopClkEn(SDIO_TypeDef *SDIOx, ControlStatus control)
{
    if (ENABLE == control) {
        SDIOx->CR |= SDIO_CR_CLK_STOP_ENABLE;
    } else {
        SDIOx->CR &= ~(SDIO_CR_CLK_STOP_ENABLE);
    }
}

/**
  * \brief  Config sdio DDR mode enable.
  * \param  SDIOx select the SDIO peripheral.
  * \param  control DDR mode enable
  *                 This parameter can be ENABLE or DISABLE.
  */
void SDIO_CfgDdrMode(SDIO_TypeDef *SDIOx, ControlStatus control)
{
    if (ENABLE == control) {
        SDIOx->CR |= SDIO_CR_DDR_ENABLE;
    } else {
        SDIOx->CR &= ~(SDIO_CR_DDR_ENABLE);
    }
}
