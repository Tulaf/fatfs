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

#ifndef _NS__UDMA_H
#define _NS__UDMA_H

/*!
 * \file     ns_udma.h
 * \brief    This file contains all the functions prototypes for the DMA firmware
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"

/**
  * \brief  DMA Init Structure definition
  */
typedef struct {
    uint32_t UDMA_DstBaseAddr;           /*!< Specifies the peripheral base address for DMAy CHannelx */
    uint32_t UDMA_SrcBaseAddr;           /*!< Specifies the memory base address for DMAy CHannelx */
    uint32_t UDMA_BufferSize;            /*!< Specifies the buffer size, in data unit, of the specified CHannel */
    uint32_t UDMA_Mode;                  /*!< Specifies the operation mode of the DMAy CHannelx */
    uint32_t UDMA_DstInc;                /*!< Specifies whether the Peripheral address register is incremented or not */
    uint32_t UDMA_SrcInc;                /*!< Specifies whether the memory address register is incremented or not */
    uint32_t UDMA_SrcWidth;              /*!< Specifies the Peripheral data width */
    uint32_t UDMA_DstWidth;              /*!< Specifies the Memory data width */
    uint32_t UDMA_Priority;              /*!< Specifies the software priority for the DMAy CHannelx */
    uint32_t UDMA_DstBaseAddr_h;           /*!< Specifies the peripheral base address for DMAy CHannelx */
    uint32_t UDMA_SrcBaseAddr_h;           /*!< Specifies the memory base address for DMAy CHannelx */
} UDMA_InitTypeDef;

/**
  * \brief  DMA Common Config Register structure definition
  */
typedef struct {
    uint32_t CG;                  /*!< Periphral Control Config */
    uint32_t EVENTID;             /*!< Event ID */
} UDMA_CommonCfg_TypeDef;

/**
  * \brief  DMA CHx Config Register structure definition
  */
typedef struct {
    uint32_t MSRCADDR;            /*!< CHx Source Data Base Address */
    uint32_t MDSTADDR;            /*!< CHx Destination Data Base Address */
    uint32_t MCTRL;               /*!< CHx M2M Transfer Type Config */
    uint32_t RPT;                 /*!< CHx M2M Transfer Repeat Time */
    uint32_t MSIZE;               /*!< CHx M2M Transfer Size */
    uint32_t MSRCADDR_H;          /*!< CHx Source Data Base Address High Bits */
    uint32_t MDRCADDR_H;          /*!< CHx Source Data Base Address High Bits */
} UDMA_CHxCfg_TypeDef;

/**
  * \brief  DMA CHx IRQ Register structure definition
  */
typedef struct {
    uint32_t CHX_IRQ_EN;             /*!< M2M IRQ Enable */
    uint32_t CHX_IRQ_STAT;           /*!< M2M IRQ Flag */
    uint32_t CHX_IRQ_CLR;            /*!< M2M IRQ Clear */
} UDMA_CHx_IRQ_TypeDef;

/* ===== DMA CG PCTL Register definition ===== */
#define UDMA_CG_PCTL_OFS                         (0UL)                                           /*!< DMA CG PCTL: Offset */
#define UDMA_CG_PCTL                             BITS(0,1)                                       /*!< DMA CG PCTL: Configurates the DMA Clock Gating */

/* ===== DMA MCTRL Register definition ===== */
#define UDMA_MCTRL_TRANS_EN_OFS                  UDMA_CH0_CFG_MCTRL_TRANS_EN_OFS                 /*!< DMA MCTRL : Transfer EN Bit Offset */
#define UDMA_MCTRL_TRANS_EN_VAL(regval)          (UDMA_CH0_CFG_MCTRL_TRANS_EN_VAL(regval))       /*!< DMA MCTRL : Transfer EN Bit Value */
#define UDMA_TRANS_ENABLE                        UDMA_CH0_CFG_MCTRL_TRANS_EN_ENABLE              /*!< DMA Transfer Enable */

#define UDMA_MCTRL_TRANS_MODE_OFS                UDMA_CH0_CFG_MCTRL_TRANS_MODE_OFS               /*!< DMA MCTRL : Transfer Mode Bits Offset */
#define UDMA_MCTRL_TRANS_MODE_VAL(regval)        (UDMA_CH0_CFG_MCTRL_TRANS_MODE(regval))         /*!< DMA MCTRL : Transfer Mode Bits Mask */
#define UDMA_MODE_NORMAL                         UDMA_CH0_CFG_MCTRL_TRANS_MODE(0)                /*!< DMA MCTRL : Single Mode Transfer */
#define UDMA_MODE_CONTINUOUS                     UDMA_CH0_CFG_MCTRL_TRANS_MODE(1)                /*!< DMA MCTRL : Continuous Mode Transfer */
#define UDMA_MODE_REPEAT                         UDMA_CH0_CFG_MCTRL_TRANS_MODE(2)                /*!< DMA MCTRL : Repeat Mode Transfer */

/**
 * \brief Check the DMA TRANSMODE bits parameters.
 * \param TRANSMODE TRANSMODE bits value to be checked.
 * \retval 0 This is not a valid TRANSMODE bits.
 * \retval 1 This is a valid TRANSMODE bits.
 */
#define IS_DMA_TRANSMODE(TRANSMODE)             ( ((TRANSMODE) == UDMA_MODE_NORMAL)      ||                                                   ((TRANSMODE) == UDMA_MODE_CONTINUOUS)  ||                                                   ((TRANSMODE) == UDMA_MODE_REPEAT) )

#define UDMA_MCTRL_PRIORITY_OFS                  UDMA_CH0_CFG_MCTRL_PRIORITY_OFS                  /*!< DMA MCTRL : Priority Bits Offset */
#define UDMA_MCTRL_PRIORITY_VAL(regval)          (UDMA_CH0_CFG_MCTRL_PRIORITY(regval))            /*!< DMA MCTRL : Priority Bits Value */
#define UDMA_PRIORITY_LOW                        UDMA_CH0_CFG_MCTRL_PRIORITY(0)                   /*!< DMA MCTRL : Priority Low */
#define UDMA_PRIORITY_MEDIUM                     UDMA_CH0_CFG_MCTRL_PRIORITY(1)                   /*!< DMA MCTRL : Priority Medium */
#define UDMA_PRIORITY_HIGH                       UDMA_CH0_CFG_MCTRL_PRIORITY(2)                   /*!< DMA MCTRL : Priority High */
#define UDMA_PRIORITY_ULTRA_HIGH                 UDMA_CH0_CFG_MCTRL_PRIORITY(3)                   /*!< DMA MCTRL : Priority Ultra High */

/**
 * \brief Check the DMA PRIORITY bits parameters.
 * \param PRIORITY PRIORITY bits value to be checked.
 * \retval 0 This is not a valid PRIORITY bits.
 * \retval 1 This is a valid PRIORITY bits.
 */
#define IS_DMA_PRIORITY(PRIORITY)               ( ((PRIORITY) == UDMA_PRIORITY_LOW)       ||                                                   ((PRIORITY) == UDMA_PRIORITY_MEDIUM)    ||                                                   ((PRIORITY) == UDMA_PRIORITY_HIGH)      ||                                                   ((PRIORITY) == UDMA_PRIORITY_ULTRA_HIGH) )

#define UDMA_MCTRL_MDNA_OFS                      UDMA_CH0_CFG_MCTRL_MDNA_OFS                     /*!< DMA MCTRL : MDNA Bit Offset */
#define UDMA_MDNA_ENABLE                         UDMA_CH0_CFG_MCTRL_MDNA_INCREASING              /*!< DMA MCTRL : MDNA Increasing Address Mode */
#define UDMA_MDNA_DISABLE                        UDMA_CH0_CFG_MCTRL_MDNA_FIXED                   /*!< DMA MCTRL : MDNA Fixed Address Mode */

/**
 * \brief Check the DMA MDNA bit parameters.
 * \param MDNA MDNA bit value to be checked.
 * \retval 0 This is not a valid MDNA bit.
 * \retval 1 This is a valid MDNA bit.
 */
#define IS_DMA_MDNA(MDNA)                       ( ((MDNA) == UDMA_MDNA_DISABLE)   ||                                                   ((MDNA) == UDMA_MDNA_ENABLE) )

#define UDMA_MCTRL_MSNA_OFS                      UDMA_CH0_CFG_MCTRL_MSNA_OFS                 /*!< DMA MCTRL : MSNA Bit Offset */
#define UDMA_MSNA_ENABLE                         UDMA_CH0_CFG_MCTRL_MSNA_INCREASING          /*!< DMA MCTRL : MSNA Increasing Address Mode */
#define UDMA_MSNA_DISABLE                        UDMA_CH0_CFG_MCTRL_MSNA_FIXED               /*!< DMA MCTRL : MSNA Fixed Address Mode */

/**
 * \brief Check the DMA MSNA bit parameters.
 * \param MSNA MSNA bit value to be checked.
 * \retval 0 This is not a valid MSNA bit.
 * \retval 1 This is a valid MSNA bit.
 */
#define IS_DMA_MSNA(MSNA)                       ( ((MSNA) == UDMA_MSNA_DISABLE)   ||                                                   ((MSNA) == UDMA_MSNA_ENABLE) )

