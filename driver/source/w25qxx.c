#include "w25qxx.h"
#include "ns_qspi.h"

uint16_t W25QXX_TYPE = W25Q128;    //Default is W25Q128

uint8_t Spi_readwrite(QSPI_TypeDef* QSPIx, uint8_t Txdata){
    uint8_t Rxdata;
    QSPI_TransmitReceive(QSPIx, &Txdata, &Rxdata);
    return Rxdata;
}

void Spi_setspeed(QSPI_TypeDef* QSPIx, uint8_t speed){
    QSPIx->SCKDIV = speed;
}

void W25QXX_CS(QSPI_TypeDef* QSPIx, uint8_t p){
    if(p == 1){
        QSPI_CS_Enable(QSPIx, QSPI_CSID_NUM_CS0, DISABLE);
    }else{
        QSPI_CS_Enable(QSPIx, QSPI_CSID_NUM_CS0, ENABLE);
    }
}

void W25QXX_Init(QSPI_TypeDef* QSPIx)
{
    uint8_t temp;
    W25QXX_CS(QSPIx, 1);
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* QSPIx parameter config */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_HIGH;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    /* QSPIx parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    QSPI_Init(QSPIx, &spi_init_struct);
    QSPIx->CSMODE |= QSPI_CSMODE_MODE_HOLD;

    Spi_setspeed(QSPIx, QSPI_SCKDIV_PRESCALER_2);
    W25QXX_TYPE = W25QXX_ReadID(QSPIx);
    if(W25QXX_TYPE == W25Q256)
    {
        temp=W25QXX_ReadSR(QSPIx, 3);
        if((temp & 0X01) != 1)  //If it is a 4-byte address mode, enter the 4-byte address mode
        {
            W25QXX_CS(QSPIx, 0);
            Spi_readwrite(QSPIx, W25X_Enable4ByteAddr);
            W25QXX_CS(QSPIx, 1);
        }
    }
}

uint8_t W25QXX_ReadSR(QSPI_TypeDef* QSPIx, uint8_t regno)
{
     uint8_t byte=0,command=0;
     switch(regno)
     {
         case 1:
             command=W25X_ReadStatusReg1; //Read status register 1 command
             break;
         case 2:
             command=W25X_ReadStatusReg2; //Read status register 2 command
             break;
         case 3:
             command=W25X_ReadStatusReg3; //Read status register 3 command
             break;
         default:
             command=W25X_ReadStatusReg1;
             break;
     }
     W25QXX_CS(QSPIx, 0); //Enable the device
     Spi_readwrite(QSPIx, command); //Send read status register command
     byte=Spi_readwrite(QSPIx, 0Xff); //read a byte
     W25QXX_CS(QSPIx, 1); //Cancel chip selection
     return byte;
}

void W25QXX_Write_SR(QSPI_TypeDef* QSPIx, uint8_t regno,uint8_t sr)
{
    uint8_t command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;
            break;
        case 2:
            command=W25X_WriteStatusReg2;
            break;
        case 3:
            command=W25X_WriteStatusReg3;
            break;
        default:
            command=W25X_WriteStatusReg1;
            break;
    }
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, command);
    Spi_readwrite(QSPIx, sr);
    W25QXX_CS(QSPIx, 1);
}

void W25QXX_Write_Enable(QSPI_TypeDef* QSPIx)
{
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_WriteEnable);
    W25QXX_CS(QSPIx, 1);
}

void W25QXX_Write_Disable(QSPI_TypeDef* QSPIx)
{
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_WriteDisable);
    W25QXX_CS(QSPIx, 1);
}

uint16_t W25QXX_ReadID(QSPI_TypeDef* QSPIx)
{
    uint16_t Temp = 0;
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_ManufactDeviceID);
    Spi_readwrite(QSPIx, 0x00);
    Spi_readwrite(QSPIx, 0x00);
    Spi_readwrite(QSPIx, 0x00);
    Temp|=Spi_readwrite(QSPIx, 0xFF)<<8;
    Temp|=Spi_readwrite(QSPIx, 0xFF);
    W25QXX_CS(QSPIx, 1);
    return Temp;
}

void W25QXX_Read(QSPI_TypeDef* QSPIx, uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint16_t i;
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_FastReadData);
    if(W25QXX_TYPE == W25Q256)
    {
        Spi_readwrite(QSPIx, (uint8_t)((ReadAddr)>>24));
    }
    Spi_readwrite(QSPIx, (uint8_t)((ReadAddr)>>16));
    Spi_readwrite(QSPIx, (uint8_t)((ReadAddr)>>8));
    Spi_readwrite(QSPIx, (uint8_t)ReadAddr);
    Spi_readwrite(QSPIx, 0XFF);
    for(i=0;i<NumByteToRead;i++)
    {
        pBuffer[i]=Spi_readwrite(QSPIx, 0XFF);
    }
    W25QXX_CS(QSPIx, 1);
}

void W25QXX_Write_Page(QSPI_TypeDef* QSPIx, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t i;
    W25QXX_Write_Enable(QSPIx);
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_PageProgram);
    if(W25QXX_TYPE == W25Q256)
    {
        Spi_readwrite(QSPIx, (uint8_t)((WriteAddr)>>24));
    }
    Spi_readwrite(QSPIx, (uint8_t)((WriteAddr)>>16));
    Spi_readwrite(QSPIx, (uint8_t)((WriteAddr)>>8));
    Spi_readwrite(QSPIx, (uint8_t)WriteAddr);
    for(i=0;i<NumByteToWrite;i++)Spi_readwrite(QSPIx, pBuffer[i]);
    W25QXX_CS(QSPIx, 1);
    W25QXX_Wait_Busy(QSPIx);
}

void W25QXX_Write_NoCheck(QSPI_TypeDef* QSPIx, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain=256-WriteAddr%256;
    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;
    while(1)
    {
        W25QXX_Write_Page(QSPIx, pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)break;
         else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;

            NumByteToWrite-=pageremain;
            if(NumByteToWrite>256)pageremain=256;
            else pageremain=NumByteToWrite;
        }
    };
}

uint8_t W25QXX_BUFFER[4096];
void W25QXX_Write(QSPI_TypeDef* QSPIx, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t * W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr/4096;//Sector address
    secoff = WriteAddr%4096;//The offset in the sector
    secremain = 4096-secoff;//The size of the remaining space in the sector
    //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//for testing
    if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//No more than 4096 bytes
    while(1)
    {
        W25QXX_Read(QSPIx, W25QXX_BUF, secpos*4096,4096);//Read the content of the entire sector
        for(i=0;i<secremain;i++)//check data
        {
            if(W25QXX_BUF[secoff+i]!=0XFF)break;//Need to erase
        }
        if(i<secremain)//Need to erase
        {
            W25QXX_Erase_Sector(QSPIx, secpos);//Erase this sector
            for(i=0;i<secremain;i++) //Copy
            {
                W25QXX_BUF[i+secoff]=pBuffer[i];
            }
            W25QXX_Write_NoCheck(QSPIx, W25QXX_BUF,secpos*4096,4096);//Write the entire sector

        }else W25QXX_Write_NoCheck(QSPIx, pBuffer,WriteAddr,secremain);//Write what has been erased, directly write to the remaining section of the sector.
        if(NumByteToWrite==secremain)break;//write finished
        else//write not finished
        {
            secpos++;//Increase sector address by 1
            secoff=0;//The offset position is 0

            pBuffer+=secremain; //pointer offset
            WriteAddr+=secremain;//write address offset
            NumByteToWrite-=secremain;
            if(NumByteToWrite>4096)secremain=4096;
            else secremain=NumByteToWrite;
        }
    };
}

// erase the whole chip, waiting too long...
void W25QXX_Erase_Chip(QSPI_TypeDef* QSPIx)
{
    W25QXX_Write_Enable(QSPIx);
    W25QXX_Wait_Busy(QSPIx);
      W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_ChipErase);
    W25QXX_CS(QSPIx, 1);
    W25QXX_Wait_Busy(QSPIx);
}

void W25QXX_Erase_Sector(QSPI_TypeDef* QSPIx, uint32_t Dst_Addr)
{
    Dst_Addr*=4096;
    W25QXX_Write_Enable(QSPIx);
    W25QXX_Wait_Busy(QSPIx);
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_SectorErase);
    if(W25QXX_TYPE == W25Q256)
    {
        Spi_readwrite(QSPIx, (uint8_t)((Dst_Addr)>>24));
    }
    Spi_readwrite(QSPIx, (uint8_t)((Dst_Addr)>>16));
    Spi_readwrite(QSPIx, (uint8_t)((Dst_Addr)>>8));
    Spi_readwrite(QSPIx, (uint8_t)Dst_Addr);
    W25QXX_CS(QSPIx, 1);
    W25QXX_Wait_Busy(QSPIx);
}

void W25QXX_Wait_Busy(QSPI_TypeDef* QSPIx)
{
    while(W25QXX_ReadSR(QSPIx, (1) & 0x01 ) == 0x01);
}

void W25QXX_PowerDown(QSPI_TypeDef* QSPIx)
{
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_PowerDown);
    W25QXX_CS(QSPIx, 1);
    delay_1ms(3);
}

void W25QXX_WAKEUP(QSPI_TypeDef* QSPIx)
{
    W25QXX_CS(QSPIx, 0);
    Spi_readwrite(QSPIx, W25X_ReleasePowerDown);
    W25QXX_CS(QSPIx, 1);
    delay_1ms(3);
}

