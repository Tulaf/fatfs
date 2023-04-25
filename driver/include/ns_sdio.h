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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _NS_SDIO_H
#define _NS_SDIO_H

/*!
 * @file     ns_sdio.h
 * @brief    This file contains all the functions prototypes for the SDIO firmware
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ns.h"
#include "ns_p2m_udma.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_sdmmc.h"

#define SDMMC_CHECK_PATTERN                ((uint32_t)0x000001AA)
/* ===== SDIO_IP Register Definition===== */
typedef enum {
    SDIO_IP_TXFULL                 = 0x1 << 3,      /*!< SDIO IP: send fifo full */
    SDIO_IP_RXEMPTY                = 0x1 << 2,      /*!< SDIO IP: receive fifo empty */
    SDIO_IP_RXIRQ                  = 0x1 << 1,      /*!< SDIO IP: Receive watermark enable */
    SDIO_IP_TXIRQ                  = 0x1 << 0,      /*!< SDIO IP: Transmit watermark enable */
} SDIO_IntFlagTypedef;

/* ===== SDIO_IE Register Definition===== */
typedef enum {
    SDIO_IE_IRQ_CHECK_EN           = 0x1 << 8,    /*!< SDIO IE: sdio check interrupt enable */
    SDIO_IE_RXOVF_ERR              = 0x1 << 7,    /*!< SDIO IE: Rx ovf error interrupt enable */
    SDIO_IE_RXUDR_ERR              = 0x1 << 6,    /*!< SDIO IE: Rx udr error interrupt enable */
    SDIO_IE_TXOVF_ERR              = 0x1 << 5,    /*!< SDIO IE: Tx ovf error interrupt enable */
    SDIO_IE_TXUDR_ERR              = 0x1 << 4,    /*!< SDIO IE: Tx udr error interrupt enable */
    SDIO_IE_ERR_IRQ                = 0x1 << 3,    /*!< SDIO IE: Error interrupt enable */
    SDIO_IE_EOT_IRQ                = 0x1 << 2,    /*!< SDIO IE: End interrupt enable */
    SDIO_IE_RX_IRQ                 = 0x1 << 1,    /*!< SDIO IE: Receive watermark enable */
    SDIO_IE_TX_IRQ                 = 0x1 << 0,    /*!< SDIO IE: Transmit watermark enable */
} SDIO_IntTypedef;

/* ===== SDIO_DMA_IE Register Definition===== */
typedef enum {
    TX_RSP_ERR_IRQ_EN               = 0x1 << 5,    /*!< SDIO DMA IE: Tx interrupt enable for channel DMA access error */
    TX_HTRANS_IRQ_EN                = 0x1 << 4,    /*!< SDIO DMA IE: Tx interrupt enable for channel half transfer */
    TX_FTRANS_IRQ_EN                = 0x1 << 3,    /*!< SDIO DMA IE: Tx interrupt enable for Channel0 full transfer */
    RX_RSP_ERR_IRQ_EN               = 0x1 << 2,    /*!< SDIO DMA IE: Rx interrupt enable for channel DMA access error */
    RX_HTRANS_IRQ_EN                = 0x1 << 1,    /*!< SDIO DMA IE: Rx interrupt enable for channel half transfer */
    RX_FTRANS_IRQ_EN                = 0x1 << 0,    /*!< SDIO DMA IE: Rx interrupt enable for Channel0 full transfer */
} SDIO_DmaIntTypedef;

/* ===== SDIO_DMA_IE_STAT Register Definition===== */
typedef enum {
    TX_RSP_ERR_IRQ_STAT               = 0x1 << 5,    /*!< SDIO DMA IE STAT: Tx interrupt irq status flag for Channel0 DMA access error */
    TX_HTRANS_IRQ_STAT                = 0x1 << 4,    /*!< SDIO DMA IE STAT: Tx interrupt irq status flag for Channel0 half transfer */
    TX_FTRANS_IRQ_STAT                = 0x1 << 3,    /*!< SDIO DMA IE STAT: Tx interrupt irq status flag for Channel0 full transfer */
    RX_RSP_ERR_IRQ_STAT               = 0x1 << 2,    /*!< SDIO DMA IE STAT: Rx interrupt irq status flag for Channel0 DMA access error */
    RX_HTRANS_IRQ_STAT                = 0x1 << 1,    /*!< SDIO DMA IE STAT: Rx interrupt irq status flag for Channel0 half transfer */
    RX_FTRANS_IRQ_STAT                = 0x1 << 0,    /*!< SDIO DMA IE STAT: Rx interrupt irq status flag for Channel0 full transfer */
} SDIO_DmaIntStatTypedef;

/* ===== SDIO_DMA_IE_CLR Register Definition===== */
typedef enum {
    TX_RSP_ERR_IRQ_CLR               = 0x1 << 5,    /*!< SDIO DMA IE CLR: Tx clear irq status flag for Channel0 DMA access error */
    TX_HTRANS_IRQ_CLR                = 0x1 << 4,    /*!< SDIO DMA IE CLR: Tx clear irq status flag for Channel0 half transfer */
    TX_FTRANS_IRQ_CLR                = 0x1 << 3,    /*!< SDIO DMA IE CLR: Tx clear irq status flag for Channel0 full transfer */
    RX_RSP_ERR_IRQ_CLR               = 0x1 << 2,    /*!< SDIO DMA IE CLR: Rx clear irq status flag for Channel0 DMA access error */
    RX_HTRANS_IRQ_CLR                = 0x1 << 1,    /*!< SDIO DMA IE CLR: Rx clear irq status flag for Channel0 half transfer */
    RX_FTRANS_IRQ_CLR                = 0x1 << 0,    /*!< SDIO DMA IE CLR: Rx clear irq status flag for Channel0 full transfer */
} SDIO_DmaIntClrTypedef;

typedef struct
{
    uint32_t SDIO_Argument;  /*!< Specifies the SDIO command argument which is sent
                                to a card as part of a command message. If a command
                                contains an argument, it must be loaded into this register
                                before writing the command to the command register */
    uint32_t SDIO_No_Rsp;   /*!< Specifies the SDIO response type.
                                This parameter can be a value of @ref SDIO_Response_Type */
    uint32_t SDIO_No_Rsp_Len;
    uint32_t SDIO_CrcEn;
    uint32_t SDIO_Busy;     /*!< Specifies whether SDIO wait for interrupt request is enabled or disabled.
                                This parameter can be a value of @ref SDIO_Wait_Interrupt_State */
    uint32_t SDIO_Power_Up;
    uint32_t SDIO_CmdIndex; /*!< Specifies the SDIO command index. It must be lower than 0x40. */
} SDIO_CmdInitTypeDef;

typedef struct
{
    uint32_t Dma_en;
    uint32_t Rx_addr;
    uint32_t Rx_size;
    uint32_t Rx_en;
    uint32_t Rx_datasize;

    uint32_t Tx_addr;
    uint32_t Tx_size;
    uint32_t Tx_en;
    uint32_t Tx_datasize;
}SDIO_DmaCfgTypeDef;

typedef struct
{
    uint32_t Data_en;
    uint32_t Data_rwn;
    uint32_t Data_mode;
    uint32_t Block_num;
    uint32_t Block_size;
} SDIO_DataSetupTypeDef;

#define SDIO_FLAG_TXDAVL                    ((uint32_t)0x00100000)
#define SDIO_FLAG_RXDAVL                    ((uint32_t)0x00200000)
#define SDIO_FLAG_SDIOIT                    ((uint32_t)0x00400000)
#define SDIO_FLAG_CEATAEND                  ((uint32_t)0x00800000)

#define SD_POLLING_MODE    	0
#define SD_DMA_MODE    		1
#define SDIO_SEND_IF_COND               ((uint32_t)0x00000008)

#define SDIO_SDR_MODE 0
#define SDIO_DDR_MODE 1

#define SDIO_GET_STATUS(__SDIO__)   __SDIO__->STATUS
#define SDIO_GET_STATU_FLAG(__SDIO__,__FLAG__) (((__SDIO__)->STATUS & (__FLAG__)) == (__FLAG__))
#define SDIO_GET_IP_FLAG(__SDIO__,__IP__)  (((__SDIO__)->IP & (__IP__)) == (__IP__))

#define SDIO_RX_SADDR_OFFSET                                           0x0 /*!< uDMA RX SDIO buffer base address */
#define SDIO_RX_SIZE_OFFSET                                            0x4 /*!< uDMA RX SDIO buffer size configuration register */
#define SDIO_RX_CFG_OFFSET                                             0x8 /*!< uDMA RX SDIO stream configuration register */
#define SDIO_CR_OFFSET                                                 0xc /*!< SDIO control register */
#define SDIO_TX_SADDR_OFFSET                                           0x10 /*!< uDMA TX SDIO buffer base address configuration register */
#define SDIO_TX_SIZE_OFFSET                                            0x14 /*!< uDMA TX SDIO buffer size configuration register */
#define SDIO_TX_CFG_OFFSET                                             0x18 /*!< uDMA TX SDIO stream configuration register */
#define SDIO_VERSION_OFFSET                                            0x1c /*!< SDIO version 1.1.2 */
#define SDIO_CMD_OP_OFFSET                                             0x20 /*!< SDIO command */
#define SDIO_CMD_ARG_OFFSET                                            0x24 /*!< SDIO argument */
#define SDIO_DATA_SETUP_OFFSET                                         0x28 /*!< SDIO Data transfer setup */
#define SDIO_START_OFFSET                                              0x2c /*!< SDIO Start */
#define SDIO_RSP0_OFFSET                                               0x30 /*!< SDIO Response 0 */
#define SDIO_RSP1_OFFSET                                               0x34 /*!< SDIO Response 1 */
#define SDIO_RSP2_OFFSET                                               0x38 /*!< SDIO Response 2 */
#define SDIO_RSP3_OFFSET                                               0x3c /*!< SDIO Response 3 */
#define SDIO_CLK_DIV_OFFSET                                            0x40 /*!< SDIO Clock Divider */
#define SDIO_STATUS_OFFSET                                             0x44 /*!< SDIO STATUS register */
#define SDIO_STOP_CMD_OP_OFFSET                                        0x48 /*!< SDIO STOP command op */
#define SDIO_STOP_CMD_ARG_OFFSET                                       0x4c /*!< SDIO STOP command arg */
#define SDIO_DATA_TIMEOUT_OFFSET                                       0x50 /*!< SDIO data timeout delay counter register */
#define SDIO_CMD_POWERUP_OFFSET                                        0x54 /*!< SDIO command power up counter config */
#define SDIO_CMD_WAIT_RSP_OFFSET                                       0x58 /*!< SDIO wait respone counter congfig */
#define SDIO_CMD_WAIT_EOT_OFFSET                                       0x5c /*!< SDIO wait eot counter congfig */
#define SDIO_TX_DATA_OFFSET                                            0x60 /*!< SDIO send data */
#define SDIO_RX_DATA_OFFSET                                            0x64 /*!< SDIO receive data */
#define SDIO_TX_MARK_OFFSET                                            0x68 /*!< SDIO Tx FIFO watermark */
#define SDIO_RX_MARK_OFFSET                                            0x6c /*!< SDIO Rx FIFO watermark */
#define SDIO_IP_OFFSET                                                 0x70 /*!< SDIO interrupt pending */
#define SDIO_IE_OFFSET                                                 0x74 /*!< SDIO interrupt enable */
#define SDIO_SAMPLE_DDR_OFFSET                                         0x78 /*!< SDIO ddr sample select */
#define SDIO_DATA_TX_DELAY_OFFSET                                      0x84 /*!< SDIO data tx delay counter register */
#define SDIO_DATA_CRC_TOKEN_OFFSET                                     0x88 /*!< SDIO crc token timeout delay counter register */
#define SDIO_CRC_VALUE_OFFSET                                          0x8c /*!< SDIO cmd crc value  */
#define SDIO_STATUS1_OFFSET                                            0x90 /*!< SDIO STATUS1 register */
#define SDIO_STOP_OFFSET                                               0x94 /*!< SDIO Start */
#define SDIO_STOP_RSP0_OFFSET                                          0xa0 /*!< SDIO Stop Response 0 */
#define SDIO_STOP_RSP1_OFFSET                                          0xa4 /*!< SDIO Stop Response 1 */
#define SDIO_STOP_RSP2_OFFSET                                          0xa8 /*!< SDIO Stop Response 2 */
#define SDIO_STOP_RSP3_OFFSET                                          0xac /*!< SDIO Stop Response 3 */

 /* ===== SDIO RX_SADDR Register definition ===== */
