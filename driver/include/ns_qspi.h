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

#ifndef _NS_QSPI_H
#define _NS_QSPI_H
/*!
 * @file     ns_qspi.h
 * @brief    This file contains all the functions prototypes for the QSPI firmware
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ns.h"

#define QSPI_BUF_DEPTH 4
/**
  * \brief  QSPI Init Structure definition
  */
typedef struct {
    uint32_t SCKDIV;                                    /*!< Specifies the QSPI Clock Divider */

    uint32_t DevMode;                                   /*!< Specifies the QSPI Operating Mode : master/slave */

    uint32_t CSMode;                                    /*!< Specifies the QSPI Chip Select Mode */
    uint32_t CPOL;                                      /*!< Specifies the QSPI Serial Clock Steady State */
    uint32_t CPHA;                                      /*!< Specifies the QSPI Clock Active Edge for the Bit Capture */

    uint32_t SSM;                                       /*!< Specifies the Software slave management */

    uint32_t Tx_DMA;
    uint32_t Rx_DMA;
    uint32_t TxDmaCont;
    uint32_t RxDmaCont;
    uint32_t ProtolMode;                                /*!< Specifies the QSPI Protocol */
    uint32_t Endian;                                    /*!< Specifies the QSPI Big Endian or Little Endian */
    uint32_t DataSize;                                  /*!< Specifies the QSPI Data Size */
    uint32_t Force;                                     /*!< Specifies the QSPI OE Crtl when not Use Other Pad */
} QSPI_InitTypeDef;

/**
 * \brief Check the QSPI periphral parameters.
 * \param PERIPH: QSPI periphral to be checked.
 * \retval 0: This is not a valid QSPI periphral.
 * \retval 1: This is a valid QSPI periphral.
 */
#define IS_QSPI_ALL_PERIPH(PERIPH)                     (((PERIPH) == QSPI_XIP0) || ((PERIPH) == QSPI1) || ((PERIPH) == QSPI2) || ((PERIPH) == QSPI3))
#define IS_QSPI_TX_RX_WaterMark(WaterMark)             ((WaterMark) <= QSPI_BUF_DEPTH)                    /*!< QSPI WaterMark[4:0] */
#define IS_FUNCTIONAL_STATE(Status)                                     (((Status) == ENABLE) || ((Status) == DISABLE))
#define IS_QSPI_DATA(DATA)                             ((DATA) <= 0xFFFFFFFF)                             /*!< USART Transmit/Receive data[31:0] */

#define QSPI_SCKDIV_OFFSET                                             0x0 /*!< Serial clock divisor */
#define QSPI_SCKMODE_OFFSET                                            0x4 /*!< Serial clock mode (master/slave) */
#define QSPI_SCKSAMPLE_OFFSET                                          0x8 /*!< SPI data sampling divisor (ddr mode) */
#define QSPI_FORCE_OFFSET                                              0xc /*!< SPI oe crtl when not use other pad */
#define QSPI_CSID_OFFSET                                               0x10 /*!< Chip select ID */
#define QSPI_CSDEF_OFFSET                                              0x14 /*!< Chip select default */
#define QSPI_CSMODE_OFFSET                                             0x18 /*!< Chip select mode */
#define QSPI_VERSION_OFFSET                                            0x1c /*!< Indicate version 1.2.7 */
#define QSPI_DELAY0_OFFSET                                             0x28 /*!< SPI_DELAY0,Delay control 0 */
#define QSPI_DELAY1_OFFSET                                             0x2c /*!< SPI_DELAY1,Delay control 1 */
#define QSPI_FIFO_NUM_OFFSET                                           0x30 /*!< SPI fifo data valid number */
#define QSPI_TSIZE_OFFSET                                              0x34 /*!< TX SPI buffer size configuration register */
#define QSPI_RSIZE_OFFSET                                              0x38 /*!< RX SPI buffer size configuration register */
#define QSPI_FMT_OFFSET                                                0x40 /*!< Frame format(master/slave) */
#define QSPI_TXDATA_OFFSET                                             0x48 /*!< Tx FIFO Data(master/slave) */
#define QSPI_RXDATA_OFFSET                                             0x4c /*!< Rx FIFO data(master/slave) */
#define QSPI_TX_MARK_OFFSET                                            0x50 /*!< Tx FIFO watermark(master/slave) */
#define QSPI_RX_MARK_OFFSET                                            0x54 /*!< Rx FIFO watermark(master/slave) */
#define QSPI_IE_OFFSET                                                 0x70 /*!< SPI interrupt enable(master/slave) */
#define QSPI_IP_OFFSET                                                 0x74 /*!< SPI interrupt pending(master/slave) */
#define QSPI_STATUS_OFFSET                                             0x7c /*!< SPI busy status */
#define QSPI_RXEDGE_OFFSET                                             0x80 /*!< SPI RX sample edge ctrl */
#define QSPI_CR_OFFSET                                                 0x84 /*!< SPI control register */

 /* ===== QSPI SCKDIV Register definition ===== */
#define QSPI_SCKDIV_PRESCALER_MASK               BITS(0,11)                                   /*!< QSPI SCKDIV: PRESCALER Bit Mask */  
#define QSPI_SCKDIV_PRESCALER_OFS                0U                                          /*!< QSPI SCKDIV: PRESCALER Bit Offset */
#define QSPI_SCKDIV_PRESCALER(regval)            (BITS(0,11) & ((uint32_t)(regval) << 0))        /*!< QSPI SCKDIV: PRESCALER Bit Value */  
#define QSPI_SCKDIV_PRESCALER_2                       QSPI_SCKDIV_PRESCALER(0)                                                /*!< 2 */
#define QSPI_SCKDIV_PRESCALER_4                       QSPI_SCKDIV_PRESCALER(1)                                                /*!< 4 */
#define QSPI_SCKDIV_PRESCALER_8                       QSPI_SCKDIV_PRESCALER(3)                                                /*!< 8 */
#define QSPI_SCKDIV_PRESCALER_16                      QSPI_SCKDIV_PRESCALER(7)                                                /*!< 16 */
#define QSPI_SCKDIV_PRESCALER_32                      QSPI_SCKDIV_PRESCALER(15)                                                /*!< 32 */
#define QSPI_SCKDIV_PRESCALER_64                      QSPI_SCKDIV_PRESCALER(31)                                                /*!< 64 */

/**
  * \brief Check the QSPI spi_sckdiv prescaler bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid prescaler bits.
  * \retval 1 This is a valid prescaler bits.
  */
