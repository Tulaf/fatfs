#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ns_qspi_w25qxx.h"
#include "ns_qspi.h"

void SPI_CS(QSPI_TypeDef* QSPIx, uint8_t p){
    if(p == 0){
        QSPI_CS_Enable(QSPIx, QSPI_CSID_NUM_CS0, DISABLE);
    }else{
        QSPI_CS_Enable(QSPIx, QSPI_CSID_NUM_CS0, ENABLE);
    }
}

void SPI_FLASH_Init(QSPI_TypeDef* QSPIx)
{
    SPI_CS(QSPIx, 0);
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* QSPI1 parameter config */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_HIGH;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_64;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    /* QSPI1 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    QSPI_Init(QSPIx, &spi_init_struct);
    QSPIx->CSMODE |= QSPI_CSMODE_MODE_HOLD;
}

/**
   * @brief Erase FLASH sector
   * @param SectorAddr: sector address to be erased
   * @retval None
   */
void SPI_FLASH_SectorErase(QSPI_TypeDef* QSPIx, uint32_t SectorAddr)
{
    /* Send FLASH write enable command */
    SPI_FLASH_WriteEnable(QSPIx);
    SPI_FLASH_WaitForWriteEnd(QSPIx);
    /* Erase sector */
    /* Select FLASH: CS low level */
    SPI_CS(QSPIx, 1);
    /* Send sector erase command */
    SPI_FLASH_SendByte(QSPIx, W25X_SectorErase);
    /*Send the high bit of the erase sector address*/
    SPI_FLASH_SendByte(QSPIx, (SectorAddr & 0xFF0000) >> 16);
    /* Send the middle bit of the erased sector address */
    SPI_FLASH_SendByte(QSPIx, (SectorAddr & 0xFF00) >> 8);
    /* Send the low bit of the erase sector address */
    SPI_FLASH_SendByte(QSPIx, SectorAddr & 0xFF);
    /* stop signal FLASH: CS high level */
    SPI_CS(QSPIx, 0);
    /* wait for erase to complete */
    SPI_FLASH_WaitForWriteEnd(QSPIx);
}

/**
   * @brief Erase FLASH sector, erase the whole chip
   * @param None
   * @retval None
   */
void SPI_FLASH_BulkErase(QSPI_TypeDef* QSPIx)
{
    /* Send FLASH write enable command */
    SPI_FLASH_WriteEnable(QSPIx);

    /* Block Erase */
    /* Select FLASH: CS low level */
    SPI_CS(QSPIx, 1);
    /* Send block erase command */
    SPI_FLASH_SendByte(QSPIx, W25X_ChipErase);
    /* stop signal FLASH: CS high level */
    SPI_CS(QSPIx, 0);
    /* wait for erase to complete */
    SPI_FLASH_WaitForWriteEnd(QSPIx);
}

/**
   * @brief Write data to FLASH page by page, you need to erase the sector before calling this function to write data
   * @param pBuffer, pointer to write data
   * @param WriteAddr, write address
   * @param NumByteToWrite, write data length, must be less than or equal to SPI_FLASH_PerWritePageSize
   * @retval None
   */
void SPI_FLASH_PageWrite(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /* Send FLASH write enable command */
    SPI_FLASH_WriteEnable(QSPIx);

    /* Select FLASH: CS low level */
    SPI_CS(QSPIx, 1);
    /* Write page write command */
    SPI_FLASH_SendByte(QSPIx, W25X_PageProgram);
    /*Send the high bit of the write address*/
    SPI_FLASH_SendByte(QSPIx, (WriteAddr & 0xFF0000) >> 16);
    /*Send the median of the write address*/
    SPI_FLASH_SendByte(QSPIx, (WriteAddr & 0xFF00) >> 8);
    /*Send the low bit of the write address*/
    SPI_FLASH_SendByte(QSPIx, WriteAddr & 0xFF);

    if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
        FLASH_ERROR("SPI_FLASH_PageWrite too large!");
    }

    /* data input*/
    while (NumByteToWrite--)
    {
        /* Send the current byte data to be written */
        SPI_FLASH_SendByte(QSPIx, *pBuffer);
        /* point to the next byte of data */
        pBuffer++;
    }

    /* stop signal FLASH: CS high level */
    SPI_CS(QSPIx, 0);

    /* Wait for writing to complete */
    SPI_FLASH_WaitForWriteEnd(QSPIx);
}