#define SDIO_RX_SADDR_ADDRESS_MASK                 BITS(0,31)                                   /*!< SDIO RX SADDR: ADDRESS Bit Mask */  
#define SDIO_RX_SADDR_ADDRESS_OFS                  0U                                          /*!< SDIO RX SADDR: ADDRESS Bit Offset */
#define SDIO_RX_SADDR_ADDRESS(regval)              (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO RX SADDR: ADDRESS Bit Value */  
 
 /* ===== SDIO RX_SIZE Register definition ===== */
#define SDIO_RX_SIZE_NUM_MASK                     BITS(0,19)                                   /*!< SDIO RX SIZE: NUM Bit Mask */  
#define SDIO_RX_SIZE_NUM_OFS                      0U                                          /*!< SDIO RX SIZE: NUM Bit Offset */
#define SDIO_RX_SIZE_NUM(regval)                  (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< SDIO RX SIZE: NUM Bit Value */  
 
 /* ===== SDIO RX_CFG Register definition ===== */
#define SDIO_RX_CFG_CONTINUOUS                   BIT(0)                                      /*!< RX channel continuous mode bitfield:  -1'b0: disable -1'b1: enable */
#define SDIO_RX_CFG_CONTINUOUS_OFS               0U                                          /*!< SDIO RX CFG: CONTINUOUS Bit Offset */
#define SDIO_RX_CFG_CONTINUOUS_VAL(regval)           (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO RX CFG: CONTINUOUS Bit Value */  
#define SDIO_RX_CFG_CONTINUOUS_DISABLE                0x0UL                                              /*!< DISABLE */
#define SDIO_RX_CFG_CONTINUOUS_ENABLE                 BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO rx_cfg continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid continuous bit.
  * \retval 1 This is a valid continuous bit.
  */
#define IS_SDIO_RX_CFG_CONTINUOUS(regval)             (\
                                       ((regval) == SDIO_RX_CFG_CONTINUOUS_DISABLE             ) || \
                                       ((regval) == SDIO_RX_CFG_CONTINUOUS_ENABLE              )  \
                                                 )

#define SDIO_RX_CFG_DATASIZE_MASK                BITS(1,2)                                   /*!< SDIO RX CFG: DATASIZE Bit Mask */  
#define SDIO_RX_CFG_DATASIZE_OFS                 1U                                          /*!< SDIO RX CFG: DATASIZE Bit Offset */
#define SDIO_RX_CFG_DATASIZE(regval)             (BITS(1,2) & ((uint32_t)(regval) << 1))        /*!< SDIO RX CFG: DATASIZE Bit Value */  
#define SDIO_RX_CFG_DATASIZE_BYTE                     SDIO_RX_CFG_DATASIZE(0)                                                /*!< BYTE */
#define SDIO_RX_CFG_DATASIZE_HALFWORD                 SDIO_RX_CFG_DATASIZE(1)                                                /*!< HALFWORD */
#define SDIO_RX_CFG_DATASIZE_WORD                     SDIO_RX_CFG_DATASIZE(2)                                                /*!< WORD */
#define SDIO_RX_CFG_DATASIZE_RESERVED                 SDIO_RX_CFG_DATASIZE(3)                                                /*!< RESERVED */

/**
  * \brief Check the SDIO rx_cfg datasize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid datasize bits.
  * \retval 1 This is a valid datasize bits.
  */
#define IS_SDIO_RX_CFG_DATASIZE(regval)               (\
                                         ((regval) == SDIO_RX_CFG_DATASIZE_BYTE                ) || \
                                         ((regval) == SDIO_RX_CFG_DATASIZE_HALFWORD            ) || \
                                         ((regval) == SDIO_RX_CFG_DATASIZE_WORD                ) || \
                                         ((regval) == SDIO_RX_CFG_DATASIZE_RESERVED            )  \
                                                 )

#define SDIO_RX_CFG_EN                           BIT(4)                                      /*!< RX channel enable and start transfer bitfield: -1'b0: disable -1'b1: enable and start the transfer */
#define SDIO_RX_CFG_EN_OFS                       4U                                          /*!< SDIO RX CFG: EN Bit Offset */
#define SDIO_RX_CFG_EN_VAL(regval)                   (BIT(4) & ((uint32_t)(regval) << 4))        /*!< SDIO RX CFG: EN Bit Value */  
#define SDIO_RX_CFG_EN_DISABLE                        0x0UL                                              /*!< DISABLE */
#define SDIO_RX_CFG_EN_ENABLE                         BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO rx_cfg en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid en bit.
  * \retval 1 This is a valid en bit.
  */
#define IS_SDIO_RX_CFG_EN(regval)                     (\
                                               ((regval) == SDIO_RX_CFG_EN_DISABLE             ) || \
                                               ((regval) == SDIO_RX_CFG_EN_ENABLE              )  \
                                                 )

#define SDIO_RX_CFG_PENDING                      BIT(5)                                      /*!< RX transfer pending in queue status flag: -1'b0: disable: no pending - no pending transfer in the queue -1'b1: enable: pending - pending transfer in the queue */
#define SDIO_RX_CFG_PENDING_DISABLE                   ((uint32_t)(0) << 5)                                                /*!< DISABLE */
#define SDIO_RX_CFG_PENDING_ENABLE                    ((uint32_t)(1) << 5)                                                /*!< ENABLE */

/**
  * \brief Check the SDIO rx_cfg pending bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid pending bit.
  * \retval 1 This is a valid pending bit.
  */
#define IS_SDIO_RX_CFG_PENDING(regval)                (\
                                          ((regval) == SDIO_RX_CFG_PENDING_DISABLE             ) || \
                                          ((regval) == SDIO_RX_CFG_PENDING_ENABLE              )  \
                                                 )

#define SDIO_RX_CFG_CLR                          BIT(6)                                      /*!< RX channel clear and stop transfer bitfield: -1'b0: disable -1'b1: enable: stop and clear - stop and clear the on-going transfer Note:The clear operation is not performed immediately.  You need to wait until TXEN is pulled down to clear it */
#define SDIO_RX_CFG_CLR_OFS                      6U                                          /*!< SDIO RX CFG: CLR Bit Offset */
#define SDIO_RX_CFG_CLR_VAL(regval)                  (BIT(6) & ((uint32_t)(regval) << 6))        /*!< SDIO RX CFG: CLR Bit Value */  

/**
  * \brief Check the SDIO rx_cfg clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid clr bit.
  * \retval 1 This is a valid clr bit.
  */
#define IS_SDIO_RX_CFG_CLR(regval)                    (\
                                                 )

 /* ===== SDIO CR Register definition ===== */
#define SDIO_CR_DMA                          BIT(0)                                      /*!< uDMA mode enable -1'b0: disable -1'b1: enable */
#define SDIO_CR_DMA_OFS                      0U                                          /*!< SDIO CR: DMA Bit Offset */
#define SDIO_CR_DMA_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO CR: DMA Bit Value */  
#define SDIO_CR_DMA_DISABLE                       0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_DMA_ENABLE                        BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr dma bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma bit.
  * \retval 1 This is a valid dma bit.
  */
#define IS_SDIO_CR_DMA(regval)                    (\
                                              ((regval) == SDIO_CR_DMA_DISABLE             ) || \
                                              ((regval) == SDIO_CR_DMA_ENABLE              )  \
                                                 )

#define SDIO_CR_DDR                          BIT(1)                                      /*!< Double Data Rate (DDR) mode enable -1'b0: disable -1'b1: enable */
#define SDIO_CR_DDR_OFS                      1U                                          /*!< SDIO CR: DDR Bit Offset */
#define SDIO_CR_DDR_VAL(regval)                  (BIT(1) & ((uint32_t)(regval) << 1))        /*!< SDIO CR: DDR Bit Value */  
#define SDIO_CR_DDR_DISABLE                       0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_DDR_ENABLE                        BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr ddr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ddr bit.
  * \retval 1 This is a valid ddr bit.
  */
#define IS_SDIO_CR_DDR(regval)                    (\
                                              ((regval) == SDIO_CR_DDR_DISABLE             ) || \
                                              ((regval) == SDIO_CR_DDR_ENABLE              )  \
                                                 )

#define SDIO_CR_AUTO_CMD12                   BIT(2)                                      /*!< This bit field is used to enable automatic transmission of cmd12 mode -1'b0: disable: turn off auto send cmd12 mode -1'b1: enable: Turn on auto send cmd12 mode */
#define SDIO_CR_AUTO_CMD12_OFS               2U                                          /*!< SDIO CR: AUTO_CMD12 Bit Offset */
#define SDIO_CR_AUTO_CMD12_VAL(regval)           (BIT(2) & ((uint32_t)(regval) << 2))        /*!< SDIO CR: AUTO_CMD12 Bit Value */  
#define SDIO_CR_AUTO_CMD12_DISABLE                0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_AUTO_CMD12_ENABLE                 BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr auto_cmd12 bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid auto_cmd12 bit.
  * \retval 1 This is a valid auto_cmd12 bit.
  */
