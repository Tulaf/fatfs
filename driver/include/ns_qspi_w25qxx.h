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
 * @file     ns_spi_w25qxx.h
 * @brief    This file contains all the functions prototypes for the SPI SDcard firmware
 */


#ifndef _NS_SPI_SDCARD_H
#define _NS_SPI_SDCARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ns.h"
#include "ns_qspi.h"

#define W25_FLASH_SIZE (1024*1024*16)
//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015     //W25Q16
#define  sFLASH_ID              0XEF4018   //W25Q128
// #define  sFLASH_ID              0XEF4017    //W25Q64

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*命令定义-开头*******************************/
#define W25X_WriteEnable              0x06
#define W25X_WriteDisable              0x04
#define W25X_ReadStatusReg            0x05
#define W25X_WriteStatusReg            0x01
#define W25X_ReadData                    0x03
#define W25X_FastReadData              0x0B
#define W25X_FastReadDual              0x3B
#define W25X_PageProgram              0x02
#define W25X_BlockErase                  0xD8
#define W25X_SectorErase              0x20
#define W25X_ChipErase                  0xC7
#define W25X_PowerDown                  0xB9
#define W25X_ReleasePowerDown        0xAB
#define W25X_DeviceID                    0xAB
#define W25X_ManufactDeviceID       0x90
#define W25X_JedecDeviceID            0x9F

/* WIP(busy) flag, FLASH is writing internally */
#define WIP_Flag 0x01
#define Dummy_Byte 0xFF
/*command definition-end *********************************/

#define SPI_FLASH_CS_LOW() GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define SPI_FLASH_CS_HIGH() GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

/*SPI interface definition - ending *****************************/

/*wait for timeout*/
#define SPIT_FLAG_TIMEOUT ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*Information output*/
#define FLASH_DEBUG_ON 1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

void SPI_FLASH_Init(QSPI_TypeDef* QSPIx);
void SPI_CS(QSPI_TypeDef* QSPIx, uint8_t p);
void SPI_FLASH_SectorErase(QSPI_TypeDef* QSPIx, uint32_t SectorAddr);
void SPI_FLASH_BulkErase(QSPI_TypeDef* QSPIx);
void SPI_FLASH_PageWrite(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(QSPI_TypeDef* QSPIx);
uint32_t SPI_FLASH_ReadDeviceID(QSPI_TypeDef* QSPIx);
void SPI_FLASH_StartReadSequence(QSPI_TypeDef* QSPIx, uint32_t ReadAddr);
void SPI_Flash_PowerDown(QSPI_TypeDef* QSPIx);
void SPI_Flash_WAKEUP(QSPI_TypeDef* QSPIx);


uint8_t SPI_FLASH_ReadByte(QSPI_TypeDef* QSPIx);
uint8_t SPI_FLASH_SendByte(QSPI_TypeDef* QSPIx, uint8_t byte);

void SPI_FLASH_WriteEnable(QSPI_TypeDef* QSPIx);
void SPI_FLASH_WaitForWriteEnd(QSPI_TypeDef* QSPIx);

#ifdef __cplusplus
}
#endif

#endif
/* _NS_SPI_SDCARD_H */
