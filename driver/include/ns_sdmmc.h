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

#ifndef _NS_SDMMC_H
#define _NS_SDMMC_H

/*!
 * @file     ns_sdio.h
 * @brief    This file contains all the functions prototypes for the SDIO firmware
 */

#ifdef __cplusplus
extern "C" {
#endif
#define SDIO_FLAG_DTIMEOUT                  ((uint32_t)0x00000008)

typedef enum
{
/**
  * @brief  SDIO specific error defines
  */
	SDMMC_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SDMMC_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SDMMC_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SDMMC_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SDMMC_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SDMMC_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SDMMC_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SDMMC_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SDMMC_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SDMMC_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SDMMC_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SDMMC_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SDMMC_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SDMMC_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SDMMC_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SDMMC_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SDMMC_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SDMMC_CC_ERROR                        = (18), /*!< Internal card controller error */
	SDMMC_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SDMMC_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SDMMC_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SDMMC_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SDMMC_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SDMMC_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SDMMC_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SDMMC_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SDMMC_INVALID_VOLTRANGE               = (27),
	SDMMC_ADDR_OUT_OF_RANGE               = (28),
	SDMMC_SWITCH_ERROR                    = (29),
	SDMMC_SDIO_DISABLED                   = (30),
	SDMMC_SDIO_FUNCTION_BUSY              = (31),
	SDMMC_SDIO_FUNCTION_FAILED            = (32),
	SDMMC_SDIO_UNKNOWN_FUNCTION           = (33),
    SDMMC_SDIO_WRONG_DIR                  = (34),

/**
  * @brief  Standard error defines
  */
	SDMMC_INTERNAL_ERROR,
	SDMMC_NOT_CONFIGURED,
	SDMMC_REQUEST_PENDING,
	SDMMC_REQUEST_NOT_APPLICABLE,
	SDMMC_INVALID_PARAMETER,
	SDMMC_UNSUPPORTED_FEATURE,
	SDMMC_UNSUPPORTED_HW,
	SDMMC_ERROR,
	SDMMC_OK = 0
} SDMMC_Error;

/**
  * @brief  SDIO Transfer state
  */
typedef enum
{
    SDMMC_TRANSFER_OK = 0,
    SDMMC_TRANSFER_BUSY = 1,
    SDMMC_TRANSFER_ERROR
} SDTransferState;

/**
  * @brief  SD Card States
  */
typedef enum
{
	SDMMC_CARD_READY                  = ((uint32_t)0x00000001),
	SDMMC_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
	SDMMC_CARD_STANDBY                = ((uint32_t)0x00000003),
	SDMMC_CARD_TRANSFER               = ((uint32_t)0x00000004),
	SDMMC_CARD_SENDING                = ((uint32_t)0x00000005),
	SDMMC_CARD_RECEIVING              = ((uint32_t)0x00000006),
	SDMMC_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
	SDMMC_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
	SDMMC_CARD_ERROR                  = ((uint32_t)0x000000FF)
} SDCardState;

/**
  * @brief  Card Specific Data: CSD Register
  */
typedef struct
{
	uint8_t  CSDStruct;            /*!< CSD structure */
	uint8_t  SysSpecVersion;       /*!< System specification version */
	uint8_t  Reserved1;            /*!< Reserved */
	uint8_t  TAAC;                 /*!< Data read access-time 1 */
	uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	uint16_t CardComdClasses;      /*!< Card command classes */
	uint8_t  RdBlockLen;           /*!< Max. read data block length */
	uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
	uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
	uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
	uint8_t  DSRImpl;              /*!< DSR implemented */
	uint8_t  Reserved2;            /*!< Reserved */
	uint32_t DeviceSize;           /*!< Device Size */
	uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
	uint8_t  EraseGrSize;          /*!< Erase group size */
	uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
	uint8_t  WrProtectGrSize;      /*!< Write protect group size */
	uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
	uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
	uint8_t  WrSpeedFact;          /*!< Write speed factor */
	uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
	uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	uint8_t  Reserved3;            /*!< Reserded */
	uint8_t  ContentProtectAppli;  /*!< Content protection application */
	uint8_t  FileFormatGrouop;     /*!< File format group */
	uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
	uint8_t  PermWrProtect;        /*!< Permanent write protection */
	uint8_t  TempWrProtect;        /*!< Temporary write protection */
	uint8_t  FileFormat;           /*!< File Format */
	uint8_t  ECC;                  /*!< ECC code */
	uint8_t  CSD_CRC;              /*!< CSD CRC */
	uint8_t  Reserved4;            /*!< always 1*/
} SDMMC_CSD;

/**
  * @brief  Card Identification Data: CID Register
  */
typedef struct
{
	uint8_t  ManufacturerID;       /*!< ManufacturerID */
	uint16_t OEM_AppliID;          /*!< OEM/Application ID */
	uint32_t ProdName1;            /*!< Product Name part1 */
	uint8_t  ProdName2;            /*!< Product Name part2*/
	uint8_t  ProdRev;              /*!< Product Revision */
	uint32_t ProdSN;               /*!< Product Serial Number */
	uint8_t  Reserved1;            /*!< Reserved1 */
	uint16_t ManufactDate;         /*!< Manufacturing Date */
	uint8_t  CID_CRC;              /*!< CID CRC */
	uint8_t  Reserved2;            /*!< always 1 */
} SDMMC_CID;

/**
  * @brief SD Card Status
  */
typedef struct
{
    uint8_t DAT_BUS_WIDTH;
    uint8_t SECURED_MODE;
    uint16_t SDMMC_CARD_TYPE;
    uint32_t SIZE_OF_PROTECTED_AREA;
    uint8_t SPEED_CLASS;
    uint8_t PERFORMANCE_MOVE;
    uint8_t AU_SIZE;
    uint16_t ERASE_SIZE;
    uint8_t ERASE_TIMEOUT;
    uint8_t ERASE_OFFSET;
} SDMMC_CardStatus;

/**
  * @brief SD Card information
  */
typedef struct
{
    SDMMC_CSD SDMMC_csd;
    SDMMC_CID SDMMC_cid;
    long long CardCapacity;
    uint32_t CardBlockSize;
    uint16_t RCA;
    uint8_t CardType;
} SDMMC_CardInfo;

typedef union {
    struct _EXT_CSD {
        uint8_t Reserved26[32];
        uint8_t FLUSH_CACHE;
        uint8_t CACHE_CTRL;
        uint8_t POWER_OFF_NOTIFICATION;
        uint8_t PACKED_FAILURE_INDEX;
        uint8_t PACKED_COMMAND_STATUS;
        uint8_t CONTEXT_CONF[15];
        uint8_t EXT_PARTITIONS_ATTRIBUTE[2];
        uint8_t EXCEPTION_EVENTS_STATUS[2];
        uint8_t EXCEPTION_EVENTS_CTRL[2];
        uint8_t DYNCAP_NEEDED;
        uint8_t CLASS_6_CTRL;
        uint8_t INI_TIMEOUT_EMU;
        uint8_t DATA_SECTOR_SIZE;
        uint8_t USE_NATIVE_SECTOR;
        uint8_t NATIVE_SECTOR_SIZE;
        uint8_t VENDOR_SPECIFIC_FIELD[64];
        uint8_t Reserved25;
        uint8_t PROGRAM_CID_CSD_DDR_SUPPORT;
        uint8_t PERIODIC_WAKEUP;
        uint8_t TCASE_SUPPORT;
        uint8_t Reserved24;
        uint8_t SEC_BAD_BLK_MGMNT;
        uint8_t Reserved23;
        uint8_t ENH_START_ADDR[4];
        uint8_t ENH_SIZE_MULT[3];
        uint8_t GP_SIZE_MULT[12];
        uint8_t PARTITION_SETTING_COMPLETED;
        uint8_t PARTITIONS_ATTRIBUTE;
        uint8_t MAX_ENH_SIZE_MULT[3];
        uint8_t PARTITIONING_SUPPORT;
        uint8_t HPI_MGMT;
        uint8_t RST_n_FUNCTION;
        uint8_t BKOPS_EN;
        uint8_t BKOPS_START;
        uint8_t SANITIZE_START;
        uint8_t WR_REL_PARAM;
        uint8_t WR_REL_SET;
        uint8_t RPMB_SIZE_MULT;
        uint8_t FW_CONFIG;
        uint8_t Reserved22;
        uint8_t USER_WP;
        uint8_t Reserved21;
        uint8_t BOOT_WP;
        uint8_t BOOT_WP_STATUS;
        uint8_t ERASE_GROUP_DEF;
        uint8_t Reserved20;
        uint8_t BOOT_BUS_CONDITIONS;
        uint8_t BOOT_CONFIG_PROT;
        uint8_t PARTITION_CONFIG;
        uint8_t Reserved19;
        uint8_t ERASED_MEM_CONT;
        uint8_t Reserved18;
        uint8_t BUS_WIDTH;
        uint8_t Reserved17;
        uint8_t HS_TIMING;
        uint8_t Reserved16;
        uint8_t POWER_CLASS;
        uint8_t Reserved15;
        uint8_t CMD_SET_REV;
        uint8_t Reserved14;
        uint8_t CMD_SET;
        uint8_t Reserved13;
        uint8_t EXT_CSD_REV;
        uint8_t Reserved12;
        uint8_t Reserved11;
        uint8_t Reserved10;
        uint8_t DEVICE_TYPE;
        uint8_t DRIVER_STRENGTH;
        uint8_t OUT_OF_INTERRUPT_TIME;
        uint8_t PARTITION_SWITCH_TIME;
        uint8_t PWR_CL_52_195;
        uint8_t PWR_CL_26_195;
        uint8_t PWR_CL_52_360;
        uint8_t PWR_CL_26_360;
        uint8_t Reserved9;
        uint8_t MIN_PERF_R_4_26;
        uint8_t MIN_PERF_W_4_26;
        uint8_t MIN_PERF_R_8_26_4_52;
        uint8_t MIN_PERF_W_8_26_4_52;
        uint8_t MIN_PERF_R_8_52;
        uint8_t MIN_PERF_W_8_52;
        uint8_t Reserved8;
        uint8_t SEC_COUNT[4];
        uint8_t Reserved7;
        uint8_t S_A_TIMEOUT;
        uint8_t Reserved6;
        uint8_t S_C_VCCQ;
        uint8_t S_C_VCC;
        uint8_t HC_WP_GRP_SIZE;
        uint8_t REL_WR_SEC_C;
        uint8_t ERASE_TIMEOUT_MULT;
        uint8_t HC_ERASE_GRP_SIZE;
        uint8_t ACC_SIZE;
        uint8_t BOOT_SIZE_MULTI;
        uint8_t Reserved5;
        uint8_t BOOT_INFO;
        uint8_t obsolete2;
        uint8_t obsolete1;
        uint8_t SEC_FEATURE_SUPPORT;
        uint8_t TRIM_MULT;
        uint8_t Reserved4;
        uint8_t MIN_PERF_DDR_R_8_52;
        uint8_t MIN_PERF_DDR_W_8_52;
        uint8_t PWR_CL_200_195;
        uint8_t PWR_CL_200_360;
        uint8_t PWR_CL_DDR_52_195;
        uint8_t PWR_CL_DDR_52_360;
        uint8_t Reserved3;
        uint8_t INI_TIMEOUT_AP;
        uint8_t CORRECTLY_PRG_SECTORS_NUM[4];
        uint8_t BKOPS_STATUS[2];
        uint8_t POWER_OFF_LONG_TIME;
        uint8_t GENERIC_CMD6_TIME;
        uint8_t CACHE_SIZE[4];
        uint8_t Reserved2[255];
        uint8_t EXT_SUPPORT;
        uint8_t LARGE_UNIT_SIZE_M1;
        uint8_t CONTEXT_CAPABILITIES;
        uint8_t TAG_RES_SIZE;
        uint8_t TAG_UNIT_SIZE;
        uint8_t DATA_TAG_SUPPORT;
        uint8_t MAX_PACKED_WRITES;
        uint8_t MAX_PACKED_READS;
        uint8_t BKOPS_SUPPORT;
        uint8_t HPI_FEATURES;
        uint8_t S_CMD_SET;
        uint8_t Reserved1[7];
    } EXT_CSD;
    uint8_t CsdBuf[512];
} EMMCEXT_CSD;

typedef struct {
    SDMMC_CSD EmmcCsd;
    EMMCEXT_CSD EmmcExtCsd;
    SDMMC_CID EmmcCid;
    uint64_t CardCapacity;  /*!< Card Capacity */
    uint32_t CardBlockSize; /*!< Card Block Size */
    uint16_t RCA;
    uint8_t CardType;
} EmmcCardInfo;

/**
  * @brief SDIO Commands Index
  */
#define SDMMC_CMD_GO_IDLE_STATE                       (0) << 8
#define SDMMC_CMD_SEND_OP_COND                        (1) << 8
#define SDMMC_CMD_ALL_SEND_CID                        (2) << 8
#define SDMMC_CMD_SET_REL_ADDR                        (3) << 8     /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SDMMC_CMD_SET_DSR                             (4) << 8
#define SDMMC_CMD_SDIO_SEN_OP_COND                    (5) << 8
#define SDMMC_CMD_HS_SWITCH                           (6) << 8
#define SDMMC_CMD_SEL_DESEL_CARD                      (7) << 8
#define SDMMC_CMD_HS_SEND_EXT_CSD                     (8) << 8
#define SDMMC_CMD_SEND_CSD                            (9) << 8
#define SDMMC_CMD_SEND_CID                            (10) << 8
#define SDMMC_CMD_READ_DAT_UNTIL_STOP                 (11) << 8    /*!< SD Card doesn't support it */
#define SDMMC_CMD_STOP_TRANSMISSION                   (12) << 8
#define SDMMC_CMD_SEND_STATUS                         (13) << 8
#define SDMMC_CMD_HS_BUSTEST_READ                     (14) << 8
#define SDMMC_CMD_GO_INACTIVE_STATE                   (15) << 8
#define SDMMC_CMD_SET_BLOCKLEN                        (16) << 8
#define SDMMC_CMD_READ_SINGLE_BLOCK                   (17) << 8
#define SDMMC_CMD_READ_MULT_BLOCK                     (18) << 8
#define SDMMC_CMD_HS_BUSTEST_WRITE                    (19) << 8
#define SDMMC_CMD_WRITE_DAT_UNTIL_STOP                (20) << 8    /*!< SD Card doesn't support it */
#define SDMMC_CMD_SET_BLOCK_COUNT                     (23) << 8    /*!< SD Card doesn't support it */
#define SDMMC_CMD_WRITE_SINGLE_BLOCK                  (24) << 8
#define SDMMC_CMD_WRITE_MULT_BLOCK                    (25) << 8
#define SDMMC_CMD_PROG_CID                            (26) << 8
#define SDMMC_CMD_PROG_CSD                            (27) << 8
#define SDMMC_CMD_SET_WRITE_PROT                      (28) << 8
#define SDMMC_CMD_CLR_WRITE_PROT                      (29) << 8
#define SDMMC_CMD_SEND_WRITE_PROT                     (30) << 8
#define SDMMC_CMD_SD_ERASE_GRP_START                  (32) << 8    /*!< To set the address of the first write                                                                     block to be erased. (For SD card only) */
#define SDMMC_CMD_SD_ERASE_GRP_END                    (33) << 8    /*!< To set the address of the last write block of the                                                                     continuous range to be erased. (For SD card only) */
#define SDMMC_CMD_ERASE_GRP_START                     (35) << 8    /*!< To set the address of the first write block to be erased.                                                                    (For MMC card only spec 3.31) */
#define SDMMC_CMD_ERASE_GRP_END                       (36) << 8    /*!< To set the address of the last write block of the                                                                     continuous range to be erased. (For MMC card only spec 3.31) */

#define SDMMC_CMD_ERASE                               (38) << 8
#define SDMMC_CMD_FAST_IO                             (39) << 8    /*!< SD Card doesn't support it */
#define SDMMC_CMD_GO_IRQ_STATE                        (40) << 8    /*!< SD Card doesn't support it */
#define SDMMC_CMD_LOCK_UNLOCK                         (42) << 8
#define SDMMC_CMD_APP_CMD                             (55) << 8
#define SDMMC_CMD_GEN_CMD                             (56) << 8
#define SDMMC_CMD_NO_CMD                              (64) << 8

/**
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD should be sent before sending these commands.
  */
#define SDMMC_CMD_APP_SD_SET_BUSWIDTH                 (6) << 8     /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_STAUS                        (13) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        (22) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_OP_COND                      (41) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SET_CLR_CARD_DETECT          (42) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SEND_SCR                     (51) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SDIO_RW_DIRECT                      (52) << 8    /*!< For SD I/O Card only */
#define SDMMC_CMD_SDIO_RW_EXTENDED                    (53) << 8    /*!< For SD I/O Card only */

/**
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands.
  */
#define SDMMC_CMD_SD_APP_GET_MKB                      (43) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_GET_MID                      (44) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SET_CER_RN1                  (45) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_GET_CER_RN2                  (46) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SET_CER_RES2                 (47) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_GET_CER_RES1                 (48) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   (18) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  (25) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SECURE_ERASE                 (38) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_CHANGE_SECURE_AREA           (49) << 8    /*!< For SD Card only */
#define SDMMC_CMD_SD_APP_SECURE_WRITE_MKB             (48) << 8    /*!< For SD Card only */

/** 
  * @brief Supported SD Memory Cards
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

#define MMC_LOW_CAPACITY_CARD                      ((uint32_t)0x00000000U) /*!< MMC Card Capacity <=2Gbytes   */
#define MMC_HIGH_CAPACITY_CARD                     ((uint32_t)0x00000001U) /*!< MMC Card Capacity >2Gbytes and <2Tbytes   */

/** 
  * @brief  SDIO Static flags, TimeOut, FIFO Address
  */
#define SDIO_STATIC_FLAGS               ((uint32_t)0x00000003)
#define SDIO_CMD0TIMEOUT                ((uint32_t)0x00010000)
#define SDIO_CMDWrongDirection          BIT(17)
#define SDIO_CMDBusyTimeout             BIT(18)
#define SDIO_DATRunning                 BITS(24,29)
#define SDIO_DATATIMEOUT                BIT(24)
#define SDIO_FIFO_Address               ((uint32_t)0x40018080)

/** 
  * @brief  Mask for errors Card Status R1 (OCR Register)
  */
#define SDMMC_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SDMMC_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SDMMC_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SDMMC_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SDMMC_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SDMMC_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SDMMC_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SDMMC_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SDMMC_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SDMMC_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SDMMC_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SDMMC_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SDMMC_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000)
#define SDMMC_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000)
#define SDMMC_OCR_CID_CSD_OVERWRIETE       ((uint32_t)0x00010000)
#define SDMMC_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SDMMC_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SDMMC_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SDMMC_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SDMMC_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)

/** 
  * @brief  Masks for R6 Response
  */
#define SDMMC_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SDMMC_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SDMMC_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

#define SDMMC_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define SDMMC_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SDMMC_STD_CAPACITY                 ((uint32_t)0x00000000)
#define SDMMC_CHECK_PATTERN                ((uint32_t)0x000001AA)
#define SDMMC_VOLTAGE_WINDOW_MMC           ((uint32_t)0x80FF8000)

#define SDMMC_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define SDMMC_ALLZERO                      ((uint32_t)0x00000000)
#define SDMMC_DATATIMEOUT                  ((uint32_t)0xFFFFFFFF)
#define SDMMC_0TO7BITS                     ((uint32_t)0x000000FF)
#define SDMMC_8TO15BITS                    ((uint32_t)0x0000FF00)
#define SDMMC_16TO23BITS                   ((uint32_t)0x00FF0000)
#define SDMMC_24TO31BITS                   ((uint32_t)0xFF000000)
#define SDMMC_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SDMMC_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define SDMMC_CARD_LOCKED                  ((uint32_t)0x02000000)
#define SDMMC_CARD_PROGRAMMING             ((uint32_t)0x00000007)
#define SDMMC_CARD_RECEIVING               ((uint32_t)0x00000006)
#define SDMMC_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define SDMMC_HALFFIFO                     ((uint32_t)0x00000008)
#define SDMMC_HALFFIFOBYTES                ((uint32_t)0x00000020)

/** 
  * @brief  Command Class Supported
  */
#define SDMMC_CCCC_LOCK_UNLOCK             ((uint32_t)0x00000080)
#define SDMMC_CCCC_WRITE_PROT              ((uint32_t)0x00000040)
#define SDMMC_CCCC_ERASE                   ((uint32_t)0x00000020)

SDMMC_Error SDMMC_Init(SDIO_TypeDef *SDIOx);
SDMMC_Error SDMMC_PowerON(SDIO_TypeDef *SDIOx);
SDMMC_Error SDMMC_PowerOFF(void);
SDMMC_Error SDMMC_InitializeCards(SDIO_TypeDef *SDIOx);
SDMMC_Error SDMMC_GetCardInfo(SDMMC_CardInfo *cardinfo);
SDMMC_Error SDMMC_EnableWideBusOperation(SDIO_TypeDef *SDIOx, uint32_t wmode);
SDMMC_Error SDMMC_SetDeviceMode(uint32_t mode);
SDMMC_Error SDMMC_SelectDeselect(SDIO_TypeDef *SDIOx, uint32_t addr);
SDMMC_Error SDMMC_SendStatus(uint32_t *pcardstatus);
SDCardState SDMMC_GetState(void);
SDMMC_Error SDMMC_ReadBlock(SDIO_TypeDef *SDIOx, uint8_t *buf,long long addr,uint16_t blksize);
SDMMC_Error SDMMC_ReadMultiBlocks(SDIO_TypeDef *SDIOx, uint8_t *buf, long long  addr,uint16_t blksize,uint32_t nblks);
SDMMC_Error SDMMC_WriteBlock(SDIO_TypeDef *SDIOx, uint8_t *buf,long long addr,  uint16_t blksize);
SDMMC_Error SDMMC_WriteMultiBlocks(SDIO_TypeDef *SDIOx, uint8_t *buf, long long addr,uint16_t blksize,uint32_t nblks);
SDMMC_Error SDMMC_ProcessIRQSrc(void);
SDMMC_Error EmmcGetCardInfo(EmmcCardInfo *E, uint32_t *CSD_Tab, uint32_t *CID_Tab, uint16_t Rca);
SDMMC_Error EmmcReadExtCsd(SDIO_TypeDef *SDIOx, EmmcCardInfo *E);

uint8_t SDMMC_ReadDisk(SDIO_TypeDef *SDIOx, uint8_t*buf, uint32_t sector, uint8_t cnt);
uint8_t SDMMC_WriteDisk(SDIO_TypeDef *SDIOx, uint8_t*buf, uint32_t sector, uint8_t cnt);
static SDMMC_Error CmdResp7Error(SDIO_TypeDef *SDIOx);
static SDMMC_Error CmdResp1Error(SDIO_TypeDef *SDIOx, uint32_t cmd);
static SDMMC_Error CmdResp3Error(SDIO_TypeDef *SDIOx);
static SDMMC_Error CmdResp2Error(SDIO_TypeDef *SDIOx);
static SDMMC_Error CmdResp6Error(SDIO_TypeDef *SDIOx, uint32_t cmd,uint16_t*prca);
static SDMMC_Error CmdError(SDIO_TypeDef *SDIOx);
#ifdef __cplusplus
}
#endif

#endif
/* _NS_SDIO_H */