#define IS_SDIO_CR_AUTO_CMD12(regval)             (\
                                       ((regval) == SDIO_CR_AUTO_CMD12_DISABLE             ) || \
                                       ((regval) == SDIO_CR_AUTO_CMD12_ENABLE              )  \
                                                 )

#define SDIO_CR_BLOCK                        BIT(3)                                      /*!< This bit field is used to enable block_num mode; -1'b0: disable: ignore block_num configuration, sending and receiving data enters the unlimited sending and receiving state, and can't stop until sending cmd12. -1'b1: enable: The number of blocks sent by SDIO controller is determined by block_num decision */
#define SDIO_CR_BLOCK_OFS                    3U                                          /*!< SDIO CR: BLOCK Bit Offset */
#define SDIO_CR_BLOCK_VAL(regval)                (BIT(3) & ((uint32_t)(regval) << 3))        /*!< SDIO CR: BLOCK Bit Value */  
#define SDIO_CR_BLOCK_DISABLE                     0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_BLOCK_ENABLE                      BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr block bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid block bit.
  * \retval 1 This is a valid block bit.
  */
#define IS_SDIO_CR_BLOCK(regval)                  (\
                                            ((regval) == SDIO_CR_BLOCK_DISABLE             ) || \
                                            ((regval) == SDIO_CR_BLOCK_ENABLE              )  \
                                                 )

#define SDIO_CR_DAT_LINE_BSY_CHECK           BIT(4)                                      /*!< This bit field is used to check data line busy when cmd is active */
#define SDIO_CR_DAT_LINE_BSY_CHECK_OFS       4U                                          /*!< SDIO CR: DAT_LINE_BSY_CHECK Bit Offset */
#define SDIO_CR_DAT_LINE_BSY_CHECK_VAL(regval)   (BIT(4) & ((uint32_t)(regval) << 4))        /*!< SDIO CR: DAT_LINE_BSY_CHECK Bit Value */  
#define SDIO_CR_CLK_STOP                     BIT(5)                                      /*!< If this bit domain is turned on, the SD card clock will be stopped each time the transfer is completed : -1'b0: disable: always open sdclk -1'b1: enable: stop sdclk */
#define SDIO_CR_CLK_STOP_OFS                 5U                                          /*!< SDIO CR: CLK_STOP Bit Offset */
#define SDIO_CR_CLK_STOP_VAL(regval)             (BIT(5) & ((uint32_t)(regval) << 5))        /*!< SDIO CR: CLK_STOP Bit Value */  
#define SDIO_CR_CLK_STOP_DISABLE                  0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_CLK_STOP_ENABLE                   BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr clk_stop bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid clk_stop bit.
  * \retval 1 This is a valid clk_stop bit.
  */
#define IS_SDIO_CR_CLK_STOP(regval)               (\
                                         ((regval) == SDIO_CR_CLK_STOP_DISABLE             ) || \
                                         ((regval) == SDIO_CR_CLK_STOP_ENABLE              )  \
                                                 )

#define SDIO_CR_HIGH_WIDTH_MODE              BIT(6)                                      /*!< High bandwidth mode is turned on, the data will be four bytes bandwidth throughput data, when there is no 4 bytes of aligned data, the data will only send and receive the number of valid bytes -1'b0: disable -1'b1: enable */
#define SDIO_CR_HIGH_WIDTH_MODE_OFS          6U                                          /*!< SDIO CR: HIGH_WIDTH_MODE Bit Offset */
#define SDIO_CR_HIGH_WIDTH_MODE_VAL(regval)      (BIT(6) & ((uint32_t)(regval) << 6))        /*!< SDIO CR: HIGH_WIDTH_MODE Bit Value */  
#define SDIO_CR_HIGH_WIDTH_MODE_DISABLE             0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_HIGH_WIDTH_MODE_ENABLE             BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr high_width_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid high_width_mode bit.
  * \retval 1 This is a valid high_width_mode bit.
  */
#define IS_SDIO_CR_HIGH_WIDTH_MODE(regval)        (\
                                  ((regval) == SDIO_CR_HIGH_WIDTH_MODE_DISABLE             ) || \
                                  ((regval) == SDIO_CR_HIGH_WIDTH_MODE_ENABLE              )  \
                                                 )

#define SDIO_CR_DATA_CRC_CHECK               BIT(7)                                      /*!< data crc check enable -1'b0: disable -1'b1: enable */
#define SDIO_CR_DATA_CRC_CHECK_OFS           7U                                          /*!< SDIO CR: DATA_CRC_CHECK Bit Offset */
#define SDIO_CR_DATA_CRC_CHECK_VAL(regval)       (BIT(7) & ((uint32_t)(regval) << 7))        /*!< SDIO CR: DATA_CRC_CHECK Bit Value */  
#define SDIO_CR_DATA_CRC_CHECK_DISABLE             0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_DATA_CRC_CHECK_ENABLE             BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr data_crc_check bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid data_crc_check bit.
  * \retval 1 This is a valid data_crc_check bit.
  */
#define IS_SDIO_CR_DATA_CRC_CHECK(regval)         (\
                                   ((regval) == SDIO_CR_DATA_CRC_CHECK_DISABLE             ) || \
                                   ((regval) == SDIO_CR_DATA_CRC_CHECK_ENABLE              )  \
                                                 )

#define SDIO_CR_DATA_BUS_SPEED               BIT(8)                                      /*!< data bus speed select -1'b0: disable -1'b1: enable */
#define SDIO_CR_DATA_BUS_SPEED_OFS           8U                                          /*!< SDIO CR: DATA_BUS_SPEED Bit Offset */
#define SDIO_CR_DATA_BUS_SPEED_VAL(regval)       (BIT(8) & ((uint32_t)(regval) << 8))        /*!< SDIO CR: DATA_BUS_SPEED Bit Value */  
#define SDIO_CR_DATA_BUS_SPEED_DISABLE             0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_DATA_BUS_SPEED_ENABLE             BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr data_bus_speed bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid data_bus_speed bit.
  * \retval 1 This is a valid data_bus_speed bit.
  */
#define IS_SDIO_CR_DATA_BUS_SPEED(regval)         (\
                                   ((regval) == SDIO_CR_DATA_BUS_SPEED_DISABLE             ) || \
                                   ((regval) == SDIO_CR_DATA_BUS_SPEED_ENABLE              )  \
                                                 )

#define SDIO_CR_IRQ_PERIOD_CHECK             BIT(9)                                      /*!< interrupt period check enable -1'b0: disable -1'b1: enable */
#define SDIO_CR_IRQ_PERIOD_CHECK_OFS         9U                                          /*!< SDIO CR: IRQ_PERIOD_CHECK Bit Offset */
#define SDIO_CR_IRQ_PERIOD_CHECK_VAL(regval)     (BIT(9) & ((uint32_t)(regval) << 9))        /*!< SDIO CR: IRQ_PERIOD_CHECK Bit Value */  
#define SDIO_CR_IRQ_PERIOD_CHECK_DISABLE             0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_IRQ_PERIOD_CHECK_ENABLE             BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cr irq_period_check bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid irq_period_check bit.
  * \retval 1 This is a valid irq_period_check bit.
  */
#define IS_SDIO_CR_IRQ_PERIOD_CHECK(regval)       (\
                                 ((regval) == SDIO_CR_IRQ_PERIOD_CHECK_DISABLE             ) || \
                                 ((regval) == SDIO_CR_IRQ_PERIOD_CHECK_ENABLE              )  \
                                                 )

#define SDIO_CR_STREAM_MODE                  BIT(10)                                      /*!< EMMC Stream mode  -1'b0: disable -1'b1: enable */
#define SDIO_CR_STREAM_MODE_OFS              10U                                          /*!< SDIO CR: STREAM_MODE Bit Offset */
#define SDIO_CR_STREAM_MODE_VAL(regval)          (BIT(10) & ((uint32_t)(regval) << 10))        /*!< SDIO CR: STREAM_MODE Bit Value */  
#define SDIO_CR_STREAM_MODE_DISABLE               0x0UL                                                  /*!< DISABLE */
#define SDIO_CR_STREAM_MODE_ENABLE                BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the SDIO cr stream_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid stream_mode bit.
  * \retval 1 This is a valid stream_mode bit.
  */
#define IS_SDIO_CR_STREAM_MODE(regval)            (\
                                      ((regval) == SDIO_CR_STREAM_MODE_DISABLE             ) || \
                                      ((regval) == SDIO_CR_STREAM_MODE_ENABLE              )  \
                                                 )

#define SDIO_CR_SLEEP_MODE                   BIT(11)                                      /*!< Sleep mode */
#define SDIO_CR_SLEEP_MODE_OFS               11U                                          /*!< SDIO CR: SLEEP_MODE Bit Offset */
#define SDIO_CR_SLEEP_MODE_VAL(regval)           (BIT(11) & ((uint32_t)(regval) << 11))        /*!< SDIO CR: SLEEP_MODE Bit Value */  
 
 /* ===== SDIO TX_SADDR Register definition ===== */
#define SDIO_TX_SADDR_ADDRESS_MASK                 BITS(0,31)                                   /*!< SDIO TX SADDR: ADDRESS Bit Mask */  
#define SDIO_TX_SADDR_ADDRESS_OFS                  0U                                          /*!< SDIO TX SADDR: ADDRESS Bit Offset */
#define SDIO_TX_SADDR_ADDRESS(regval)              (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO TX SADDR: ADDRESS Bit Value */  
 
 /* ===== SDIO TX_SIZE Register definition ===== */
#define SDIO_TX_SIZE_TX_SIZE_MASK                 BITS(0,19)                                   /*!< SDIO TX SIZE: TX_SIZE Bit Mask */  
#define SDIO_TX_SIZE_TX_SIZE_OFS                  0U                                          /*!< SDIO TX SIZE: TX_SIZE Bit Offset */
#define SDIO_TX_SIZE_TX_SIZE(regval)              (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< SDIO TX SIZE: TX_SIZE Bit Value */  
 
 /* ===== SDIO TX_CFG Register definition ===== */