#define UDMA_MCTRL_MDWIDTH_OFS                   UDMA_CH0_CFG_MCTRL_MDWIDTH_OFS                  /*!< DMA MCTRL : MDWIDTH Bits Offset */
#define UDMA_MCTRL_MDWIDTH(regval)               (UDMA_CH0_CFG_MCTRL_MDWIDTH(regval))            /*!< DMA MCTRL : MDWIDTH Bits Value */
#define UDMA_MDWIDTH_8BIT                        UDMA_CH0_CFG_MCTRL_MDWIDTH(0)                   /*!< DMA MCTRL : MDWIDTH 8 Bit */
#define UDMA_MDWIDTH_16BIT                       UDMA_CH0_CFG_MCTRL_MDWIDTH(1)                   /*!< DMA MCTRL : MDWIDTH 16 Bit */
#define UDMA_MDWIDTH_32BIT                       UDMA_CH0_CFG_MCTRL_MDWIDTH(2)                   /*!< DMA MCTRL : MDWIDTH 32 Bit */
#define UDMA_MDWIDTH_64BIT                       UDMA_CH0_CFG_MCTRL_MDWIDTH(3)                   /*!< DMA MCTRL : MDWIDTH 64 Bit */
#define UDMA_MDWIDTH_128BIT                      UDMA_CH0_CFG_MCTRL_MDWIDTH(4)                   /*!< DMA MCTRL : MDWIDTH 128 Bit */

/**
 * \brief Check the DMA MDWIDTH bits parameters.
 * \param MDWIDTH MDWIDTH bits value to be checked.
 * \retval 0 This is not a valid MDWIDTH bits.
 * \retval 1 This is a valid MDWIDTH bits.
 */
#define IS_DMA_MDWIDTH(MDWIDTH)                 ( ((MDWIDTH)  == UDMA_MDWIDTH_8BIT)  ||                                                   ((MDWIDTH) == UDMA_MDWIDTH_16BIT) ||                                                   ((MDWIDTH) == UDMA_MDWIDTH_32BIT) ||                                                   ((MDWIDTH) == UDMA_MDWIDTH_64BIT) ||                                                   ((MDWIDTH) == UDMA_MDWIDTH_128BIT) )

#define UDMA_MCTRL_MSWIDTH_OFS                   UDMA_CH0_CFG_MCTRL_MSWIDTH_OFS                  /*!< DMA MCTRL : MSWIDTH Bits Offset */
#define UDMA_MCTRL_MSWIDTH(regval)               (UDMA_CH0_CFG_MCTRL_MSWIDTH(regval))            /*!< DMA MCTRL : MSWIDTH Bits Value */
#define UDMA_MSWIDTH_8BIT                        UDMA_CH0_CFG_MCTRL_MSWIDTH(0)                   /*!< DMA MCTRL : MSWIDTH 8 Bit */
#define UDMA_MSWIDTH_16BIT                       UDMA_CH0_CFG_MCTRL_MSWIDTH(1)                   /*!< DMA MCTRL : MSWIDTH 16 Bit */
#define UDMA_MSWIDTH_32BIT                       UDMA_CH0_CFG_MCTRL_MSWIDTH(2)                   /*!< DMA MCTRL : MSWIDTH 32 Bit */
#define UDMA_MSWIDTH_64BIT                       UDMA_CH0_CFG_MCTRL_MSWIDTH(3)                   /*!< DMA MCTRL : MSWIDTH 64 Bit */
#define UDMA_MSWIDTH_128BIT                      UDMA_CH0_CFG_MCTRL_MSWIDTH(4)                   /*!< DMA MCTRL : MSWIDTH 128 Bit */

/**
 * \brief Check the DMA MSWIDTH bits parameters.
 * \param MSWIDTH MSWIDTH bits value to be checked.
 * \retval 0 This is not a valid MSWIDTH bits.
 * \retval 1 This is a valid MSWIDTH bits.
 */
#define IS_DMA_MSWIDTH(MSWIDTH)                 ( ((MSWIDTH)  == UDMA_MSWIDTH_8BIT) ||                                                   ((MSWIDTH) == UDMA_MSWIDTH_16BIT) ||                                                   ((MSWIDTH) == UDMA_MSWIDTH_32BIT) ||                                                   ((MSWIDTH) == UDMA_MSWIDTH_64BIT) ||                                                   ((MSWIDTH) == UDMA_MSWIDTH_128BIT) )

#define UDMA_MCTRL_MDBURST_OFS                   UDMA_CH0_CFG_MCTRL_MDBURST_OFS                  /*!< DMA MCTRL : MDBURST Bits Offset */
#define UDMA_MCTRL_MDBURST_MASK                  UDMA_CH0_CFG_MCTRL_MDBURST_MASK                 /*!< DMA MCTRL : MDBURST Bits Mask */
#define UDMA_MCTRL_MDBURST_TRANS_NUM(regval)     (UDMA_CH0_CFG_MCTRL_MDBURST(regval))            /*!< DMA MCTRL : MDBURST Transfer Number */
#define UDMA_MDBURST_1_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(0)                   /*!< 1_TRANSFER */
#define UDMA_MDBURST_2_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(1)                   /*!< 2_TRANSFER */
#define UDMA_MDBURST_3_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(2)                   /*!< 3_TRANSFER */
#define UDMA_MDBURST_4_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(3)                   /*!< 4_TRANSFER */
#define UDMA_MDBURST_5_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(4)                   /*!< 5_TRANSFER */
#define UDMA_MDBURST_6_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(5)                   /*!< 6_TRANSFER */
#define UDMA_MDBURST_7_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(6)                   /*!< 7_TRANSFER */
#define UDMA_MDBURST_8_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(7)                   /*!< 8_TRANSFER */
#define UDMA_MDBURST_9_TRANSFER                  UDMA_MCTRL_MDBURST_TRANS_NUM(8)                   /*!< 9_TRANSFER */
#define UDMA_MDBURST_10_TRANSFER                 UDMA_MCTRL_MDBURST_TRANS_NUM(9)                   /*!< 10_TRANSFER */
#define UDMA_MDBURST_11_TRANSFER                 UDMA_MCTRL_MDBURST_TRANS_NUM(10)                  /*!< 11_TRANSFER */
#define UDMA_MDBURST_12_TRANSFER                 UDMA_MCTRL_MDBURST_TRANS_NUM(11)                  /*!< 12_TRANSFER */
#define UDMA_MDBURST_13_TRANSFER                 UDMA_MCTRL_MDBURST_TRANS_NUM(12)                  /*!< 13_TRANSFER */
#define UDMA_MDBURST_14_TRANSFER                 UDMA_MCTRL_MDBURST_TRANS_NUM(13)                  /*!< 14_TRANSFER */
#define UDMA_MDBURST_15_TRANSFER                 UDMA_MCTRL_MDBURST_TRANS_NUM(14)                  /*!< 15_TRANSFER */
#define UDMA_MDBURST_16_TRANSFER                 UDMA_MCTRL_MDBURST_TRANS_NUM(15)                  /*!< 16_TRANSFER */

/**
 * \brief Check the DMA MDBURST bits parameters.
 * \param MDBURST MDBURST bits value to be checked.
 * \retval 0 This is not a valid MDBURST bits.
 * \retval 1 This is a valid MDBURST bits.
 */
#define IS_DMA_MDBURST(MDBURST)                 ( (MDBURST)  == UDMA_MDBURST_1_TRANSFER)  ||                                                   ((MDBURST) == UDMA_MDBURST_2_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_3_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_4_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_5_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_6_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_7_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_8_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_9_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_10_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_11_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_12_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_13_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_14_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_15_TRANSFER) ||                                                   ((MDBURST) == UDMA_MDBURST_16_TRANSFER) 

#define UDMA_MCTRL_MSBURST_OFS                   UDMA_CH0_CFG_MCTRL_MSBURST_OFS                  /*!< DMA MCTRL : MSBURST Bits Offset */
#define UDMA_MCTRL_MSBURST_MASK                  UDMA_CH0_CFG_MCTRL_MSBURST_MASK                 /*!< DMA MCTRL : MSBURST Bits Mask */
#define UDMA_MCTRL_MSBURST_TRANS_NUM(regval)     (UDMA_CH0_CFG_MCTRL_MSBURST(regval))            /*!< DMA MCTRL : MSBURST Transfer Number */
#define UDMA_MSBURST_1_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(0)                 /*!< 1_TRANSFER */
#define UDMA_MSBURST_2_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(1)                 /*!< 2_TRANSFER */
#define UDMA_MSBURST_3_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(2)                 /*!< 3_TRANSFER */
#define UDMA_MSBURST_4_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(3)                 /*!< 4_TRANSFER */
#define UDMA_MSBURST_5_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(4)                 /*!< 5_TRANSFER */
#define UDMA_MSBURST_6_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(5)                 /*!< 6_TRANSFER */
#define UDMA_MSBURST_7_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(6)                 /*!< 7_TRANSFER */
#define UDMA_MSBURST_8_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(7)                 /*!< 8_TRANSFER */
#define UDMA_MSBURST_9_TRANSFER                  UDMA_MCTRL_MSBURST_TRANS_NUM(8)                 /*!< 9_TRANSFER */
#define UDMA_MSBURST_10_TRANSFER                 UDMA_MCTRL_MSBURST_TRANS_NUM(9)                 /*!< 10_TRANSFER */
#define UDMA_MSBURST_11_TRANSFER                 UDMA_MCTRL_MSBURST_TRANS_NUM(10)                /*!< 11_TRANSFER */
#define UDMA_MSBURST_12_TRANSFER                 UDMA_MCTRL_MSBURST_TRANS_NUM(11)                /*!< 12_TRANSFER */
#define UDMA_MSBURST_13_TRANSFER                 UDMA_MCTRL_MSBURST_TRANS_NUM(12)                /*!< 13_TRANSFER */
#define UDMA_MSBURST_14_TRANSFER                 UDMA_MCTRL_MSBURST_TRANS_NUM(13)                /*!< 14_TRANSFER */
#define UDMA_MSBURST_15_TRANSFER                 UDMA_MCTRL_MSBURST_TRANS_NUM(14)                /*!< 15_TRANSFER */
#define UDMA_MSBURST_16_TRANSFER                 UDMA_MCTRL_MSBURST_TRANS_NUM(15)                /*!< 16_TRANSFER */