#define IS_QSPI_SPI_SCKDIV_PRESCALER(regval)              (\
                                        ((regval) == QSPI_SCKDIV_PRESCALER_2                   ) || \
                                        ((regval) == QSPI_SCKDIV_PRESCALER_4                   ) || \
                                        ((regval) == QSPI_SCKDIV_PRESCALER_8                   ) || \
                                        ((regval) == QSPI_SCKDIV_PRESCALER_16                  ) || \
                                        ((regval) == QSPI_SCKDIV_PRESCALER_32                  ) || \
                                        ((regval) == QSPI_SCKDIV_PRESCALER_64                  )  \
                                                 )

 /* ===== QSPI SCKMODE Register definition ===== */
#define QSPI_SCKMODE_CPHA                         BIT(0)                                      /*!< Serial clock phase - 1'b0: edge1 - 1'b1: edge2 */
#define QSPI_SCKMODE_CPHA_OFS                     0U                                          /*!< QSPI SCKMODE: CPHA Bit Offset */
#define QSPI_SCKMODE_CPHA_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI SCKMODE: CPHA Bit Value */  
#define QSPI_SCKMODE_CPHA_EDGE1                        0x0UL                                             /*!< EDGE1 */
#define QSPI_SCKMODE_CPHA_EDGE2                        BIT(0)                                                   /*!< EDGE2 */

/**
  * \brief Check the QSPI spi_sckmode cpha bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cpha bit.
  * \retval 1 This is a valid cpha bit.
  */
#define IS_QSPI_SPI_SCKMODE_CPHA(regval)                   (\
                                             ((regval) == QSPI_SCKMODE_CPHA_EDGE1               ) || \
                                             ((regval) == QSPI_SCKMODE_CPHA_EDGE2               )  \
                                                 )

#define QSPI_SCKMODE_CPOL                         BIT(1)                                      /*!< Serial clock polarity - 1'b0: low levle is idle state of SCK - 1'b1: high levle is idle state of SCK */
#define QSPI_SCKMODE_CPOL_OFS                     1U                                          /*!< QSPI SCKMODE: CPOL Bit Offset */
#define QSPI_SCKMODE_CPOL_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI SCKMODE: CPOL Bit Value */  
#define QSPI_SCKMODE_CPOL_LOW                          0x0UL                                             /*!< LOW */
#define QSPI_SCKMODE_CPOL_HIGH                         BIT(1)                                                   /*!< HIGH */

/**
  * \brief Check the QSPI spi_sckmode cpol bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cpol bit.
  * \retval 1 This is a valid cpol bit.
  */
#define IS_QSPI_SPI_SCKMODE_CPOL(regval)                   (\
                                             ((regval) == QSPI_SCKMODE_CPOL_LOW                 ) || \
                                             ((regval) == QSPI_SCKMODE_CPOL_HIGH                )  \
                                                 )

 /* ===== QSPI SCKSAMPLE Register definition ===== */
#define QSPI_SCKSAMPLE_SCKSAMPLE_MASK               BITS(0,11)                                   /*!< QSPI SCKSAMPLE: SCKSAMPLE Bit Mask */  
#define QSPI_SCKSAMPLE_SCKSAMPLE_OFS                0U                                          /*!< QSPI SCKSAMPLE: SCKSAMPLE Bit Offset */
#define QSPI_SCKSAMPLE_SCKSAMPLE(regval)            (BITS(0,11) & ((uint32_t)(regval) << 0))        /*!< QSPI SCKSAMPLE: SCKSAMPLE Bit Value */  
 
 /* ===== QSPI FORCE Register definition ===== */
#define QSPI_FORCE_EN                           BIT(0)                                      /*!< SPI oe crtl when not use other pad */
#define QSPI_FORCE_EN_OFS                       0U                                          /*!< QSPI FORCE: EN Bit Offset */
#define QSPI_FORCE_EN_VAL(regval)                   (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI FORCE: EN Bit Value */  
#define QSPI_FORCE_WP                           BIT(1)                                      /*!< SPI write protect ctrl */
#define QSPI_FORCE_WP_OFS                       1U                                          /*!< QSPI FORCE: WP Bit Offset */
#define QSPI_FORCE_WP_VAL(regval)                   (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI FORCE: WP Bit Value */  
 
 /* ===== QSPI CSID Register definition ===== */
#define QSPI_CSID_NUM_MASK                     BITS(0,2)                                   /*!< QSPI CSID: NUM Bit Mask */  
#define QSPI_CSID_NUM_OFS                      0U                                          /*!< QSPI CSID: NUM Bit Offset */
#define QSPI_CSID_NUM(regval)                  (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< QSPI CSID: NUM Bit Value */  
#define QSPI_CSID_NUM_CS                            QSPI_CSID_NUM(0)                                                  /*!< CS */
#define QSPI_CSID_NUM_CS0                           QSPI_CSID_NUM(1)                                                  /*!< CS0 */
#define QSPI_CSID_NUM_CS1                           QSPI_CSID_NUM(2)                                                  /*!< CS1 */
#define QSPI_CSID_NUM_CS2                           QSPI_CSID_NUM(3)                                                  /*!< CS2 */
#define QSPI_CSID_NUM_CS3                           QSPI_CSID_NUM(4)                                                  /*!< CS3 */

/**
  * \brief Check the QSPI spi_csid num bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid num bits.
  * \retval 1 This is a valid num bits.
  */
#define IS_QSPI_SPI_CSID_NUM(regval)                    (\
                                              ((regval) == QSPI_CSID_NUM_CS                  ) || \
                                              ((regval) == QSPI_CSID_NUM_CS0                 ) || \
                                              ((regval) == QSPI_CSID_NUM_CS1                 ) || \
                                              ((regval) == QSPI_CSID_NUM_CS2                 ) || \
                                              ((regval) == QSPI_CSID_NUM_CS3                 )  \
                                                 )

 /* ===== QSPI CSDEF Register definition ===== */
#define QSPI_CSDEF_WIDTH_MASK                   BITS(0,3)                                   /*!< QSPI CSDEF: WIDTH Bit Mask */  
#define QSPI_CSDEF_WIDTH_OFS                    0U                                          /*!< QSPI CSDEF: WIDTH Bit Offset */
#define QSPI_CSDEF_WIDTH(regval)                (BITS(0,3) & ((uint32_t)(regval) << 0))        /*!< QSPI CSDEF: WIDTH Bit Value */  
 
 /* ===== QSPI CSMODE Register definition ===== */
