/******************************************************************************
 * @file     ns.h
 * @brief    NMSIS Core Peripheral Access Layer Header File for
 *           ns SoC
 * @version  V1.00
 * @date     22. Nov 2019
 ******************************************************************************/
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
#ifndef __NS_H__
#define __NS_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Nuclei
  * @{
  */

/** @addtogroup NS
  * @{
  */

/** @addtogroup Configuration_of_NMSIS
  * @{
  */

/** \brief SoC Download mode definition */
typedef enum {
    DOWNLOAD_MODE_FLASHXIP = 0,         /*!< Flashxip download mode */
    DOWNLOAD_MODE_FLASH = 1,            /*!< Flash download mode */
    DOWNLOAD_MODE_ILM = 2,              /*!< ilm download mode */
    DOWNLOAD_MODE_DDR = 3,              /*!< ddr download mode */
    DOWNLOAD_MODE_MAX,
} DownloadMode_Type;

/** \brief CPU Hart ID Information */
typedef union  HartID_Info {
  struct {
  volatile  uint32_t  core_id :4;                  /*!< core ids */
  volatile  uint32_t  processor_id :6;             /*!< processor id */
  volatile  uint32_t  RECV:22;                     /*!< reserved */ 
  }b;
  volatile uint32_t d;
} HartID_Info_Typedef;

/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */
/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn
{
/* =======================================  Nuclei Core Specific Interrupt Numbers  ======================================== */

    Reserved0_IRQn            =   0,              /*!<  Internal reserved */
    Reserved1_IRQn            =   1,              /*!<  Internal reserved */
    Reserved2_IRQn            =   2,              /*!<  Internal reserved */
    SysTimerSW_IRQn           =   3,              /*!<  System Timer SW interrupt */
    Reserved3_IRQn            =   4,              /*!<  Internal reserved */
    Reserved4_IRQn            =   5,              /*!<  Internal reserved */
    Reserved5_IRQn            =   6,              /*!<  Internal reserved */
    SysTimer_IRQn             =   7,              /*!<  System Timer Interrupt */
    Reserved6_IRQn            =   8,              /*!<  Internal reserved */
    Reserved7_IRQn            =   9,              /*!<  Internal reserved */
    Reserved8_IRQn            =  10,              /*!<  Internal reserved */
    Reserved9_IRQn            =  11,              /*!<  Internal reserved */
    Reserved10_IRQn           =  12,              /*!<  Internal reserved */
    Reserved11_IRQn           =  13,              /*!<  Internal reserved */
    Reserved12_IRQn           =  14,              /*!<  Internal reserved */
    Reserved13_IRQn           =  15,              /*!<  Internal reserved */
    Reserved14_IRQn           =  16,              /*!<  Internal reserved */
    Reserved15_IRQn           =  17,              /*!<  Internal reserved */
    Reserved16_IRQn           =  18,              /*!<  Internal reserved */

/* ===========================================  NS Specific Interrupt Numbers  ========================================= */
/* ToDo: add here your device specific external interrupt numbers. 19~1023 is reserved number for user. Maxmum interrupt supported
         could get from clicinfo.NUM_INTERRUPT. According the interrupt handlers defined in startup_Device.s
         eg.: Interrupt for Timer#1       eclic_tim1_handler   ->   TIM1_IRQn */
    
    INTER_CORE_IRQn                   = 19,
    UDMA0_IRQn                   = 20,
    USART0_IRQn                   = 21,
    I3C0_IRQn                   = 22,
    I3C0_DMA_IRQn                   = 23,
    QSPI_XIP0_IRQn                   = 24,
    QSPI1_IRQn                   = 25,
    SDIO0_IRQn                   = 26,
    SDIO0_DMA_IRQn                   = 27,
    Reserved17_IRQn  =  28,           /*!<  Internal reserved */
    Reserved18_IRQn  =  29,           /*!<  Internal reserved */
    Reserved19_IRQn  =  30,           /*!<  Internal reserved */
    Reserved20_IRQn  =  31,           /*!<  Internal reserved */
    Reserved21_IRQn  =  32,           /*!<  Internal reserved */
    Reserved22_IRQn  =  33,           /*!<  Internal reserved */
    Reserved23_IRQn  =  34,           /*!<  Internal reserved */
    Reserved24_IRQn  =  35,           /*!<  Internal reserved */
    Reserved25_IRQn  =  36,           /*!<  Internal reserved */
    Reserved26_IRQn  =  37,           /*!<  Internal reserved */
    Reserved27_IRQn  =  38,           /*!<  Internal reserved */
    Reserved28_IRQn  =  39,           /*!<  Internal reserved */
    Reserved29_IRQn  =  40,           /*!<  Internal reserved */
    Reserved30_IRQn  =  41,           /*!<  Internal reserved */
    Reserved31_IRQn  =  42,           /*!<  Internal reserved */
    Reserved32_IRQn  =  43,           /*!<  Internal reserved */
    Reserved33_IRQn  =  44,           /*!<  Internal reserved */
    Reserved34_IRQn  =  45,           /*!<  Internal reserved */
    Reserved35_IRQn  =  46,           /*!<  Internal reserved */
    Reserved36_IRQn  =  47,           /*!<  Internal reserved */
    Reserved37_IRQn  =  48,           /*!<  Internal reserved */
    Reserved38_IRQn  =  49,           /*!<  Internal reserved */
    Reserved39_IRQn  =  50,           /*!<  Internal reserved */
    Reserved40_IRQn  =  51,           /*!<  Internal reserved */
    Reserved41_IRQn  =  52,           /*!<  Internal reserved */
    Reserved42_IRQn  =  53,           /*!<  Internal reserved */
    Reserved43_IRQn  =  54,           /*!<  Internal reserved */
    Reserved44_IRQn  =  55,           /*!<  Internal reserved */
    Reserved45_IRQn  =  56,           /*!<  Internal reserved */
    Reserved46_IRQn  =  57,           /*!<  Internal reserved */
    Reserved47_IRQn  =  58,           /*!<  Internal reserved */
    Reserved48_IRQn  =  59,           /*!<  Internal reserved */
    Reserved49_IRQn  =  60,           /*!<  Internal reserved */
    Reserved50_IRQn  =  61,           /*!<  Internal reserved */
    Reserved51_IRQn  =  62,           /*!<  Internal reserved */
    Reserved52_IRQn  =  63,           /*!<  Internal reserved */
    Reserved53_IRQn  =  64,           /*!<  Internal reserved */
    Reserved54_IRQn  =  65,           /*!<  Internal reserved */
    Reserved55_IRQn  =  66,           /*!<  Internal reserved */
    Reserved56_IRQn  =  67,           /*!<  Internal reserved */
    Reserved57_IRQn  =  68,           /*!<  Internal reserved */
    Reserved58_IRQn  =  69,           /*!<  Internal reserved */
    Reserved59_IRQn  =  70,           /*!<  Internal reserved */
    Reserved60_IRQn  =  71,           /*!<  Internal reserved */
    Reserved61_IRQn  =  72,           /*!<  Internal reserved */
    Reserved62_IRQn  =  73,           /*!<  Internal reserved */
    Reserved63_IRQn  =  74,           /*!<  Internal reserved */
    Reserved64_IRQn  =  75,           /*!<  Internal reserved */
    Reserved65_IRQn  =  76,           /*!<  Internal reserved */
    Reserved66_IRQn  =  77,           /*!<  Internal reserved */
    Reserved67_IRQn  =  78,           /*!<  Internal reserved */
    Reserved68_IRQn  =  79,           /*!<  Internal reserved */
    Reserved69_IRQn  =  80,           /*!<  Internal reserved */
    Reserved70_IRQn  =  81,           /*!<  Internal reserved */
    Reserved71_IRQn  =  82,           /*!<  Internal reserved */
    Reserved72_IRQn  =  83,           /*!<  Internal reserved */
    Reserved73_IRQn  =  84,           /*!<  Internal reserved */
    Reserved74_IRQn  =  85,           /*!<  Internal reserved */
    Reserved75_IRQn  =  86,           /*!<  Internal reserved */
    Reserved76_IRQn  =  87,           /*!<  Internal reserved */
    Reserved77_IRQn  =  88,           /*!<  Internal reserved */
    Reserved78_IRQn  =  89,           /*!<  Internal reserved */
    Reserved79_IRQn  =  90,           /*!<  Internal reserved */
    Reserved80_IRQn  =  91,           /*!<  Internal reserved */
    Reserved81_IRQn  =  92,           /*!<  Internal reserved */
    Reserved82_IRQn  =  93,           /*!<  Internal reserved */
    Reserved83_IRQn  =  94,           /*!<  Internal reserved */
    Reserved84_IRQn  =  95,           /*!<  Internal reserved */
    Reserved85_IRQn  =  96,           /*!<  Internal reserved */
    Reserved86_IRQn  =  97,           /*!<  Internal reserved */
    Reserved87_IRQn  =  98,           /*!<  Internal reserved */
    Reserved88_IRQn  =  99,           /*!<  Internal reserved */
    Reserved89_IRQn  =  100,           /*!<  Internal reserved */
    Reserved90_IRQn  =  101,           /*!<  Internal reserved */
    Reserved91_IRQn  =  102,           /*!<  Internal reserved */
    Reserved92_IRQn  =  103,           /*!<  Internal reserved */
    Reserved93_IRQn  =  104,           /*!<  Internal reserved */
    Reserved94_IRQn  =  105,           /*!<  Internal reserved */
    Reserved95_IRQn  =  106,           /*!<  Internal reserved */
    Reserved96_IRQn  =  107,           /*!<  Internal reserved */
    Reserved97_IRQn  =  108,           /*!<  Internal reserved */
    Reserved98_IRQn  =  109,           /*!<  Internal reserved */
    Reserved99_IRQn  =  110,           /*!<  Internal reserved */
    Reserved100_IRQn  =  111,           /*!<  Internal reserved */
    Reserved101_IRQn  =  112,           /*!<  Internal reserved */
    Reserved102_IRQn  =  113,           /*!<  Internal reserved */
    Reserved103_IRQn  =  114,           /*!<  Internal reserved */
    Reserved104_IRQn  =  115,           /*!<  Internal reserved */
    Reserved105_IRQn  =  116,           /*!<  Internal reserved */
    Reserved106_IRQn  =  117,           /*!<  Internal reserved */
    Reserved107_IRQn  =  118,           /*!<  Internal reserved */
    Reserved108_IRQn  =  119,           /*!<  Internal reserved */
    Reserved109_IRQn  =  120,           /*!<  Internal reserved */
    Reserved110_IRQn  =  121,           /*!<  Internal reserved */
    Reserved111_IRQn  =  122,           /*!<  Internal reserved */
    Reserved112_IRQn  =  123,           /*!<  Internal reserved */
    Reserved113_IRQn  =  124,           /*!<  Internal reserved */
    Reserved114_IRQn  =  125,           /*!<  Internal reserved */
    Reserved115_IRQn  =  126,           /*!<  Internal reserved */
    Reserved116_IRQn  =  127,           /*!<  Internal reserved */

    SOC_INT_MAX,
} IRQn_Type;
/**
  * @}
  */

/** @addtogroup Exception_Code_Definition
  * @{
  */
/* =========================================================================================================================== */
/* ================                                  Exception Code Definition                                ================ */
/* =========================================================================================================================== */

typedef enum EXCn {
/* =======================================  Nuclei N/NX Specific Exception Code  ======================================== */
    InsUnalign_EXCn          =   0,              /*!<  Instruction address misaligned */
    InsAccFault_EXCn         =   1,              /*!<  Instruction access fault */
    IlleIns_EXCn             =   2,              /*!<  Illegal instruction */
    Break_EXCn               =   3,              /*!<  Beakpoint */
    LdAddrUnalign_EXCn       =   4,              /*!<  Load address misaligned */
    LdFault_EXCn             =   5,              /*!<  Load access fault */
    StAddrUnalign_EXCn       =   6,              /*!<  Store or AMO address misaligned */
    StAccessFault_EXCn       =   7,              /*!<  Store or AMO access fault */
    UmodeEcall_EXCn          =   8,              /*!<  Environment call from User mode */
    MmodeEcall_EXCn          =  11,              /*!<  Environment call from Machine mode */
    NMI_EXCn                 = 0xfff,            /*!<  NMI interrupt */
} EXCn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ToDo: set the defines according your Device */
/* ToDo: define the correct core revision */
#if __riscv_xlen == 32

#ifndef __NUCLEI_CORE_REV
#define __NUCLEI_N_REV            0x0104    /*!< Core Revision r1p4 */
#else
#define __NUCLEI_N_REV            __NUCLEI_CORE_REV
#endif
typedef uint32_t addr_xlen   ;

#elif __riscv_xlen == 64

#ifndef __NUCLEI_CORE_REV
#define __NUCLEI_NX_REV           0x0100    /*!< Core Revision r1p0 */
#else
#define __NUCLEI_NX_REV           __NUCLEI_CORE_REV
#endif
typedef uint64_t addr_xlen   ;
#endif /* __riscv_xlen == 64 */

extern volatile HartID_Info_Typedef hart_id;
/* ToDo: define the correct core features for the  */
#define __ECLIC_PRESENT           1                     /*!< Set to 1 if ECLIC is present */
#define __ECLIC_BASEADDR          0xc020000UL  /*!< Set to ECLIC baseaddr of your device */
#define __ECLIC_INTNUM            None        /*!< Set to 1 - 1024, total interrupt number of ECLIC Unit */

#define __PLIC_PRESENT            0                     /*!< Set to 1 if PLIC is present */

#define __SYSTIMER_PRESENT        1                     /*!< Set to 1 if System Timer is present */
#define __SYSTIMER_BASEADDR       0x0c030000UL  /*!< Set to SysTimer baseaddr of your device */
#define __CLUSTER_IDU_PRESENT     0                       /*!< Set to 1 if cluster idu Unit is present */
/*!< Set to 0, 1, or 2, 0 not present, 1 single floating point unit present, 2 double floating point unit present */
#if !defined(__riscv_flen)
#define __FPU_PRESENT             0
#elif __riscv_flen == 32
#define __FPU_PRESENT             1
#else
#define __FPU_PRESENT             2
#endif

/* __riscv_bitmanip/__riscv_dsp/__riscv_vector is introduced
 * in nuclei gcc 10.2 when b/p/v extension compiler option is selected.
 * For example:
 * -march=rv32imacb -mabi=ilp32 : __riscv_bitmanip macro will be defined
 * -march=rv32imacp -mabi=ilp32 : __riscv_dsp macro will be defined
 * -march=rv64imacv -mabi=lp64 : __riscv_vector macro will be defined
 */
#if defined(__riscv_bitmanip)
#define __BITMANIP_PRESENT        1                     /*!< Set to 1 if Bitmainpulation extension is present */
#else
#define __BITMANIP_PRESENT        0                     /*!< Set to 1 if Bitmainpulation extension is present */
#endif
#if defined(__riscv_dsp)
#define __DSP_PRESENT             1                     /*!< Set to 1 if Partial SIMD(DSP) extension is present */
#else
#define __DSP_PRESENT             0                     /*!< Set to 1 if Partial SIMD(DSP) extension is present */
#endif
#if defined(__riscv_vector)
#define __VECTOR_PRESENT          1                     /*!< Set to 1 if Vector extension is present */
#else
#define __VECTOR_PRESENT          0                     /*!< Set to 1 if Vector extension is present */
#endif

#define __PMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __PMP_ENTRY_NUM           8               /*!< Set to 8 or 16, the number of PMP entries */

#define __ICACHE_PRESENT          1                  /*!< Set to 1 if I-Cache is present */

#define __DCACHE_PRESENT          1                  /*!< Set to 1 if D-Cache is present */
#define __CCM_PRESENT           1                       /*!< Set to 1 if Cache Control and Mantainence Unit is present */
#define __Vendor_SysTickConfig    0                            /*!< Set to 1 if different SysTick Config is used */
#define __Vendor_EXCEPTION        0                            /*!< Set to 1 if vendor exception hander is present */

/** @} */ /* End of group Configuration_of_NMSIS */

#include <nmsis_core.h>                            /*!< Nuclei N/NX class processor and core peripherals */
/* ToDo: include your system_ns.h file
         replace 'Device' with your device name */
#include "system_ns.h"                    /*!< ns System */

/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

#define RTC_FREQ 32768

#define SOC_TIMER_FREQ RTC_FREQ
/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
  * @
  */

/****************************************************************************
 * TODO: Platform definitions
 *****************************************************************************/

#define SOC_ECLIC_NUM_INTERRUPTS    None

#define SOC_MTIMER_HANDLER          eclic_mtip_handler
#define SOC_SOFTINT_HANDLER         eclic_msip_handler

#define GPIO_BIT_ALL_ZERO           (0x0)
#define GPIO_BIT_ALL_ONE            (0xFFFFFFFF)

/* enum definitions */
typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} EventStatus, ControlStatus, FunctionalState;