/**
 * \brief Check the DMA MSBURST bits parameters.
 * \param MSBURST MSBURST bits value to be checked.
 * \retval 0 This is not a valid MSBURST bits.
 * \retval 1 This is a valid MSBURST bits.
 */
#define IS_DMA_MSBURST(MSBURST)                 ( (MSBURST)  == UDMA_MSBURST_1_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_2_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_3_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_4_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_5_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_6_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_7_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_8_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_9_TRANSFER)  ||                                                   ((MSBURST) == UDMA_MSBURST_10_TRANSFER) ||                                                   ((MSBURST) == UDMA_MSBURST_11_TRANSFER) ||                                                   ((MSBURST) == UDMA_MSBURST_12_TRANSFER) ||                                                   ((MSBURST) == UDMA_MSBURST_13_TRANSFER) ||                                                   ((MSBURST) == UDMA_MSBURST_14_TRANSFER) ||                                                   ((MSBURST) == UDMA_MSBURST_15_TRANSFER) ||                                                   ((MSBURST) == UDMA_MSBURST_16_TRANSFER)

/* ===== DMA TRANS RPT Register definition ===== */
#define UDMA_TRANS_RPT_OFS                       UDMA_CH0_CFG_MRPT_TRANS_RPT_OFS                 /*!< DMA TRANS RPT : Offset */
#define UDMA_TRANS_RPT_MASK                      UDMA_CH0_CFG_MRPT_TRANS_RPT_MASK                /*!< DMA TRANS RPT : Mask */
#define UDMA_TRANS_RPT_VAL(regval)               (UDMA_CH0_CFG_MRPT_TRANS_RPT(regval))           /*!< DMA TRANS RPT : Transfer Repeat Number */

/* ===== DMA M2M IT Register definition ===== */
#define UDMA_FTRANS_IRQ                          UDMA_CH0_IRQ_EN_FTRANS                          /*!< Interrupt Enable for Channelx Full Transfer */
#define UDMA_HTRANS_IRQ                          UDMA_CH0_IRQ_EN_HTRANS                          /*!< Interrupt Enable for ChannelX Half Transfer */
#define UDMA_ERR_IRQ                             UDMA_CH0_IRQ_EN_RSP_ERR                         /*!< Interrupt Enable for Channel DMA Access Error */

/**
 * \brief Check the UDMA IF bit parameters.
 * \param IF IF bit value to be checked.
 * \retval 0 This is not a valid IF bit.
 * \retval 1 This is a valid IF bit.
 */
#define IS_UDMA_GET_IT(IF)                      ( ((IF) == UDMA_FTRANS_IRQ)   ||                                                   ((IF) == UDMA_HTRANS_IRQ)   ||                                                   ((IF) == UDMA_ERR_IRQ) )

/**
 * \brief Check the UDMA IE bit parameters.
 * \param IE IE bit value to be checked.
 * \retval 0 This is not a valid IE bit.
 * \retval 1 This is a valid IE bit.
 */
#define IS_UDMA_CONFIG_IE(IE)                   ( ((IE) == UDMA_FTRANS_IRQ)   ||                                                   ((IE) == UDMA_HTRANS_IRQ)   ||                                                   ((IE) == UDMA_ERR_IRQ) )

/* ===== DMA IT CLEAR Register definition ===== */
#define UDMA_FTRANS_IRQ_CLR                     UDMA_CH0_IRQ_CLR_FTRANS                          /*!< Clear Interrupt Enable for Channelx Full Transfer */
#define UDMA_HTRANS_IRQ_CLR                     UDMA_CH0_IRQ_CLR_HTRANS                          /*!< Clear Interrupt Enable for ChannelX Half Transfer */
#define UDMA_ERR_IRQ_CLR                        UDMA_CH0_IRQ_CLR_RSP_ERR                         /*!< Clear Interrupt Enable for Channel DMA Access Error */

/**
 * \brief Check the UDMA Clear IT bit parameters.
 * \param IT Clear IT bit value to be checked.
 * \retval 0 This is not a valid Clear IT bit.
 * \retval 1 This is a valid Clear IT bit.
 */
#define IS_UDMA_CLR_IT(IT)                      ( ((IT) == UDMA_FTRANS_IRQ_CLR)   ||                                                   ((IT) == UDMA_HTRANS_IRQ_CLR)   ||                                                   ((IT) == UDMA_ERR_IRQ_CLR) )

#define UDMA_CH0_CFG_MSRCADDR_OFFSET                                   0x8 /*!< Source Data Base Address Register */
#define UDMA_CH0_CFG_MDSTADDR_OFFSET                                   0xc /*!< Destination Data Base Address Register */
#define UDMA_CH0_CFG_MCTRL_OFFSET                                      0x10 /*!< Mem2mem control Register */
#define UDMA_CH0_CFG_MRPT_OFFSET                                       0x14 /*!< Transfer Repeat Number Register */
#define UDMA_CH0_CFG_MSIZE_OFFSET                                      0x18 /*!< Transfer Size Register */
#define UDMA_CH0_CFG_MSRCADDR_H_OFFSET                                 0x1c /*!< Source Data Base Address Register */
#define UDMA_CH0_CFG_MDSTADDR_H_OFFSET                                 0x20 /*!< Destination Data Base Address Register */
#define UDMA_CH1_CFG_MSRCADDR_OFFSET                                   0x24 /*!< Source Data Base Address Register */
#define UDMA_CH1_CFG_MDSTADDR_OFFSET                                   0x28 /*!< Destination Data Base Address Register */
#define UDMA_CH1_CFG_MCTRL_OFFSET                                      0x2c /*!< Mem2mem control Register */
#define UDMA_CH1_CFG_MRPT_OFFSET                                       0x30 /*!< Transfer Repeat Number Register */
#define UDMA_CH1_CFG_MSIZE_OFFSET                                      0x34 /*!< Transfer Size Register */
#define UDMA_CH1_CFG_MSRCADDR_H_OFFSET                                 0x38 /*!< Source Data Base Address Register */
#define UDMA_CH1_CFG_MDSTADDR_H_OFFSET                                 0x3c /*!< Destination Data Base Address Register */
#define UDMA_CH0_IRQ_EN_OFFSET                                         0x800 /*!< M2M Channel0 Interrupt Enable Register */
#define UDMA_CH0_IRQ_STAT_OFFSET                                       0x804 /*!< M2M Channel0 Interrupt Status Flag Register */
#define UDMA_CH0_IRQ_CLR_OFFSET                                        0x808 /*!< M2M Channel0 Interrupt Clear Status Register */
#define UDMA_CH1_IRQ_EN_OFFSET                                         0x80c /*!< M2M Channel1 Interrupt Enable Register */
#define UDMA_CH1_IRQ_STAT_OFFSET                                       0x810 /*!< M2M Channel1 Interrupt Status Flag Register */
#define UDMA_CH1_IRQ_CLR_OFFSET                                        0x814 /*!< M2M Channel1 Interrupt Clear Status Register */
#define UDMA_VERSION_OFFSET                                            0xffc /*!< uDMA Version Register */

 /* ===== UDMA CH0_CFG_MSRCADDR Register definition ===== */
#define UDMA_CH0_CFG_MSRCADDR_L_32_MASK                    BITS(0,31)                                   /*!< UDMA CH0 CFG MSRCADDR: L_32 Bit Mask */  
#define UDMA_CH0_CFG_MSRCADDR_L_32_OFS                     0U                                          /*!< UDMA CH0 CFG MSRCADDR: L_32 Bit Offset */
#define UDMA_CH0_CFG_MSRCADDR_L_32(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 CFG MSRCADDR: L_32 Bit Value */  
 
 /* ===== UDMA CH0_CFG_MDSTADDR Register definition ===== */
#define UDMA_CH0_CFG_MDSTADDR_L_32_MASK                    BITS(0,31)                                   /*!< UDMA CH0 CFG MDSTADDR: L_32 Bit Mask */  
#define UDMA_CH0_CFG_MDSTADDR_L_32_OFS                     0U                                          /*!< UDMA CH0 CFG MDSTADDR: L_32 Bit Offset */
#define UDMA_CH0_CFG_MDSTADDR_L_32(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 CFG MDSTADDR: L_32 Bit Value */  
 
 /* ===== UDMA CH0_CFG_MCTRL Register definition ===== */