#define QSPI_CSMODE_MODE_MASK                    BITS(0,1)                                   /*!< QSPI CSMODE: MODE Bit Mask */  
#define QSPI_CSMODE_MODE_OFS                     0U                                          /*!< QSPI CSMODE: MODE Bit Offset */
#define QSPI_CSMODE_MODE(regval)                 (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< QSPI CSMODE: MODE Bit Value */  
#define QSPI_CSMODE_MODE_AUTO                         QSPI_CSMODE_MODE(0)                                                /*!< AUTO */
#define QSPI_CSMODE_MODE_HOLD                         QSPI_CSMODE_MODE(2)                                                /*!< HOLD */
#define QSPI_CSMODE_MODE_OFF                          QSPI_CSMODE_MODE(3)                                                /*!< OFF */

/**
  * \brief Check the QSPI spi_csmode mode bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mode bits.
  * \retval 1 This is a valid mode bits.
  */
#define IS_QSPI_SPI_CSMODE_MODE(regval)                   (\
                                             ((regval) == QSPI_CSMODE_MODE_AUTO                ) || \
                                             ((regval) == QSPI_CSMODE_MODE_HOLD                ) || \
                                             ((regval) == QSPI_CSMODE_MODE_OFF                 )  \
                                                 )

 /* ===== QSPI VERSION Register definition ===== */
#define QSPI_VERSION_NUM                          BITS(0,31)                
 
 /* ===== QSPI DELAY0 Register definition ===== */
#define QSPI_DELAY0_CSSCK_MASK                   BITS(0,7)                                   /*!< QSPI DELAY0: CSSCK Bit Mask */  
#define QSPI_DELAY0_CSSCK_OFS                    0U                                          /*!< QSPI DELAY0: CSSCK Bit Offset */
#define QSPI_DELAY0_CSSCK(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< QSPI DELAY0: CSSCK Bit Value */  
#define QSPI_DELAY0_SCKCS_MASK                   BITS(16,23)                                   /*!< QSPI DELAY0: SCKCS Bit Mask */  
#define QSPI_DELAY0_SCKCS_OFS                    16U                                          /*!< QSPI DELAY0: SCKCS Bit Offset */
#define QSPI_DELAY0_SCKCS(regval)                (BITS(16,23) & ((uint32_t)(regval) << 16))        /*!< QSPI DELAY0: SCKCS Bit Value */  
 
 /* ===== QSPI DELAY1 Register definition ===== */
#define QSPI_DELAY1_INTERCS_MASK                 BITS(0,7)                                   /*!< QSPI DELAY1: INTERCS Bit Mask */  
#define QSPI_DELAY1_INTERCS_OFS                  0U                                          /*!< QSPI DELAY1: INTERCS Bit Offset */
#define QSPI_DELAY1_INTERCS(regval)              (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< QSPI DELAY1: INTERCS Bit Value */  
#define QSPI_DELAY1_INTERXFR_MASK                BITS(16,23)                                   /*!< QSPI DELAY1: INTERXFR Bit Mask */  
#define QSPI_DELAY1_INTERXFR_OFS                 16U                                          /*!< QSPI DELAY1: INTERXFR Bit Offset */
#define QSPI_DELAY1_INTERXFR(regval)             (BITS(16,23) & ((uint32_t)(regval) << 16))        /*!< QSPI DELAY1: INTERXFR Bit Value */  
 
 /* ===== QSPI FIFO_NUM Register definition ===== */
#define QSPI_FIFO_NUM_TX                           BITS(0,15)                
#define QSPI_FIFO_NUM_RX                           BITS(16,31)                
 
 /* ===== QSPI TSIZE Register definition ===== */
#define QSPI_TSIZE_NUM_MASK                     BITS(0,31)                                   /*!< QSPI TSIZE: NUM Bit Mask */  
#define QSPI_TSIZE_NUM_OFS                      0U                                          /*!< QSPI TSIZE: NUM Bit Offset */
#define QSPI_TSIZE_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI TSIZE: NUM Bit Value */  
 
 /* ===== QSPI RSIZE Register definition ===== */
#define QSPI_RSIZE_NUM_MASK                     BITS(0,31)                                   /*!< QSPI RSIZE: NUM Bit Mask */  
#define QSPI_RSIZE_NUM_OFS                      0U                                          /*!< QSPI RSIZE: NUM Bit Offset */
#define QSPI_RSIZE_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI RSIZE: NUM Bit Value */  
 
 /* ===== QSPI FMT Register definition ===== */
#define QSPI_FMT_PROTO_MASK                   BITS(0,1)                                   /*!< QSPI FMT: PROTO Bit Mask */  
#define QSPI_FMT_PROTO_OFS                    0U                                          /*!< QSPI FMT: PROTO Bit Offset */
#define QSPI_FMT_PROTO(regval)                (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< QSPI FMT: PROTO Bit Value */  
#define QSPI_FMT_PROTO_SINGLE                      QSPI_FMT_PROTO(0)                                                   /*!< SINGLE */
#define QSPI_FMT_PROTO_DUAL                        QSPI_FMT_PROTO(1)                                                   /*!< DUAL */
#define QSPI_FMT_PROTO_QUAD                        QSPI_FMT_PROTO(2)                                                   /*!< QUAD */
#define QSPI_FMT_PROTO_OCTOL                       QSPI_FMT_PROTO(3)                                                   /*!< OCTOL */

/**
  * \brief Check the QSPI spi_fmt proto bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid proto bits.
  * \retval 1 This is a valid proto bits.
  */
#define IS_QSPI_SPI_FMT_PROTO(regval)                  (\
                                            ((regval) == QSPI_FMT_PROTO_SINGLE              ) || \
                                            ((regval) == QSPI_FMT_PROTO_DUAL                ) || \
                                            ((regval) == QSPI_FMT_PROTO_QUAD                ) || \
                                            ((regval) == QSPI_FMT_PROTO_OCTOL               )  \
                                                 )

#define QSPI_FMT_ENDIAN                       BIT(2)                                      /*!< SPI endianness - 1'b0: MSB: Transmit most-significant bit (MSB) first - 1'b1: LSB: Transmit least-significant bit (LSB) first */
#define QSPI_FMT_ENDIAN_OFS                   2U                                          /*!< QSPI FMT: ENDIAN Bit Offset */
#define QSPI_FMT_ENDIAN_VAL(regval)               (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI FMT: ENDIAN Bit Value */  
#define QSPI_FMT_ENDIAN_MSB                        0x0UL                                                 /*!< MSB */
#define QSPI_FMT_ENDIAN_LSB                        BIT(2)                                                   /*!< LSB */

/**
  * \brief Check the QSPI spi_fmt endian bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid endian bit.
  * \retval 1 This is a valid endian bit.
  */
