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
#ifndef _NS_IOMUX_H
#define _NS_IOMUX_H

/*!
 * @file     ns_iomux.h
 * @brief    This file contains all the functions prototypes for the IOMUX firmware
 */
#ifdef __cplusplus
 extern "C" {
#endif
#include "ns.h"

/*
********************PAD Info Start********************
*/

#define PAD0           0
#define PAD0_LS_GROUP  None
#define PAD0_HS0_PAD_QSPI1_SCK       PAD_QSPI1_SCK
#define PAD0_HS1_PAD_QSPI3_SCK       PAD_QSPI3_SCK
#define PAD0_HS2_PAD_QSPI5_SCK       PAD_QSPI5_SCK
        
#define PAD1           1
#define PAD1_LS_GROUP  None
#define PAD1_HS0_PAD_QSPI1_CS_0       PAD_QSPI1_CS_0
#define PAD1_HS1_PAD_QSPI3_CS_0       PAD_QSPI3_CS_0
#define PAD1_HS2_PAD_QSPI5_CS_0       PAD_QSPI5_CS_0
        
#define PAD2           2
#define PAD2_LS_GROUP  None
#define PAD2_HS0_PAD_QSPI1_DQ_0       PAD_QSPI1_DQ_0
#define PAD2_HS1_PAD_QSPI3_DQ_0       PAD_QSPI3_DQ_0
#define PAD2_HS2_PAD_QSPI5_DQ_0       PAD_QSPI5_DQ_0
        
#define PAD3           3
#define PAD3_LS_GROUP  None
#define PAD3_HS0_PAD_QSPI1_DQ_1       PAD_QSPI1_DQ_1
#define PAD3_HS1_PAD_QSPI3_DQ_1       PAD_QSPI3_DQ_1
#define PAD3_HS2_PAD_QSPI5_DQ_1       PAD_QSPI5_DQ_1
        
#define PAD4           4
#define PAD4_LS_GROUP  None
#define PAD4_HS0_PAD_QSPI1_DQ_2       PAD_QSPI1_DQ_2
#define PAD4_HS1_PAD_QSPI3_DQ_2       PAD_QSPI3_DQ_2
#define PAD4_HS2_PAD_QSPI5_DQ_2       PAD_QSPI5_DQ_2
        
#define PAD5           5
#define PAD5_LS_GROUP  None
#define PAD5_HS0_PAD_QSPI1_DQ_3       PAD_QSPI1_DQ_3
#define PAD5_HS1_PAD_QSPI3_DQ_3       PAD_QSPI3_DQ_3
#define PAD5_HS2_PAD_QSPI5_DQ_3       PAD_QSPI5_DQ_3
        
#define PAD6           6
#define PAD6_LS_GROUP  None
#define PAD6_HS0_PAD_USART1_RX       PAD_USART1_RX
#define PAD6_HS1_PAD_USART3_RX       PAD_USART3_RX
#define PAD6_HS2_PAD_USART5_RX       PAD_USART5_RX
        
#define PAD7           7
#define PAD7_LS_GROUP  None
#define PAD7_HS0_PAD_USART1_TX       PAD_USART1_TX
#define PAD7_HS1_PAD_USART3_TX       PAD_USART3_TX
#define PAD7_HS2_PAD_USART5_TX       PAD_USART5_TX
        
#define PAD8           8
#define PAD8_LS_GROUP  None
#define PAD8_HS0_PAD_USART1_CTS       PAD_USART1_CTS
#define PAD8_HS1_PAD_USART3_CTS       PAD_USART3_CTS
#define PAD8_HS2_PAD_USART5_CTS       PAD_USART5_CTS
        
#define PAD9           9
#define PAD9_LS_GROUP  None
#define PAD9_HS0_PAD_USART1_RTS       PAD_USART1_RTS
#define PAD9_HS1_PAD_USART3_RTS       PAD_USART3_RTS
#define PAD9_HS2_PAD_USART5_RTS       PAD_USART5_RTS
        
#define PAD10           10
#define PAD10_LS_GROUP  None
#define PAD10_HS0_PAD_USART1_TX_CLK       PAD_USART1_TX_CLK
#define PAD10_HS1_PAD_USART3_TX_CLK       PAD_USART3_TX_CLK
#define PAD10_HS2_PAD_USART5_TX_CLK       PAD_USART5_TX_CLK
        
#define PAD11           11
#define PAD11_LS_GROUP  None
#define PAD11_HS0_PAD_I2C0_SCL       PAD_I2C0_SCL
#define PAD11_HS1_PAD_I2C1_SCL       PAD_I2C1_SCL
#define PAD11_HS2_PAD_I2C3_SCL       PAD_I2C3_SCL
        
#define PAD12           12
#define PAD12_LS_GROUP  None
#define PAD12_HS0_PAD_I2C0_SDA       PAD_I2C0_SDA
#define PAD12_HS1_PAD_I2C1_SDA       PAD_I2C1_SDA
#define PAD12_HS2_PAD_I2C3_SDA       PAD_I2C3_SDA
        
#define PAD13           13
#define PAD13_LS_GROUP  None
#define PAD13_HS0_NONE       NONE
#define PAD13_HS1_NONE       NONE
#define PAD13_HS2_NONE       NONE
        
#define PAD14           14
#define PAD14_LS_GROUP  None
#define PAD14_HS0_NONE       NONE
#define PAD14_HS1_NONE       NONE
#define PAD14_HS2_NONE       NONE
        
#define PAD15           15
#define PAD15_LS_GROUP  None
#define PAD15_HS0_NONE       NONE
#define PAD15_HS1_NONE       NONE
#define PAD15_HS2_NONE       NONE
        
#define PAD16           16
#define PAD16_LS_GROUP  None
#define PAD16_HS0_NONE       NONE
#define PAD16_HS1_NONE       NONE
#define PAD16_HS2_NONE       NONE
        
#define PAD17           17
#define PAD17_LS_GROUP  None
#define PAD17_HS0_NONE       NONE
#define PAD17_HS1_NONE       NONE
#define PAD17_HS2_NONE       NONE
        
#define PAD18           18
#define PAD18_LS_GROUP  None
#define PAD18_HS0_NONE       NONE
#define PAD18_HS1_NONE       NONE
#define PAD18_HS2_NONE       NONE
        
#define PAD19           19
#define PAD19_LS_GROUP  None
#define PAD19_HS0_NONE       NONE
#define PAD19_HS1_NONE       NONE
#define PAD19_HS2_NONE       NONE
        
#define PAD20           20
#define PAD20_LS_GROUP  None
#define PAD20_HS0_NONE       NONE
#define PAD20_HS1_NONE       NONE
#define PAD20_HS2_NONE       NONE
        
#define PAD21           21
#define PAD21_LS_GROUP  None
#define PAD21_HS0_NONE       NONE
#define PAD21_HS1_NONE       NONE
#define PAD21_HS2_NONE       NONE
        
#define PAD22           22
#define PAD22_LS_GROUP  None
#define PAD22_HS0_PAD_QSPI2_SCK       PAD_QSPI2_SCK
#define PAD22_HS1_PAD_QSPI4_SCK       PAD_QSPI4_SCK
#define PAD22_HS2_PAD_QSPI6_SCK       PAD_QSPI6_SCK
        
#define PAD23           23
#define PAD23_LS_GROUP  None
#define PAD23_HS0_PAD_QSPI2_CS_0       PAD_QSPI2_CS_0
#define PAD23_HS1_PAD_QSPI4_CS_0       PAD_QSPI4_CS_0
#define PAD23_HS2_PAD_QSPI6_CS_0       PAD_QSPI6_CS_0
        
#define PAD24           24
#define PAD24_LS_GROUP  None
#define PAD24_HS0_PAD_QSPI2_DQ_0       PAD_QSPI2_DQ_0
#define PAD24_HS1_PAD_QSPI4_DQ_0       PAD_QSPI4_DQ_0
#define PAD24_HS2_PAD_QSPI6_DQ_0       PAD_QSPI6_DQ_0
        
#define PAD25           25
#define PAD25_LS_GROUP  None
#define PAD25_HS0_PAD_QSPI2_DQ_1       PAD_QSPI2_DQ_1
#define PAD25_HS1_PAD_QSPI4_DQ_1       PAD_QSPI4_DQ_1
#define PAD25_HS2_PAD_QSPI6_DQ_1       PAD_QSPI6_DQ_1
        
#define PAD26           26
#define PAD26_LS_GROUP  None
#define PAD26_HS0_PAD_QSPI2_DQ_2       PAD_QSPI2_DQ_2
#define PAD26_HS1_PAD_QSPI4_DQ_2       PAD_QSPI4_DQ_2
#define PAD26_HS2_PAD_QSPI6_DQ_2       PAD_QSPI6_DQ_2
        
#define PAD27           27
#define PAD27_LS_GROUP  None
#define PAD27_HS0_PAD_QSPI2_DQ_3       PAD_QSPI2_DQ_3
#define PAD27_HS1_PAD_QSPI4_DQ_3       PAD_QSPI4_DQ_3
#define PAD27_HS2_PAD_QSPI6_DQ_3       PAD_QSPI6_DQ_3
        
#define PAD28           28
#define PAD28_LS_GROUP  None
#define PAD28_HS0_PAD_USART2_RX       PAD_USART2_RX
#define PAD28_HS1_PAD_USART4_RX       PAD_USART4_RX
#define PAD28_HS2_PAD_USART6_RX       PAD_USART6_RX
        
#define PAD29           29
#define PAD29_LS_GROUP  None
#define PAD29_HS0_PAD_USART2_TX       PAD_USART2_TX
#define PAD29_HS1_PAD_USART4_TX       PAD_USART4_TX
#define PAD29_HS2_PAD_USART6_TX       PAD_USART6_TX
        
#define PAD30           30
#define PAD30_LS_GROUP  None
#define PAD30_HS0_PAD_USART2_CTS       PAD_USART2_CTS
#define PAD30_HS1_PAD_USART4_CTS       PAD_USART4_CTS
#define PAD30_HS2_PAD_USART6_CTS       PAD_USART6_CTS
        
#define PAD31           31
#define PAD31_LS_GROUP  None
#define PAD31_HS0_PAD_USART2_RTS       PAD_USART2_RTS
#define PAD31_HS1_PAD_USART4_RTS       PAD_USART4_RTS
#define PAD31_HS2_PAD_USART6_RTS       PAD_USART6_RTS
        
#define PAD32           32
#define PAD32_LS_GROUP  None
#define PAD32_HS0_PAD_USART2_TX_CLK       PAD_USART2_TX_CLK
#define PAD32_HS1_PAD_USART4_TX_CLK       PAD_USART4_TX_CLK
#define PAD32_HS2_PAD_USART6_TX_CLK       PAD_USART6_TX_CLK
        
#define PAD33           33
#define PAD33_LS_GROUP  None
#define PAD33_HS0_PAD_I2C2_SCL       PAD_I2C2_SCL
#define PAD33_HS1_PAD_I2C4_SCL       PAD_I2C4_SCL
#define PAD33_HS2_PAD_I2C2ICB0_SCL       PAD_I2C2ICB0_SCL
        
#define PAD34           34
#define PAD34_LS_GROUP  None
#define PAD34_HS0_PAD_I2C2_SDA       PAD_I2C2_SDA
#define PAD34_HS1_PAD_I2C4_SDA       PAD_I2C4_SDA
#define PAD34_HS2_PAD_I2C2ICB0_SDA       PAD_I2C2ICB0_SDA
        
#define PAD35           35
#define PAD35_LS_GROUP  None
#define PAD35_HS0_PAD_QSPI1_CS_1       PAD_QSPI1_CS_1
#define PAD35_HS1_PAD_QSPI3_CS_1       PAD_QSPI3_CS_1
#define PAD35_HS2_PAD_QSPI5_CS_1       PAD_QSPI5_CS_1
        
#define PAD36           36
#define PAD36_LS_GROUP  None
#define PAD36_HS0_PAD_QSPI1_CS_2       PAD_QSPI1_CS_2
#define PAD36_HS1_PAD_QSPI3_CS_2       PAD_QSPI3_CS_2
#define PAD36_HS2_PAD_QSPI5_CS_2       PAD_QSPI5_CS_2
        
#define PAD37           37
#define PAD37_LS_GROUP  None
#define PAD37_HS0_PAD_QSPI1_CS_3       PAD_QSPI1_CS_3
#define PAD37_HS1_PAD_QSPI3_CS_3       PAD_QSPI3_CS_3
#define PAD37_HS2_PAD_QSPI5_CS_3       PAD_QSPI5_CS_3
        
#define PAD38           38
#define PAD38_LS_GROUP  None
#define PAD38_HS0_PAD_QSPI2_CS_1       PAD_QSPI2_CS_1
#define PAD38_HS1_PAD_QSPI4_CS_1       PAD_QSPI4_CS_1
#define PAD38_HS2_PAD_QSPI6_CS_1       PAD_QSPI6_CS_1
        
#define PAD39           39
#define PAD39_LS_GROUP  None
#define PAD39_HS0_PAD_QSPI2_CS_2       PAD_QSPI2_CS_2
#define PAD39_HS1_PAD_QSPI4_CS_2       PAD_QSPI4_CS_2
#define PAD39_HS2_PAD_QSPI6_CS_2       PAD_QSPI6_CS_2
        
#define PAD40           40
#define PAD40_LS_GROUP  None
#define PAD40_HS0_PAD_QSPI2_CS_3       PAD_QSPI2_CS_3
#define PAD40_HS1_PAD_QSPI4_CS_3       PAD_QSPI4_CS_3
#define PAD40_HS2_PAD_QSPI6_CS_3       PAD_QSPI6_CS_3
        
#define PAD41           41
#define PAD41_LS_GROUP  None
#define PAD41_HS0_PAD_QSPI1_DQ_4       PAD_QSPI1_DQ_4
#define PAD41_HS1_PAD_QSPI3_DQ_4       PAD_QSPI3_DQ_4
#define PAD41_HS2_PAD_QSPI5_DQ_4       PAD_QSPI5_DQ_4
        
#define PAD42           42
#define PAD42_LS_GROUP  None
#define PAD42_HS0_PAD_QSPI1_DQ_5       PAD_QSPI1_DQ_5
#define PAD42_HS1_PAD_QSPI3_DQ_5       PAD_QSPI3_DQ_5
#define PAD42_HS2_PAD_QSPI5_DQ_5       PAD_QSPI5_DQ_5
        
#define PAD43           43
#define PAD43_LS_GROUP  None
#define PAD43_HS0_PAD_QSPI1_DQ_6       PAD_QSPI1_DQ_6
#define PAD43_HS1_PAD_QSPI3_DQ_6       PAD_QSPI3_DQ_6
#define PAD43_HS2_PAD_QSPI5_DQ_6       PAD_QSPI5_DQ_6
        
#define PAD44           44
#define PAD44_LS_GROUP  None
#define PAD44_HS0_PAD_QSPI1_DQ_7       PAD_QSPI1_DQ_7
#define PAD44_HS1_PAD_QSPI3_DQ_7       PAD_QSPI3_DQ_7
#define PAD44_HS2_PAD_QSPI5_DQ_7       PAD_QSPI5_DQ_7
        
#define PAD45           45
#define PAD45_LS_GROUP  None
#define PAD45_HS0_PAD_QSPI2_DQ_4       PAD_QSPI2_DQ_4
#define PAD45_HS1_PAD_QSPI4_DQ_4       PAD_QSPI4_DQ_4
#define PAD45_HS2_PAD_QSPI6_DQ_4       PAD_QSPI6_DQ_4
        
#define PAD46           46
#define PAD46_LS_GROUP  None
#define PAD46_HS0_PAD_QSPI2_DQ_5       PAD_QSPI2_DQ_5
#define PAD46_HS1_PAD_QSPI4_DQ_5       PAD_QSPI4_DQ_5
#define PAD46_HS2_PAD_QSPI6_DQ_5       PAD_QSPI6_DQ_5
        
#define PAD47           47
#define PAD47_LS_GROUP  None
#define PAD47_HS0_PAD_QSPI2_DQ_6       PAD_QSPI2_DQ_6
#define PAD47_HS1_PAD_QSPI4_DQ_6       PAD_QSPI4_DQ_6
#define PAD47_HS2_PAD_QSPI6_DQ_6       PAD_QSPI6_DQ_6
        
#define PAD48           48
#define PAD48_LS_GROUP  None
#define PAD48_HS0_PAD_QSPI2_DQ_7       PAD_QSPI2_DQ_7
#define PAD48_HS1_PAD_QSPI4_DQ_7       PAD_QSPI4_DQ_7
#define PAD48_HS2_PAD_QSPI6_DQ_7       PAD_QSPI6_DQ_7
        /*
********************PAD Info End********************
*
********************LS Info Start********************
*
********************LS Info End********************
*/
#define QSPI1_SCK_IOF_OVAL     0
#define QSPI1_SCK_IOF_IVAL     0
#define QSPI1_SCK_HS_SEL       1
#define QSPI1_SCK_IOMUX_SEL    IOMUX
    
#define QSPI1_CS_0_IOF_OVAL     1
#define QSPI1_CS_0_IOF_IVAL     1
#define QSPI1_CS_0_HS_SEL       1
#define QSPI1_CS_0_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_0_IOF_OVAL     2
#define QSPI1_DQ_0_IOF_IVAL     2
#define QSPI1_DQ_0_HS_SEL       1
#define QSPI1_DQ_0_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_1_IOF_OVAL     3
#define QSPI1_DQ_1_IOF_IVAL     3
#define QSPI1_DQ_1_HS_SEL       1
#define QSPI1_DQ_1_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_2_IOF_OVAL     4
#define QSPI1_DQ_2_IOF_IVAL     4
#define QSPI1_DQ_2_HS_SEL       1
#define QSPI1_DQ_2_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_3_IOF_OVAL     5
#define QSPI1_DQ_3_IOF_IVAL     5
#define QSPI1_DQ_3_HS_SEL       1
#define QSPI1_DQ_3_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_4_IOF_OVAL     41
#define QSPI1_DQ_4_IOF_IVAL     41
#define QSPI1_DQ_4_HS_SEL       1
#define QSPI1_DQ_4_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_5_IOF_OVAL     42
#define QSPI1_DQ_5_IOF_IVAL     42
#define QSPI1_DQ_5_HS_SEL       1
#define QSPI1_DQ_5_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_6_IOF_OVAL     43
#define QSPI1_DQ_6_IOF_IVAL     43
#define QSPI1_DQ_6_HS_SEL       1
#define QSPI1_DQ_6_IOMUX_SEL    IOMUX
    
#define QSPI1_DQ_7_IOF_OVAL     44
#define QSPI1_DQ_7_IOF_IVAL     44
#define QSPI1_DQ_7_HS_SEL       1
#define QSPI1_DQ_7_IOMUX_SEL    IOMUX
    
#define   LS_SRC_SEL_OFS                  0x0000
#define   HS_CHNL_SEL_OFS                 0x4000
#define   LS_SRC0_IVAL_SEL_OFS            0x8000

#define   PHY_CNTRL_OFS                   0xC000
#define   CNTRL_SEL_OFS                   0x10000
#define   DBG_SRC_SEL_OFS                 0x14000
#define   DBG_SRC_SEL                     DBG_SRC_SEL_OFS
#define   IOF_LOOP_BACK_EN                 0x18000
#define   IOF_HWIOF_OVAL_SEL                0x1C000

#define   DI_VAL                BIT(0)
#define   DI_INV                BIT(1)
#define   IE_EN                 BIT(2)
#define   IE_INV                BIT(3)
#define   OE_EN                 BIT(4)
#define   OE_INV                BIT(5)
#define   DO_VAL                BIT(6)
#define   DO_INV                BIT(7)
#define   DS                    BITS(8,11)
#define   PD                    BIT(16)
#define   PU                    BIT(18)
#define   SPU                   BIT(19)
#define   ST                    BIT(20)
#define   SL                    BIT(21)
#define   ANALOG_EN             BIT(23)
#define   BUS_KEEP              BIT(24)

#define  IE_SEL                 BIT(0)
#define  PD_SEL                 BIT(1)
#define  PU_SEL                 BIT(2)
#define  OE_SEL                 BIT(3)
#define  DO_SEL_OFS             4UL
#define  BUS_KEEP_SEL           BIT(6)
         
#define DO_SEL(regval)                                  (BITS(4,5) & ((uint32_t)(regval) << 4))
#define DO_SEL_HS                                       DO_SEL(1)              /*!<  DO_SEL_HS*/
#define DO_SEL_PHY                                      DO_SEL(0)              /*!<  DO_SEL_PHY   */
#define DO_SEL_OE                                       DO_SEL(2)              /*!<  DO_SEL_OE */

#define BUS_KEEP_SEL    BIT(6)

void iomux_ls_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_ls_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
uint8_t iomux_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  pad_grp,uint8_t ls_per_grp);
uint8_t iomux_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  pad_grp,uint8_t ls_per_grp);
void iomux_ls_iof_pullup_cfg(unsigned long IO_MUX_BASE, uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_ls_iof_pulldown_cfg(unsigned long IO_MUX_BASE, uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_ls_iof_loopback_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_ival_num ,uint32_t PER_iof_oval_num ,uint32_t pad_num, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_pu_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pd_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pu_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pd_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pad_phy_ctrl(unsigned long IO_MUX_BASE, uint32_t pad_num ,uint32_t ctrl );
#ifdef __cplusplus
}
#endif
#endif