#define UDMA_CH0_CFG_MCTRL_TRANS_EN                     BIT(0)                                      /*!< DMA transfer enable, asserting the bit will start a mem2mem DMA transfer, it can be cleared by software and the transfer will stop after current burst is finished. It can also be cleared by hardware when a complete transfer is finished or a transfer error response occurs in which case current transfer will be aborted. - 1'b0: disable: DMA transfer disable - 1'b1: enable: DMA transfer enable */
#define UDMA_CH0_CFG_MCTRL_TRANS_EN_OFS                 0U                                          /*!< UDMA CH0 CFG MCTRL: TRANS_EN Bit Offset */
#define UDMA_CH0_CFG_MCTRL_TRANS_EN_VAL(regval)             (BIT(0) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 CFG MCTRL: TRANS_EN Bit Value */  
#define UDMA_CH0_CFG_MCTRL_TRANS_EN_DISABLE                  0x0UL                                       /*!< DISABLE */
#define UDMA_CH0_CFG_MCTRL_TRANS_EN_ENABLE                   BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl trans_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid trans_en bit.
  * \retval 1 This is a valid trans_en bit.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_TRANS_EN(regval)               (\
                                         ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_EN_DISABLE             ) || \
                                         ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_EN_ENABLE              )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_TRANS_STAT                   BIT(1)                                      /*!< Transfer type status This bit accurately reflects the real channel status. Program bit 0 trans_en cannot guarantee the channel to be active since other higher priority channel could still have ongoing transfer and this channel can only be queued for further processing. Software can only read an active ‘1’ when this channel is actually doing the transfer. - 1'b0: inactive: cannot guarantee the channel to be active - 1'b1: active: this channel is actually doing the transfer */
#define UDMA_CH0_CFG_MCTRL_TRANS_STAT_INACTIVE               ((uint32_t)(0) << 1)                                         /*!< INACTIVE */
#define UDMA_CH0_CFG_MCTRL_TRANS_STAT_ACTIVE                 ((uint32_t)(1) << 1)                                         /*!< ACTIVE */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl trans_stat bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid trans_stat bit.
  * \retval 1 This is a valid trans_stat bit.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_TRANS_STAT(regval)             (\
                                       ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_STAT_INACTIVE            ) || \
                                       ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_STAT_ACTIVE              )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_TRANS_MODE_MASK              BITS(6,7)                                   /*!< UDMA CH0 CFG MCTRL: TRANS_MODE Bit Mask */  
#define UDMA_CH0_CFG_MCTRL_TRANS_MODE_OFS               6U                                          /*!< UDMA CH0 CFG MCTRL: TRANS_MODE Bit Offset */
#define UDMA_CH0_CFG_MCTRL_TRANS_MODE(regval)           (BITS(6,7) & ((uint32_t)(regval) << 6))        /*!< UDMA CH0 CFG MCTRL: TRANS_MODE Bit Value */  
#define UDMA_CH0_CFG_MCTRL_TRANS_MODE_NORMAL                 UDMA_CH0_CFG_MCTRL_TRANS_MODE(0)                                         /*!< NORMAL */
#define UDMA_CH0_CFG_MCTRL_TRANS_MODE_CONTINUOUS             UDMA_CH0_CFG_MCTRL_TRANS_MODE(1)                                         /*!< CONTINUOUS */
#define UDMA_CH0_CFG_MCTRL_TRANS_MODE_REPEAT                 UDMA_CH0_CFG_MCTRL_TRANS_MODE(2)                                         /*!< REPEAT */
#define UDMA_CH0_CFG_MCTRL_TRANS_MODE_RESERVED               UDMA_CH0_CFG_MCTRL_TRANS_MODE(3)                                         /*!< RESERVED */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl trans_mode bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid trans_mode bits.
  * \retval 1 This is a valid trans_mode bits.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_TRANS_MODE(regval)             (\
                                       ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_MODE_NORMAL              ) || \
                                       ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_MODE_CONTINUOUS          ) || \
                                       ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_MODE_REPEAT              ) || \
                                       ((regval) == UDMA_CH0_CFG_MCTRL_TRANS_MODE_RESERVED            )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_PRIORITY_MASK                BITS(8,9)                                   /*!< UDMA CH0 CFG MCTRL: PRIORITY Bit Mask */  