typedef enum {
    FALSE = 0,
    TRUE = !FALSE
} BOOL;

typedef enum {
    RESET = 0,
    SET = 1,
    MAX = 0X7FFFFFFF
} FlagStatus;

typedef enum {
    ERROR = 0,
    SUCCESS = !ERROR
} ErrStatus;

/**
  * \brief  UDMA PeriphCfg Structure definition
  */
typedef struct {
    __IOM uint32_t RX_SADDR;                /*!< RX CHannel UDMA Transfer Address of Associated Buffer */
    __IOM uint32_t RX_SIZE;                 /*!< RX CHannel UDMA Transfer Size of Buffer */
    __IOM uint32_t RX_CFG;                  /*!< RX CHannel UDMA Transfer Configuration */
    __IOM uint32_t TX_SADDR;                /*!< TX CHannel UDMA Transfer Address of Associated Buffer */
    __IOM uint32_t TX_SIZE;                 /*!< TX CHannel UDMA Transfer Size of Buffer */
    __IOM uint32_t TX_CFG;                  /*!< TX CHannel UDMA Transfer Configuration */
    __IOM uint32_t CMD_SADDR;               /*!< TX CHannel UDMA Transfer Address of Associated Buffer */
    __IOM uint32_t CMD_SIZE;                /*!< TX CHannel UDMA Transfer Size of Buffer */
    __IOM uint32_t CMD_CFG;                 /*!< TX CHannel UDMA Transfer Configuration */
} UDMA_PeriphCfg_TypeDef;

