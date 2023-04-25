
/*
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
#include "ns_udma.h"
#include "ns_conf.h"

/**
  * \brief  Initializes the UDMA memory to memory.
  *
  * \param  UDMAy_CHannelx the configuration of the UDMA memory to memory.
  * \param  UDMA_InitStruct the init structure of the UDMA memory to memory.
  */
void UDMA_Init(UDMA_CHxCfg_TypeDef* UDMAy_CHannelx, UDMA_InitTypeDef* UDMA_InitStruct)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_DMA_PRIORITY(UDMA_InitStruct->UDMA_Priority));
    assert_param(IS_DMA_TRANSMODE(UDMA_InitStruct->UDMA_Mode));
    assert_param(IS_DMA_MDNA(UDMA_InitStruct->UDMA_DstInc));
    assert_param(IS_DMA_MSNA(UDMA_InitStruct->UDMA_SrcInc));
    assert_param(IS_DMA_MSWIDTH(UDMA_InitStruct->UDMA_SrcWidth));
    assert_param(IS_DMA_MDWIDTH(UDMA_InitStruct->UDMA_DstWidth));

    /*--------------------------- DMAy CHannelx  Configuration -----------------*/
    
    /* Configure DMAy CHannelx: data transfer, data size, priority level and mode */
    tmpreg |=  UDMA_InitStruct->UDMA_Mode |
            UDMA_InitStruct->UDMA_DstInc | UDMA_InitStruct->UDMA_SrcInc |
            UDMA_InitStruct->UDMA_DstWidth | UDMA_InitStruct->UDMA_SrcWidth |
            UDMA_InitStruct->UDMA_Priority;

    /* Write to DMAy CHannelx*/
    UDMAy_CHannelx->MCTRL = tmpreg;

    /*--------------------------- DMAy CHannelx  Configuration ---------------*/
    /* Write to DMAy CHannelx  */
    UDMAy_CHannelx->MSIZE = UDMA_InitStruct->UDMA_BufferSize;

    /*--------------------------- DMAy CHannelx CPAR Configuration ----------------*/
    /* Write to DMAy CHannelx  */
    UDMAy_CHannelx->MDSTADDR = CAL_ADDR(UDMA_InitStruct->UDMA_DstBaseAddr) ;
    UDMAy_CHannelx->MSRCADDR = CAL_ADDR(UDMA_InitStruct->UDMA_SrcBaseAddr) ;

    /*--------------------------- DMAy CHannelx CMAR Configuration ----------------*/
    UDMAy_CHannelx->MDRCADDR_H = UDMA_InitStruct->UDMA_DstBaseAddr_h;
    UDMAy_CHannelx->MSRCADDR_H = UDMA_InitStruct->UDMA_SrcBaseAddr_h;
}

/**
  * \brief  Fills each UDMA_InitStruct member with its default value.
  *
  * \param  UDMA_InitStruct pointer to a UDMA_InitTypeDef structure which will be initialized.
  */
void UDMA_StructInit(UDMA_InitTypeDef* UDMA_InitStruct)
{
    /*-------------- Reset DMA init structure parameters values ------------------*/
    /* Initialize the UDMA_PeripheralBaseAddr member */
    UDMA_InitStruct->UDMA_DstBaseAddr = 0;
    UDMA_InitStruct->UDMA_DstBaseAddr_h = 0;
    /* Initialize the UDMA_MemoryBaseAddr member */
    UDMA_InitStruct->UDMA_SrcBaseAddr = 0;
    UDMA_InitStruct->UDMA_SrcBaseAddr_h = 0;
    /* Initialize the UDMA_BufferSize member */
    UDMA_InitStruct->UDMA_BufferSize = 0;
    /* Initialize the UDMA_PeripheralInc member */
    UDMA_InitStruct->UDMA_DstInc = UDMA_MDNA_DISABLE;
    /* Initialize the UDMA_MemoryInc member */
    UDMA_InitStruct->UDMA_SrcInc = UDMA_MSNA_DISABLE;
    /* Initialize the UDMA_PeripheralDataSize member */
    UDMA_InitStruct->UDMA_DstWidth = UDMA_MDWIDTH_8BIT;
    /* Initialize the UDMA_MemoryDataSize member */
    UDMA_InitStruct->UDMA_SrcWidth = UDMA_MSWIDTH_8BIT;
    /* Initialize the UDMA_Mode member */
    UDMA_InitStruct->UDMA_Mode = UDMA_MODE_NORMAL;
    /* Initialize the UDMA_Priority member */
    UDMA_InitStruct->UDMA_Priority = UDMA_PRIORITY_LOW;
}