#define SDIO_TX_CFG_CONTINUOUS                   BIT(0)                                      /*!< TX channel continuous mode bitfield:  -1'b0: disable -1'b1: enable */
#define SDIO_TX_CFG_CONTINUOUS_OFS               0U                                          /*!< SDIO TX CFG: CONTINUOUS Bit Offset */
#define SDIO_TX_CFG_CONTINUOUS_VAL(regval)           (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO TX CFG: CONTINUOUS Bit Value */  
#define SDIO_TX_CFG_CONTINUOUS_DISABLE                0x0UL                                              /*!< DISABLE */
#define SDIO_TX_CFG_CONTINUOUS_ENABLE                 BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO tx_cfg continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid continuous bit.
  * \retval 1 This is a valid continuous bit.
  */
#define IS_SDIO_TX_CFG_CONTINUOUS(regval)             (\
                                       ((regval) == SDIO_TX_CFG_CONTINUOUS_DISABLE             ) || \
                                       ((regval) == SDIO_TX_CFG_CONTINUOUS_ENABLE              )  \
                                                 )

#define SDIO_TX_CFG_DATASIZE_MASK                BITS(1,2)                                   /*!< SDIO TX CFG: DATASIZE Bit Mask */  
#define SDIO_TX_CFG_DATASIZE_OFS                 1U                                          /*!< SDIO TX CFG: DATASIZE Bit Offset */
#define SDIO_TX_CFG_DATASIZE(regval)             (BITS(1,2) & ((uint32_t)(regval) << 1))        /*!< SDIO TX CFG: DATASIZE Bit Value */  
#define SDIO_TX_CFG_DATASIZE_BYTE                     SDIO_TX_CFG_DATASIZE(0)                                                /*!< BYTE */
#define SDIO_TX_CFG_DATASIZE_HALFWORD                 SDIO_TX_CFG_DATASIZE(1)                                                /*!< HALFWORD */
#define SDIO_TX_CFG_DATASIZE_WORD                     SDIO_TX_CFG_DATASIZE(2)                                                /*!< WORD */
#define SDIO_TX_CFG_DATASIZE_RESERVED                 SDIO_TX_CFG_DATASIZE(3)                                                /*!< RESERVED */

/**
  * \brief Check the SDIO tx_cfg datasize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid datasize bits.
  * \retval 1 This is a valid datasize bits.
  */
#define IS_SDIO_TX_CFG_DATASIZE(regval)               (\
                                         ((regval) == SDIO_TX_CFG_DATASIZE_BYTE                ) || \
                                         ((regval) == SDIO_TX_CFG_DATASIZE_HALFWORD            ) || \
                                         ((regval) == SDIO_TX_CFG_DATASIZE_WORD                ) || \
                                         ((regval) == SDIO_TX_CFG_DATASIZE_RESERVED            )  \
                                                 )

#define SDIO_TX_CFG_EN                           BIT(4)                                      /*!< TX channel enable and start transfer bitfield: -1'b0: disable -1'b1: enable and start the transfer */
#define SDIO_TX_CFG_EN_OFS                       4U                                          /*!< SDIO TX CFG: EN Bit Offset */
#define SDIO_TX_CFG_EN_VAL(regval)                   (BIT(4) & ((uint32_t)(regval) << 4))        /*!< SDIO TX CFG: EN Bit Value */  
#define SDIO_TX_CFG_EN_DISABLE                        0x0UL                                              /*!< DISABLE */
#define SDIO_TX_CFG_EN_ENABLE                         BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO tx_cfg en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid en bit.
  * \retval 1 This is a valid en bit.
  */
#define IS_SDIO_TX_CFG_EN(regval)                     (\
                                               ((regval) == SDIO_TX_CFG_EN_DISABLE             ) || \
                                               ((regval) == SDIO_TX_CFG_EN_ENABLE              )  \
                                                 )

#define SDIO_TX_CFG_PENDING                      BIT(5)                                      /*!< TX transfer pending in queue status flag: -1'b0:  disable: no pending - no pending transfer in the queue -1'b1:  enable: pending - pending transfer in the queue */
#define SDIO_TX_CFG_PENDING_DISABLE                   ((uint32_t)(0) << 5)                                                /*!< DISABLE */
#define SDIO_TX_CFG_PENDING_ENABLE                    ((uint32_t)(1) << 5)                                                /*!< ENABLE */

/**
  * \brief Check the SDIO tx_cfg pending bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid pending bit.
  * \retval 1 This is a valid pending bit.
  */
#define IS_SDIO_TX_CFG_PENDING(regval)                (\
                                          ((regval) == SDIO_TX_CFG_PENDING_DISABLE             ) || \
                                          ((regval) == SDIO_TX_CFG_PENDING_ENABLE              )  \
                                                 )

#define SDIO_TX_CFG_CLR                          BIT(6)                                      /*!< TX channel clear and stop transfer bitfield: -1'b0: disable -1'b1:  enable: stop and clear - stop and clear the on-going transfer Note:The clear operation is not performed immediately.  You need to wait until TXEN is pulled down to clear it */
#define SDIO_TX_CFG_CLR_OFS                      6U                                          /*!< SDIO TX CFG: CLR Bit Offset */
#define SDIO_TX_CFG_CLR_VAL(regval)                  (BIT(6) & ((uint32_t)(regval) << 6))        /*!< SDIO TX CFG: CLR Bit Value */  

/**
  * \brief Check the SDIO tx_cfg clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid clr bit.
  * \retval 1 This is a valid clr bit.
  */
#define IS_SDIO_TX_CFG_CLR(regval)                    (\
                                                 )

 /* ===== SDIO VERSION Register definition ===== */
#define SDIO_VERSION_NUM                          BITS(0,31)                
 
 /* ===== SDIO CMD_OP Register definition ===== */
#define SDIO_CMD_OP_RSP                          BIT(0)                                      /*!< This bit field indicates whether to receive a response -1'b0: no response received -1'b1: yes: Receive 48bit response (default) */
#define SDIO_CMD_OP_RSP_OFS                      0U                                          /*!< SDIO CMD OP: RSP Bit Offset */
#define SDIO_CMD_OP_RSP_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO CMD OP: RSP Bit Value */  
#define SDIO_CMD_OP_RSP_NO                            0x0UL                                              /*!< NO */
#define SDIO_CMD_OP_RSP_YES                           BIT(0)                                                   /*!< YES */

/**
  * \brief Check the SDIO cmd_op rsp bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rsp bit.
  * \retval 1 This is a valid rsp bit.
  */
#define IS_SDIO_CMD_OP_RSP(regval)                    (\
                                              ((regval) == SDIO_CMD_OP_RSP_NO                  ) || \
                                              ((regval) == SDIO_CMD_OP_RSP_YES                 )  \
                                                 )

#define SDIO_CMD_OP_RSP_LEN                      BIT(1)                                      /*!< This bit field represents the received response length selection -1'b0: 48:  receive 48bit response -1'b1: 136: Receive 136bit response */
#define SDIO_CMD_OP_RSP_LEN_OFS                  1U                                          /*!< SDIO CMD OP: RSP_LEN Bit Offset */
#define SDIO_CMD_OP_RSP_LEN_VAL(regval)              (BIT(1) & ((uint32_t)(regval) << 1))        /*!< SDIO CMD OP: RSP_LEN Bit Value */  
#define SDIO_CMD_OP_RSP_LEN_48                        0x0UL                                              /*!< 48 */
#define SDIO_CMD_OP_RSP_LEN_136                       BIT(1)                                                   /*!< 136 */

/**
  * \brief Check the SDIO cmd_op rsp_len bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rsp_len bit.
  * \retval 1 This is a valid rsp_len bit.
  */
#define IS_SDIO_CMD_OP_RSP_LEN(regval)                (\
                                          ((regval) == SDIO_CMD_OP_RSP_LEN_48                  ) || \
                                          ((regval) == SDIO_CMD_OP_RSP_LEN_136                 )  \
                                                 )

#define SDIO_CMD_OP_CRC                          BIT(2)                                      /*!< This bit field indicates whether to enable CRC checking -1'b0: disable:  close CRC check -1'b1: enable:  Turn on CRC check */
#define SDIO_CMD_OP_CRC_OFS                      2U                                          /*!< SDIO CMD OP: CRC Bit Offset */
#define SDIO_CMD_OP_CRC_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< SDIO CMD OP: CRC Bit Value */  
#define SDIO_CMD_OP_CRC_DISABLE                       0x0UL                                              /*!< DISABLE */
#define SDIO_CMD_OP_CRC_ENABLE                        BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cmd_op crc bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid crc bit.
  * \retval 1 This is a valid crc bit.
  */
#define IS_SDIO_CMD_OP_CRC(regval)                    (\
                                              ((regval) == SDIO_CMD_OP_CRC_DISABLE             ) || \
                                              ((regval) == SDIO_CMD_OP_CRC_ENABLE              )  \
                                                 )

#define SDIO_CMD_OP_BUSY                         BIT(3)                                      /*!< This bit field indicates whether to enable busy check -1'b0: disable: off -1'b1: enable:  open */
#define SDIO_CMD_OP_BUSY_OFS                     3U                                          /*!< SDIO CMD OP: BUSY Bit Offset */
#define SDIO_CMD_OP_BUSY_VAL(regval)                 (BIT(3) & ((uint32_t)(regval) << 3))        /*!< SDIO CMD OP: BUSY Bit Value */  
#define SDIO_CMD_OP_BUSY_DISABLE                      0x0UL                                              /*!< DISABLE */
#define SDIO_CMD_OP_BUSY_ENABLE                       BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cmd_op busy bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid busy bit.
  * \retval 1 This is a valid busy bit.
  */
#define IS_SDIO_CMD_OP_BUSY(regval)                   (\
                                             ((regval) == SDIO_CMD_OP_BUSY_DISABLE             ) || \
                                             ((regval) == SDIO_CMD_OP_BUSY_ENABLE              )  \
                                                 )

#define SDIO_CMD_OP_POWER_UP                     BIT(4)                                      /*!< This bit field indicates whether to enable power on initialization -1'b0: disable: off -1'b1: enable:  open */
#define SDIO_CMD_OP_POWER_UP_OFS                 4U                                          /*!< SDIO CMD OP: POWER_UP Bit Offset */
#define SDIO_CMD_OP_POWER_UP_VAL(regval)             (BIT(4) & ((uint32_t)(regval) << 4))        /*!< SDIO CMD OP: POWER_UP Bit Value */  
#define SDIO_CMD_OP_POWER_UP_DISABLE                  0x0UL                                              /*!< DISABLE */
#define SDIO_CMD_OP_POWER_UP_ENABLE                   BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cmd_op power_up bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid power_up bit.
  * \retval 1 This is a valid power_up bit.
  */
#define IS_SDIO_CMD_OP_POWER_UP(regval)               (\
                                         ((regval) == SDIO_CMD_OP_POWER_UP_DISABLE             ) || \
                                         ((regval) == SDIO_CMD_OP_POWER_UP_ENABLE              )  \
                                                 )