/**
   * @brief Write data to FLASH, you need to erase the sector before calling this function to write data
   * @param pBuffer, pointer to write data
   * @param WriteAddr, write address
   * @param NumByteToWrite, write data length
   * @retval None
   */
void SPI_FLASH_BufferWrite(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    /*mod operation to find the remainder, if writeAddr is an integer multiple of SPI_FLASH_PageSize, the value of the operation result Addr is 0*/
    Addr = WriteAddr % SPI_FLASH_PageSize;

    /* The difference is count data values, which can be aligned to the page address*/
    count = SPI_FLASH_PageSize - Addr;
    /* Calculate how many integer pages to write*/
    NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
    /*mod operation to find the remainder, calculate the number of bytes remaining less than one page*/
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    /* Addr=0, then WriteAddr is just aligned by page */
    if (Addr == 0)
    {
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
        /* first write the integer page */
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*If there is extra data that is less than one page, write it out*/
            SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, NumOfSingle);
        }
    }
    /* If the address is not aligned with SPI_FLASH_PageSize */
    else
    {
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            /*The remaining count positions of the current page are smaller than NumOfSingle, and one page cannot be finished*/
            if (NumOfSingle > count)
            {
                temp = NumOfSingle - count;
                /* fill the current page first */
                SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, count);

                WriteAddr += count;
                pBuffer += count;
                /* Write the rest of the data */
                SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, temp);
            }
            else /*The remaining count positions of the current page can finish writing NumOfSingle data*/
            {
                SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            /*Address misalignment and extra count are processed separately, do not add this operation*/
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            /* Write count data first, in order to align the address to be written next time */
            SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, count);

            /* Then repeat the address alignment */
            WriteAddr += count;
            pBuffer += count;
            /*Write all integer pages*/
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*If there is extra data that is less than one page, write it out*/
            if (NumOfSingle != 0)
            {
                SPI_FLASH_PageWrite(QSPIx, pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/**
   * @brief read FLASH data
   * @param pBuffer, the pointer to store the read data
   * @param ReadAddr, read address
   * @param NumByteToRead, read data length
   * @retval None
   */
void SPI_FLASH_BufferRead(QSPI_TypeDef* QSPIx, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /* Dynamically apply for memory to prevent stack overflow */
    // pBuffer = (uint8_t*)malloc(sizeof(uint8_t) * NumByteToRead);
    /* Select FLASH: CS low level */
    SPI_CS(QSPIx, 1);

    /* Send read command */
    SPI_FLASH_SendByte(QSPIx, W25X_ReadData);

    /* send read address high */
    SPI_FLASH_SendByte(QSPIx, (ReadAddr & 0xFF0000) >> 16);
    /* Send read address bit */
    SPI_FLASH_SendByte(QSPIx, (ReadAddr& 0xFF00) >> 8);
    /* send read address low */
    SPI_FLASH_SendByte(QSPIx, ReadAddr & 0xFF);

    /* read data */
    while (NumByteToRead--) /* while there is data to be read */
    {
        /* read a byte */
        *pBuffer = SPI_FLASH_SendByte(QSPIx, Dummy_Byte);
        /* point to the next byte buffer */
        printf("%#x ",*pBuffer);
        pBuffer++;
    }

    // free(pBuffer);
    SPI_CS(QSPIx, 0);
}
/**
   * @brief read FLASH ID
   * @param None
   * @retval FLASH ID
   */
uint32_t SPI_FLASH_ReadID(QSPI_TypeDef* QSPIx)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* Start communication: CS low level */
    SPI_CS(QSPIx, 1);

    /* Send JEDEC command, read ID */
    SPI_FLASH_SendByte(QSPIx, W25X_JedecDeviceID);

    /* Read a byte of data */
    Temp0 = SPI_FLASH_SendByte(QSPIx, Dummy_Byte);

    /* Read a byte of data */
    Temp1 = SPI_FLASH_SendByte(QSPIx, Dummy_Byte);

    /* Read a byte of data */
    Temp2 = SPI_FLASH_SendByte(QSPIx, Dummy_Byte);

    /* Stop communication: CS high level */
    SPI_CS(QSPIx, 0);

    /* Combine the data as the return value of the function */
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    printf("flash_id = %#x\n",Temp);
    return Temp;
}
  /**
   * @brief read FLASH Device ID
   * @param None
   * @retval FLASH Device ID
   */
uint32_t SPI_FLASH_ReadDeviceID(QSPI_TypeDef* QSPIx)
{
    uint32_t Temp = 0;

    /* Select the FLASH: Chip Select low */
    SPI_CS(QSPIx, 1);

    /* Send "RDID" instruction */
    SPI_FLASH_SendByte(QSPIx, W25X_DeviceID);
    SPI_FLASH_SendByte(QSPIx, Dummy_Byte);
    SPI_FLASH_SendByte(QSPIx, Dummy_Byte);
    SPI_FLASH_SendByte(QSPIx, Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp = SPI_FLASH_SendByte(QSPIx, Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    SPI_CS(QSPIx, 0);

    return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(QSPI_TypeDef* QSPIx, uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_CS(QSPIx, 1);

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(QSPIx, W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte(QSPIx, (ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte(QSPIx, (ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(QSPIx, ReadAddr & 0xFF);
}


 /**
   * @brief Use SPI to read a byte of data
   * @param None
   * @retval returns the received data
   */
uint8_t SPI_FLASH_ReadByte(QSPI_TypeDef* QSPIx)
{
    return (SPI_FLASH_SendByte(QSPIx, Dummy_Byte));
}

/**
   * @brief Use SPI to send a byte of data
   * @param byte: the data to be sent
   * @retval returns the received data
   */
uint8_t SPI_FLASH_SendByte(QSPI_TypeDef* QSPIx, uint8_t Txdata)
{
    uint8_t Rxdata;
    QSPI_TransmitReceive(QSPIx, &Txdata, &Rxdata);
    return Rxdata;
}

/**
   * @brief Send write enable command to FLASH
   * @param none
   * @retval none
   */
void SPI_FLASH_WriteEnable(QSPI_TypeDef* QSPIx)
{
    SPI_CS(QSPIx, 1);
    SPI_FLASH_SendByte(QSPIx, W25X_WriteEnable);
    SPI_CS(QSPIx, 0);
}

/**
   * @brief Wait for the WIP(BUSY) flag to be set to 0, that is, wait until the internal data of FLASH is written
   * @param none
   * @retval none
   */
void SPI_FLASH_WaitForWriteEnd(QSPI_TypeDef* QSPIx)
{
    uint8_t FLASH_Status = 0;
    SPI_CS(QSPIx, 1);
    SPI_FLASH_SendByte(QSPIx, W25X_ReadStatusReg);

    do
    {
        FLASH_Status = SPI_FLASH_SendByte(QSPIx, Dummy_Byte);
    }
    while ((FLASH_Status & WIP_Flag) == SET);
    SPI_CS(QSPIx, 0);
}

//Enter power-down mode
void SPI_Flash_PowerDown(QSPI_TypeDef* QSPIx)
{
    /* Communication start: CS low */
    SPI_CS(QSPIx, 1);

    /* Send power down command */
    SPI_FLASH_SendByte(QSPIx, W25X_PowerDown);

    /*communication end: CS high */
    SPI_CS(QSPIx, 0);
}

//wake
void SPI_Flash_WAKEUP(QSPI_TypeDef* QSPIx)
{
    /*select FLASH: CS low */
    SPI_CS(QSPIx, 1);

    /* Send power on command */
    SPI_FLASH_SendByte(QSPIx, W25X_ReleasePowerDown);

    /* stop signal FLASH: CS high */
    SPI_CS(QSPIx, 0);
}