/**
  * \brief  UDMA PeriphCfg Structure definition
  */
typedef struct {
    __IOM uint32_t RX_SADDR_H;                /*!< RX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
    __IOM uint32_t TX_SADDR_H;                /*!< TX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
    __IOM uint32_t CMD_SADDR_H;               /*!< CMD CHannel UDMA Transfer High 32bit Address of Associated Buffer */
} UDMA_PeriphCfg_CMD_Hi_TypeDef;

/**
  * \brief  UDMA PeriphCfg Structure definition
  */
typedef struct {
    __IOM uint32_t RX_SADDR_H;                /*!< RX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
    __IOM uint32_t TX_SADDR_H;                /*!< TX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
} UDMA_PeriphCfg_Hi_TypeDef;

typedef struct {
    volatile uint32_t rx_saddr;   /**< RX CHannel uDMA transfer address of associated buffer */
    volatile uint32_t rx_size;    /**< RX CHannel uDMA transfer size of buffer */
    volatile uint32_t rx_cfg;     /**< RX CHannel uDMA transfer configuration */
    volatile uint32_t tx_saddr;   /**<  TX CHannel uDMA transfer address of associated buffer */
    volatile uint32_t tx_size;    /**<  TX CHannel uDMA transfer size of buffer */
    volatile uint32_t tx_cfg;     /**<  TX CHannel uDMA transfer configuration */
    volatile uint32_t cmd_saddr;  /**<  TX CHannel uDMA transfer address of associated buffer */
    volatile uint32_t cmd_size;   /**<  TX CHannel uDMA transfer size of buffer */
    volatile uint32_t cmd_cfg;    /**<  TX CHannel uDMA transfer configuration */
} udma_core_t;

typedef struct {
    __IOM uint32_t CMD_SADDR;   /**< cmd UDMA buffer transfer address register, offset: 0x10 */
    __IOM uint32_t CMD_SIZE;    /**< cmd UDMA buffer transfer size register, offset: 0x14 */
    __IOM uint32_t CMD_CFG;     /**< cmd UDMA transfer configuration register, offset: 0x18 */
} cmd_t;

typedef enum {
    RX_CHANNEL      = 0,
    TX_CHANNEL      = 1,
    COMMAND_CHANNEL = 2
} udma_channel_e;

/* ----------------------------------------------------------------------------
   -- UDMA Common Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UDMA_Register_Masks UDMA Register Masks
 * @
 */
/*! @name RX_SADDR - RX TX UDMA buffer transfer address register */

/**
  * @brief UDMA (UDMA)
  */
typedef struct {
    __IOM uint32_t CG;
    __IOM uint32_t EVENTID;
} UDMA_TypeDef;

typedef struct {
    __IOM uint32_t MSRCADDR;
    __IOM uint32_t MDSTADDR;
    __IOM uint32_t MCTRL;
    __IOM uint32_t RPT;
    __IOM uint32_t MSIZE;
    __IOM uint32_t MSRCADDR_H;
    __IOM uint32_t MDSTADDR_H;
} UDMA_CHx_Cfg_TypeDef;

typedef struct {
    __IOM uint32_t MSRCADDR;
    __IOM uint32_t MDSTADDR;
    __IOM uint32_t MCTRL;
    __IOM uint32_t MSIZE;
    __IOM uint32_t MSRCADDR_H;
    __IOM uint32_t MDSTADDR_H;
} UDMA_PA_CHx_Cfg_TypeDef;

typedef struct {
    __IOM uint32_t CHX_IRQ_EN;
    __IOM uint32_t CHX_IRQ_STAT;
    __IOM uint32_t CHX_IRQ_CLR;
} UDMA_CHx_Irq_TypeDef;

/**
  * \brief  i3c Register Structure definition
  */