#define UDMA_CH0_CFG_MCTRL_PRIORITY_OFS                 8U                                          /*!< UDMA CH0 CFG MCTRL: PRIORITY Bit Offset */
#define UDMA_CH0_CFG_MCTRL_PRIORITY(regval)             (BITS(8,9) & ((uint32_t)(regval) << 8))        /*!< UDMA CH0 CFG MCTRL: PRIORITY Bit Value */  
#define UDMA_CH0_CFG_MCTRL_PRIORITY_LOW                      UDMA_CH0_CFG_MCTRL_PRIORITY(0)                                         /*!< LOW */
#define UDMA_CH0_CFG_MCTRL_PRIORITY_MED                      UDMA_CH0_CFG_MCTRL_PRIORITY(1)                                         /*!< MED */
#define UDMA_CH0_CFG_MCTRL_PRIORITY_HIGH                     UDMA_CH0_CFG_MCTRL_PRIORITY(2)                                         /*!< HIGH */
#define UDMA_CH0_CFG_MCTRL_PRIORITY_ULTRA_HIGH               UDMA_CH0_CFG_MCTRL_PRIORITY(3)                                         /*!< ULTRA_HIGH */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl priority bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid priority bits.
  * \retval 1 This is a valid priority bits.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_PRIORITY(regval)               (\
                                         ((regval) == UDMA_CH0_CFG_MCTRL_PRIORITY_LOW                 ) || \
                                         ((regval) == UDMA_CH0_CFG_MCTRL_PRIORITY_MED                 ) || \
                                         ((regval) == UDMA_CH0_CFG_MCTRL_PRIORITY_HIGH                ) || \
                                         ((regval) == UDMA_CH0_CFG_MCTRL_PRIORITY_ULTRA_HIGH          )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_MDNA                         BIT(12)                                      /*!< Next address generation algorithm for transmitting data to destination memory - 1'b0: increasing: Increasing address mode - 1'b1: fixed: Fixed address In case fixed address is configured, start destination address is forced to be aligned */
#define UDMA_CH0_CFG_MCTRL_MDNA_OFS                     12U                                          /*!< UDMA CH0 CFG MCTRL: MDNA Bit Offset */
#define UDMA_CH0_CFG_MCTRL_MDNA_VAL(regval)                 (BIT(12) & ((uint32_t)(regval) << 12))        /*!< UDMA CH0 CFG MCTRL: MDNA Bit Value */  
#define UDMA_CH0_CFG_MCTRL_MDNA_INCREASING                   0x0UL                                       /*!< INCREASING */
#define UDMA_CH0_CFG_MCTRL_MDNA_FIXED                        BIT(12)                                                  /*!< FIXED */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl mdna bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid mdna bit.
  * \retval 1 This is a valid mdna bit.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_MDNA(regval)                   (\
                                             ((regval) == UDMA_CH0_CFG_MCTRL_MDNA_INCREASING          ) || \
                                             ((regval) == UDMA_CH0_CFG_MCTRL_MDNA_FIXED               )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_MSNA                         BIT(13)                                      /*!< Next address generation algorithm for fetching data from source memory - 1'b0: increasing: Increasing address mode - 1'b1: fixed: Fixed address In case fixed address is configured, start source address is forced to be aligned */
#define UDMA_CH0_CFG_MCTRL_MSNA_OFS                     13U                                          /*!< UDMA CH0 CFG MCTRL: MSNA Bit Offset */
#define UDMA_CH0_CFG_MCTRL_MSNA_VAL(regval)                 (BIT(13) & ((uint32_t)(regval) << 13))        /*!< UDMA CH0 CFG MCTRL: MSNA Bit Value */  
#define UDMA_CH0_CFG_MCTRL_MSNA_INCREASING                   0x0UL                                       /*!< INCREASING */
#define UDMA_CH0_CFG_MCTRL_MSNA_FIXED                        BIT(13)                                                  /*!< FIXED */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl msna bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid msna bit.
  * \retval 1 This is a valid msna bit.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_MSNA(regval)                   (\
                                             ((regval) == UDMA_CH0_CFG_MCTRL_MSNA_INCREASING          ) || \
                                             ((regval) == UDMA_CH0_CFG_MCTRL_MSNA_FIXED               )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_MDWIDTH_MASK                 BITS(16,18)                                   /*!< UDMA CH0 CFG MCTRL: MDWIDTH Bit Mask */  
#define UDMA_CH0_CFG_MCTRL_MDWIDTH_OFS                  16U                                          /*!< UDMA CH0 CFG MCTRL: MDWIDTH Bit Offset */
#define UDMA_CH0_CFG_MCTRL_MDWIDTH(regval)              (BITS(16,18) & ((uint32_t)(regval) << 16))        /*!< UDMA CH0 CFG MCTRL: MDWIDTH Bit Value */  
#define UDMA_CH0_CFG_MCTRL_MDWIDTH_8BIT                      UDMA_CH0_CFG_MCTRL_MDWIDTH(0)                                         /*!< 8BIT */
#define UDMA_CH0_CFG_MCTRL_MDWIDTH_16BIT                     UDMA_CH0_CFG_MCTRL_MDWIDTH(1)                                         /*!< 16BIT */
#define UDMA_CH0_CFG_MCTRL_MDWIDTH_32BIT                     UDMA_CH0_CFG_MCTRL_MDWIDTH(2)                                         /*!< 32BIT */
#define UDMA_CH0_CFG_MCTRL_MDWIDTH_64BIT                     UDMA_CH0_CFG_MCTRL_MDWIDTH(3)                                         /*!< 64BIT */
#define UDMA_CH0_CFG_MCTRL_MDWIDTH_128BIT                    UDMA_CH0_CFG_MCTRL_MDWIDTH(4)                                         /*!< 128BIT */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl mdwidth bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mdwidth bits.
  * \retval 1 This is a valid mdwidth bits.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_MDWIDTH(regval)                (\
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDWIDTH_8BIT                ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDWIDTH_16BIT               ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDWIDTH_32BIT               ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDWIDTH_64BIT               ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDWIDTH_128BIT              )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_MSWIDTH_MASK                 BITS(21,23)                                   /*!< UDMA CH0 CFG MCTRL: MSWIDTH Bit Mask */  
#define UDMA_CH0_CFG_MCTRL_MSWIDTH_OFS                  21U                                          /*!< UDMA CH0 CFG MCTRL: MSWIDTH Bit Offset */
#define UDMA_CH0_CFG_MCTRL_MSWIDTH(regval)              (BITS(21,23) & ((uint32_t)(regval) << 21))        /*!< UDMA CH0 CFG MCTRL: MSWIDTH Bit Value */  
#define UDMA_CH0_CFG_MCTRL_MSWIDTH_8BIT                      UDMA_CH0_CFG_MCTRL_MSWIDTH(0)                                         /*!< 8BIT */
#define UDMA_CH0_CFG_MCTRL_MSWIDTH_16BIT                     UDMA_CH0_CFG_MCTRL_MSWIDTH(1)                                         /*!< 16BIT */
#define UDMA_CH0_CFG_MCTRL_MSWIDTH_32BIT                     UDMA_CH0_CFG_MCTRL_MSWIDTH(2)                                         /*!< 32BIT */
#define UDMA_CH0_CFG_MCTRL_MSWIDTH_64BIT                     UDMA_CH0_CFG_MCTRL_MSWIDTH(3)                                         /*!< 64BIT */
#define UDMA_CH0_CFG_MCTRL_MSWIDTH_128BIT                    UDMA_CH0_CFG_MCTRL_MSWIDTH(4)                                         /*!< 128BIT */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl mswidth bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mswidth bits.
  * \retval 1 This is a valid mswidth bits.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_MSWIDTH(regval)                (\
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSWIDTH_8BIT                ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSWIDTH_16BIT               ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSWIDTH_32BIT               ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSWIDTH_64BIT               ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSWIDTH_128BIT              )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_MDBURST_MASK                 BITS(24,27)                                   /*!< UDMA CH0 CFG MCTRL: MDBURST Bit Mask */  
#define UDMA_CH0_CFG_MCTRL_MDBURST_OFS                  24U                                          /*!< UDMA CH0 CFG MCTRL: MDBURST Bit Offset */
#define UDMA_CH0_CFG_MCTRL_MDBURST(regval)              (BITS(24,27) & ((uint32_t)(regval) << 24))        /*!< UDMA CH0 CFG MCTRL: MDBURST Bit Value */  
#define UDMA_CH0_CFG_MCTRL_MDBURST_1_TRANSFER                UDMA_CH0_CFG_MCTRL_MDBURST(0)                                         /*!< 1_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MDBURST_2_TRANSFER                UDMA_CH0_CFG_MCTRL_MDBURST(1)                                         /*!< 2_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MDBURST_3_TRANSFER                UDMA_CH0_CFG_MCTRL_MDBURST(2)                                         /*!< 3_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MDBURST_4_TRANSFER                UDMA_CH0_CFG_MCTRL_MDBURST(3)                                         /*!< 4_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MDBURST_5_TRANSFER                UDMA_CH0_CFG_MCTRL_MDBURST(4)                                         /*!< 5_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MDBURST_6_TRANSFER                UDMA_CH0_CFG_MCTRL_MDBURST(5)                                         /*!< 6_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MDBURST_7_TRANSFER                UDMA_CH0_CFG_MCTRL_MDBURST(6)                                         /*!< 7_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MDBURST_16_TRANSFER               UDMA_CH0_CFG_MCTRL_MDBURST(15)                                         /*!< 16_TRANSFER */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl mdburst bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mdburst bits.
  * \retval 1 This is a valid mdburst bits.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_MDBURST(regval)                (\
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_1_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_2_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_3_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_4_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_5_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_6_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_7_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MDBURST_16_TRANSFER         )  \
                                                 )

#define UDMA_CH0_CFG_MCTRL_MSBURST_MASK                 BITS(28,31)                                   /*!< UDMA CH0 CFG MCTRL: MSBURST Bit Mask */  
#define UDMA_CH0_CFG_MCTRL_MSBURST_OFS                  28U                                          /*!< UDMA CH0 CFG MCTRL: MSBURST Bit Offset */
#define UDMA_CH0_CFG_MCTRL_MSBURST(regval)              (BITS(28,31) & ((uint32_t)(regval) << 28))        /*!< UDMA CH0 CFG MCTRL: MSBURST Bit Value */  
#define UDMA_CH0_CFG_MCTRL_MSBURST_1_TRANSFER                UDMA_CH0_CFG_MCTRL_MSBURST(0)                                         /*!< 1_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MSBURST_2_TRANSFER                UDMA_CH0_CFG_MCTRL_MSBURST(1)                                         /*!< 2_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MSBURST_3_TRANSFER                UDMA_CH0_CFG_MCTRL_MSBURST(2)                                         /*!< 3_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MSBURST_4_TRANSFER                UDMA_CH0_CFG_MCTRL_MSBURST(3)                                         /*!< 4_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MSBURST_5_TRANSFER                UDMA_CH0_CFG_MCTRL_MSBURST(4)                                         /*!< 5_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MSBURST_6_TRANSFER                UDMA_CH0_CFG_MCTRL_MSBURST(5)                                         /*!< 6_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MSBURST_7_TRANSFER                UDMA_CH0_CFG_MCTRL_MSBURST(6)                                         /*!< 7_TRANSFER */
#define UDMA_CH0_CFG_MCTRL_MSBURST_16_TRANSFER               UDMA_CH0_CFG_MCTRL_MSBURST(15)                                         /*!< 16_TRANSFER */

/**
  * \brief Check the UDMA dma_ch0_cfg_mctrl msburst bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid msburst bits.
  * \retval 1 This is a valid msburst bits.
  */
#define IS_UDMA_DMA_CH0_CFG_MCTRL_MSBURST(regval)                (\
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_1_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_2_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_3_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_4_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_5_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_6_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_7_TRANSFER          ) || \
                                          ((regval) == UDMA_CH0_CFG_MCTRL_MSBURST_16_TRANSFER         )  \
                                                 )

 /* ===== UDMA CH0_CFG_MRPT Register definition ===== */
#define UDMA_CH0_CFG_MRPT_TRANS_RPT_MASK               BITS(0,11)                                   /*!< UDMA CH0 CFG MRPT: TRANS_RPT Bit Mask */  
#define UDMA_CH0_CFG_MRPT_TRANS_RPT_OFS                0U                                          /*!< UDMA CH0 CFG MRPT: TRANS_RPT Bit Offset */
#define UDMA_CH0_CFG_MRPT_TRANS_RPT(regval)            (BITS(0,11) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 CFG MRPT: TRANS_RPT Bit Value */  
 
 /* ===== UDMA CH0_CFG_MSIZE Register definition ===== */
#define UDMA_CH0_CFG_MSIZE_TSIZE_MASK                   BITS(0,19)                                   /*!< UDMA CH0 CFG MSIZE: TSIZE Bit Mask */  
#define UDMA_CH0_CFG_MSIZE_TSIZE_OFS                    0U                                          /*!< UDMA CH0 CFG MSIZE: TSIZE Bit Offset */
#define UDMA_CH0_CFG_MSIZE_TSIZE(regval)                (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 CFG MSIZE: TSIZE Bit Value */  
 
 /* ===== UDMA CH0_CFG_MSRCADDR_H Register definition ===== */
#define UDMA_CH0_CFG_MSRCADDR_H_32_MASK                      BITS(0,5)                                   /*!< UDMA CH0 CFG MSRCADDR H: 32 Bit Mask */  
#define UDMA_CH0_CFG_MSRCADDR_H_32_OFS                       0U                                          /*!< UDMA CH0 CFG MSRCADDR H: 32 Bit Offset */
#define UDMA_CH0_CFG_MSRCADDR_H_32(regval)                   (BITS(0,5) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 CFG MSRCADDR H: 32 Bit Value */  
 
 /* ===== UDMA CH0_CFG_MDSTADDR_H Register definition ===== */
#define UDMA_CH0_CFG_MDSTADDR_H_32_MASK                      BITS(0,5)                                   /*!< UDMA CH0 CFG MDSTADDR H: 32 Bit Mask */  
#define UDMA_CH0_CFG_MDSTADDR_H_32_OFS                       0U                                          /*!< UDMA CH0 CFG MDSTADDR H: 32 Bit Offset */
#define UDMA_CH0_CFG_MDSTADDR_H_32(regval)                   (BITS(0,5) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 CFG MDSTADDR H: 32 Bit Value */  
 
 /* ===== UDMA CH1_CFG_MSRCADDR Register definition ===== */
#define UDMA_CH1_CFG_MSRCADDR_L_32_MASK                    BITS(0,31)                                   /*!< UDMA CH1 CFG MSRCADDR: L_32 Bit Mask */  
#define UDMA_CH1_CFG_MSRCADDR_L_32_OFS                     0U                                          /*!< UDMA CH1 CFG MSRCADDR: L_32 Bit Offset */
#define UDMA_CH1_CFG_MSRCADDR_L_32(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 CFG MSRCADDR: L_32 Bit Value */  
 
 /* ===== UDMA CH1_CFG_MDSTADDR Register definition ===== */