#define IS_QSPI_SPI_FMT_ENDIAN(regval)                 (\
                                           ((regval) == QSPI_FMT_ENDIAN_MSB                 ) || \
                                           ((regval) == QSPI_FMT_ENDIAN_LSB                 )  \
                                                 )

#define QSPI_FMT_DIR                          BIT(3)                                      /*!< SPI I/O direction. This is reset to 1 for flash-enabled SPI controllers, 0 otherwise. - 1'b0: RX:  Rx: For dual and quad protocols, the DQ pins are tri-stated. For the single protocol, the DQ0 pin is driven with the transmit data as normal. - 1'b1: TX:  Tx: The receive FIFO is not populated. */
#define QSPI_FMT_DIR_OFS                      3U                                          /*!< QSPI FMT: DIR Bit Offset */
#define QSPI_FMT_DIR_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI FMT: DIR Bit Value */  
#define QSPI_FMT_DIR_RX                            0x0UL                                                 /*!< RX */
#define QSPI_FMT_DIR_TX                            BIT(3)                                                   /*!< TX */

/**
  * \brief Check the QSPI spi_fmt dir bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dir bit.
  * \retval 1 This is a valid dir bit.
  */
#define IS_QSPI_SPI_FMT_DIR(regval)                    (\
                                              ((regval) == QSPI_FMT_DIR_RX                  ) || \
                                              ((regval) == QSPI_FMT_DIR_TX                  )  \
                                                 )

#define QSPI_FMT_LEN_MASK                     BITS(16,21)                                   /*!< QSPI FMT: LEN Bit Mask */  
#define QSPI_FMT_LEN_OFS                      16U                                          /*!< QSPI FMT: LEN Bit Offset */
#define QSPI_FMT_LEN(regval)                  (BITS(16,21) & ((uint32_t)(regval) << 16))        /*!< QSPI FMT: LEN Bit Value */  
#define QSPI_FMT_LEN_8B                            QSPI_FMT_LEN(8)                                                   /*!< 8B */
#define QSPI_FMT_LEN_16B                           QSPI_FMT_LEN(16)                                                   /*!< 16B */
#define QSPI_FMT_LEN_32B                           QSPI_FMT_LEN(32)                                                   /*!< 32B */

/**
  * \brief Check the QSPI spi_fmt len bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid len bits.
  * \retval 1 This is a valid len bits.
  */
#define IS_QSPI_SPI_FMT_LEN(regval)                    (\
                                              ((regval) == QSPI_FMT_LEN_8B                  ) || \
                                              ((regval) == QSPI_FMT_LEN_16B                 ) || \
                                              ((regval) == QSPI_FMT_LEN_32B                 )  \
                                                 )

 /* ===== QSPI TXDATA Register definition ===== */
#define QSPI_TXDATA_VAL_MASK                     BITS(0,31)                                   /*!< QSPI TXDATA: VAL Bit Mask */  
#define QSPI_TXDATA_VAL_OFS                      0U                                          /*!< QSPI TXDATA: VAL Bit Offset */
#define QSPI_TXDATA_VAL(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI TXDATA: VAL Bit Value */  
 
 /* ===== QSPI RXDATA Register definition ===== */
#define QSPI_RXDATA_VAL                          BITS(0,31)                
 
 /* ===== QSPI TX_MARK Register definition ===== */
#define QSPI_TX_MARK_NUM_MASK                     BITS(0,2)                                   /*!< QSPI TX MARK: NUM Bit Mask */  
#define QSPI_TX_MARK_NUM_OFS                      0U                                          /*!< QSPI TX MARK: NUM Bit Offset */
#define QSPI_TX_MARK_NUM(regval)                  (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< QSPI TX MARK: NUM Bit Value */  
 
 /* ===== QSPI RX_MARK Register definition ===== */
#define QSPI_RX_MARK_NUM_MASK                     BITS(0,2)                                   /*!< QSPI RX MARK: NUM Bit Mask */  
#define QSPI_RX_MARK_NUM_OFS                      0U                                          /*!< QSPI RX MARK: NUM Bit Offset */
#define QSPI_RX_MARK_NUM(regval)                  (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< QSPI RX MARK: NUM Bit Value */  
 
 /* ===== QSPI IE Register definition ===== */
#define QSPI_IE_TX                           BIT(0)                                      /*!< Transmit watermark enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_TX_OFS                       0U                                          /*!< QSPI IE: TX Bit Offset */
#define QSPI_IE_TX_VAL(regval)                   (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI IE: TX Bit Value */  
#define QSPI_IE_TX_DISABLE                        0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_TX_ENABLE                         BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie tx bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx bit.
  * \retval 1 This is a valid tx bit.
  */
#define IS_QSPI_SPI_IE_TX(regval)                     (\
                                               ((regval) == QSPI_IE_TX_DISABLE             ) || \
                                               ((regval) == QSPI_IE_TX_ENABLE              )  \
                                                 )

#define QSPI_IE_RX                           BIT(1)                                      /*!< Receive watermark enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_RX_OFS                       1U                                          /*!< QSPI IE: RX Bit Offset */
#define QSPI_IE_RX_VAL(regval)                   (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI IE: RX Bit Value */  
#define QSPI_IE_RX_DISABLE                        0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_RX_ENABLE                         BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie rx bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx bit.
  * \retval 1 This is a valid rx bit.
  */
#define IS_QSPI_SPI_IE_RX(regval)                     (\
                                               ((regval) == QSPI_IE_RX_DISABLE             ) || \
                                               ((regval) == QSPI_IE_RX_ENABLE              )  \
                                                 )

#define QSPI_IE_TX_UDR                       BIT(2)                                      /*!< txudr error irq enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_TX_UDR_OFS                   2U                                          /*!< QSPI IE: TX_UDR Bit Offset */
#define QSPI_IE_TX_UDR_VAL(regval)               (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI IE: TX_UDR Bit Value */  
#define QSPI_IE_TX_UDR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_TX_UDR_ENABLE                     BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie tx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_udr bit.
  * \retval 1 This is a valid tx_udr bit.
  */
#define IS_QSPI_SPI_IE_TX_UDR(regval)                 (\
                                           ((regval) == QSPI_IE_TX_UDR_DISABLE             ) || \
                                           ((regval) == QSPI_IE_TX_UDR_ENABLE              )  \
                                                 )

#define QSPI_IE_RX_OVR                       BIT(3)                                      /*!< rxovr error irq enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_RX_OVR_OFS                   3U                                          /*!< QSPI IE: RX_OVR Bit Offset */
#define QSPI_IE_RX_OVR_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI IE: RX_OVR Bit Value */  
#define QSPI_IE_RX_OVR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_RX_OVR_ENABLE                     BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie rx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_ovr bit.
  * \retval 1 This is a valid rx_ovr bit.
  */