#define SDIO_CMD_OP_CRC_CHECK                    BIT(5)                                      /*!< This bit field indicates whether to enable crc error check -1'b0: disable: off -1'b1: enable:  open */
#define SDIO_CMD_OP_CRC_CHECK_OFS                5U                                          /*!< SDIO CMD OP: CRC_CHECK Bit Offset */
#define SDIO_CMD_OP_CRC_CHECK_VAL(regval)            (BIT(5) & ((uint32_t)(regval) << 5))        /*!< SDIO CMD OP: CRC_CHECK Bit Value */  
#define SDIO_CMD_OP_CRC_CHECK_DISABLE                 0x0UL                                              /*!< DISABLE */
#define SDIO_CMD_OP_CRC_CHECK_ENABLE                  BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cmd_op crc_check bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid crc_check bit.
  * \retval 1 This is a valid crc_check bit.
  */
#define IS_SDIO_CMD_OP_CRC_CHECK(regval)              (\
                                        ((regval) == SDIO_CMD_OP_CRC_CHECK_DISABLE             ) || \
                                        ((regval) == SDIO_CMD_OP_CRC_CHECK_ENABLE              )  \
                                                 )

#define SDIO_CMD_OP_STOP_CMD                     BIT(6)                                      /*!< This bit field indicates whether cmd whether abort command -1'b0: disable: no stop command -1'b1: enable: stop command */
#define SDIO_CMD_OP_STOP_CMD_OFS                 6U                                          /*!< SDIO CMD OP: STOP_CMD Bit Offset */
#define SDIO_CMD_OP_STOP_CMD_VAL(regval)             (BIT(6) & ((uint32_t)(regval) << 6))        /*!< SDIO CMD OP: STOP_CMD Bit Value */  
#define SDIO_CMD_OP_STOP_CMD_DISABLE                  0x0UL                                              /*!< DISABLE */
#define SDIO_CMD_OP_STOP_CMD_ENABLE                   BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO cmd_op stop_cmd bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid stop_cmd bit.
  * \retval 1 This is a valid stop_cmd bit.
  */
#define IS_SDIO_CMD_OP_STOP_CMD(regval)               (\
                                         ((regval) == SDIO_CMD_OP_STOP_CMD_DISABLE             ) || \
                                         ((regval) == SDIO_CMD_OP_STOP_CMD_ENABLE              )  \
                                                 )

#define SDIO_CMD_OP_INDEX_MASK                   BITS(8,13)                                   /*!< SDIO CMD OP: INDEX Bit Mask */  
#define SDIO_CMD_OP_INDEX_OFS                    8U                                          /*!< SDIO CMD OP: INDEX Bit Offset */
#define SDIO_CMD_OP_INDEX(regval)                (BITS(8,13) & ((uint32_t)(regval) << 8))        /*!< SDIO CMD OP: INDEX Bit Value */  
 
 /* ===== SDIO CMD_ARG Register definition ===== */
#define SDIO_CMD_ARG_ARG_MASK                     BITS(0,31)                                   /*!< SDIO CMD ARG: ARG Bit Mask */  
#define SDIO_CMD_ARG_ARG_OFS                      0U                                          /*!< SDIO CMD ARG: ARG Bit Offset */
#define SDIO_CMD_ARG_ARG(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO CMD ARG: ARG Bit Value */  
 
 /* ===== SDIO DATA_SETUP Register definition ===== */
#define SDIO_DATA_SETUP_CHANNEL                      BIT(0)                                      /*!< SDIO data channel enable -1'b0: disable: off -1'b1: enable: open */
#define SDIO_DATA_SETUP_CHANNEL_OFS                  0U                                          /*!< SDIO DATA SETUP: CHANNEL Bit Offset */
#define SDIO_DATA_SETUP_CHANNEL_VAL(regval)              (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO DATA SETUP: CHANNEL Bit Value */  
#define SDIO_DATA_SETUP_CHANNEL_DISABLE                   0x0UL                                          /*!< DISABLE */
#define SDIO_DATA_SETUP_CHANNEL_ENABLE                    BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO data_setup channel bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid channel bit.
  * \retval 1 This is a valid channel bit.
  */
#define IS_SDIO_DATA_SETUP_CHANNEL(regval)                (\
                                          ((regval) == SDIO_DATA_SETUP_CHANNEL_DISABLE             ) || \
                                          ((regval) == SDIO_DATA_SETUP_CHANNEL_ENABLE              )  \
                                                 )

#define SDIO_DATA_SETUP_RWN                          BIT(1)                                      /*!< SDIO read / write enable -1'b0: write enable -1'b1: read enable */
#define SDIO_DATA_SETUP_RWN_OFS                      1U                                          /*!< SDIO DATA SETUP: RWN Bit Offset */
#define SDIO_DATA_SETUP_RWN_VAL(regval)                  (BIT(1) & ((uint32_t)(regval) << 1))        /*!< SDIO DATA SETUP: RWN Bit Value */  
#define SDIO_DATA_SETUP_RWN_WRITE                         0x0UL                                          /*!< WRITE */
#define SDIO_DATA_SETUP_RWN_READ                          BIT(1)                                                   /*!< READ */

/**
  * \brief Check the SDIO data_setup rwn bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rwn bit.
  * \retval 1 This is a valid rwn bit.
  */
#define IS_SDIO_DATA_SETUP_RWN(regval)                    (\
                                              ((regval) == SDIO_DATA_SETUP_RWN_WRITE               ) || \
                                              ((regval) == SDIO_DATA_SETUP_RWN_READ                )  \
                                                 )

#define SDIO_DATA_SETUP_MODE_MASK                    BITS(2,3)                                   /*!< SDIO DATA SETUP: MODE Bit Mask */  
#define SDIO_DATA_SETUP_MODE_OFS                     2U                                          /*!< SDIO DATA SETUP: MODE Bit Offset */
#define SDIO_DATA_SETUP_MODE(regval)                 (BITS(2,3) & ((uint32_t)(regval) << 2))        /*!< SDIO DATA SETUP: MODE Bit Value */  
#define SDIO_DATA_SETUP_MODE_SINGLE                       SDIO_DATA_SETUP_MODE(0)                                            /*!< SINGLE */
#define SDIO_DATA_SETUP_MODE_QUAD                         SDIO_DATA_SETUP_MODE(1)                                            /*!< QUAD */
#define SDIO_DATA_SETUP_MODE_OCTOL                        SDIO_DATA_SETUP_MODE(2)                                            /*!< OCTOL */

/**
  * \brief Check the SDIO data_setup mode bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mode bits.
  * \retval 1 This is a valid mode bits.
  */
#define IS_SDIO_DATA_SETUP_MODE(regval)                   (\
                                             ((regval) == SDIO_DATA_SETUP_MODE_SINGLE              ) || \
                                             ((regval) == SDIO_DATA_SETUP_MODE_QUAD                ) || \
                                             ((regval) == SDIO_DATA_SETUP_MODE_OCTOL               )  \
                                                 )

#define SDIO_DATA_SETUP_BLOCK_NUM_MASK               BITS(4,19)                                   /*!< SDIO DATA SETUP: BLOCK_NUM Bit Mask */  
#define SDIO_DATA_SETUP_BLOCK_NUM_OFS                4U                                          /*!< SDIO DATA SETUP: BLOCK_NUM Bit Offset */
#define SDIO_DATA_SETUP_BLOCK_NUM(regval)            (BITS(4,19) & ((uint32_t)(regval) << 4))        /*!< SDIO DATA SETUP: BLOCK_NUM Bit Value */  
#define SDIO_DATA_SETUP_BLOCK_SIZE_MASK              BITS(20,31)                                   /*!< SDIO DATA SETUP: BLOCK_SIZE Bit Mask */  
#define SDIO_DATA_SETUP_BLOCK_SIZE_OFS               20U                                          /*!< SDIO DATA SETUP: BLOCK_SIZE Bit Offset */
#define SDIO_DATA_SETUP_BLOCK_SIZE(regval)           (BITS(20,31) & ((uint32_t)(regval) << 20))        /*!< SDIO DATA SETUP: BLOCK_SIZE Bit Value */  
 
 /* ===== SDIO START Register definition ===== */
#define SDIO_START_ENABLE                       BIT(0)                                      /*!< Start transmission, write 1 to generate pulse and reset automatically */
#define SDIO_START_ENABLE_OFS                   0U                                          /*!< SDIO START: ENABLE Bit Offset */
#define SDIO_START_ENABLE_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO START: ENABLE Bit Value */  
 
 /* ===== SDIO RSP0 Register definition ===== */
#define SDIO_RSP0_NUM                          BITS(0,31)                
 
 /* ===== SDIO RSP1 Register definition ===== */
#define SDIO_RSP1_NUM                          BITS(0,31)                
 
 /* ===== SDIO RSP2 Register definition ===== */
#define SDIO_RSP2_NUM                          BITS(0,31)                
 
 /* ===== SDIO RSP3 Register definition ===== */
#define SDIO_RSP3_NUM                          BITS(0,31)                
 
 /* ===== SDIO CLK_DIV Register definition ===== */
#define SDIO_CLK_DIV_NUM_MASK                     BITS(0,31)                                   /*!< SDIO CLK DIV: NUM Bit Mask */  
#define SDIO_CLK_DIV_NUM_OFS                      0U                                          /*!< SDIO CLK DIV: NUM Bit Offset */
#define SDIO_CLK_DIV_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO CLK DIV: NUM Bit Value */  
 
 /* ===== SDIO STATUS Register definition ===== */