typedef struct {
  __IOM uint32_t  RX_SADDR;                                 /*!< Offset: 0x0 RW RX SADDR Register */
  __IOM uint32_t  RX_SIZE;                                  /*!< Offset: 0x4 RW RX SIZE Register */
  __IOM uint32_t  RX_CFG;                                   /*!< Offset: 0x8 RW RX CFG Register */
  __IOM uint32_t  TX_SADDR;                                 /*!< Offset: 0xc RW TX SADDR Register */
  __IOM uint32_t  TX_SIZE;                                  /*!< Offset: 0x10 RW TX SIZE Register */
  __IOM uint32_t  TX_CFG;                                   /*!< Offset: 0x14 RW TX CFG Register */
  __IOM uint32_t  CMD_SADDR;                                /*!< Offset: 0x18 RW CMD SADDR Register */
  __IOM uint32_t  CMD_SIZE;                                 /*!< Offset: 0x1c RW CMD SIZE Register */
  __IOM uint32_t  CMD_CFG;                                  /*!< Offset: 0x20 RW CMD CFG Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x24 RW STATUS Register */
  __IOM uint32_t  DIV;                                      /*!< Offset: 0x28 RW DIV Register */
  __IOM uint32_t  SETUP;                                    /*!< Offset: 0x2c RW SETUP Register */
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x30 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x34 RW RXDATA Register */
  __IOM uint32_t  INT_IE;                                   /*!< Offset: 0x38 RW INT IE Register */
  __IOM uint32_t  SLAVE_ADDRESS;                            /*!< Offset: 0x3c RW SLAVE ADDRESS Register */
  __IOM uint32_t  INT_CLR;                                  /*!< Offset: 0x40 RW INT CLR Register */
  __IOM uint32_t  TXE_TIME;                                 /*!< Offset: 0x44 RW TXE TIME Register */
  __IOM uint32_t  TIME_OUT;                                 /*!< Offset: 0x48 RW TIME OUT Register */
  __IOM uint32_t  PID_LOW;                                  /*!< Offset: 0x4c RW PID LOW Register */
  __IOM uint32_t  PID_HIGH;                                 /*!< Offset: 0x50 RW PID HIGH Register */
  __IOM uint32_t  MASTER_BCR;                               /*!< Offset: 0x54 RW MASTER BCR Register */
  __IOM uint32_t  MASTER_DCR;                               /*!< Offset: 0x58 RW MASTER DCR Register */
  __IOM uint32_t  TSCO;                                     /*!< Offset: 0x5c RW TSCO Register */
  __IOM uint32_t  DMA_SLV_TX_TSU;                           /*!< Offset: 0x60 RW DMA SLV TX TSU Register */
  __IOM uint32_t  DMA_RX_DATASIZE;                          /*!< Offset: 0x64 RW DMA RX DATASIZE Register */
  __IOM uint32_t  DMA_TX_DATASIZE;                          /*!< Offset: 0x68 RW DMA TX DATASIZE Register */
  __IM  uint32_t  RESERVECD27[11];                          /*!< Offset: 0x6c RO RESERVECD27[11] Register */
  __IOM uint32_t  DEBUG;                                    /*!< Offset: 0x98 RW DEBUG Register */
  __IOM uint32_t  IP_VERSION;                               /*!< Offset: 0x9c RW IP VERSION Register */
}I3C_TypeDef;

/**
  * \brief  usart Register Structure definition
  */
typedef struct {
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x0 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x4 RW RXDATA Register */
  __IOM uint32_t  TXCTRL;                                   /*!< Offset: 0x8 RW TXCTRL Register */
  __IOM uint32_t  RXCTRL;                                   /*!< Offset: 0xc RW RXCTRL Register */
  __IOM uint32_t  INT_EN;                                   /*!< Offset: 0x10 RW INT EN Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x14 RW STATUS Register */
  __IOM uint32_t  DIV;                                      /*!< Offset: 0x18 RW DIV Register */
  __IOM uint32_t  SETUP;                                    /*!< Offset: 0x1c RW SETUP Register */
  __IM  uint32_t  RESERVECD8[1];                            /*!< Offset: 0x20 RO RESERVECD8[1] Register */
  __IOM uint32_t  RX_SIZE;                                  /*!< Offset: 0x24 RW RX SIZE Register */
  __IM  uint32_t  RESERVECD10[2];                           /*!< Offset: 0x28 RO RESERVECD10[2] Register */
  __IOM uint32_t  TX_SIZE;                                  /*!< Offset: 0x30 RW TX SIZE Register */
  __IM  uint32_t  RESERVECD13[4];                           /*!< Offset: 0x34 RO RESERVECD13[4] Register */
  __IOM uint32_t  RX_IDLE;                                  /*!< Offset: 0x44 RW RX IDLE Register */
  __IOM uint32_t  RX_WM;                                    /*!< Offset: 0x48 RW RX WM Register */
  __IOM uint32_t  RX_FIFO_LEFT_ENTRY;                       /*!< Offset: 0x4c RW RX FIFO LEFT ENTRY Register */
  __IOM uint32_t  TX_FIFO_LEFT_ENTRY;                       /*!< Offset: 0x50 RW TX FIFO LEFT ENTRY Register */
  __IOM uint32_t  TX_DATASIZE;                              /*!< Offset: 0x54 RW TX DATASIZE Register */
  __IOM uint32_t  RX_DATASIZE;                              /*!< Offset: 0x58 RW RX DATASIZE Register */
  __IOM uint32_t  RX_LOW_LEVEL_CNT;                         /*!< Offset: 0x5c RW RX LOW LEVEL CNT Register */
  __IM  uint32_t  RESERVECD24[8];                           /*!< Offset: 0x60 RO RESERVECD24[8] Register */
  __IOM uint32_t  IP_VERSION;                               /*!< Offset: 0x80 RW IP VERSION Register */
}USART_TypeDef;

/**
  * \brief  qspi_xip Register Structure definition
  */
typedef struct {
  __IOM uint32_t  SCKDIV;                                   /*!< Offset: 0x0 RW SCKDIV Register */
  __IOM uint32_t  SCKMODE;                                  /*!< Offset: 0x4 RW SCKMODE Register */
  __IOM uint32_t  SCKSAMPLE;                                /*!< Offset: 0x8 RW SCKSAMPLE Register */
  __IOM uint32_t  FORCE;                                    /*!< Offset: 0xc RW FORCE Register */
  __IOM uint32_t  CSID;                                     /*!< Offset: 0x10 RW CSID Register */
  __IOM uint32_t  CSDEF;                                    /*!< Offset: 0x14 RW CSDEF Register */
  __IOM uint32_t  CSMODE;                                   /*!< Offset: 0x18 RW CSMODE Register */
  __IOM uint32_t  VERSION;                                  /*!< Offset: 0x1c RW VERSION Register */
  __IOM uint32_t  ADDR_WRAP;                                /*!< Offset: 0x20 RW ADDR WRAP Register */
  __IOM uint32_t  BOUNDARY_CFG;                             /*!< Offset: 0x24 RW BOUNDARY CFG Register */
  __IOM uint32_t  DELAY0;                                   /*!< Offset: 0x28 RW DELAY0 Register */
  __IOM uint32_t  DELAY1;                                   /*!< Offset: 0x2c RW DELAY1 Register */
  __IOM uint32_t  FIFO_NUM;                                 /*!< Offset: 0x30 RW FIFO NUM Register */
  __IOM uint32_t  TSIZE;                                    /*!< Offset: 0x34 RW TSIZE Register */
  __IOM uint32_t  RSIZE;                                    /*!< Offset: 0x38 RW RSIZE Register */
  __IM  uint32_t  RESERVECD15[1];                           /*!< Offset: 0x3c RO RESERVECD15[1] Register */
  __IOM uint32_t  FMT;                                      /*!< Offset: 0x40 RW FMT Register */
  __IM  uint32_t  RESERVECD17[1];                           /*!< Offset: 0x44 RO RESERVECD17[1] Register */
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x48 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x4c RW RXDATA Register */
  __IOM uint32_t  TX_MARK;                                  /*!< Offset: 0x50 RW TX MARK Register */
  __IOM uint32_t  RX_MARK;                                  /*!< Offset: 0x54 RW RX MARK Register */
  __IM  uint32_t  RESERVECD22[2];                           /*!< Offset: 0x58 RO RESERVECD22[2] Register */
  __IOM uint32_t  FCTRL;                                    /*!< Offset: 0x60 RW FCTRL Register */
  __IOM uint32_t  FFMT;                                     /*!< Offset: 0x64 RW FFMT Register */
  __IM  uint32_t  RESERVECD26[2];                           /*!< Offset: 0x68 RO RESERVECD26[2] Register */
  __IOM uint32_t  IE;                                       /*!< Offset: 0x70 RW IE Register */
  __IOM uint32_t  IP;                                       /*!< Offset: 0x74 RW IP Register */
  __IOM uint32_t  FFMT1;                                    /*!< Offset: 0x78 RW FFMT1 Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x7c RW STATUS Register */
  __IOM uint32_t  RXEDGE;                                   /*!< Offset: 0x80 RW RXEDGE Register */
  __IOM uint32_t  CR;                                       /*!< Offset: 0x84 RW CR Register */
}QSPI_XIP_TypeDef;