#define IS_QSPI_SPI_IE_RX_OVR(regval)                 (\
                                           ((regval) == QSPI_IE_RX_OVR_DISABLE             ) || \
                                           ((regval) == QSPI_IE_RX_OVR_ENABLE              )  \
                                                 )

#define QSPI_IE_RX_UDR                       BIT(4)                                      /*!< rxudr error irq enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_RX_UDR_OFS                   4U                                          /*!< QSPI IE: RX_UDR Bit Offset */
#define QSPI_IE_RX_UDR_VAL(regval)               (BIT(4) & ((uint32_t)(regval) << 4))        /*!< QSPI IE: RX_UDR Bit Value */  
#define QSPI_IE_RX_UDR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_RX_UDR_ENABLE                     BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie rx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_udr bit.
  * \retval 1 This is a valid rx_udr bit.
  */
#define IS_QSPI_SPI_IE_RX_UDR(regval)                 (\
                                           ((regval) == QSPI_IE_RX_UDR_DISABLE             ) || \
                                           ((regval) == QSPI_IE_RX_UDR_ENABLE              )  \
                                                 )

#define QSPI_IE_TX_OVR                       BIT(5)                                      /*!< txovr error irq enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_TX_OVR_OFS                   5U                                          /*!< QSPI IE: TX_OVR Bit Offset */
#define QSPI_IE_TX_OVR_VAL(regval)               (BIT(5) & ((uint32_t)(regval) << 5))        /*!< QSPI IE: TX_OVR Bit Value */  
#define QSPI_IE_TX_OVR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_TX_OVR_ENABLE                     BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie tx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_ovr bit.
  * \retval 1 This is a valid tx_ovr bit.
  */
#define IS_QSPI_SPI_IE_TX_OVR(regval)                 (\
                                           ((regval) == QSPI_IE_TX_OVR_DISABLE             ) || \
                                           ((regval) == QSPI_IE_TX_OVR_ENABLE              )  \
                                                 )

#define QSPI_IE_DONE                         BIT(7)                                      /*!< spi tx done & TX fifo is empty irq enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_DONE_OFS                     7U                                          /*!< QSPI IE: DONE Bit Offset */
#define QSPI_IE_DONE_VAL(regval)                 (BIT(7) & ((uint32_t)(regval) << 7))        /*!< QSPI IE: DONE Bit Value */  
#define QSPI_IE_DONE_DISABLE                      0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_DONE_ENABLE                       BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie done bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid done bit.
  * \retval 1 This is a valid done bit.
  */
#define IS_QSPI_SPI_IE_DONE(regval)                   (\
                                             ((regval) == QSPI_IE_DONE_DISABLE             ) || \
                                             ((regval) == QSPI_IE_DONE_ENABLE              )  \
                                                 )

#define QSPI_IE_TX_DONE                      BIT(11)                                      /*!< spi tx done irq enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_TX_DONE_OFS                  11U                                          /*!< QSPI IE: TX_DONE Bit Offset */
#define QSPI_IE_TX_DONE_VAL(regval)              (BIT(11) & ((uint32_t)(regval) << 11))        /*!< QSPI IE: TX_DONE Bit Value */  
#define QSPI_IE_TX_DONE_DISABLE                   0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_TX_DONE_ENABLE                    BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie tx_done bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_done bit.
  * \retval 1 This is a valid tx_done bit.
  */
#define IS_QSPI_SPI_IE_TX_DONE(regval)                (\
                                          ((regval) == QSPI_IE_TX_DONE_DISABLE             ) || \
                                          ((regval) == QSPI_IE_TX_DONE_ENABLE              )  \
                                                 )

#define QSPI_IE_RX_DONE                      BIT(12)                                      /*!< spi rx done irq enable - 1'b0: disable - 1'b1: enable */
#define QSPI_IE_RX_DONE_OFS                  12U                                          /*!< QSPI IE: RX_DONE Bit Offset */
#define QSPI_IE_RX_DONE_VAL(regval)              (BIT(12) & ((uint32_t)(regval) << 12))        /*!< QSPI IE: RX_DONE Bit Value */  
#define QSPI_IE_RX_DONE_DISABLE                   0x0UL                                                  /*!< DISABLE */
#define QSPI_IE_RX_DONE_ENABLE                    BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI spi_ie rx_done bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_done bit.
  * \retval 1 This is a valid rx_done bit.
  */
#define IS_QSPI_SPI_IE_RX_DONE(regval)                (\
                                          ((regval) == QSPI_IE_RX_DONE_DISABLE             ) || \
                                          ((regval) == QSPI_IE_RX_DONE_ENABLE              )  \
                                                 )

 /* ===== QSPI IP Register definition ===== */
#define QSPI_IP_TX                           BIT(0)                                      /*!< Transmit watermark enable */
#define QSPI_IP_RX                           BIT(1)                                      /*!< Receive watermark enable */
 
 /* ===== QSPI STATUS Register definition ===== */
#define QSPI_STATUS_BUSY                         BIT(0)                                      /*!< SPI Transmit is busy (master/slave) */
#define QSPI_STATUS_RX_OVR                       BIT(2)                                      /*!< pi rx fifo Overrun flag: - 1'b0: no over - 1'b1: yes: over Note: Software write 1 clear 0 */
#define QSPI_STATUS_RX_OVR_OFS                   2U                                          /*!< QSPI STATUS: RX_OVR Bit Offset */
#define QSPI_STATUS_RX_OVR_VAL(regval)               (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI STATUS: RX_OVR Bit Value */  
#define QSPI_STATUS_RX_OVR_NO                         0x0UL                                              /*!< NO */
#define QSPI_STATUS_RX_OVR_YES                        BIT(2)                                                   /*!< YES */

/**
  * \brief Check the QSPI spi_status rx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_ovr bit.
  * \retval 1 This is a valid rx_ovr bit.
  */
#define IS_QSPI_SPI_STATUS_RX_OVR(regval)                 (\
                                           ((regval) == QSPI_STATUS_RX_OVR_NO                  ) || \
                                           ((regval) == QSPI_STATUS_RX_OVR_YES                 )  \
                                                 )

#define QSPI_STATUS_TX_UDR                       BIT(3)                                      /*!< spi tx fifo Underrun flag: - 1'b0: no under  err - 1'b1: yes: under err Note: Software write 1 clear 0 */
#define QSPI_STATUS_TX_UDR_OFS                   3U                                          /*!< QSPI STATUS: TX_UDR Bit Offset */
#define QSPI_STATUS_TX_UDR_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI STATUS: TX_UDR Bit Value */  
#define QSPI_STATUS_TX_UDR_NO                         0x0UL                                              /*!< NO */
#define QSPI_STATUS_TX_UDR_YES                        BIT(3)                                                   /*!< YES */

