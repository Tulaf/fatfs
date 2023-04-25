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

/*!
 * @file     ns_spi_sdcard.h
 * @brief    This file contains all the functions prototypes for the SPI SDcard firmware
 */

#ifndef _NS_SPI_SDCARD_H
#define _NS_SPI_SDCARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ns.h"
#include "ns_qspi.h"

extern uint8_t SD_TYPE;

#define ERR         0x00
#define MMC                0x01
#define V1                0x02
#define V2                0x04
#define V2HC            0x06

#define DUMMY_BYTE                 0xFF 
#define MSD_BLOCKSIZE             512

#define CMD0    0
#define CMD1    1
#define CMD8    8
#define CMD9    9
#define CMD10   10
#define CMD12   12
#define CMD16   16
#define CMD17   17
#define CMD18   18
#define CMD23   23
#define CMD24   24
#define CMD25   25
#define CMD41   41
#define CMD55   55
#define CMD58   58
#define CMD59   59

#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

enum _CD_HOLD
{
    HOLD = 0,
    RELEASE = 1,
};

typedef struct               /* Card Specific Data */
{
    uint8_t  CSDStruct;            /* CSD structure */
    uint8_t  SysSpecVersion;       /* System specification version */
    uint8_t  Reserved1;            /* Reserved */
    uint8_t  TAAC;                 /* Data read access-time 1 */
    uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */
    uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */
    uint16_t CardComdClasses;      /* Card command classes */
    uint8_t  RdBlockLen;           /* Max. read data block length */
    uint8_t  PartBlockRead;        /* Partial blocks for read allowed */
    uint8_t  WrBlockMisalign;      /* Write block misalignment */
    uint8_t  RdBlockMisalign;      /* Read block misalignment */
    uint8_t  DSRImpl;              /* DSR implemented */
    uint8_t  Reserved2;            /* Reserved */
    uint32_t DeviceSize;           /* Device Size */
    uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
    uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
    uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
    uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
    uint8_t  DeviceSizeMul;        /* Device size multiplier */
    uint8_t  EraseGrSize;          /* Erase group size */
    uint8_t  EraseGrMul;           /* Erase group size multiplier */
    uint8_t  WrProtectGrSize;      /* Write protect group size */
    uint8_t  WrProtectGrEnable;    /* Write protect group enable */
    uint8_t  ManDeflECC;           /* Manufacturer default ECC */
    uint8_t  WrSpeedFact;          /* Write speed factor */
    uint8_t  MaxWrBlockLen;        /* Max. write data block length */
    uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */
    uint8_t  Reserved3;            /* Reserded */
    uint8_t  ContentProtectAppli;  /* Content protection application */
    uint8_t  FileFormatGrouop;     /* File format group */
    uint8_t  CopyFlag;             /* Copy flag (OTP) */
    uint8_t  PermWrProtect;        /* Permanent write protection */
    uint8_t  TempWrProtect;        /* Temporary write protection */
    uint8_t  FileFormat;           /* File Format */
    uint8_t  ECC;                  /* ECC code */
    uint8_t  CSD_CRC;              /* CSD CRC */
    uint8_t  Reserved4;            /* always 1*/
}
MSD_CSD;

typedef struct                 /*Card Identification Data*/
{
    uint8_t  ManufacturerID;       /* ManufacturerID */
    uint16_t OEM_AppliID;          /* OEM/Application ID */
    uint32_t ProdName1;            /* Product Name part1 */
    uint8_t  ProdName2;            /* Product Name part2*/
    uint8_t  ProdRev;              /* Product Revision */
    uint32_t ProdSN;               /* Product Serial Number */
    uint8_t  Reserved1;            /* Reserved1 */
    uint16_t ManufactDate;         /* Manufacturing Date */
    uint8_t  CID_CRC;              /* CID CRC */
    uint8_t  Reserved2;            /* always 1 */
}
MSD_CID;

typedef struct
{
    MSD_CSD CSD;
    MSD_CID CID;
    uint32_t Capacity;              /* Card Capacity */
    uint32_t BlockSize;             /* Card Block Size */
    uint16_t RCA;
    uint8_t CardType;
    uint32_t SpaceTotal;            /* Total space size in file system */
    uint32_t SpaceFree;               /* Free space size in file system */
}
MSD_CARDINFO, *PMSD_CARDINFO;

extern MSD_CARDINFO SD0_CardInfo;

uint8_t         SD_init(QSPI_TypeDef* QSPIx);
void            SD_CS(QSPI_TypeDef* QSPIx, uint8_t p);
uint32_t        SD_GetSectorCount(QSPI_TypeDef* QSPIx);
uint8_t         SD_GETCID (QSPI_TypeDef* QSPIx, uint8_t *cid_data);
uint8_t         SD_GETCSD(QSPI_TypeDef* QSPIx, uint8_t *csd_data);
int             MSD0_GetCardInfo(QSPI_TypeDef* QSPIx, PMSD_CARDINFO SD0_CardInfo);
uint8_t         SD_ReceiveData(QSPI_TypeDef* QSPIx, uint8_t *data, uint16_t len);
uint8_t         SD_SendBlock(QSPI_TypeDef* QSPIx, uint8_t*buf,uint8_t cmd);
uint8_t         SD_ReadDisk(QSPI_TypeDef* QSPIx, uint8_t*buf,uint32_t sector,uint8_t cnt);
uint8_t         SD_WriteDisk(QSPI_TypeDef* QSPIx, uint8_t*buf,uint32_t sector,uint8_t cnt);

void SPI_setspeed(QSPI_TypeDef* QSPIx, uint8_t speed);
uint8_t spi_readwrite(QSPI_TypeDef* QSPIx, uint8_t Txdata);

#ifdef __cplusplus
}
#endif

#endif
/* _NS_SPI_SDCARD_H */