/**
  * \brief  qspi Register Structure definition
  */
typedef struct {
  __IOM uint32_t  SCKDIV;                                   /*!< Offset: 0x0 RW SCKDIV Register */
  __IOM uint32_t  SCKMODE;                                  /*!< Offset: 0x4 RW SCKMODE Register */
  __IOM uint32_t  SCKSAMPLE;                                /*!< Offset: 0x8 RW SCKSAMPLE Register */
  __IOM uint32_t  FORCE;                                    /*!< Offset: 0xc RW FORCE Register */
  __IOM uint32_t  CSID;                                     /*!< Offset: 0x10 RW CSID Register */
  __IOM uint32_t  CSDEF;                                    /*!< Offset: 0x14 RW CSDEF Register */
  __IOM uint32_t  CSMODE;                                   /*!< Offset: 0x18 RW CSMODE Register */
  __IOM uint32_t  VERSION;                                  /*!< Offset: 0x1c RW VERSION Register */
  __IM  uint32_t  RESERVECD8[2];                            /*!< Offset: 0x20 RO RESERVECD8[2] Register */
  __IOM uint32_t  DELAY0;                                   /*!< Offset: 0x28 RW DELAY0 Register */
  __IOM uint32_t  DELAY1;                                   /*!< Offset: 0x2c RW DELAY1 Register */
  __IOM uint32_t  FIFO_NUM;                                 /*!< Offset: 0x30 RW FIFO NUM Register */
  __IOM uint32_t  TSIZE;                                    /*!< Offset: 0x34 RW TSIZE Register */
  __IOM uint32_t  RSIZE;                                    /*!< Offset: 0x38 RW RSIZE Register */
  __IM  uint32_t  RESERVECD15[1];                           /*!< Offset: 0x3c RO RESERVECD15[1] Register */
  __IOM uint32_t  FMT;                                      /*!< Offset: 0x40 RW FMT Register */
  __IM  uint32_t  RESERVECD17[1];                           /*!< Offset: 0x44 RO RESERVECD17[1] Register */
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x48 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x4c RW RXDATA Register */
  __IOM uint32_t  TX_MARK;                                  /*!< Offset: 0x50 RW TX MARK Register */
  __IOM uint32_t  RX_MARK;                                  /*!< Offset: 0x54 RW RX MARK Register */
  __IM  uint32_t  RESERVECD22[6];                           /*!< Offset: 0x58 RO RESERVECD22[6] Register */
  __IOM uint32_t  IE;                                       /*!< Offset: 0x70 RW IE Register */
  __IOM uint32_t  IP;                                       /*!< Offset: 0x74 RW IP Register */
  __IM  uint32_t  RESERVECD30[1];                           /*!< Offset: 0x78 RO RESERVECD30[1] Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x7c RW STATUS Register */
  __IOM uint32_t  RXEDGE;                                   /*!< Offset: 0x80 RW RXEDGE Register */
  __IOM uint32_t  CR;                                       /*!< Offset: 0x84 RW CR Register */
}QSPI_TypeDef;

typedef struct {
    __IOM uint32_t SYSRESET;
    __IOM uint32_t NMI;
    __IOM uint32_t DCLS;
} MISC_CTL_TypeDef;

typedef struct {
    __IOM uint32_t CR1;                          /*!< Offset: 0x000 (R/W) ADV-TIMER Control register 1 */
    __IOM uint32_t CR2;                          /*!< Offset: 0x004 (R/W) ADV-TIMER Control register 2 */
    __IOM uint32_t SMCR;                         /*!< Offset: 0x008 (R/W) ADV-TIMER Slave mode contorl register */
    __IOM uint32_t DIER;                         /*!< Offset: 0x00C (R/W) ADV-TIMER DMA/interrupt enable register */
    __IOM uint32_t SR;                           /*!< Offset: 0x010 (R/W) ADV-TIMER Status register */
    __IOM uint32_t EGR;                          /*!< Offset: 0x014 (R/W) ADV-TIMER Event generation register */
    __IOM uint32_t CCMR1;                        /*!< Offset: 0x018 (R/W) ADV-TIMER Capture/compare mode register 1 */
    __IOM uint32_t CCMR2;                        /*!< Offset: 0x01C (R/W) ADV-TIMER Capture/compare mode register 2 */
    __IOM uint32_t CCER;                         /*!< Offset: 0x020 (R/W) ADV-TIMER Capture/compare enable register */
    __IOM uint32_t CNT;                          /*!< Offset: 0x024 (R/W) ADV-TIMER Counter register */
    __IOM uint32_t PSC;                          /*!< Offset: 0x028 (R/W) ADV-TIMER Perscaler register */
    __IOM uint32_t ARR;                          /*!< Offset: 0x02C (R/W) ADV-TIMER Auto-reload register */
    __IOM uint32_t RCR;                          /*!< Offset: 0x030 (R/W) ADV-TIMER Repetition counter register */
    __IOM uint32_t CCR1;                         /*!< Offset: 0x034 (R/W) ADV-TIMER Capture/compare register 1 */
    __IOM uint32_t CCR2;                         /*!< Offset: 0x038 (R/W) ADV-TIMER Capture/compare register 2 */
    __IOM uint32_t CCR3;                         /*!< Offset: 0x03C (R/W) ADV-TIMER Capture/compare register 3 */
    __IOM uint32_t CCR4;                         /*!< Offset: 0x040 (R/W) ADV-TIMER Capture/compare register 4 */
    __IOM uint32_t BDTR;                         /*!< Offset: 0x044 (R/W) ADV-TIMER Break and dead-time register */
    __IOM uint32_t DCR;                          /*!< Offset: 0x048 (R/W) ADV-TIMER DMA control register */
    __IOM uint32_t DMAR;                         /*!< Offset: 0x04C (R/W) ADV-TIMER DMA address for full transfer */
    __IOM uint32_t VER;                          /*!< Offset: 0x050 (R/W) ADV-TIMER IP version register */
} ADVANCED_TIMER_TypeDef;

/**
  * \brief  sdio Register Structure definition
  */
