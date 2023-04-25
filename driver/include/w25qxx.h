#ifndef __W25QXX_H__
#define __W25QXX_H__
#include "ns.h"

//W25X series/Q series chip list
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

extern uint16_t W25QXX_TYPE;

//command table
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg1		0x05
#define W25X_ReadStatusReg2		0x35
#define W25X_ReadStatusReg3		0x15
#define W25X_WriteStatusReg1    0x01
#define W25X_WriteStatusReg2    0x31
#define W25X_WriteStatusReg3    0x11
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

void W25QXX_Init(QSPI_TypeDef* QSPIx);                                 //spi init
uint16_t W25QXX_ReadID(QSPI_TypeDef* QSPIx);                           //Read FLASH ID
uint8_t W25QXX_ReadSR(QSPI_TypeDef* QSPIx, uint8_t regno);                   //read status register
void W25QXX_4ByteAddr_Enable(QSPI_TypeDef* QSPIx);                     //Enable 4-byte address mode
void W25QXX_Write_SR(QSPI_TypeDef* QSPIx, uint8_t regno,uint8_t sr);         //write status register
void W25QXX_Write_Enable(QSPI_TypeDef* QSPIx);                         //write enable
void W25QXX_Write_Disable(QSPI_TypeDef* QSPIx);                        //write protection
void W25QXX_Write_NoCheck(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25QXX_Read(QSPI_TypeDef* QSPIx, uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);            //read flash
void W25QXX_Write(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);       //write to flash
void W25QXX_Erase_Chip(QSPI_TypeDef* QSPIx);                           //whole chip erase
void W25QXX_Erase_Sector(QSPI_TypeDef* QSPIx, uint32_t Dst_Addr);            //Sector erase
void W25QXX_Wait_Busy(QSPI_TypeDef* QSPIx);                            //wait for idle
void W25QXX_PowerDown(QSPI_TypeDef* QSPIx);                            //Enter power down mode
void W25QXX_WAKEUP(QSPI_TypeDef* QSPIx);                               // wake up
void W25QXX_CS(QSPI_TypeDef* QSPIx, uint8_t p);                              // CS control
#endif