/**
  * \brief Check the QSPI spi_status tx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_udr bit.
  * \retval 1 This is a valid tx_udr bit.
  */
#define IS_QSPI_SPI_STATUS_TX_UDR(regval)                 (\
                                           ((regval) == QSPI_STATUS_TX_UDR_NO                  ) || \
                                           ((regval) == QSPI_STATUS_TX_UDR_YES                 )  \
                                                 )

#define QSPI_STATUS_TX_FULL                      BIT(4)                                      /*!< tx_fifo full flag */
#define QSPI_STATUS_RX_EMPTY                     BIT(5)                                      /*!< rx fifo empty flag */
#define QSPI_STATUS_RX_UDR                       BIT(9)                                      /*!< spi rx fifo udr flag: - 1'b0: no over - 1'b1: yes: over Note: Software write 1 clear 0 */
#define QSPI_STATUS_RX_UDR_OFS                   9U                                          /*!< QSPI STATUS: RX_UDR Bit Offset */
#define QSPI_STATUS_RX_UDR_VAL(regval)               (BIT(9) & ((uint32_t)(regval) << 9))        /*!< QSPI STATUS: RX_UDR Bit Value */  
#define QSPI_STATUS_RX_UDR_NO                         0x0UL                                              /*!< NO */
#define QSPI_STATUS_RX_UDR_YES                        BIT(9)                                                   /*!< YES */

/**
  * \brief Check the QSPI spi_status rx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_udr bit.
  * \retval 1 This is a valid rx_udr bit.
  */
#define IS_QSPI_SPI_STATUS_RX_UDR(regval)                 (\
                                           ((regval) == QSPI_STATUS_RX_UDR_NO                  ) || \
                                           ((regval) == QSPI_STATUS_RX_UDR_YES                 )  \
                                                 )

#define QSPI_STATUS_TX_OVR                       BIT(10)                                      /*!< spi tx fifo ovr flag: - 1'b0: no under  err - 1'b1: yes: under err Note: Software write 1 clear 0 */
#define QSPI_STATUS_TX_OVR_OFS                   10U                                          /*!< QSPI STATUS: TX_OVR Bit Offset */
#define QSPI_STATUS_TX_OVR_VAL(regval)               (BIT(10) & ((uint32_t)(regval) << 10))        /*!< QSPI STATUS: TX_OVR Bit Value */  
#define QSPI_STATUS_TX_OVR_NO                         0x0UL                                              /*!< NO */
#define QSPI_STATUS_TX_OVR_YES                        BIT(10)                                                  /*!< YES */

/**
  * \brief Check the QSPI spi_status tx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_ovr bit.
  * \retval 1 This is a valid tx_ovr bit.
  */
#define IS_QSPI_SPI_STATUS_TX_OVR(regval)                 (\
                                           ((regval) == QSPI_STATUS_TX_OVR_NO                  ) || \
                                           ((regval) == QSPI_STATUS_TX_OVR_YES                 )  \
                                                 )

#define QSPI_STATUS_TX_EMPTY                     BIT(11)                                      /*!< tx fifo empty flag */
#define QSPI_STATUS_RX_FULL                      BIT(12)                                      /*!< rx fifo full flag */
#define QSPI_STATUS_DONE                         BIT(13)                                      /*!< spi tx done & tx fifo is empty Note: Software write 1 clear 0 */
#define QSPI_STATUS_DONE_OFS                     13U                                          /*!< QSPI STATUS: DONE Bit Offset */
#define QSPI_STATUS_DONE_VAL(regval)                 (BIT(13) & ((uint32_t)(regval) << 13))        /*!< QSPI STATUS: DONE Bit Value */  
#define QSPI_STATUS_TX_DONE                      BIT(17)                                      /*!< spi tx done Note: Software write 1 clear 0 */
#define QSPI_STATUS_TX_DONE_OFS                  17U                                          /*!< QSPI STATUS: TX_DONE Bit Offset */
#define QSPI_STATUS_TX_DONE_VAL(regval)              (BIT(17) & ((uint32_t)(regval) << 17))        /*!< QSPI STATUS: TX_DONE Bit Value */  
#define QSPI_STATUS_RX_DONE                      BIT(18)                                      /*!< spi rx done Note: Software write 1 clear 0 */
#define QSPI_STATUS_RX_DONE_OFS                  18U                                          /*!< QSPI STATUS: RX_DONE Bit Offset */
#define QSPI_STATUS_RX_DONE_VAL(regval)              (BIT(18) & ((uint32_t)(regval) << 18))        /*!< QSPI STATUS: RX_DONE Bit Value */  
 
 /* ===== QSPI RXEDGE Register definition ===== */
#define QSPI_RXEDGE_RXEDGE                       BIT(0)                                      /*!< - 1'b0: posedge sck sample - 1'b1: negedge sck sample */
#define QSPI_RXEDGE_RXEDGE_OFS                   0U                                          /*!< QSPI RXEDGE: RXEDGE Bit Offset */
#define QSPI_RXEDGE_RXEDGE_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI RXEDGE: RXEDGE Bit Value */  
#define QSPI_RXEDGE_RXEDGE_POSEDGE                    0x0UL                                              /*!< POSEDGE */
#define QSPI_RXEDGE_RXEDGE_NEGEDGE                    BIT(0)                                                   /*!< NEGEDGE */

/**
  * \brief Check the QSPI spi_rxedge rxedge bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rxedge bit.
  * \retval 1 This is a valid rxedge bit.
  */
#define IS_QSPI_SPI_RXEDGE_RXEDGE(regval)                 (\
                                           ((regval) == QSPI_RXEDGE_RXEDGE_POSEDGE             ) || \
                                           ((regval) == QSPI_RXEDGE_RXEDGE_NEGEDGE             )  \
                                                 )

 /* ===== QSPI CR Register definition ===== */
#define QSPI_CR_MODE                         BIT(0)                                      /*!< SPI Master Mode selection Select - 1'b0: slave - 1'b1: master */
#define QSPI_CR_MODE_OFS                     0U                                          /*!< QSPI CR: MODE Bit Offset */
#define QSPI_CR_MODE_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI CR: MODE Bit Value */  
#define QSPI_CR_MODE_SLAVE                        0x0UL                                                  /*!< SLAVE */
#define QSPI_CR_MODE_MASTER                       BIT(0)                                                   /*!< MASTER */