typedef struct {
  __IOM uint32_t  RX_SADDR;                                 /*!< Offset: 0x0 RW RX SADDR Register */
  __IOM uint32_t  RX_SIZE;                                  /*!< Offset: 0x4 RW RX SIZE Register */
  __IOM uint32_t  RX_CFG;                                   /*!< Offset: 0x8 RW RX CFG Register */
  __IOM uint32_t  CR;                                       /*!< Offset: 0xc RW CR Register */
  __IOM uint32_t  TX_SADDR;                                 /*!< Offset: 0x10 RW TX SADDR Register */
  __IOM uint32_t  TX_SIZE;                                  /*!< Offset: 0x14 RW TX SIZE Register */
  __IOM uint32_t  TX_CFG;                                   /*!< Offset: 0x18 RW TX CFG Register */
  __IOM uint32_t  VERSION;                                  /*!< Offset: 0x1c RW VERSION Register */
  __IOM uint32_t  CMD_OP;                                   /*!< Offset: 0x20 RW CMD OP Register */
  __IOM uint32_t  CMD_ARG;                                  /*!< Offset: 0x24 RW CMD ARG Register */
  __IOM uint32_t  DATA_SETUP;                               /*!< Offset: 0x28 RW DATA SETUP Register */
  __IOM uint32_t  START;                                    /*!< Offset: 0x2c RW START Register */
  __IOM uint32_t  RSP0;                                     /*!< Offset: 0x30 RW RSP0 Register */
  __IOM uint32_t  RSP1;                                     /*!< Offset: 0x34 RW RSP1 Register */
  __IOM uint32_t  RSP2;                                     /*!< Offset: 0x38 RW RSP2 Register */
  __IOM uint32_t  RSP3;                                     /*!< Offset: 0x3c RW RSP3 Register */
  __IOM uint32_t  CLK_DIV;                                  /*!< Offset: 0x40 RW CLK DIV Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x44 RW STATUS Register */
  __IOM uint32_t  STOP_CMD_OP;                              /*!< Offset: 0x48 RW STOP CMD OP Register */
  __IOM uint32_t  STOP_CMD_ARG;                             /*!< Offset: 0x4c RW STOP CMD ARG Register */
  __IOM uint32_t  DATA_TIMEOUT;                             /*!< Offset: 0x50 RW DATA TIMEOUT Register */
  __IOM uint32_t  CMD_POWERUP;                              /*!< Offset: 0x54 RW CMD POWERUP Register */
  __IOM uint32_t  CMD_WAIT_RSP;                             /*!< Offset: 0x58 RW CMD WAIT RSP Register */
  __IOM uint32_t  CMD_WAIT_EOT;                             /*!< Offset: 0x5c RW CMD WAIT EOT Register */
  __IOM uint32_t  TX_DATA;                                  /*!< Offset: 0x60 RW TX DATA Register */
  __IOM uint32_t  RX_DATA;                                  /*!< Offset: 0x64 RW RX DATA Register */
  __IOM uint32_t  TX_MARK;                                  /*!< Offset: 0x68 RW TX MARK Register */
  __IOM uint32_t  RX_MARK;                                  /*!< Offset: 0x6c RW RX MARK Register */
  __IOM uint32_t  IP;                                       /*!< Offset: 0x70 RW IP Register */
  __IOM uint32_t  IE;                                       /*!< Offset: 0x74 RW IE Register */
  __IOM uint32_t  SAMPLE_DDR;                               /*!< Offset: 0x78 RW SAMPLE DDR Register */
  __IM  uint32_t  RESERVECD31[2];                           /*!< Offset: 0x7c RO RESERVECD31[2] Register */
  __IOM uint32_t  DATA_TX_DELAY;                            /*!< Offset: 0x84 RW DATA TX DELAY Register */
  __IOM uint32_t  DATA_CRC_TOKEN;                           /*!< Offset: 0x88 RW DATA CRC TOKEN Register */
  __IOM uint32_t  CRC_VALUE;                                /*!< Offset: 0x8c RW CRC VALUE Register */
  __IOM uint32_t  STATUS1;                                  /*!< Offset: 0x90 RW STATUS1 Register */
  __IOM uint32_t  STOP;                                     /*!< Offset: 0x94 RW STOP Register */
  __IM  uint32_t  RESERVECD38[2];                           /*!< Offset: 0x98 RO RESERVECD38[2] Register */
  __IOM uint32_t  STOP_RSP0;                                /*!< Offset: 0xa0 RW STOP RSP0 Register */
  __IOM uint32_t  STOP_RSP1;                                /*!< Offset: 0xa4 RW STOP RSP1 Register */
  __IOM uint32_t  STOP_RSP2;                                /*!< Offset: 0xa8 RW STOP RSP2 Register */
  __IOM uint32_t  STOP_RSP3;                                /*!< Offset: 0xac RW STOP RSP3 Register */
}SDIO_TypeDef;

/* =========================================  End of section using anonymous unions  ========================================= */
#if defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */

/* ToDo: add here your device peripherals base addresses
         following is an example for timer */
/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */
/* Peripheral and SRAM base address */
#define QSPI_FLASH_BASE                           (0x20000000UL)     /*!< (FLASH     ) Base Address */
#define ONCHIP_ILM_BASE                       (0x08000000UL)          /*!< (ILM       ) Base Address */
#define ONCHIP_ILM_SIZE                       (0x10000)
#define ONCHIP_DLM_BASE                       (0x09000000UL)          /*!< (DLM       ) Base Address */
#define ONCHIP_DLM_SIZE                       (0x10000)

/* Peripheral memory map */

#define MISC_CTL_BASE                             (0x18100000UL)
#define UDMA0_BASE                                (0x10100000UL)
#define USART0_BASE                               (0x10120000UL)
#define I3C0_BASE                                 (0x10160000UL)
#define I3C0_DMA_BASE                             (0x10161000UL)
#define QSPI_XIP0_BASE                            (0x10180000UL)
#define QSPI1_BASE                                (0x101a0000UL)
#define SDIO0_BASE                                (0x10400000UL)
#define SDIO0_DMA_BASE                            (0x10401000UL)
#define SRAM0_MEM_BASE                            (0x30000000UL)
#define SRAM1_MEM_BASE                            (0x30010000UL)
#define TEST_SRAM_MEM_BASE                        (0x1c000000UL)
#define AXI_MST0_SYNC_BASE                        (0x18300000UL)
#define AHB_MST0_SYNC_BASE                        (0x18320000UL)
#define ICB_MST0_SYNC_BASE                        (0x18340000UL)
#define AXI_SLV0_SYNC_MEM_BASE                    (0x40000000UL)
#define AHB_SLV0_SYNC_MEM_BASE                    (0x15000000UL)
#define APB_SLV0_SYNC_MEM_BASE                    (0x16000000UL)
#define ICB_SLV0_SYNC_MEM_BASE                    (0x17000000UL)
#define SOC_GLUE_BASE                             (0x10000000UL)
#define IDU_BASE                                  (0x10010000UL)
#define IOMUX_BASE                                (0x18000000UL)

#define UDMA0_CH0_M2M_CFG_BASE                 (UDMA0_BASE + 0x8)
#define UDMA0_CH0_M2M_IRQ_BASE                 (UDMA0_BASE + 0x800)
#define UDMA0_CH1_M2M_CFG_BASE                 (UDMA0_BASE + 0x24)
#define UDMA0_CH1_M2M_IRQ_BASE                 (UDMA0_BASE + 0x80c)