#define SDIO_STATUS_EOT                          BIT(0)                                      /*!< Transmission end flag, write 1 clear 0 */
#define SDIO_STATUS_EOT_OFS                      0U                                          /*!< SDIO STATUS: EOT Bit Offset */
#define SDIO_STATUS_EOT_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO STATUS: EOT Bit Value */  
#define SDIO_STATUS_ERR                          BIT(1)                                      /*!< Transmission error flag, write 1 clear 0 */
#define SDIO_STATUS_ERR_OFS                      1U                                          /*!< SDIO STATUS: ERR Bit Offset */
#define SDIO_STATUS_ERR_VAL(regval)                  (BIT(1) & ((uint32_t)(regval) << 1))        /*!< SDIO STATUS: ERR Bit Value */  
#define SDIO_STATUS_TXUDR_ERR                    BIT(2)                                      /*!< tx udr error flag , write 1 clear 0 */
#define SDIO_STATUS_TXUDR_ERR_OFS                2U                                          /*!< SDIO STATUS: TXUDR_ERR Bit Offset */
#define SDIO_STATUS_TXUDR_ERR_VAL(regval)            (BIT(2) & ((uint32_t)(regval) << 2))        /*!< SDIO STATUS: TXUDR_ERR Bit Value */  
#define SDIO_STATUS_TXOVF_ERR                    BIT(3)                                      /*!< tx ovf error flag , write 1 clear 0 */
#define SDIO_STATUS_TXOVF_ERR_OFS                3U                                          /*!< SDIO STATUS: TXOVF_ERR Bit Offset */
#define SDIO_STATUS_TXOVF_ERR_VAL(regval)            (BIT(3) & ((uint32_t)(regval) << 3))        /*!< SDIO STATUS: TXOVF_ERR Bit Value */  
#define SDIO_STATUS_RXUDR_ERR                    BIT(4)                                      /*!< rx udr error flag , write 1 clear 0 */
#define SDIO_STATUS_RXUDR_ERR_OFS                4U                                          /*!< SDIO STATUS: RXUDR_ERR Bit Offset */
#define SDIO_STATUS_RXUDR_ERR_VAL(regval)            (BIT(4) & ((uint32_t)(regval) << 4))        /*!< SDIO STATUS: RXUDR_ERR Bit Value */  
#define SDIO_STATUS_RXOVF_ERR                    BIT(5)                                      /*!< rx ovf error flag , write 1 clear 0 */
#define SDIO_STATUS_RXOVF_ERR_OFS                5U                                          /*!< SDIO STATUS: RXOVF_ERR Bit Offset */
#define SDIO_STATUS_RXOVF_ERR_VAL(regval)            (BIT(5) & ((uint32_t)(regval) << 5))        /*!< SDIO STATUS: RXOVF_ERR Bit Value */  
#define SDIO_STATUS_BUSY                         BIT(6)                                      /*!< sdio busy flag  */
#define SDIO_STATUS_CLEAR_FIFO_STAR              BIT(7)                                      /*!< sdio clear fifo  */
#define SDIO_STATUS_CLEAR_FIFO_STAR_OFS          7U                                          /*!< SDIO STATUS: CLEAR_FIFO_STAR Bit Offset */
#define SDIO_STATUS_CLEAR_FIFO_STAR_VAL(regval)      (BIT(7) & ((uint32_t)(regval) << 7))        /*!< SDIO STATUS: CLEAR_FIFO_STAR Bit Value */  
#define SDIO_STATUS_IRQ_CHECK                    BIT(8)                                      /*!< sdio interrupt check  */
#define SDIO_STATUS_IRQ_CHECK_OFS                8U                                          /*!< SDIO STATUS: IRQ_CHECK Bit Offset */
#define SDIO_STATUS_IRQ_CHECK_VAL(regval)            (BIT(8) & ((uint32_t)(regval) << 8))        /*!< SDIO STATUS: IRQ_CHECK Bit Value */  
#define SDIO_STATUS_BLOCK_DONE                   BIT(9)                                      /*!< sdio block done */
#define SDIO_STATUS_BLOCK_DONE_OFS               9U                                          /*!< SDIO STATUS: BLOCK_DONE Bit Offset */
#define SDIO_STATUS_BLOCK_DONE_VAL(regval)           (BIT(9) & ((uint32_t)(regval) << 9))        /*!< SDIO STATUS: BLOCK_DONE Bit Value */  
#define SDIO_STATUS_BUSY0_END                    BIT(10)                                      /*!< sdio sleep done */
#define SDIO_STATUS_BUSY0_END_OFS                10U                                          /*!< SDIO STATUS: BUSY0_END Bit Offset */
#define SDIO_STATUS_BUSY0_END_VAL(regval)            (BIT(10) & ((uint32_t)(regval) << 10))        /*!< SDIO STATUS: BUSY0_END Bit Value */  
#define SDIO_STATUS_CMD_ERR                      BITS(16,21)                
#define SDIO_STATUS_CMD_ERR_NO                        ((uint32_t)(0) << 16)                                                /*!< NO */
#define SDIO_STATUS_CMD_ERR_TIMEOUT                   ((uint32_t)(1) << 16)                                                /*!< TIMEOUT */
#define SDIO_STATUS_CMD_ERR_DIR                       ((uint32_t)(2) << 16)                                                /*!< DIR */
#define SDIO_STATUS_CMD_ERR_BUSY                      ((uint32_t)(4) << 16)                                                /*!< BUSY */
#define SDIO_STATUS_CMD_ERR_CRCERR                    ((uint32_t)(8) << 16)                                                /*!< CRCERR */

/**
  * \brief Check the SDIO status cmd_err bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cmd_err bits.
  * \retval 1 This is a valid cmd_err bits.
  */
#define IS_SDIO_STATUS_CMD_ERR(regval)                (\
                                          ((regval) == SDIO_STATUS_CMD_ERR_NO                  ) || \
                                          ((regval) == SDIO_STATUS_CMD_ERR_TIMEOUT             ) || \
                                          ((regval) == SDIO_STATUS_CMD_ERR_DIR                 ) || \
                                          ((regval) == SDIO_STATUS_CMD_ERR_BUSY                ) || \
                                          ((regval) == SDIO_STATUS_CMD_ERR_CRCERR              )  \
                                                 )

#define SDIO_STATUS_DATA_ERR                     BITS(24,29)                
#define SDIO_STATUS_DATA_ERR_NO                       ((uint32_t)(0) << 24)                                                /*!< NO */
#define SDIO_STATUS_DATA_ERR_TIMEOUT                  ((uint32_t)(1) << 24)                                                /*!< TIMEOUT */
#define SDIO_STATUS_DATA_ERR_BUSY                     ((uint32_t)(2) << 24)                                                /*!< BUSY */
#define SDIO_STATUS_DATA_ERR_CRCTIMEOUT               ((uint32_t)(3) << 24)                                                /*!< CRCTIMEOUT */
#define SDIO_STATUS_DATA_ERR_CRCERR                   ((uint32_t)(4) << 24)                                                /*!< CRCERR */

/**
  * \brief Check the SDIO status data_err bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid data_err bits.
  * \retval 1 This is a valid data_err bits.
  */
#define IS_SDIO_STATUS_DATA_ERR(regval)               (\
                                         ((regval) == SDIO_STATUS_DATA_ERR_NO                  ) || \
                                         ((regval) == SDIO_STATUS_DATA_ERR_TIMEOUT             ) || \
                                         ((regval) == SDIO_STATUS_DATA_ERR_BUSY                ) || \
                                         ((regval) == SDIO_STATUS_DATA_ERR_CRCTIMEOUT          ) || \
                                         ((regval) == SDIO_STATUS_DATA_ERR_CRCERR              )  \
                                                 )

#define SDIO_STATUS_DATA_RUNING                  BIT(30)                                      /*!< This bit field indicates whether the data channel is working 0: the data channel is not working 1:The data channel is working */
 
 /* ===== SDIO STOP_CMD_OP Register definition ===== */
#define SDIO_STOP_CMD_OP_TYPE_NO_RSP                  BIT(0)                                      /*!< This bit field indicates whether to receive a response -1'b0: no response received -1'b1: yes: Receive 48bit response (default) */
#define SDIO_STOP_CMD_OP_TYPE_NO_RSP_OFS              0U                                          /*!< SDIO STOP CMD OP: TYPE_NO_RSP Bit Offset */
#define SDIO_STOP_CMD_OP_TYPE_NO_RSP_VAL(regval)          (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO STOP CMD OP: TYPE_NO_RSP Bit Value */  
#define SDIO_STOP_CMD_OP_TYPE_NO_RSP_NO                    0x0UL                                         /*!< NO */
#define SDIO_STOP_CMD_OP_TYPE_NO_RSP_YES                   BIT(0)                                                   /*!< YES */

/**
  * \brief Check the SDIO stop_cmd_op type_no_rsp bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid type_no_rsp bit.
  * \retval 1 This is a valid type_no_rsp bit.
  */
#define IS_SDIO_STOP_CMD_OP_TYPE_NO_RSP(regval)            (\
                                      ((regval) == SDIO_STOP_CMD_OP_TYPE_NO_RSP_NO                  ) || \
                                      ((regval) == SDIO_STOP_CMD_OP_TYPE_NO_RSP_YES                 )  \
                                                 )

#define SDIO_STOP_CMD_OP_TYPE_136BIT                  BIT(1)                                      /*!< This bit field represents the received response length selection -1'b0: 48: receive 48bit response -1'b1: 136: Receive 136bit response */
#define SDIO_STOP_CMD_OP_TYPE_136BIT_OFS              1U                                          /*!< SDIO STOP CMD OP: TYPE_136BIT Bit Offset */
#define SDIO_STOP_CMD_OP_TYPE_136BIT_VAL(regval)          (BIT(1) & ((uint32_t)(regval) << 1))        /*!< SDIO STOP CMD OP: TYPE_136BIT Bit Value */  
#define SDIO_STOP_CMD_OP_TYPE_136BIT_48                    0x0UL                                         /*!< 48 */
#define SDIO_STOP_CMD_OP_TYPE_136BIT_136                   BIT(1)                                                   /*!< 136 */

/**
  * \brief Check the SDIO stop_cmd_op type_136bit bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid type_136bit bit.
  * \retval 1 This is a valid type_136bit bit.
  */
#define IS_SDIO_STOP_CMD_OP_TYPE_136BIT(regval)            (\
                                      ((regval) == SDIO_STOP_CMD_OP_TYPE_136BIT_48                  ) || \
                                      ((regval) == SDIO_STOP_CMD_OP_TYPE_136BIT_136                 )  \
                                                 )

#define SDIO_STOP_CMD_OP_TYPE_CRC                     BIT(2)                                      /*!< This bit field indicates whether to enable CRC checking -1'b0: disable: close CRC check -1'b1: enable: Turn on CRC check */
#define SDIO_STOP_CMD_OP_TYPE_CRC_OFS                 2U                                          /*!< SDIO STOP CMD OP: TYPE_CRC Bit Offset */
#define SDIO_STOP_CMD_OP_TYPE_CRC_VAL(regval)             (BIT(2) & ((uint32_t)(regval) << 2))        /*!< SDIO STOP CMD OP: TYPE_CRC Bit Value */  
#define SDIO_STOP_CMD_OP_TYPE_CRC_DISABLE                  0x0UL                                         /*!< DISABLE */
#define SDIO_STOP_CMD_OP_TYPE_CRC_ENABLE                   BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO stop_cmd_op type_crc bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid type_crc bit.
  * \retval 1 This is a valid type_crc bit.
  */