/**
  * \brief Check the QSPI spi_cr mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid mode bit.
  * \retval 1 This is a valid mode bit.
  */
#define IS_QSPI_SPI_CR_MODE(regval)                   (\
                                             ((regval) == QSPI_CR_MODE_SLAVE               ) || \
                                             ((regval) == QSPI_CR_MODE_MASTER              )  \
                                                 )

#define QSPI_CR_DMA                          BIT(1)                                      /*!< DMA mode enable - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_DMA_OFS                      1U                                          /*!< QSPI CR: DMA Bit Offset */
#define QSPI_CR_DMA_VAL(regval)                  (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI CR: DMA Bit Value */  
#define QSPI_CR_DMA_DISABLE                       0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_DMA_ENABLE                        BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr dma bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma bit.
  * \retval 1 This is a valid dma bit.
  */
#define IS_QSPI_SPI_CR_DMA(regval)                    (\
                                              ((regval) == QSPI_CR_DMA_DISABLE             ) || \
                                              ((regval) == QSPI_CR_DMA_ENABLE              )  \
                                                 )

#define QSPI_CR_DDR                          BIT(2)                                      /*!< Double Data Rate (DDR) mode enable - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_DDR_OFS                      2U                                          /*!< QSPI CR: DDR Bit Offset */
#define QSPI_CR_DDR_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI CR: DDR Bit Value */  
#define QSPI_CR_DDR_DISABLE                       0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_DDR_ENABLE                        BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr ddr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ddr bit.
  * \retval 1 This is a valid ddr bit.
  */
#define IS_QSPI_SPI_CR_DDR(regval)                    (\
                                              ((regval) == QSPI_CR_DDR_DISABLE             ) || \
                                              ((regval) == QSPI_CR_DDR_ENABLE              )  \
                                                 )

#define QSPI_CR_CSI                          BIT(3)                                      /*!< Internal slave select (slave mode vaild) - 1'b0: on - 1'b1: off */
#define QSPI_CR_CSI_OFS                      3U                                          /*!< QSPI CR: CSI Bit Offset */
#define QSPI_CR_CSI_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI CR: CSI Bit Value */  
#define QSPI_CR_CSI_ON                            0x0UL                                                  /*!< ON */
#define QSPI_CR_CSI_OFF                           BIT(3)                                                   /*!< OFF */

/**
  * \brief Check the QSPI spi_cr csi bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid csi bit.
  * \retval 1 This is a valid csi bit.
  */
#define IS_QSPI_SPI_CR_CSI(regval)                    (\
                                              ((regval) == QSPI_CR_CSI_ON                  ) || \
                                              ((regval) == QSPI_CR_CSI_OFF                 )  \
                                                 )

#define QSPI_CR_CSOE                         BIT(4)                                      /*!< CS output enable - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_CSOE_OFS                     4U                                          /*!< QSPI CR: CSOE Bit Offset */
#define QSPI_CR_CSOE_VAL(regval)                 (BIT(4) & ((uint32_t)(regval) << 4))        /*!< QSPI CR: CSOE Bit Value */  
#define QSPI_CR_CSOE_DISABLE                      0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_CSOE_ENABLE                       BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr csoe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid csoe bit.
  * \retval 1 This is a valid csoe bit.
  */
#define IS_QSPI_SPI_CR_CSOE(regval)                   (\
                                             ((regval) == QSPI_CR_CSOE_DISABLE             ) || \
                                             ((regval) == QSPI_CR_CSOE_ENABLE              )  \
                                                 )

#define QSPI_CR_SSM                          BIT(5)                                      /*!< Software slave management - 1'b0: hard - 1'b1: soft */
#define QSPI_CR_SSM_OFS                      5U                                          /*!< QSPI CR: SSM Bit Offset */
#define QSPI_CR_SSM_VAL(regval)                  (BIT(5) & ((uint32_t)(regval) << 5))        /*!< QSPI CR: SSM Bit Value */  
#define QSPI_CR_SSM_HARD                          0x0UL                                                  /*!< HARD */
#define QSPI_CR_SSM_SOFT                          BIT(5)                                                   /*!< SOFT */

/**
  * \brief Check the QSPI spi_cr ssm bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ssm bit.
  * \retval 1 This is a valid ssm bit.
  */
#define IS_QSPI_SPI_CR_SSM(regval)                    (\
                                              ((regval) == QSPI_CR_SSM_HARD                ) || \
                                              ((regval) == QSPI_CR_SSM_SOFT                )  \
                                                 )

#define QSPI_CR_HDSMODE                      BIT(6)                                      /*!< Half-duplex SPI is 3 wires Mode (SS, SCK, SIO bidirectional data) - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_HDSMODE_OFS                  6U                                          /*!< QSPI CR: HDSMODE Bit Offset */
#define QSPI_CR_HDSMODE_VAL(regval)              (BIT(6) & ((uint32_t)(regval) << 6))        /*!< QSPI CR: HDSMODE Bit Value */  
#define QSPI_CR_HDSMODE_DISABLE                   0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_HDSMODE_ENABLE                    BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr hdsmode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid hdsmode bit.
  * \retval 1 This is a valid hdsmode bit.
  */
#define IS_QSPI_SPI_CR_HDSMODE(regval)                (\
                                          ((regval) == QSPI_CR_HDSMODE_DISABLE             ) || \
                                          ((regval) == QSPI_CR_HDSMODE_ENABLE              )  \
                                                 )

#define QSPI_CR_DMA_TX                       BIT(8)                                      /*!< ST dma tx enable - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_DMA_TX_OFS                   8U                                          /*!< QSPI CR: DMA_TX Bit Offset */
#define QSPI_CR_DMA_TX_VAL(regval)               (BIT(8) & ((uint32_t)(regval) << 8))        /*!< QSPI CR: DMA_TX Bit Value */  
#define QSPI_CR_DMA_TX_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_DMA_TX_ENABLE                     BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr dma_tx bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_tx bit.
  * \retval 1 This is a valid dma_tx bit.
  */
#define IS_QSPI_SPI_CR_DMA_TX(regval)                 (\
                                           ((regval) == QSPI_CR_DMA_TX_DISABLE             ) || \
                                           ((regval) == QSPI_CR_DMA_TX_ENABLE              )  \
                                                 )

#define QSPI_CR_DMA_RX                       BIT(9)                                      /*!< ST dma rx enable - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_DMA_RX_OFS                   9U                                          /*!< QSPI CR: DMA_RX Bit Offset */
#define QSPI_CR_DMA_RX_VAL(regval)               (BIT(9) & ((uint32_t)(regval) << 9))        /*!< QSPI CR: DMA_RX Bit Value */  
#define QSPI_CR_DMA_RX_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_DMA_RX_ENABLE                     BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr dma_rx bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_rx bit.
  * \retval 1 This is a valid dma_rx bit.
  */
