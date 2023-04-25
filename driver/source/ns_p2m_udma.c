
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

#include "ns_p2m_udma.h"
#include "ns_conf.h"
/*!
 * \file     ns__p2m_dma.c
 * \brief    This file contains all the functions prototypes for the DMA firmware
  */

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

/**
  * \brief  Enables or disables the specified P2M UDMA interrupts.
  * \param  UDMAChannelx: the P2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg UDMAx_CH0_P2M_IRQ
  *     \arg UDMAx_CH1_P2M_IRQ
  *     \arg UDMAx_CH2_P2M_IRQ
  *     \arg UDMAx_CH3_P2M_IRQ
  *     \arg UDMAx_CH4_P2M_IRQ
  *     \arg UDMAx_CH5_P2M_IRQ
  *     \arg UDMAx_CH6_P2M_IRQ
  *     \arg UDMAx_CH7_P2M_IRQ
  *     \arg UDMAx_CH8_P2M_IRQ
  *     \arg UDMAx_CH9_P2M_IRQ
  * \param  UDMA_IE: specifies the P2M UDMA interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     \arg UDMA_RX_FTRANS_IRQ
  *     \arg UDMA_RX_HTRANS_IRQ
  *     \arg UDMA_RX_ERR_IRQ
  *     \arg UDMA_TX_FTRANS_IRQ
  *     \arg UDMA_RX_HTRANS_IRQ
  *     \arg UDMA_TX_ERR_IRQ
  *     \arg UDMA_CMD_FTRANS_IRQ
  *     \arg UDMA_CMD_HTRANS_IRQ
  *     \arg UDMA_CMD_ERR_IRQ
  * \param  Status: Status of the specified UDMA interrupts.
  *                 This parameter can be: ENABLE or DISABLE.
  */
void UDMA_P2M_ITConfig(UDMA_P2M_CHx_Irq_TypeDef* UDMAChannelx, uint32_t UDMA_IE, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_P2M_UDMA_CONFIG_IE(UDMA_IE));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        UDMAChannelx->CHX_IRQ_EN |= UDMA_IE;
    } else {
        UDMAChannelx->CHX_IRQ_EN &= ~UDMA_IE;
    }
}

/**
  * \brief  Initializes the UDMA enqueue channel.
  * \param  UDMA_Struct: general struct of the UDMA peripheral.
  * \param  Addr: channel UDMA transfer address of associated buffer.
  * \param  Size: channel UDMA transfer size of buffer.
  * \param  Cfg:  channel UDMA transfer configuration.
  * \param  Channel: Channel of the UDMA Request sources.
  *  This parameter can be one of the following values:
  *     \arg RX_CHANNEL
  *     \arg TX_CHANNEL
  *     \arg COMMAND_CHANNEL
  */
void UDMA_EnqueueChannel(UDMA_PeriphCfg_TypeDef *UDMA_Struct, uint32_t Addr, uint32_t Size, uint32_t Config, uint32_t Channel)
{
    /* Check the parameters */
    assert_param(IS_UDMA_CHANNEL(Channel));

    switch (Channel) {
        
        case (RX_CHANNEL):
            hal_write32(&UDMA_Struct->RX_SADDR, Addr);
            hal_write32(&UDMA_Struct->RX_SIZE, Size);
            hal_write32(&UDMA_Struct->RX_CFG, Config);
            break;
        
        case (TX_CHANNEL):
            hal_write32(&UDMA_Struct->TX_SADDR, Addr);
            hal_write32(&UDMA_Struct->TX_SIZE, Size);
            hal_write32(&UDMA_Struct->TX_CFG, Config);
            break;
        
        case (COMMAND_CHANNEL):
            hal_write32(&UDMA_Struct->CMD_SADDR, Addr);
            hal_write32(&UDMA_Struct->CMD_SIZE, Size);
            hal_write32(&UDMA_Struct->CMD_CFG, Config);
            break;
        default:
            break;
    }
}