#define UDMA_CH1_CFG_MDSTADDR_L_32_MASK                    BITS(0,31)                                   /*!< UDMA CH1 CFG MDSTADDR: L_32 Bit Mask */  
#define UDMA_CH1_CFG_MDSTADDR_L_32_OFS                     0U                                          /*!< UDMA CH1 CFG MDSTADDR: L_32 Bit Offset */
#define UDMA_CH1_CFG_MDSTADDR_L_32(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 CFG MDSTADDR: L_32 Bit Value */  
 
 /* ===== UDMA CH1_CFG_MCTRL Register definition ===== */
#define UDMA_CH1_CFG_MCTRL_TRANS_EN                     BIT(0)                                      /*!< DMA transfer enable, asserting the bit will start a mem2mem DMA transfer, it can be cleared by software and the transfer will stop after current burst is finished. It can also be cleared by hardware when a complete transfer is finished or a transfer error response occurs in which case current transfer will be aborted. - 1'b0: disable: DMA transfer disable - 1'b1: enable: DMA transfer enable */
#define UDMA_CH1_CFG_MCTRL_TRANS_EN_OFS                 0U                                          /*!< UDMA CH1 CFG MCTRL: TRANS_EN Bit Offset */
#define UDMA_CH1_CFG_MCTRL_TRANS_EN_VAL(regval)             (BIT(0) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 CFG MCTRL: TRANS_EN Bit Value */  
#define UDMA_CH1_CFG_MCTRL_TRANS_EN_DISABLE                  0x0UL                                       /*!< DISABLE */
#define UDMA_CH1_CFG_MCTRL_TRANS_EN_ENABLE                   BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl trans_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid trans_en bit.
  * \retval 1 This is a valid trans_en bit.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_TRANS_EN(regval)               (\
                                         ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_EN_DISABLE             ) || \
                                         ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_EN_ENABLE              )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_TRANS_STAT                   BIT(1)                                      /*!< Transfer type status This bit accurately reflects the real channel status. Program bit 0 trans_en cannot guarantee the channel to be active since other higher priority channel could still have ongoing transfer and this channel can only be queued for further processing. Software can only read an active ‘1’ when this channel is actually doing the transfer. - 1'b0: inactive: cannot guarantee the channel to be active - 1'b1: active: this channel is actually doing the transfer */
#define UDMA_CH1_CFG_MCTRL_TRANS_STAT_INACTIVE               ((uint32_t)(0) << 1)                                         /*!< INACTIVE */
#define UDMA_CH1_CFG_MCTRL_TRANS_STAT_ACTIVE                 ((uint32_t)(1) << 1)                                         /*!< ACTIVE */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl trans_stat bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid trans_stat bit.
  * \retval 1 This is a valid trans_stat bit.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_TRANS_STAT(regval)             (\
                                       ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_STAT_INACTIVE            ) || \
                                       ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_STAT_ACTIVE              )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_TRANS_MODE_MASK              BITS(6,7)                                   /*!< UDMA CH1 CFG MCTRL: TRANS_MODE Bit Mask */  
#define UDMA_CH1_CFG_MCTRL_TRANS_MODE_OFS               6U                                          /*!< UDMA CH1 CFG MCTRL: TRANS_MODE Bit Offset */
#define UDMA_CH1_CFG_MCTRL_TRANS_MODE(regval)           (BITS(6,7) & ((uint32_t)(regval) << 6))        /*!< UDMA CH1 CFG MCTRL: TRANS_MODE Bit Value */  
#define UDMA_CH1_CFG_MCTRL_TRANS_MODE_NORMAL                 UDMA_CH1_CFG_MCTRL_TRANS_MODE(0)                                         /*!< NORMAL */
#define UDMA_CH1_CFG_MCTRL_TRANS_MODE_CONTINUOUS             UDMA_CH1_CFG_MCTRL_TRANS_MODE(1)                                         /*!< CONTINUOUS */
#define UDMA_CH1_CFG_MCTRL_TRANS_MODE_REPEAT                 UDMA_CH1_CFG_MCTRL_TRANS_MODE(2)                                         /*!< REPEAT */
#define UDMA_CH1_CFG_MCTRL_TRANS_MODE_RESERVED               UDMA_CH1_CFG_MCTRL_TRANS_MODE(3)                                         /*!< RESERVED */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl trans_mode bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid trans_mode bits.
  * \retval 1 This is a valid trans_mode bits.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_TRANS_MODE(regval)             (\
                                       ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_MODE_NORMAL              ) || \
                                       ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_MODE_CONTINUOUS          ) || \
                                       ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_MODE_REPEAT              ) || \
                                       ((regval) == UDMA_CH1_CFG_MCTRL_TRANS_MODE_RESERVED            )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_PRIORITY_MASK                BITS(8,9)                                   /*!< UDMA CH1 CFG MCTRL: PRIORITY Bit Mask */  