/**
  * \brief  Enables or disables the DMA transfer.
  *
  * \param  UDMAy_CHannelx the configuration of the UDMA memory to memory.
  * \param  Status Status of the DMA channel transfer.
  *                 This parameter can be: ENABLE or DISABLE.
  */
void M2M_DMA_Cmd(UDMA_CHxCfg_TypeDef* UDMAy_CHannelx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        UDMAy_CHannelx->MCTRL |= UDMA_TRANS_ENABLE;
    } else {
        UDMAy_CHannelx->MCTRL &= ~UDMA_TRANS_ENABLE;
    }
}

/**
  * \brief  Checks whether the specified M2M DMAy CHannelx interrupt has occurred or not.
  *
  * \param  UDMAChannelx the M2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg UDMAx_CH0_M2M_IRQ
  *     \arg UDMAx_CH1_M2M_IRQ
  *     \arg UDMAx_CH2_M2M_IRQ
  *     \arg UDMAx_CH3_M2M_IRQ
  *     \arg UDMAx_CH4_M2M_IRQ
  *     \arg UDMAx_CH5_M2M_IRQ
  *     \arg UDMAx_CH6_M2M_IRQ
  *     \arg UDMAx_CH7_M2M_IRQ
  * \param  UDMA_IT specifies the UDMA interrupt source to check.
  *   This parameter can be one of the following values:
  *     \arg UDMA_FTRANS_IRQ
  *     \arg UDMA_HTRANS_IRQ
  *     \arg UDMA_ERR_IRQ
  *
  * \retval SET the specified DMA M2M Interrupt is SET.
  * \retval RESET the specified DMA M2M Interrupt is RESET.
  */
FlagStatus UDMA_GetITStatus(UDMA_CHx_IRQ_TypeDef * UDMAChannelx, uint32_t UDMA_IT)
{
    /* Check the parameters */
    assert_param(IS_UDMA_GET_IT(UDMA_IT));

    FlagStatus bitstatus = RESET;
    if (UDMAChannelx->CHX_IRQ_STAT & UDMA_IT) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
  * \brief  Clears the specified M2M UDMA interrupt source.
  *
  * \param  UDMAChannelx the M2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg UDMAx_CH0_M2M_IRQ
  *     \arg UDMAx_CH1_M2M_IRQ
  *     \arg UDMAx_CH2_M2M_IRQ
  *     \arg UDMAx_CH3_M2M_IRQ
  *     \arg UDMAx_CH4_M2M_IRQ
  *     \arg UDMAx_CH5_M2M_IRQ
  *     \arg UDMAx_CH6_M2M_IRQ
  *     \arg UDMAx_CH7_M2M_IRQ
  * \param  UDMA_IT specifies the UDMA interrupt source to check.
  *   This parameter can be one of the following values:
  *     \arg UDMA_FTRANS_IRQ_CLR
  *     \arg UDMA_HTRANS_IRQ_CLR
  *     \arg UDMA_ERR_IRQ_CLR
  */
void UDMA_ClearITStatus(UDMA_CHx_IRQ_TypeDef* UDMAChannelx, uint32_t UDMA_IT)
{
    /* Check the parameters */
    assert_param(IS_UDMA_CLR_IT(UDMA_IT));

    UDMAChannelx->CHX_IRQ_CLR = UDMA_IT;
}

/**
  * \brief  Enables or disables the specified M2M UDMA interrupts.
  *
  * \param  UDMAChannelx the M2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg UDMAx_CH0_M2M_IRQ
  *     \arg UDMAx_CH1_M2M_IRQ
  *     \arg UDMAx_CH2_M2M_IRQ
  *     \arg UDMAx_CH3_M2M_IRQ
  *     \arg UDMAx_CH4_M2M_IRQ
  *     \arg UDMAx_CH5_M2M_IRQ
  *     \arg UDMAx_CH6_M2M_IRQ
  *     \arg UDMAx_CH7_M2M_IRQ
  * \param  UDMA_IE specifies the M2M UDMA interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     \arg UDMA_FTRANS_IRQ
  *     \arg UDMA_HTRANS_IRQ
  *     \arg UDMA_ERR_IRQ
  * \param  Status Status of the specified UDMA interrupts.
  *                 This parameter can be: ENABLE or DISABLE.
  */
void UDMA_ITConfig(UDMA_CHx_IRQ_TypeDef* UDMAChannelx, uint32_t UDMA_IE, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_UDMA_CONFIG_IE(UDMA_IE));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        UDMAChannelx->CHX_IRQ_EN |= UDMA_IE;
    } else {
        UDMAChannelx->CHX_IRQ_EN &= ~UDMA_IE;
    }
}