#define IS_SDIO_STOP_CMD_OP_TYPE_CRC(regval)               (\
                                         ((regval) == SDIO_STOP_CMD_OP_TYPE_CRC_DISABLE             ) || \
                                         ((regval) == SDIO_STOP_CMD_OP_TYPE_CRC_ENABLE              )  \
                                                 )

#define SDIO_STOP_CMD_OP_TYPE_BUSY                    BIT(3)                                      /*!< This bit field indicates whether to enable busy check -1'b0: disable: off -1'b1: enable: open */
#define SDIO_STOP_CMD_OP_TYPE_BUSY_OFS                3U                                          /*!< SDIO STOP CMD OP: TYPE_BUSY Bit Offset */
#define SDIO_STOP_CMD_OP_TYPE_BUSY_VAL(regval)            (BIT(3) & ((uint32_t)(regval) << 3))        /*!< SDIO STOP CMD OP: TYPE_BUSY Bit Value */  
#define SDIO_STOP_CMD_OP_TYPE_BUSY_DISABLE                 0x0UL                                         /*!< DISABLE */
#define SDIO_STOP_CMD_OP_TYPE_BUSY_ENABLE                  BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO stop_cmd_op type_busy bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid type_busy bit.
  * \retval 1 This is a valid type_busy bit.
  */
#define IS_SDIO_STOP_CMD_OP_TYPE_BUSY(regval)              (\
                                        ((regval) == SDIO_STOP_CMD_OP_TYPE_BUSY_DISABLE             ) || \
                                        ((regval) == SDIO_STOP_CMD_OP_TYPE_BUSY_ENABLE              )  \
                                                 )

#define SDIO_STOP_CMD_OP_TYPE_POWER_UP                BIT(4)                                      /*!< This bit field indicates whether to enable power on initialization -1'b0: disable: off -1'b1: enable: open */
#define SDIO_STOP_CMD_OP_TYPE_POWER_UP_OFS            4U                                          /*!< SDIO STOP CMD OP: TYPE_POWER_UP Bit Offset */
#define SDIO_STOP_CMD_OP_TYPE_POWER_UP_VAL(regval)        (BIT(4) & ((uint32_t)(regval) << 4))        /*!< SDIO STOP CMD OP: TYPE_POWER_UP Bit Value */  
#define SDIO_STOP_CMD_OP_TYPE_POWER_UP_DISABLE             0x0UL                                         /*!< DISABLE */
#define SDIO_STOP_CMD_OP_TYPE_POWER_UP_ENABLE              BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO stop_cmd_op type_power_up bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid type_power_up bit.
  * \retval 1 This is a valid type_power_up bit.
  */
#define IS_SDIO_STOP_CMD_OP_TYPE_POWER_UP(regval)          (\
                                    ((regval) == SDIO_STOP_CMD_OP_TYPE_POWER_UP_DISABLE             ) || \
                                    ((regval) == SDIO_STOP_CMD_OP_TYPE_POWER_UP_ENABLE              )  \
                                                 )

#define SDIO_STOP_CMD_OP_TYPE_CRC_CHECK               BIT(5)                                      /*!< This bit field indicates whether to enable crc error check -1'b0: disable: off -1'b1: enable: open */
#define SDIO_STOP_CMD_OP_TYPE_CRC_CHECK_OFS           5U                                          /*!< SDIO STOP CMD OP: TYPE_CRC_CHECK Bit Offset */
#define SDIO_STOP_CMD_OP_TYPE_CRC_CHECK_VAL(regval)       (BIT(5) & ((uint32_t)(regval) << 5))        /*!< SDIO STOP CMD OP: TYPE_CRC_CHECK Bit Value */  
#define SDIO_STOP_CMD_OP_TYPE_CRC_CHECK_DISABLE             0x0UL                                         /*!< DISABLE */
#define SDIO_STOP_CMD_OP_TYPE_CRC_CHECK_ENABLE             BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the SDIO stop_cmd_op type_crc_check bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid type_crc_check bit.
  * \retval 1 This is a valid type_crc_check bit.
  */
#define IS_SDIO_STOP_CMD_OP_TYPE_CRC_CHECK(regval)         (\
                                   ((regval) == SDIO_STOP_CMD_OP_TYPE_CRC_CHECK_DISABLE             ) || \
                                   ((regval) == SDIO_STOP_CMD_OP_TYPE_CRC_CHECK_ENABLE              )  \
                                                 )

#define SDIO_STOP_CMD_OP_INDEX_MASK                   BITS(8,13)                                   /*!< SDIO STOP CMD OP: INDEX Bit Mask */  
#define SDIO_STOP_CMD_OP_INDEX_OFS                    8U                                          /*!< SDIO STOP CMD OP: INDEX Bit Offset */
#define SDIO_STOP_CMD_OP_INDEX(regval)                (BITS(8,13) & ((uint32_t)(regval) << 8))        /*!< SDIO STOP CMD OP: INDEX Bit Value */  
 
 /* ===== SDIO STOP_CMD_ARG Register definition ===== */
#define SDIO_STOP_CMD_ARG_CMD_ARG_MASK                 BITS(0,31)                                   /*!< SDIO STOP CMD ARG: CMD_ARG Bit Mask */  
#define SDIO_STOP_CMD_ARG_CMD_ARG_OFS                  0U                                          /*!< SDIO STOP CMD ARG: CMD_ARG Bit Offset */
#define SDIO_STOP_CMD_ARG_CMD_ARG(regval)              (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO STOP CMD ARG: CMD_ARG Bit Value */  
 
 /* ===== SDIO DATA_TIMEOUT Register definition ===== */
#define SDIO_DATA_TIMEOUT_CNT_MASK                     BITS(0,31)                                   /*!< SDIO DATA TIMEOUT: CNT Bit Mask */  
#define SDIO_DATA_TIMEOUT_CNT_OFS                      0U                                          /*!< SDIO DATA TIMEOUT: CNT Bit Offset */
#define SDIO_DATA_TIMEOUT_CNT(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO DATA TIMEOUT: CNT Bit Value */  
 
 /* ===== SDIO CMD_POWERUP Register definition ===== */
#define SDIO_CMD_POWERUP_CNT_MASK                     BITS(0,15)                                   /*!< SDIO CMD POWERUP: CNT Bit Mask */  
#define SDIO_CMD_POWERUP_CNT_OFS                      0U                                          /*!< SDIO CMD POWERUP: CNT Bit Offset */
#define SDIO_CMD_POWERUP_CNT(regval)                  (BITS(0,15) & ((uint32_t)(regval) << 0))        /*!< SDIO CMD POWERUP: CNT Bit Value */  
 
 /* ===== SDIO CMD_WAIT_RSP Register definition ===== */
#define SDIO_CMD_WAIT_RSP_CNT_MASK                     BITS(0,15)                                   /*!< SDIO CMD WAIT RSP: CNT Bit Mask */  
#define SDIO_CMD_WAIT_RSP_CNT_OFS                      0U                                          /*!< SDIO CMD WAIT RSP: CNT Bit Offset */
#define SDIO_CMD_WAIT_RSP_CNT(regval)                  (BITS(0,15) & ((uint32_t)(regval) << 0))        /*!< SDIO CMD WAIT RSP: CNT Bit Value */  
 
 /* ===== SDIO CMD_WAIT_EOT Register definition ===== */
#define SDIO_CMD_WAIT_EOT_CNT_MASK                     BITS(0,15)                                   /*!< SDIO CMD WAIT EOT: CNT Bit Mask */  
#define SDIO_CMD_WAIT_EOT_CNT_OFS                      0U                                          /*!< SDIO CMD WAIT EOT: CNT Bit Offset */
#define SDIO_CMD_WAIT_EOT_CNT(regval)                  (BITS(0,15) & ((uint32_t)(regval) << 0))        /*!< SDIO CMD WAIT EOT: CNT Bit Value */  
 
 /* ===== SDIO TX_DATA Register definition ===== */
#define SDIO_TX_DATA_NUM_MASK                     BITS(0,31)                                   /*!< SDIO TX DATA: NUM Bit Mask */  
#define SDIO_TX_DATA_NUM_OFS                      0U                                          /*!< SDIO TX DATA: NUM Bit Offset */
#define SDIO_TX_DATA_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO TX DATA: NUM Bit Value */  
 
 /* ===== SDIO RX_DATA Register definition ===== */
#define SDIO_RX_DATA_NUM                          BITS(0,31)                
 
 /* ===== SDIO TX_MARK Register definition ===== */
#define SDIO_TX_MARK_NUM_MASK                     BITS(0,6)                                   /*!< SDIO TX MARK: NUM Bit Mask */  
#define SDIO_TX_MARK_NUM_OFS                      0U                                          /*!< SDIO TX MARK: NUM Bit Offset */
#define SDIO_TX_MARK_NUM(regval)                  (BITS(0,6) & ((uint32_t)(regval) << 0))        /*!< SDIO TX MARK: NUM Bit Value */  
 
 /* ===== SDIO RX_MARK Register definition ===== */
#define SDIO_RX_MARK_NUM_MASK                     BITS(0,6)                                   /*!< SDIO RX MARK: NUM Bit Mask */  
#define SDIO_RX_MARK_NUM_OFS                      0U                                          /*!< SDIO RX MARK: NUM Bit Offset */
#define SDIO_RX_MARK_NUM(regval)                  (BITS(0,6) & ((uint32_t)(regval) << 0))        /*!< SDIO RX MARK: NUM Bit Value */  
 
 /* ===== SDIO IP Register definition ===== */
#define SDIO_IP_TX_IRQ                       BIT(0)                                      /*!< Transmit watermark enable */
#define SDIO_IP_RX_IRQ                       BIT(1)                                      /*!< Receive watermark enable */
#define SDIO_IP_RX_EMPTY                     BIT(2)                                      /*!< receive fifo empty */
#define SDIO_IP_TX_FULL                      BIT(3)                                      /*!< send fifo full */
#define SDIO_IP_TX_EMPTY                     BIT(4)                                      /*!< send fifo empty */
#define SDIO_IP_RX_FULL                      BIT(5)                                      /*!< receive fifo full */
 
 /* ===== SDIO IE Register definition ===== */
