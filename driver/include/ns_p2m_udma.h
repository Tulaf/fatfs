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

#ifndef _NS_PA2M_UDMA_H
#define _NS_PA2M_UDMA_H
/*!
 * \file     ns__p2m_dma.h
 * \brief    This file contains all the functions prototypes for the DMA firmware
  */

#ifdef __cplusplus
 extern "C" {
#endif
#include "ns.h"

/**
  * \brief  DMA PA CHx Config Register structure definition
  */
typedef struct {
  __IO  uint32_t MSRCADDR;              /*!< PA CHx Source Data Base Address */
  __IO  uint32_t MDSTADDR;              /*!< PA CHx Destination Data Base Address */
  __IO  uint32_t MCTRL;                 /*!< PA CHx M2M Transfer Type Config */
  __IOM uint32_t MSIZE;                 /*!< PA CHx M2M Transfer Size */
  __IOM uint32_t MSRCADDR_H;            /*!< PA CHx Source Data Base Address High Bits */
  __IOM uint32_t MSRCADDR_L;            /*!< PA CHx Source Data Base Address Low Bits */
} UDMA_P2M_CHxCfg_TypeDef;

typedef struct
{
  __IO uint32_t CHX_IRQ_EN;
  __IO uint32_t CHX_IRQ_STAT;
  __IO uint32_t CHX_IRQ_CLR;
} UDMA_P2M_CHx_Irq_TypeDef;

/* ===== All DMA Periphral Channel definition ===== */
#define RX_CHANNEL                              (0UL)                                           /*!< DMA Periphral RX Channel */
#define TX_CHANNEL                              (1UL)                                           /*!< DMA Periphral TX Channel */
#define COMMAND_CHANNEL                         (2UL)                                           /*!< DMA Periphral COMMAND Channel */

/**
 * \brief Check the UDMA channel parameters.
 * \param CHANNEL: UDMA channel to be checked.
 * \retval 0: This is not a valid UDMA channel.
 * \retval 1: This is a valid UDMA channel.
 */
#define IS_UDMA_CHANNEL(CHANNEL)                ( ((CHANNEL) == RX_CHANNEL)   ||                                                   ((CHANNEL) == TX_CHANNEL)   ||                                                   ((CHANNEL) == COMMAND_CHANNEL) )

/*!
 * @addtogroup UDMA_Register_Masks UDMA Register Masks
 * @
 */
/*! @name RX_SADDR - RX TX UDMA buffer transfer address register */
#define UDMA_SADDR_ADDR_MASK                 (0xFFFFU)
#define UDMA_SADDR_ADDR_SHIFT                (0U)
#define UDMA_SADDR_ADDR(x)                   (((uint32_t)(((uint32_t)(x)))) & UDMA_SADDR_ADDR_MASK)

/*! @name RX_SIZE - RX TX UDMA buffer transfer size register */
#define UDMA_SIZE_SIZE_MASK                  (0x1FFFFU)
#define UDMA_SIZE_SIZE_SHIFT                 (0U)
#define UDMA_SIZE_SIZE(x)                    (((uint32_t)(((uint32_t)(x)) << UDMA_SIZE_SIZE_SHIFT)) & UDMA_SIZE_SIZE_MASK)

/*! @name RX_CFG - RX TX UDMA transfer configuration register */
#define UDMA_CFG_CONTINOUS_MASK(x)              (0x1U)<<x
#define UDMA_CFG_CONTINOUS_SHIFT(x)             (x)/* UDMA_CFG_CONTINOUS_SHIFT*/
#define UDMA_CFG_CONTINOUS(x)                (((uint32_t)(((uint32_t)(0x1)) << x ))  & UDMA_CFG_CONTINOUS_MASK(x))
#define UDMA_CFG_DATA_SIZE_MASK              (0x6U)
#define UDMA_CFG_DATA_SIZE_SHIFT             (1U)
#define UDMA_CFG_DATA_SIZE(x)                (((uint32_t)(((uint32_t)(x)) << UDMA_CFG_DATA_SIZE_SHIFT)) & UDMA_CFG_DATA_SIZE_MASK)
#define UDMA_CFG_EN_MASK(x)                     (0x1U)<<x
#define UDMA_CFG_EN(x)                       (((uint32_t)(((uint32_t)(0x1)) << x )) & UDMA_CFG_EN_MASK(x))
#define UDMA_CFG_PENDING_MASK                (0x20U)
#define UDMA_CFG_PENDING_SHIFT               (5U)
#define UDMA_CFG_PENDING(x)                  (((uint32_t)(((uint32_t)(x)) << UDMA_CFG_PENDING_SHIFT)) & UDMA_CFG_PENDING_MASK)
#define UDMA_CFG_CLR_MASK                    (0x20U)
#define UDMA_CFG_CLR_SHIFT                   (5U)
#define UDMA_CFG_CLR(x)                      (((uint32_t)(((uint32_t)(x)) << UDMA_CFG_CLR_SHIFT)) & UDMA_CFG_CLR_MASK)
#define UDMA_SADDR_ADDR_MASK                 (0xFFFFU)
#define UDMA_SADDR_ADDR_SHIFT                (0U)
#define UDMA_SADDR_ADDR(x)                   (((uint32_t)(((uint32_t)(x)))) & UDMA_SADDR_ADDR_MASK)

/* ===== DMA P2M IT definition ===== */
#define UDMA_RX_FTRANS_IRQ                       BIT(0)                                          /*!< RX Interrupt Enable for Channelx Full Transfer */
#define UDMA_RX_HTRANS_IRQ                       BIT(1)                                          /*!< RX Interrupt Enable for ChannelX Half Transfer */
#define UDMA_RX_ERR_IRQ                          BIT(2)                                          /*!< RX Interrupt Enable for Channel DMA Access Error */
#define UDMA_TX_FTRANS_IRQ                       BIT(3)                                          /*!< TX Interrupt Enable for Channelx Full Transfer */
#define UDMA_TX_HTRANS_IRQ                       BIT(4)                                          /*!< TX Interrupt Enable for Channelx Half Transfer */
#define UDMA_TX_ERR_IRQ                          BIT(5)                                          /*!< TX Interrupt Enable for Channel DMA Access Error */
#define UDMA_CMD_FTRANS_IRQ                      BIT(6)                                          /*!< CMD Interrupt Enable for Channelx Full Transfer */
#define UDMA_CMD_HTRANS_IRQ                      BIT(7)                                          /*!< CMD Interrupt Enable for Channelx Half Transfer */
#define UDMA_CMD_ERR_IRQ                         BIT(8)                                          /*!< CMD Interrupt Enable for Channel DMA Access Error */

/**
 * \brief Check the P2M UDMA IE bit parameters.
 * \param IE: IE bit value to be checked.
 * \retval 0: This is not a valid IE bit.
 * \retval 1: This is a valid IE bit.
 */
#define IS_P2M_UDMA_CONFIG_IE(IE)             ( ((IE) == UDMA_RX_FTRANS_IRQ)   ||                                                 ((IE) == UDMA_RX_HTRANS_IRQ)   ||                                                 ((IE) == UDMA_RX_ERR_IRQ)      ||                                                 ((IE) == UDMA_TX_FTRANS_IRQ)   ||                                                 ((IE) == UDMA_TX_HTRANS_IRQ)   ||                                                 ((IE) == UDMA_TX_ERR_IRQ)      ||                                                 ((IE) == UDMA_CMD_FTRANS_IRQ)  ||                                                 ((IE) == UDMA_CMD_HTRANS_IRQ)  ||                                                 ((IE) == UDMA_CMD_ERR_IRQ) )

/**
 * \brief Check the P2M UDMA Clear IT bit parameters.
 * \param IT: Clear IT bit value to be checked.
 * \retval 0: This is not a valid Clear IT bit.
 * \retval 1: This is a valid Clear IT bit.
 */
#define IS_P2M_UDMA_CLR_IT(IT)                ( ((IT) == UDMA_RX_FTRANS_IRQ)   ||                                                 ((IT) == UDMA_RX_HTRANS_IRQ)   ||                                                 ((IT) == UDMA_RX_ERR_IRQ)      ||                                                 ((IT) == UDMA_TX_FTRANS_IRQ)   ||                                                 ((IT) == UDMA_TX_HTRANS_IRQ)   ||                                                 ((IT) == UDMA_TX_ERR_IRQ)      ||                                                 ((IT) == UDMA_CMD_FTRANS_IRQ)  ||                                                 ((IT) == UDMA_CMD_HTRANS_IRQ)  ||                                                 ((IT) == UDMA_CMD_ERR_IRQ) )

/**
 * \brief Check the P2M UDMA IF bit parameters.
 * \param IF: IF bit value to be checked.
 * \retval 0: This is not a valid IF bit.
 * \retval 1: This is a valid IF bit.
 */
#define IS_P2M_UDMA_GET_IT(IF)                ( ((IF) == UDMA_RX_FTRANS_IRQ)   ||                                                 ((IF) == UDMA_RX_HTRANS_IRQ)   ||                                                 ((IF) == UDMA_RX_ERR_IRQ)      ||                                                 ((IF) == UDMA_TX_FTRANS_IRQ)   ||                                                 ((IF) == UDMA_TX_HTRANS_IRQ)   ||                                                 ((IF) == UDMA_TX_ERR_IRQ)      ||                                                 ((IF) == UDMA_CMD_FTRANS_IRQ)  ||                                                 ((IF) == UDMA_CMD_HTRANS_IRQ)  ||                                                 ((IF) == UDMA_CMD_ERR_IRQ) )

FlagStatus UDMA_P2M_GetITStatus(UDMA_P2M_CHx_Irq_TypeDef * DMAChannelx, uint32_t UDMA_IT);
void UDMA_P2M_Clear_ITStatus(UDMA_P2M_CHx_Irq_TypeDef* DMAChannelx, uint32_t UDMA_IT);
void UDMA_P2M_ITConfig(UDMA_P2M_CHx_Irq_TypeDef* DMAChannelx, uint32_t UDMA_IE, ControlStatus Status);
void UDMA_EnqueueChannel(UDMA_PeriphCfg_TypeDef *UDMA_Struct, uint32_t Addr, uint32_t Size, uint32_t Config, uint32_t Channel);
void UDMA_ClearChannel(UDMA_PeriphCfg_TypeDef *UDMA_Struct, uint32_t Channel);
static inline void hal_compiler_barrier();
static inline void hal_write32(volatile void *addr, uint32_t value);

#ifdef __cplusplus
}
#endif
#endif