/**
  * \brief  Configurates the UDMA repeat transfer mode.
  *
  * \param  UDMAy_CHannelx the configuration of the UDMA memory to memory.
  * \param  RPTLoopNum the repeat loop number of dma transfer.
  */
void UDMA_RptConfig(UDMA_CHxCfg_TypeDef* UDMAy_CHannelx, uint16_t RPTLoopNum)
{
    UDMAy_CHannelx->RPT = RPTLoopNum & UDMA_TRANS_RPT_MASK;
}

/**
  * \brief  Configurates the UDMA burst mode.
  *
  * \param  UDMAy_CHannelx the configuration of the UDMA memory to memory.
  * \param  Status Status of the specified UDMA burst mode.
  *   This parameter can be: ENABLE or DISABLE.
  * \param  Msbusrt number of source transfers in a burst.
  *   This parameter can be one of the following values:
  *     \arg UDMA_MSBURST_1_TRANSFER
  *     \arg UDMA_MSBURST_2_TRANSFER
  *     \arg UDMA_MSBURST_3_TRANSFER
  *     \arg UDMA_MSBURST_4_TRANSFER
  *     \arg UDMA_MSBURST_5_TRANSFER
  *     \arg UDMA_MSBURST_6_TRANSFER
  *     \arg UDMA_MSBURST_7_TRANSFER
  *     \arg UDMA_MSBURST_8_TRANSFER
  *     \arg UDMA_MSBURST_9_TRANSFER
  *     \arg UDMA_MSBURST_10_TRANSFER
  *     \arg UDMA_MSBURST_11_TRANSFER
  *     \arg UDMA_MSBURST_12_TRANSFER
  *     \arg UDMA_MSBURST_13_TRANSFER
  *     \arg UDMA_MSBURST_14_TRANSFER
  *     \arg UDMA_MSBURST_15_TRANSFER
  *     \arg UDMA_MSBURST_16_TRANSFER
  * \param  Mdbusrt number of destination transfers in a burst.
  *   This parameter can be one of the following values:
  *     \arg UDMA_MDBURST_1_TRANSFER
  *     \arg UDMA_MDBURST_2_TRANSFER
  *     \arg UDMA_MDBURST_3_TRANSFER
  *     \arg UDMA_MDBURST_4_TRANSFER
  *     \arg UDMA_MDBURST_5_TRANSFER
  *     \arg UDMA_MDBURST_6_TRANSFER
  *     \arg UDMA_MDBURST_7_TRANSFER
  *     \arg UDMA_MDBURST_8_TRANSFER
  *     \arg UDMA_MDBURST_9_TRANSFER
  *     \arg UDMA_MDBURST_10_TRANSFER
  *     \arg UDMA_MDBURST_11_TRANSFER
  *     \arg UDMA_MDBURST_12_TRANSFER
  *     \arg UDMA_MDBURST_13_TRANSFER
  *     \arg UDMA_MDBURST_14_TRANSFER
  *     \arg UDMA_MDBURST_15_TRANSFER
  *     \arg UDMA_MDBURST_16_TRANSFER
  */
void UDMA_BurstConfig(UDMA_CHxCfg_TypeDef* UDMAy_CHannelx, ControlStatus Status, uint32_t Msbusrt, uint32_t Mdbusrt)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Status));
    assert_param(IS_DMA_MSBURST(Msbusrt));
    assert_param(IS_DMA_MDBURST(Mdbusrt));

    if (Status) {
        UDMAy_CHannelx->MCTRL |=  Mdbusrt;
        UDMAy_CHannelx->MCTRL |=  Msbusrt;
    } else {
        UDMAy_CHannelx->MCTRL &= ~Mdbusrt;
        UDMAy_CHannelx->MCTRL &= ~Msbusrt;
    }
}

/**
  * \brief  Configurates the UDMA clock gating.
  *
  * \param  Dma the configuration of the UDMA clock.
  * \param  Status Status of the specified UDMA burst mode.
  *   This parameter can be: ENABLE or DISABLE.
  */
void UDMA_ClockGating(UDMA_CommonCfg_TypeDef* Dma, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        Dma->CG |= UDMA_CG_PCTL;
    } else {
        Dma->CG &= ~UDMA_CG_PCTL;
    }
}

static inline void hal_compiler_barrier()
{
    __asm__ __volatile__ ("" : : : "memory");
}

static inline void hal_write32(volatile void *addr, uint32_t value)
{
    hal_compiler_barrier();
    *(volatile uint32_t *)(volatile uint32_t)(addr) = (uint32_t)value;
    hal_compiler_barrier();
}