#define SDIO_IE_TX_IRQ_EN                    BIT(0)                                      /*!< Transmit watermark enable */
#define SDIO_IE_TX_IRQ_EN_OFS                0U                                          /*!< SDIO IE: TX_IRQ_EN Bit Offset */
#define SDIO_IE_TX_IRQ_EN_VAL(regval)            (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO IE: TX_IRQ_EN Bit Value */  
#define SDIO_IE_RX_IRQ_EN                    BIT(1)                                      /*!< Receive watermark enable */
#define SDIO_IE_RX_IRQ_EN_OFS                1U                                          /*!< SDIO IE: RX_IRQ_EN Bit Offset */
#define SDIO_IE_RX_IRQ_EN_VAL(regval)            (BIT(1) & ((uint32_t)(regval) << 1))        /*!< SDIO IE: RX_IRQ_EN Bit Value */  
#define SDIO_IE_EOT_IRQ_EN                   BIT(2)                                      /*!< end interrupt enable */
#define SDIO_IE_EOT_IRQ_EN_OFS               2U                                          /*!< SDIO IE: EOT_IRQ_EN Bit Offset */
#define SDIO_IE_EOT_IRQ_EN_VAL(regval)           (BIT(2) & ((uint32_t)(regval) << 2))        /*!< SDIO IE: EOT_IRQ_EN Bit Value */  
#define SDIO_IE_ERR_IRQ_EN                   BIT(3)                                      /*!< error interrupt enable */
#define SDIO_IE_ERR_IRQ_EN_OFS               3U                                          /*!< SDIO IE: ERR_IRQ_EN Bit Offset */
#define SDIO_IE_ERR_IRQ_EN_VAL(regval)           (BIT(3) & ((uint32_t)(regval) << 3))        /*!< SDIO IE: ERR_IRQ_EN Bit Value */  
#define SDIO_IE_TXUDR_ERR_EN                 BIT(4)                                      /*!< tx udr error interrupt enable */
#define SDIO_IE_TXUDR_ERR_EN_OFS             4U                                          /*!< SDIO IE: TXUDR_ERR_EN Bit Offset */
#define SDIO_IE_TXUDR_ERR_EN_VAL(regval)         (BIT(4) & ((uint32_t)(regval) << 4))        /*!< SDIO IE: TXUDR_ERR_EN Bit Value */  
#define SDIO_IE_TXOVF_ERR_EN                 BIT(5)                                      /*!< tx ovf error interrupt enable */
#define SDIO_IE_TXOVF_ERR_EN_OFS             5U                                          /*!< SDIO IE: TXOVF_ERR_EN Bit Offset */
#define SDIO_IE_TXOVF_ERR_EN_VAL(regval)         (BIT(5) & ((uint32_t)(regval) << 5))        /*!< SDIO IE: TXOVF_ERR_EN Bit Value */  
#define SDIO_IE_RXUDR_ERR_EN                 BIT(6)                                      /*!< rx udr error interrupt enable */
#define SDIO_IE_RXUDR_ERR_EN_OFS             6U                                          /*!< SDIO IE: RXUDR_ERR_EN Bit Offset */
#define SDIO_IE_RXUDR_ERR_EN_VAL(regval)         (BIT(6) & ((uint32_t)(regval) << 6))        /*!< SDIO IE: RXUDR_ERR_EN Bit Value */  
#define SDIO_IE_RXOVF_ERR_EN                 BIT(7)                                      /*!< rx ovf error interrupt enable */
#define SDIO_IE_RXOVF_ERR_EN_OFS             7U                                          /*!< SDIO IE: RXOVF_ERR_EN Bit Offset */
#define SDIO_IE_RXOVF_ERR_EN_VAL(regval)         (BIT(7) & ((uint32_t)(regval) << 7))        /*!< SDIO IE: RXOVF_ERR_EN Bit Value */  
#define SDIO_IE_IRQ_CHECK_EN                 BIT(8)                                      /*!< sdio check interrupt enable */
#define SDIO_IE_IRQ_CHECK_EN_OFS             8U                                          /*!< SDIO IE: IRQ_CHECK_EN Bit Offset */
#define SDIO_IE_IRQ_CHECK_EN_VAL(regval)         (BIT(8) & ((uint32_t)(regval) << 8))        /*!< SDIO IE: IRQ_CHECK_EN Bit Value */  
#define SDIO_IE_BLOCK_DONE_EN                BIT(9)                                      /*!< sdio blcok done */
#define SDIO_IE_BLOCK_DONE_EN_OFS            9U                                          /*!< SDIO IE: BLOCK_DONE_EN Bit Offset */
#define SDIO_IE_BLOCK_DONE_EN_VAL(regval)        (BIT(9) & ((uint32_t)(regval) << 9))        /*!< SDIO IE: BLOCK_DONE_EN Bit Value */  
#define SDIO_IE_BUSY0_END_EN                 BIT(10)                                      /*!< sdio cmd5 done enable */
#define SDIO_IE_BUSY0_END_EN_OFS             10U                                          /*!< SDIO IE: BUSY0_END_EN Bit Offset */
#define SDIO_IE_BUSY0_END_EN_VAL(regval)         (BIT(10) & ((uint32_t)(regval) << 10))        /*!< SDIO IE: BUSY0_END_EN Bit Value */  
 
 /* ===== SDIO SAMPLE_DDR Register definition ===== */
#define SDIO_SAMPLE_DDR_DELAY_MASK                   BITS(0,31)                                   /*!< SDIO SAMPLE DDR: DELAY Bit Mask */  
#define SDIO_SAMPLE_DDR_DELAY_OFS                    0U                                          /*!< SDIO SAMPLE DDR: DELAY Bit Offset */
#define SDIO_SAMPLE_DDR_DELAY(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO SAMPLE DDR: DELAY Bit Value */  
 
 /* ===== SDIO DATA_TX_DELAY Register definition ===== */
#define SDIO_DATA_TX_DELAY_CNT_MASK                     BITS(0,31)                                   /*!< SDIO DATA TX DELAY: CNT Bit Mask */  
#define SDIO_DATA_TX_DELAY_CNT_OFS                      0U                                          /*!< SDIO DATA TX DELAY: CNT Bit Offset */
#define SDIO_DATA_TX_DELAY_CNT(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< SDIO DATA TX DELAY: CNT Bit Value */  
 
 /* ===== SDIO DATA_CRC_TOKEN Register definition ===== */
#define SDIO_DATA_CRC_TOKEN_CNT_MASK                     BITS(0,15)                                   /*!< SDIO DATA CRC TOKEN: CNT Bit Mask */  
#define SDIO_DATA_CRC_TOKEN_CNT_OFS                      0U                                          /*!< SDIO DATA CRC TOKEN: CNT Bit Offset */
#define SDIO_DATA_CRC_TOKEN_CNT(regval)                  (BITS(0,15) & ((uint32_t)(regval) << 0))        /*!< SDIO DATA CRC TOKEN: CNT Bit Value */  
 
 /* ===== SDIO CRC_VALUE Register definition ===== */
#define SDIO_CRC_VALUE_NORMAL_CMD                   BITS(0,6)                
#define SDIO_CRC_VALUE_STOP_CMD                     BITS(8,14)                
 
 /* ===== SDIO STATUS1 Register definition ===== */
#define SDIO_STATUS1_BLOCK_NUM                    BITS(0,15)                
 
 /* ===== SDIO STOP Register definition ===== */
#define SDIO_STOP_ENABLE                       BIT(0)                                      /*!< stop transmission, write 1 to generate pulse and reset automatically */
#define SDIO_STOP_ENABLE_OFS                   0U                                          /*!< SDIO STOP: ENABLE Bit Offset */
#define SDIO_STOP_ENABLE_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< SDIO STOP: ENABLE Bit Value */  
 
 /* ===== SDIO STOP_RSP0 Register definition ===== */
#define SDIO_STOP_RSP0_NUM                          BITS(0,31)                
 
 /* ===== SDIO STOP_RSP1 Register definition ===== */
#define SDIO_STOP_RSP1_NUM                          BITS(0,31)                
 
 /* ===== SDIO STOP_RSP2 Register definition ===== */
#define SDIO_STOP_RSP2_NUM                          BITS(0,31)                
 
 /* ===== SDIO STOP_RSP3 Register definition ===== */
#define SDIO_STOP_RSP3_NUM                          BITS(0,31)                

void SDIO_DmaCfgStructInit(SDIO_DmaCfgTypeDef *SDIO_DmaCfgStruct);
void SDIO_DMA_Config(SDIO_TypeDef *SDIOx, SDIO_DmaCfgTypeDef *SDIO_DmaCfgStruct);
void SDIO_CmdInitStructInit(SDIO_TypeDef *SDIOx, SDIO_CmdInitTypeDef *SDIO_CmdInitStruct);
void SDIO_SendCommand(SDIO_TypeDef *SDIOx, SDIO_CmdInitTypeDef *SDIO_CmdInitStruct);
void SDIO_DataSetupStructInit(SDIO_TypeDef *SDIOx, SDIO_DataSetupTypeDef *SDIO_DataSetupStruct);
void SDIO_DataSetup(SDIO_TypeDef *SDIOx, SDIO_DataSetupTypeDef *SDIO_DataSetupStruct);
void SDIO_Clock_Set( SDIO_TypeDef *SDIOx, uint32_t  clkdiv);
void SDIO_ClearDataSetup(SDIO_TypeDef *SDIOx );
void SDIO_SetDateTimeout(SDIO_TypeDef *SDIOx, uint32_t timeout);
void SDIO_SendData(SDIO_TypeDef *SDIOx, uint32_t data);
uint32_t SDIO_ReadData(SDIO_TypeDef *SDIOx);
uint32_t SDIO_GetFlagStatus(SDIO_TypeDef *SDIOx, uint32_t status);
void SDIO_ClearFlag(SDIO_TypeDef *SDIOx, uint32_t status);
uint32_t SDIO_GetIPStatus(SDIO_TypeDef *SDIOx, uint32_t status);
uint32_t SDIO_SetTxMark(SDIO_TypeDef *SDIOx, uint32_t depth);
uint32_t SDIO_SetRxMark(SDIO_TypeDef *SDIOx, uint32_t depth);
uint32_t SDIO_InterruptEn(SDIO_TypeDef *SDIOx, SDIO_IntTypedef interrupt, ControlStatus control);
uint32_t SDIO_DmaInterruptEn(UDMA_P2M_CHx_Irq_TypeDef *SDIO_DMA, SDIO_DmaIntTypedef interrupt, ControlStatus control);
uint32_t SDIO_DmaInterruptClr(UDMA_P2M_CHx_Irq_TypeDef *SDIO_DMA, SDIO_DmaIntClrTypedef interrupt_clr);
uint32_t SDIO_DmaGetIntStat(UDMA_P2M_CHx_Irq_TypeDef *SDIO_DMA, SDIO_DmaIntStatTypedef status);
void SDIO_StopClkEn(SDIO_TypeDef *SDIOx, ControlStatus control);
void SDIO_DmaEn(SDIO_TypeDef *SDIOx, ControlStatus control);
void SDIO_CfgDdrMode(SDIO_TypeDef *SDIOx, ControlStatus control);
#ifdef __cplusplus
}
#endif

#endif
/* _NS_SDIO_H */