#define IS_QSPI_SPI_CR_DMA_RX(regval)                 (\
                                           ((regval) == QSPI_CR_DMA_RX_DISABLE             ) || \
                                           ((regval) == QSPI_CR_DMA_RX_ENABLE              )  \
                                                 )

#define QSPI_CR_DMA_RCONTINOUS_MODE          BIT(10)                                      /*!< ST dma rxcontinous mode - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_DMA_RCONTINOUS_MODE_OFS      10U                                          /*!< QSPI CR: DMA_RCONTINOUS_MODE Bit Offset */
#define QSPI_CR_DMA_RCONTINOUS_MODE_VAL(regval)  (BIT(10) & ((uint32_t)(regval) << 10))        /*!< QSPI CR: DMA_RCONTINOUS_MODE Bit Value */  
#define QSPI_CR_DMA_RCONTINOUS_MODE_DISABLE             0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_DMA_RCONTINOUS_MODE_ENABLE             BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr dma_rcontinous_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_rcontinous_mode bit.
  * \retval 1 This is a valid dma_rcontinous_mode bit.
  */
#define IS_QSPI_SPI_CR_DMA_RCONTINOUS_MODE(regval)    (\
                              ((regval) == QSPI_CR_DMA_RCONTINOUS_MODE_DISABLE             ) || \
                              ((regval) == QSPI_CR_DMA_RCONTINOUS_MODE_ENABLE              )  \
                                                 )

#define QSPI_CR_DMA_TCONTINOUS_MODE          BIT(11)                                      /*!< ST dma txcontinous mode - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_DMA_TCONTINOUS_MODE_OFS      11U                                          /*!< QSPI CR: DMA_TCONTINOUS_MODE Bit Offset */
#define QSPI_CR_DMA_TCONTINOUS_MODE_VAL(regval)  (BIT(11) & ((uint32_t)(regval) << 11))        /*!< QSPI CR: DMA_TCONTINOUS_MODE Bit Value */  
#define QSPI_CR_DMA_TCONTINOUS_MODE_DISABLE             0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_DMA_TCONTINOUS_MODE_ENABLE             BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr dma_tcontinous_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_tcontinous_mode bit.
  * \retval 1 This is a valid dma_tcontinous_mode bit.
  */
#define IS_QSPI_SPI_CR_DMA_TCONTINOUS_MODE(regval)    (\
                              ((regval) == QSPI_CR_DMA_TCONTINOUS_MODE_DISABLE             ) || \
                              ((regval) == QSPI_CR_DMA_TCONTINOUS_MODE_ENABLE              )  \
                                                 )

#define QSPI_CR_RECEIVE_DATA                 BIT(13)                                      /*!< RX fifo receive data enable - 1'b0: disable - 1'b1: enable */
#define QSPI_CR_RECEIVE_DATA_OFS             13U                                          /*!< QSPI CR: RECEIVE_DATA Bit Offset */
#define QSPI_CR_RECEIVE_DATA_VAL(regval)         (BIT(13) & ((uint32_t)(regval) << 13))        /*!< QSPI CR: RECEIVE_DATA Bit Value */  
#define QSPI_CR_RECEIVE_DATA_DISABLE              0x0UL                                                  /*!< DISABLE */
#define QSPI_CR_RECEIVE_DATA_ENABLE               BIT(13)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI spi_cr receive_data bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid receive_data bit.
  * \retval 1 This is a valid receive_data bit.
  */
#define IS_QSPI_SPI_CR_RECEIVE_DATA(regval)           (\
                                     ((regval) == QSPI_CR_RECEIVE_DATA_DISABLE             ) || \
                                     ((regval) == QSPI_CR_RECEIVE_DATA_ENABLE              )  \
                                                 )

/* ===== QSPI exported functions ===== */
void QSPI_Init(QSPI_TypeDef* QSPIx, QSPI_InitTypeDef* QSPI_Struct);
void QSPI_StructInit(QSPI_InitTypeDef* QSPI_Struct);

void QSPI_SSM_Switch(QSPI_TypeDef* QSPIx, ControlStatus Status);

void QSPI_RxEdgeConfig(QSPI_TypeDef* QSPIx, ControlStatus Status);
void QSPI_TxmarkConfig(QSPI_TypeDef* QSPIx, uint32_t Mark);
void QSPI_RxmarkConfig(QSPI_TypeDef* QSPIx, uint32_t Mark);
void QSPI_DirectionConfig(QSPI_TypeDef* QSPIx, uint32_t Direction);
void QSPI_ITConfig(QSPI_TypeDef* QSPIx, uint32_t QSPI_IE, ControlStatus Status);
FlagStatus QSPI_GetITConfig(QSPI_TypeDef* QSPIx, uint32_t QSPI_IE);
void QSPI_HDSCmd(QSPI_TypeDef* QSPIx, ControlStatus Status);
FlagStatus QSPI_GetITStatus(QSPI_TypeDef* QSPIx, uint32_t QSPI_IT);
FlagStatus QSPI_GetFlag(QSPI_TypeDef* QSPIx, uint32_t QSPI_FLAG);
void QSPI_ClearFlag(QSPI_TypeDef* QSPIx, uint32_t QSPI_FLAG);
void QSPI_CSModeConfig(QSPI_TypeDef* QSPIx, uint32_t CsMode);
void QSPI_CSIDConfig(QSPI_TypeDef* QSPIx, uint32_t CsID);
void QSPI_Delay0Config(QSPI_TypeDef* QSPIx, uint32_t Sckcs, uint32_t Cssck);
void QSPI_Delay1Config(QSPI_TypeDef* QSPIx, uint32_t Intercs, uint32_t Interxfr);
void QSPI_SendData(QSPI_TypeDef* QSPIx, uint32_t Data);
uint32_t QSPI_ReceiveData(QSPI_TypeDef* QSPIx);
uint32_t QSPI_TxReadEntry(QSPI_TypeDef* QSPIx);
uint32_t QSPI_RxReadEntry(QSPI_TypeDef* QSPIx);

FlagStatus QSPI_TransmitReceive(QSPI_TypeDef *QSPIx, uint8_t *pTxData, uint8_t *pRxData);
void QSPI_CS_Enable(QSPI_TypeDef* QSPIx, uint8_t csid, ControlStatus Status);

#ifdef __cplusplus
}
#endif

#endif
/* _QSPI_H*/