/**
  * \brief  Clears the specified P2M UDMA interrupt source.
  * \param  UDMAChannelx: the P2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg UDMAx_CH0_P2M_IRQ
  *     \arg UDMAx_CH1_P2M_IRQ
  *     \arg UDMAx_CH2_P2M_IRQ
  *     \arg UDMAx_CH3_P2M_IRQ
  *     \arg UDMAx_CH4_P2M_IRQ
  *     \arg UDMAx_CH5_P2M_IRQ
  *     \arg UDMAx_CH6_P2M_IRQ
  *     \arg UDMAx_CH7_P2M_IRQ
  *     \arg UDMAx_CH8_P2M_IRQ
  *     \arg UDMAx_CH9_P2M_IRQ
  * \param  UDMA_IT: specifies the UDMA interrupt source to check.
  *   This parameter can be one of the following values:
  *     \arg UDMA_RX_FTRANS_IRQ
  *     \arg UDMA_RX_HTRANS_IRQ
  *     \arg UDMA_RX_ERR_IRQ
  *     \arg UDMA_TX_FTRANS_IRQ
  *     \arg UDMA_RX_HTRANS_IRQ
  *     \arg UDMA_TX_ERR_IRQ
  *     \arg UDMA_CMD_FTRANS_IRQ
  *     \arg UDMA_CMD_HTRANS_IRQ
  *     \arg UDMA_CMD_ERR_IRQ
  */
void UDMA_P2M_Clear_ITStatus(UDMA_P2M_CHx_Irq_TypeDef* UDMAChannelx, uint32_t UDMA_IT)
{
    /* Check the parameters */
    assert_param(IS_P2M_UDMA_CLR_IT(UDMA_IT));

    UDMAChannelx->CHX_IRQ_CLR = UDMA_IT;
}

/**
  * \brief  Checks whether the specified P2M DMAy CHannelx interrupt has occurred or not.
  * \param  UDMAChannelx: the P2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg UDMAx_CH0_P2M_IRQ
  *     \arg UDMAx_CH1_P2M_IRQ
  *     \arg UDMAx_CH2_P2M_IRQ
  *     \arg UDMAx_CH3_P2M_IRQ
  *     \arg UDMAx_CH4_P2M_IRQ
  *     \arg UDMAx_CH5_P2M_IRQ
  *     \arg UDMAx_CH6_P2M_IRQ
  *     \arg UDMAx_CH7_P2M_IRQ
  *     \arg UDMAx_CH8_P2M_IRQ
  *     \arg UDMAx_CH9_P2M_IRQ
  * \param  UDMA_IT: specifies the UDMA interrupt source to check.
  *   This parameter can be one of the following values:
  *     \arg UDMA_RX_FTRANS_IRQ
  *     \arg UDMA_RX_HTRANS_IRQ
  *     \arg UDMA_RX_ERR_IRQ
  *     \arg UDMA_TX_FTRANS_IRQ
  *     \arg UDMA_RX_HTRANS_IRQ
  *     \arg UDMA_TX_ERR_IRQ
  *     \arg UDMA_CMD_FTRANS_IRQ
  *     \arg UDMA_CMD_HTRANS_IRQ
  *     \arg UDMA_CMD_ERR_IRQ
  * \retval SET: the specified DMA P2M Interrupt is SET.
  * \retval RESET: the specified DMA P2M Interrupt is RESET.
  */
FlagStatus UDMA_P2M_GetITStatus(UDMA_P2M_CHx_Irq_TypeDef * UDMAChannelx, uint32_t UDMA_IT)
{
    /* Check the parameters */
    assert_param(IS_P2M_UDMA_GET_IT(UDMA_IT));

    FlagStatus bitstatus = RESET;
    if (UDMAChannelx->CHX_IRQ_STAT & UDMA_IT) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
  * \brief  Clears the UDMA enqueue channel.
  * \param  UDMA_Struct: general struct of the UDMA peripheral.
  * \param  Channel: Channel of the UDMA Request sources.
  *  This parameter can be one of the following values:
  *     \arg RX_CHANNEL
  *     \arg TX_CHANNEL
  *     \arg COMMAND_CHANNEL
  */
void I2C_ClearChannel(UDMA_PeriphCfg_TypeDef *UDMA_Struct, uint32_t Channel)
{
    /* Check the parameters */
    assert_param(IS_UDMA_CHANNEL(Channel));

    switch (Channel) {
        case RX_CHANNEL:
            hal_write32(&(UDMA_Struct->RX_CFG), UDMA_CFG_CLR(1));
            break;
        case TX_CHANNEL:
            hal_write32(&(UDMA_Struct->TX_CFG), UDMA_CFG_CLR(1));
            break;
        default :
            break;
    }
}