#define UDMA_CH1_CFG_MCTRL_PRIORITY_OFS                 8U                                          /*!< UDMA CH1 CFG MCTRL: PRIORITY Bit Offset */
#define UDMA_CH1_CFG_MCTRL_PRIORITY(regval)             (BITS(8,9) & ((uint32_t)(regval) << 8))        /*!< UDMA CH1 CFG MCTRL: PRIORITY Bit Value */  
#define UDMA_CH1_CFG_MCTRL_PRIORITY_LOW                      UDMA_CH1_CFG_MCTRL_PRIORITY(0)                                         /*!< LOW */
#define UDMA_CH1_CFG_MCTRL_PRIORITY_MED                      UDMA_CH1_CFG_MCTRL_PRIORITY(1)                                         /*!< MED */
#define UDMA_CH1_CFG_MCTRL_PRIORITY_HIGH                     UDMA_CH1_CFG_MCTRL_PRIORITY(2)                                         /*!< HIGH */
#define UDMA_CH1_CFG_MCTRL_PRIORITY_ULTRA_HIGH               UDMA_CH1_CFG_MCTRL_PRIORITY(3)                                         /*!< ULTRA_HIGH */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl priority bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid priority bits.
  * \retval 1 This is a valid priority bits.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_PRIORITY(regval)               (\
                                         ((regval) == UDMA_CH1_CFG_MCTRL_PRIORITY_LOW                 ) || \
                                         ((regval) == UDMA_CH1_CFG_MCTRL_PRIORITY_MED                 ) || \
                                         ((regval) == UDMA_CH1_CFG_MCTRL_PRIORITY_HIGH                ) || \
                                         ((regval) == UDMA_CH1_CFG_MCTRL_PRIORITY_ULTRA_HIGH          )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_MDNA                         BIT(12)                                      /*!< Next address generation algorithm for transmitting data to destination memory - 1'b0: increasing: Increasing address mode - 1'b1: fixed: Fixed address In case fixed address is configured, start destination address is forced to be aligned */
#define UDMA_CH1_CFG_MCTRL_MDNA_OFS                     12U                                          /*!< UDMA CH1 CFG MCTRL: MDNA Bit Offset */
#define UDMA_CH1_CFG_MCTRL_MDNA_VAL(regval)                 (BIT(12) & ((uint32_t)(regval) << 12))        /*!< UDMA CH1 CFG MCTRL: MDNA Bit Value */  
#define UDMA_CH1_CFG_MCTRL_MDNA_INCREASING                   0x0UL                                       /*!< INCREASING */
#define UDMA_CH1_CFG_MCTRL_MDNA_FIXED                        BIT(12)                                                  /*!< FIXED */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl mdna bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid mdna bit.
  * \retval 1 This is a valid mdna bit.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_MDNA(regval)                   (\
                                             ((regval) == UDMA_CH1_CFG_MCTRL_MDNA_INCREASING          ) || \
                                             ((regval) == UDMA_CH1_CFG_MCTRL_MDNA_FIXED               )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_MSNA                         BIT(13)                                      /*!< Next address generation algorithm for fetching data from source memory - 1'b0: increasing: Increasing address mode - 1'b1: fixed: Fixed address In case fixed address is configured, start source address is forced to be aligned */
#define UDMA_CH1_CFG_MCTRL_MSNA_OFS                     13U                                          /*!< UDMA CH1 CFG MCTRL: MSNA Bit Offset */
#define UDMA_CH1_CFG_MCTRL_MSNA_VAL(regval)                 (BIT(13) & ((uint32_t)(regval) << 13))        /*!< UDMA CH1 CFG MCTRL: MSNA Bit Value */  
#define UDMA_CH1_CFG_MCTRL_MSNA_INCREASING                   0x0UL                                       /*!< INCREASING */
#define UDMA_CH1_CFG_MCTRL_MSNA_FIXED                        BIT(13)                                                  /*!< FIXED */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl msna bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid msna bit.
  * \retval 1 This is a valid msna bit.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_MSNA(regval)                   (\
                                             ((regval) == UDMA_CH1_CFG_MCTRL_MSNA_INCREASING          ) || \
                                             ((regval) == UDMA_CH1_CFG_MCTRL_MSNA_FIXED               )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_MDWIDTH_MASK                 BITS(16,18)                                   /*!< UDMA CH1 CFG MCTRL: MDWIDTH Bit Mask */  
#define UDMA_CH1_CFG_MCTRL_MDWIDTH_OFS                  16U                                          /*!< UDMA CH1 CFG MCTRL: MDWIDTH Bit Offset */
#define UDMA_CH1_CFG_MCTRL_MDWIDTH(regval)              (BITS(16,18) & ((uint32_t)(regval) << 16))        /*!< UDMA CH1 CFG MCTRL: MDWIDTH Bit Value */  
#define UDMA_CH1_CFG_MCTRL_MDWIDTH_8BIT                      UDMA_CH1_CFG_MCTRL_MDWIDTH(0)                                         /*!< 8BIT */
#define UDMA_CH1_CFG_MCTRL_MDWIDTH_16BIT                     UDMA_CH1_CFG_MCTRL_MDWIDTH(1)                                         /*!< 16BIT */
#define UDMA_CH1_CFG_MCTRL_MDWIDTH_32BIT                     UDMA_CH1_CFG_MCTRL_MDWIDTH(2)                                         /*!< 32BIT */
#define UDMA_CH1_CFG_MCTRL_MDWIDTH_64BIT                     UDMA_CH1_CFG_MCTRL_MDWIDTH(3)                                         /*!< 64BIT */
#define UDMA_CH1_CFG_MCTRL_MDWIDTH_128BIT                    UDMA_CH1_CFG_MCTRL_MDWIDTH(4)                                         /*!< 128BIT */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl mdwidth bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mdwidth bits.
  * \retval 1 This is a valid mdwidth bits.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_MDWIDTH(regval)                (\
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDWIDTH_8BIT                ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDWIDTH_16BIT               ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDWIDTH_32BIT               ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDWIDTH_64BIT               ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDWIDTH_128BIT              )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_MSWIDTH_MASK                 BITS(21,23)                                   /*!< UDMA CH1 CFG MCTRL: MSWIDTH Bit Mask */  
#define UDMA_CH1_CFG_MCTRL_MSWIDTH_OFS                  21U                                          /*!< UDMA CH1 CFG MCTRL: MSWIDTH Bit Offset */
#define UDMA_CH1_CFG_MCTRL_MSWIDTH(regval)              (BITS(21,23) & ((uint32_t)(regval) << 21))        /*!< UDMA CH1 CFG MCTRL: MSWIDTH Bit Value */  
#define UDMA_CH1_CFG_MCTRL_MSWIDTH_8BIT                      UDMA_CH1_CFG_MCTRL_MSWIDTH(0)                                         /*!< 8BIT */
#define UDMA_CH1_CFG_MCTRL_MSWIDTH_16BIT                     UDMA_CH1_CFG_MCTRL_MSWIDTH(1)                                         /*!< 16BIT */
#define UDMA_CH1_CFG_MCTRL_MSWIDTH_32BIT                     UDMA_CH1_CFG_MCTRL_MSWIDTH(2)                                         /*!< 32BIT */
#define UDMA_CH1_CFG_MCTRL_MSWIDTH_64BIT                     UDMA_CH1_CFG_MCTRL_MSWIDTH(3)                                         /*!< 64BIT */
#define UDMA_CH1_CFG_MCTRL_MSWIDTH_128BIT                    UDMA_CH1_CFG_MCTRL_MSWIDTH(4)                                         /*!< 128BIT */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl mswidth bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mswidth bits.
  * \retval 1 This is a valid mswidth bits.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_MSWIDTH(regval)                (\
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSWIDTH_8BIT                ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSWIDTH_16BIT               ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSWIDTH_32BIT               ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSWIDTH_64BIT               ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSWIDTH_128BIT              )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_MDBURST_MASK                 BITS(24,27)                                   /*!< UDMA CH1 CFG MCTRL: MDBURST Bit Mask */  
#define UDMA_CH1_CFG_MCTRL_MDBURST_OFS                  24U                                          /*!< UDMA CH1 CFG MCTRL: MDBURST Bit Offset */
#define UDMA_CH1_CFG_MCTRL_MDBURST(regval)              (BITS(24,27) & ((uint32_t)(regval) << 24))        /*!< UDMA CH1 CFG MCTRL: MDBURST Bit Value */  
#define UDMA_CH1_CFG_MCTRL_MDBURST_1_TRANSFER                UDMA_CH1_CFG_MCTRL_MDBURST(0)                                         /*!< 1_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MDBURST_2_TRANSFER                UDMA_CH1_CFG_MCTRL_MDBURST(1)                                         /*!< 2_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MDBURST_3_TRANSFER                UDMA_CH1_CFG_MCTRL_MDBURST(2)                                         /*!< 3_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MDBURST_4_TRANSFER                UDMA_CH1_CFG_MCTRL_MDBURST(3)                                         /*!< 4_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MDBURST_5_TRANSFER                UDMA_CH1_CFG_MCTRL_MDBURST(4)                                         /*!< 5_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MDBURST_6_TRANSFER                UDMA_CH1_CFG_MCTRL_MDBURST(5)                                         /*!< 6_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MDBURST_7_TRANSFER                UDMA_CH1_CFG_MCTRL_MDBURST(6)                                         /*!< 7_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MDBURST_16_TRANSFER               UDMA_CH1_CFG_MCTRL_MDBURST(15)                                         /*!< 16_TRANSFER */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl mdburst bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mdburst bits.
  * \retval 1 This is a valid mdburst bits.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_MDBURST(regval)                (\
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_1_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_2_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_3_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_4_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_5_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_6_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_7_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MDBURST_16_TRANSFER         )  \
                                                 )

#define UDMA_CH1_CFG_MCTRL_MSBURST_MASK                 BITS(28,31)                                   /*!< UDMA CH1 CFG MCTRL: MSBURST Bit Mask */  
#define UDMA_CH1_CFG_MCTRL_MSBURST_OFS                  28U                                          /*!< UDMA CH1 CFG MCTRL: MSBURST Bit Offset */
#define UDMA_CH1_CFG_MCTRL_MSBURST(regval)              (BITS(28,31) & ((uint32_t)(regval) << 28))        /*!< UDMA CH1 CFG MCTRL: MSBURST Bit Value */  
#define UDMA_CH1_CFG_MCTRL_MSBURST_1_TRANSFER                UDMA_CH1_CFG_MCTRL_MSBURST(0)                                         /*!< 1_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MSBURST_2_TRANSFER                UDMA_CH1_CFG_MCTRL_MSBURST(1)                                         /*!< 2_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MSBURST_3_TRANSFER                UDMA_CH1_CFG_MCTRL_MSBURST(2)                                         /*!< 3_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MSBURST_4_TRANSFER                UDMA_CH1_CFG_MCTRL_MSBURST(3)                                         /*!< 4_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MSBURST_5_TRANSFER                UDMA_CH1_CFG_MCTRL_MSBURST(4)                                         /*!< 5_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MSBURST_6_TRANSFER                UDMA_CH1_CFG_MCTRL_MSBURST(5)                                         /*!< 6_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MSBURST_7_TRANSFER                UDMA_CH1_CFG_MCTRL_MSBURST(6)                                         /*!< 7_TRANSFER */
#define UDMA_CH1_CFG_MCTRL_MSBURST_16_TRANSFER               UDMA_CH1_CFG_MCTRL_MSBURST(15)                                         /*!< 16_TRANSFER */

/**
  * \brief Check the UDMA dma_ch1_cfg_mctrl msburst bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid msburst bits.
  * \retval 1 This is a valid msburst bits.
  */
#define IS_UDMA_DMA_CH1_CFG_MCTRL_MSBURST(regval)                (\
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_1_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_2_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_3_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_4_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_5_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_6_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_7_TRANSFER          ) || \
                                          ((regval) == UDMA_CH1_CFG_MCTRL_MSBURST_16_TRANSFER         )  \
                                                 )

 /* ===== UDMA CH1_CFG_MRPT Register definition ===== */
#define UDMA_CH1_CFG_MRPT_TRANS_RPT_MASK               BITS(0,11)                                   /*!< UDMA CH1 CFG MRPT: TRANS_RPT Bit Mask */  
#define UDMA_CH1_CFG_MRPT_TRANS_RPT_OFS                0U                                          /*!< UDMA CH1 CFG MRPT: TRANS_RPT Bit Offset */
#define UDMA_CH1_CFG_MRPT_TRANS_RPT(regval)            (BITS(0,11) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 CFG MRPT: TRANS_RPT Bit Value */  
 
 /* ===== UDMA CH1_CFG_MSIZE Register definition ===== */
#define UDMA_CH1_CFG_MSIZE_TSIZE_MASK                   BITS(0,19)                                   /*!< UDMA CH1 CFG MSIZE: TSIZE Bit Mask */  
#define UDMA_CH1_CFG_MSIZE_TSIZE_OFS                    0U                                          /*!< UDMA CH1 CFG MSIZE: TSIZE Bit Offset */
#define UDMA_CH1_CFG_MSIZE_TSIZE(regval)                (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 CFG MSIZE: TSIZE Bit Value */  
 
 /* ===== UDMA CH1_CFG_MSRCADDR_H Register definition ===== */
#define UDMA_CH1_CFG_MSRCADDR_H_32_MASK                      BITS(0,5)                                   /*!< UDMA CH1 CFG MSRCADDR H: 32 Bit Mask */  
#define UDMA_CH1_CFG_MSRCADDR_H_32_OFS                       0U                                          /*!< UDMA CH1 CFG MSRCADDR H: 32 Bit Offset */
#define UDMA_CH1_CFG_MSRCADDR_H_32(regval)                   (BITS(0,5) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 CFG MSRCADDR H: 32 Bit Value */  
 
 /* ===== UDMA CH1_CFG_MDSTADDR_H Register definition ===== */