#define UDMA0_CH0_PA2M_CFG_BASE                (UDMA0_BASE + 0x400)
#define UDMA0_CH0_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa00)
#define UDMA0_CH1_PA2M_CFG_BASE                (UDMA0_BASE + 0x418)
#define UDMA0_CH1_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa0c)
#define UDMA0_CH2_PA2M_CFG_BASE                (UDMA0_BASE + 0x430)
#define UDMA0_CH2_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa18)
#define UDMA0_CH3_PA2M_CFG_BASE                (UDMA0_BASE + 0x448)
#define UDMA0_CH3_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa24)
#define UDMA0_CH4_PA2M_CFG_BASE                (UDMA0_BASE + 0x460)
#define UDMA0_CH4_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa30)
#define UDMA0_CH5_PA2M_CFG_BASE                (UDMA0_BASE + 0x478)
#define UDMA0_CH5_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa3c)
#define UDMA0_CH6_PA2M_CFG_BASE                (UDMA0_BASE + 0x490)
#define UDMA0_CH6_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa48)
#define UDMA0_CH7_PA2M_CFG_BASE                (UDMA0_BASE + 0x4a8)
#define UDMA0_CH7_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa54)
#define UDMA0_CH8_PA2M_CFG_BASE                (UDMA0_BASE + 0x4c0)
#define UDMA0_CH8_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa60)
#define UDMA0_CH9_PA2M_CFG_BASE                (UDMA0_BASE + 0x4d8)
#define UDMA0_CH9_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa6c)
#define UDMA0_CH10_PA2M_CFG_BASE                (UDMA0_BASE + 0x4f0)
#define UDMA0_CH10_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa78)
#define UDMA0_CH11_PA2M_CFG_BASE                (UDMA0_BASE + 0x508)
#define UDMA0_CH11_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa84)
#define UDMA0_CH12_PA2M_CFG_BASE                (UDMA0_BASE + 0x520)
#define UDMA0_CH12_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa90)
#define UDMA0_CH13_PA2M_CFG_BASE                (UDMA0_BASE + 0x538)
#define UDMA0_CH13_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xa9c)
#define UDMA0_CH14_PA2M_CFG_BASE                (UDMA0_BASE + 0x550)
#define UDMA0_CH14_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xaa8)
#define UDMA0_CH15_PA2M_CFG_BASE                (UDMA0_BASE + 0x568)
#define UDMA0_CH15_PAM2M_IRQ_BASE              (UDMA0_BASE + 0xab4)

#define I3C0_DMA_CH0_P2M_IRQ_BASE                 (I3C0_DMA_BASE + 0xc00)

#define SDIO0_DMA_CH0_P2M_IRQ_BASE                 (SDIO0_DMA_BASE + 0xc00)

#define UDMA0_CH1_PA2M_CFG_BASE        (UDMA0_BASE + 0x418)
#define UDMA0_CH1_PA2M_IRQ_BASE        (UDMA0_BASE + 0xa0c)
#define UDMA0_CH2_PA2M_CFG_BASE        (UDMA0_BASE + 0x430)
#define UDMA0_CH2_PA2M_IRQ_BASE        (UDMA0_BASE + 0xa18)
#define UDMA0_CH3_PA2M_CFG_BASE        (UDMA0_BASE + 0x448)
#define UDMA0_CH3_PA2M_IRQ_BASE        (UDMA0_BASE + 0xa24)
#define UDMA0_CH4_PA2M_CFG_BASE        (UDMA0_BASE + 0x460)
#define UDMA0_CH4_PA2M_IRQ_BASE        (UDMA0_BASE + 0xa30)

/** @} */ /* End of group Device_Peripheral_peripheralAddr */

/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */

/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */

#define MISC_CTL                                  ((MISC_CTL_TypeDef *) MISC_CTL_BASE)

#define UDMA0                                     ((UDMA_TypeDef *) UDMA0_BASE)

#define USART0                                    ((USART_TypeDef *) USART0_BASE)

#define I3C0                                      ((I3C_TypeDef *) I3C0_BASE)

#define I3C0_DMA                                  ((UDMA_TypeDef *) I3C0_DMA_BASE)

#define QSPI_XIP0                                 ((QSPI_XIP_TypeDef *) QSPI_XIP0_BASE)

#define QSPI1                                     ((QSPI_TypeDef *) QSPI1_BASE)

#define SDIO0                                     ((SDIO_TypeDef *) SDIO0_BASE)

#define SDIO0_DMA                                 ((UDMA_TypeDef *) SDIO0_DMA_BASE)

#define SRAM0_MEM                                 ((ICB_SRAM_TypeDef *) SRAM0_MEM_BASE)

#define SRAM1_MEM                                 ((ICB_SRAM_TypeDef *) SRAM1_MEM_BASE)

#define TEST_SRAM_MEM                             ((ICB_SRAM_TypeDef *) TEST_SRAM_MEM_BASE)

#define AXI_MST0_SYNC                             ((DUMMY_MASTER_TypeDef *) AXI_MST0_SYNC_BASE)

#define AHB_MST0_SYNC                             ((DUMMY_MASTER_TypeDef *) AHB_MST0_SYNC_BASE)

#define ICB_MST0_SYNC                             ((DUMMY_MASTER_TypeDef *) ICB_MST0_SYNC_BASE)

#define AXI_SLV0_SYNC_MEM                         ((ICB_SRAM_TypeDef *) AXI_SLV0_SYNC_MEM_BASE)

#define AHB_SLV0_SYNC_MEM                         ((ICB_SRAM_TypeDef *) AHB_SLV0_SYNC_MEM_BASE)

#define APB_SLV0_SYNC_MEM                         ((ICB_SRAM_TypeDef *) APB_SLV0_SYNC_MEM_BASE)

#define ICB_SLV0_SYNC_MEM                         ((ICB_SRAM_TypeDef *) ICB_SLV0_SYNC_MEM_BASE)

#define SOC_GLUE                                  ((SOC_GLUE_TypeDef *) SOC_GLUE_BASE)

#define IDU                                       ((IDU_TypeDef *) IDU_BASE)
/**
  * @}
  */

/** @addtogroup UDMA_Information
  * @{
  */
#define UDMA0_M2M_CH0                       ((UDMA_CHxCfg_TypeDef *) UDMA0_CH0_M2M_CFG_BASE)
#define UDMA0_CH0_M2M_IRQ               ((UDMA_CHx_IRQ_TypeDef *) UDMA0_CH0_M2M_IRQ_BASE)
#define UDMA0_M2M_CH1                       ((UDMA_CHxCfg_TypeDef *) UDMA0_CH1_M2M_CFG_BASE)
#define UDMA0_CH1_M2M_IRQ               ((UDMA_CHx_IRQ_TypeDef *) UDMA0_CH1_M2M_IRQ_BASE)
#define UDMA0_PAM2M_CH0                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH0_PA2M_CFG_BASE)
#define UDMA0_CH0_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH0_PA2M_BASE)
#define UDMA0_PAM2M_CH1                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define UDMA0_CH1_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_BASE)
#define UDMA0_PAM2M_CH2                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define UDMA0_CH2_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_BASE)
#define UDMA0_PAM2M_CH3                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH3_PA2M_CFG_BASE)
#define UDMA0_CH3_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH3_PA2M_BASE)
#define UDMA0_PAM2M_CH4                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH4_PA2M_CFG_BASE)
#define UDMA0_CH4_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH4_PA2M_BASE)
#define UDMA0_PAM2M_CH5                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH5_PA2M_CFG_BASE)
#define UDMA0_CH5_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH5_PA2M_BASE)
#define UDMA0_PAM2M_CH6                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH6_PA2M_CFG_BASE)
#define UDMA0_CH6_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH6_PA2M_BASE)
#define UDMA0_PAM2M_CH7                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define UDMA0_CH7_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_BASE)
#define UDMA0_PAM2M_CH8                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH8_PA2M_CFG_BASE)
#define UDMA0_CH8_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH8_PA2M_BASE)
#define UDMA0_PAM2M_CH9                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH9_PA2M_CFG_BASE)
#define UDMA0_CH9_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH9_PA2M_BASE)
#define UDMA0_PAM2M_CH10                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH10_PA2M_CFG_BASE)
#define UDMA0_CH10_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH10_PA2M_BASE)
#define UDMA0_PAM2M_CH11                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH11_PA2M_CFG_BASE)
#define UDMA0_CH11_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH11_PA2M_BASE)
#define UDMA0_PAM2M_CH12                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH12_PA2M_CFG_BASE)
#define UDMA0_CH12_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH12_PA2M_BASE)
#define UDMA0_PAM2M_CH13                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH13_PA2M_CFG_BASE)
#define UDMA0_CH13_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH13_PA2M_BASE)
#define UDMA0_PAM2M_CH14                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH14_PA2M_CFG_BASE)
#define UDMA0_CH14_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH14_PA2M_BASE)
#define UDMA0_PAM2M_CH15                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH15_PA2M_CFG_BASE)
#define UDMA0_CH15_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH15_PA2M_BASE)