#define UDMA_CH1_CFG_MDSTADDR_H_32_MASK                      BITS(0,5)                                   /*!< UDMA CH1 CFG MDSTADDR H: 32 Bit Mask */  
#define UDMA_CH1_CFG_MDSTADDR_H_32_OFS                       0U                                          /*!< UDMA CH1 CFG MDSTADDR H: 32 Bit Offset */
#define UDMA_CH1_CFG_MDSTADDR_H_32(regval)                   (BITS(0,5) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 CFG MDSTADDR H: 32 Bit Value */  
 
 /* ===== UDMA CH0_IRQ_EN Register definition ===== */
#define UDMA_CH0_IRQ_EN_FTRANS                       BIT(0)                                      /*!< interrupt enable for Channel0 full transfer */
#define UDMA_CH0_IRQ_EN_FTRANS_OFS                   0U                                          /*!< UDMA CH0 IRQ EN: FTRANS Bit Offset */
#define UDMA_CH0_IRQ_EN_FTRANS_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 IRQ EN: FTRANS Bit Value */  
#define UDMA_CH0_IRQ_EN_HTRANS                       BIT(1)                                      /*!< Interrupt enable for Channel0 half transfer */
#define UDMA_CH0_IRQ_EN_HTRANS_OFS                   1U                                          /*!< UDMA CH0 IRQ EN: HTRANS Bit Offset */
#define UDMA_CH0_IRQ_EN_HTRANS_VAL(regval)               (BIT(1) & ((uint32_t)(regval) << 1))        /*!< UDMA CH0 IRQ EN: HTRANS Bit Value */  
#define UDMA_CH0_IRQ_EN_RSP_ERR                      BIT(2)                                      /*!< Interrupt enable for Channel0 DMA access error */
#define UDMA_CH0_IRQ_EN_RSP_ERR_OFS                  2U                                          /*!< UDMA CH0 IRQ EN: RSP_ERR Bit Offset */
#define UDMA_CH0_IRQ_EN_RSP_ERR_VAL(regval)              (BIT(2) & ((uint32_t)(regval) << 2))        /*!< UDMA CH0 IRQ EN: RSP_ERR Bit Value */  
 
 /* ===== UDMA CH0_IRQ_STAT Register definition ===== */
#define UDMA_CH0_IRQ_STAT_FTRANS                       BIT(0)                                      /*!< interrupt irq status flag for Channel0 full transfer */
#define UDMA_CH0_IRQ_STAT_HTRANS                       BIT(1)                                      /*!< interrupt irq status flag for Channel0 half transfer */
#define UDMA_CH0_IRQ_STAT_RSP_ERR                      BIT(2)                                      /*!< interrupt irq status flag for Channel0 DMA access error */
 
 /* ===== UDMA CH0_IRQ_CLR Register definition ===== */
#define UDMA_CH0_IRQ_CLR_FTRANS                       BIT(0)                                      /*!< clear irq status flag for Channel0 full transfer */
#define UDMA_CH0_IRQ_CLR_FTRANS_OFS                   0U                                          /*!< UDMA CH0 IRQ CLR: FTRANS Bit Offset */
#define UDMA_CH0_IRQ_CLR_FTRANS_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< UDMA CH0 IRQ CLR: FTRANS Bit Value */  
#define UDMA_CH0_IRQ_CLR_HTRANS                       BIT(1)                                      /*!< clear irq status flag for Channel0 half transfer */
#define UDMA_CH0_IRQ_CLR_HTRANS_OFS                   1U                                          /*!< UDMA CH0 IRQ CLR: HTRANS Bit Offset */
#define UDMA_CH0_IRQ_CLR_HTRANS_VAL(regval)               (BIT(1) & ((uint32_t)(regval) << 1))        /*!< UDMA CH0 IRQ CLR: HTRANS Bit Value */  
#define UDMA_CH0_IRQ_CLR_RSP_ERR                      BIT(2)                                      /*!< clear irq status flag for Channel0 DMA access error */
#define UDMA_CH0_IRQ_CLR_RSP_ERR_OFS                  2U                                          /*!< UDMA CH0 IRQ CLR: RSP_ERR Bit Offset */
#define UDMA_CH0_IRQ_CLR_RSP_ERR_VAL(regval)              (BIT(2) & ((uint32_t)(regval) << 2))        /*!< UDMA CH0 IRQ CLR: RSP_ERR Bit Value */  
 
 /* ===== UDMA CH1_IRQ_EN Register definition ===== */
#define UDMA_CH1_IRQ_EN_FTRANS                       BIT(0)                                      /*!< interrupt enable for Channel1 full transfer */
#define UDMA_CH1_IRQ_EN_FTRANS_OFS                   0U                                          /*!< UDMA CH1 IRQ EN: FTRANS Bit Offset */
#define UDMA_CH1_IRQ_EN_FTRANS_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 IRQ EN: FTRANS Bit Value */  
#define UDMA_CH1_IRQ_EN_HTRANS                       BIT(1)                                      /*!< Interrupt enable for Channel1 half transfer */
#define UDMA_CH1_IRQ_EN_HTRANS_OFS                   1U                                          /*!< UDMA CH1 IRQ EN: HTRANS Bit Offset */
#define UDMA_CH1_IRQ_EN_HTRANS_VAL(regval)               (BIT(1) & ((uint32_t)(regval) << 1))        /*!< UDMA CH1 IRQ EN: HTRANS Bit Value */  
#define UDMA_CH1_IRQ_EN_RSP_ERR                      BIT(2)                                      /*!< Interrupt enable for Channel1 DMA access error */
#define UDMA_CH1_IRQ_EN_RSP_ERR_OFS                  2U                                          /*!< UDMA CH1 IRQ EN: RSP_ERR Bit Offset */
#define UDMA_CH1_IRQ_EN_RSP_ERR_VAL(regval)              (BIT(2) & ((uint32_t)(regval) << 2))        /*!< UDMA CH1 IRQ EN: RSP_ERR Bit Value */  
 
 /* ===== UDMA CH1_IRQ_STAT Register definition ===== */
#define UDMA_CH1_IRQ_STAT_FTRANS                       BIT(0)                                      /*!< interrupt irq status flag for Channel1 full transfer */
#define UDMA_CH1_IRQ_STAT_HTRANS                       BIT(1)                                      /*!< interrupt irq status flag for Channel1 half transfer */
#define UDMA_CH1_IRQ_STAT_RSP_ERR                      BIT(2)                                      /*!< interrupt irq status flag for Channel1 DMA access error */
 
 /* ===== UDMA CH1_IRQ_CLR Register definition ===== */
#define UDMA_CH1_IRQ_CLR_FTRANS                       BIT(0)                                      /*!< clear irq status flag for Channel1 full transfer */
#define UDMA_CH1_IRQ_CLR_FTRANS_OFS                   0U                                          /*!< UDMA CH1 IRQ CLR: FTRANS Bit Offset */
#define UDMA_CH1_IRQ_CLR_FTRANS_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< UDMA CH1 IRQ CLR: FTRANS Bit Value */  
#define UDMA_CH1_IRQ_CLR_HTRANS                       BIT(1)                                      /*!< clear irq status flag for Channel1 half transfer */
#define UDMA_CH1_IRQ_CLR_HTRANS_OFS                   1U                                          /*!< UDMA CH1 IRQ CLR: HTRANS Bit Offset */
#define UDMA_CH1_IRQ_CLR_HTRANS_VAL(regval)               (BIT(1) & ((uint32_t)(regval) << 1))        /*!< UDMA CH1 IRQ CLR: HTRANS Bit Value */  
#define UDMA_CH1_IRQ_CLR_RSP_ERR                      BIT(2)                                      /*!< clear irq status flag for Channel1 DMA access error */
#define UDMA_CH1_IRQ_CLR_RSP_ERR_OFS                  2U                                          /*!< UDMA CH1 IRQ CLR: RSP_ERR Bit Offset */
#define UDMA_CH1_IRQ_CLR_RSP_ERR_VAL(regval)              (BIT(2) & ((uint32_t)(regval) << 2))        /*!< UDMA CH1 IRQ CLR: RSP_ERR Bit Value */  
 
 /* ===== UDMA VERSION Register definition ===== */
#define UDMA_VERSION_VER                          BITS(0,31)                

/* ===== DMA exported functions ===== */

void UDMA_Init(UDMA_CHxCfg_TypeDef* DMAy_CHannelx, UDMA_InitTypeDef* UDMA_InitStruct);
void UDMA_StructInit(UDMA_InitTypeDef* UDMA_InitStruct);
void M2M_DMA_Cmd(UDMA_CHxCfg_TypeDef* DMAy_CHannelx, ControlStatus Status);
FlagStatus UDMA_GetITStatus(UDMA_CHx_IRQ_TypeDef * DMAChannelx, uint32_t UDMA_IT);
void UDMA_ClearITStatus(UDMA_CHx_IRQ_TypeDef* DMAChannelx, uint32_t UDMA_IT);
void UDMA_ITConfig(UDMA_CHx_IRQ_TypeDef* DMAChannelx, uint32_t UDMA_IE, ControlStatus Status);
void UDMA_RptConfig(UDMA_CHxCfg_TypeDef* DMAy_CHannelx, uint16_t RPTLoopNum);
void UDMA_BurstConfig(UDMA_CHxCfg_TypeDef* DMAy_CHannelx, ControlStatus Status, uint32_t Msbusrt, uint32_t Mdbusrt);
void UDMA_ClockGating(UDMA_CommonCfg_TypeDef* Dma, ControlStatus Status);
static inline void hal_compiler_barrier();
static inline void hal_write32(volatile void *addr, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* NUCLEI_DMA_H */