#define I3C0_DMA_I3C0_P2M_IRQ           ((UDMA_P2M_CHx_Irq_TypeDef *) I3C0_DMA_CH0_P2M_IRQ_BASE)

#define SDIO0_DMA_SDIO0_P2M_IRQ           ((UDMA_P2M_CHx_Irq_TypeDef *) SDIO0_DMA_CH0_P2M_IRQ_BASE)

/******************** udma0 infomation ********************/

#define USART0_USART_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define USART0_USART_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_IRQ_BASE)
#define UDMA_SEL_USART0_USART_DMA_TX                      PA2M_PER_CHANNEL2

#define USART0_USART_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define USART0_USART_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_IRQ_BASE)
#define UDMA_SEL_USART0_USART_DMA_RX                      PA2M_PER_CHANNEL3

#define QSPI_XIP0_TX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH3_PA2M_CFG_BASE)
#define QSPI_XIP0_TX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH3_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI_XIP0_TX_DMA                      PA2M_PER_CHANNEL2

#define QSPI_XIP0_RX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH4_PA2M_CFG_BASE)
#define QSPI_XIP0_RX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH4_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI_XIP0_RX_DMA                      PA2M_PER_CHANNEL2

#define QSPI1_TX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define QSPI1_TX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI1_TX_DMA                      PA2M_PER_CHANNEL4

#define QSPI1_RX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define QSPI1_RX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI1_RX_DMA                      PA2M_PER_CHANNEL5

/**
  * @}
  */

/* Macros for Bit Operations */
#define _REG8P(p, i)                        ((volatile uint8_t *) ((uintptr_t)((p) + (i))))
#define _REG16P(p, i)                       ((volatile uint16_t *) ((uintptr_t)((p) + (i))))
#define _REG32P(p, i)                       ((volatile uint32_t *) ((uintptr_t)((p) + (i))))
#define _REG64P(p, i)                       ((volatile uint64_t *) ((uintptr_t)((p) + (i))))
#define _REG8(p, i)                         (*(_REG8P(p, i)))
#define _REG16(p, i)                        (*(_REG16P(p, i)))
#define _REG32(p, i)                        (*(_REG32P(p, i)))
#define _REG64(p, i)                        (*(_REG64P(p, i)))
#define REG8(addr)                          _REG8((addr), 0)
#define REG16(addr)                         _REG16((addr), 0)
#define REG32(addr)                         _REG32((addr), 0)
#define REG64(addr)                         _REG64((addr), 0)

/* Macros for Bit Operations */
#if __riscv_xlen == 32
#define BITMASK_MAX                         0xFFFFFFFFUL
#define BITOFS_MAX                          31
#else
#define BITMASK_MAX                         0xFFFFFFFFFFFFFFFFULL
#define BITOFS_MAX                          63
#endif

#define BIT(ofs)                            (0x1UL << (ofs))
#define BITS(start, end)                    ((BITMASK_MAX) << (start) & (BITMASK_MAX) >> (BITOFS_MAX - (end)))

#define SET_VAL(regval, start, end,val)     ((regval) |= (val<<start)&(BITS((start), (end))))
#define GET_BIT(regval, bitofs)             (((regval) >> (bitofs)) & 0x1)
#define SET_BIT(regval, bitofs)             ((regval) |= BIT(bitofs))
#define CLR_BIT(regval, bitofs)             ((regval) &= (~BIT(bitofs)))
#define FLIP_BIT(regval, bitofs)            ((regval) ^= BIT(bitofs))
#define WRITE_BIT(regval, bitofs, val)      CLR_BIT(regval, bitofs); ((regval) |= ((val) << bitofs) & BIT(bitofs))
#define CHECK_BIT(regval, bitofs)           (!!((regval) & (0x1UL<<(bitofs))))
#define GET_BITS(regval, start, end)        (((regval) & BITS((start), (end))) >> (start))
#define SET_BITS(regval, start, end)        ((regval) |= BITS((start), (end)))
#define CLR_BITS(regval, start, end)        ((regval) &= (~BITS((start), (end))))
#define FLIP_BITS(regval, start, end)       ((regval) ^= BITS((start), (end)))
#define CHECK_BITS_ALL(regval, start, end)  (!((~(regval)) & BITS((start), (end))))
#define CHECK_BITS_ANY(regval, start, end)  ((regval) & BITS((start), (end)))
#define WRITE_BITS(regval, start, end, val) CLR_BITS(regval, start, end); ((regval) |= ((val) << start) & BITS((start), (end)))
#define ADDR_CHECK(addr,uplimit,lowlimit)   ((addr)<=(uplimit))&&((addr)>=(lowlimit))
#define DLM_ADDR_CHECK(addr)                 ADDR_CHECK(addr,ONCHIP_DLM_BASE+ONCHIP_DLM_SIZE,ONCHIP_DLM_BASE)   
 #if defined(__SMP_DMA_ADDR_OFFSET) && __SMP_DMA_ADDR_OFFSET>0
#define CAL_ADDR(addr)    DLM_ADDR_CHECK(addr) ? (addr+ (hart_id.b.core_id*__SMP_DMA_ADDR_OFFSET)) : addr                                                                                    
#else                                                              
#define CAL_ADDR(addr)    addr                                                                
#endif 
#define BITMASK_SET(regval, mask)           ((regval) |= (mask))
#define BITMASK_CLR(regval, mask)           ((regval) &= (~(mask)))
#define BITMASK_FLIP(regval, mask)          ((regval) ^= (mask))
#define BITMASK_CHECK_ALL(regval, mask)     (!((~(regval)) & (mask)))
#define BITMASK_CHECK_ANY(regval, mask)     ((regval) & (mask))

#define ADDR8(addr)                         ((uint8_t)(uintptr_t)(addr))
#define ADDR16(addr)                        ((uint16_t)(uintptr_t)(addr))
#define ADDR32(addr)                        ((uint32_t)(uintptr_t)(addr))
#define ADDR64(addr)                        ((uint64_t)(uintptr_t)(addr))
#define ADDR8P(addr)                        ((uint8_t *)(uintptr_t)(addr))
#define ADDR16P(addr)                       ((uint16_t *)(uintptr_t)(addr))
#define ADDR32P(addr)                       ((uint32_t *)(uintptr_t)(addr))
#define ADDR64P(addr)                       ((uint64_t *)(uintptr_t)(addr))

extern uint32_t get_cpu_freq(void);
extern void delay_1ms(uint32_t count);
extern void simulation_pass(void);
extern void simulation_fail(void);
extern void simulation_check(void);
extern void do_check(void);
/** @} */ /* End of group Nuclei */

#ifdef __cplusplus
}
#endif

#endif  /* __NSH__ */
