/*--------------------------- Include ---------------------------*/
#include "nuclei_sdk_hal.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ff.h"
#include "diskio.h"

/*-------------------------- Variable ---------------------------*/
extern uint32_t CSD_Tab[4];
extern uint32_t CID_Tab[4];
extern uint32_t RCA;
extern SDMMC_CardInfo SDCardInfo;
extern EmmcCardInfo MyEmmcCardInfo;
extern uint8_t CardType;
extern uint32_t DeviceMode;
extern uint32_t BusWidth;
extern uint32_t BusMode;

#define FATFS_WR_SIZE 1024*8

void show_sdcard_info(void)
{
    printf("MyEmmcCardInfo.CardType %d\n", MyEmmcCardInfo.CardType);
    switch (MyEmmcCardInfo.CardType) {
        case SDIO_STD_CAPACITY_SD_CARD_V1_1:
            printf("Card Type:SDSC V1.1\r\n");
            break;
        case SDIO_STD_CAPACITY_SD_CARD_V2_0:
            printf("Card Type:SDSC V2.0\r\n");
            break;
        case SDIO_HIGH_CAPACITY_SD_CARD:
            printf("Card Type:SDHC V2.0\r\n");
            break;
        case SDIO_MULTIMEDIA_CARD:
            printf("Card Type:MMC Card\r\n");
            break;
    }
    printf("Card ManufacturerID:%d\r\n", MyEmmcCardInfo.EmmcCid.ManufacturerID);
    printf("Card RCA:%d\r\n", MyEmmcCardInfo.RCA);
    printf("Card Capacity:%d MB\r\n", (uint32_t)(MyEmmcCardInfo.CardCapacity >> 20));
    printf("Card BlockSize:%d\r\n\r\n", MyEmmcCardInfo.CardBlockSize);
}

void sdio_config(void)
{
    SDIO_SetDateTimeout(SDIO0, 0xFFFFFFFF);
    SDIO_Clock_Set(SDIO0, 0x31);
    SDIO_StopClkEn(SDIO0, ENABLE);
    SDIO_DmaInterruptEn(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_EN, ENABLE);
    SDIO_DmaInterruptEn(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_EN, ENABLE);
}

__attribute__ ((aligned (4))) uint8_t buffer_all[FATFS_WR_SIZE];
void copyfile(uint8_t * srcfilename, uint8_t * destfilename)
{

    UINT br,bw;
    uint32_t f_res;
    FIL file_src;
    FIL file_dest;

    f_res = f_open(&file_src, srcfilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
    if (f_res == FR_OK)
    {
        f_res = f_open(&file_dest, destfilename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ | FA_OPEN_ALWAYS);
            if(f_res!= FR_OK)
            {
                return ;
            }
    }
    printf("start copy file.... \r\n");
    while (f_res == 0)
    {
            f_res = f_read(&file_src, buffer_all, FATFS_WR_SIZE, &br);
            printf("f_res = %d,br= %d   ", f_res, br);
            if (f_res || br == 0) break;
            f_res = f_write(&file_dest, buffer_all, br, &bw);
            printf("f_res = %d,br= %d,bw=%d \r\n", f_res, br, bw);
            if (f_res || bw < br) break;
    }
    f_close(&file_src);
    f_close(&file_dest);
    printf("\r\ncopyfile finish\r\n");
}

int main(void)
{
    #ifdef MISC_HAS_SDIO0_HAS_CLK
    sdio0_clk_en(ENABLE);
    sdio0_dma_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_SDIO0_RST
    sdio0_set_rst(DISABLE);
    sdio0_set_rst(ENABLE);
    sdio0_dma_set_rst(DISABLE);
    sdio0_dma_set_rst(ENABLE);
    #endif

    /* default mode: dma 4-lines mode */
    CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1;
    DeviceMode = SD_DMA_MODE;
    BusWidth = SDIO_DATA_SETUP_MODE_QUAD;
    BusMode = SDIO_SDR_MODE;

    sdio_config();
    int i, j;
    FIL file;
    FIL file1;
    FATFS fatfs;
    static FRESULT res;
    FILINFO fno;
    unsigned int counter;

    BYTE work[FF_MAX_SS]; /* Working buffer */

    MKFS_PARM fs_parm = {
        /* filesystem parameter: format = FAT32, other use default val */
        .fmt = FM_FAT32, .n_fat = 0, .au_size = 0, .align = 0, .n_root = 0,
    };

lab:
    res = f_mount(&fatfs, "0:", 1); 
    if (res == FR_NO_FILESYSTEM) {
        printf("start formatting...\r\n");
        res = f_mkfs((const TCHAR *)"0:", &fs_parm, work, sizeof(work)); 
        if (res != FR_OK) {
            printf("f_mkfs fail : %d!\r\n", res);
        } else {
            printf("f_mkfs successful!\r\n");
            goto lab; 
        }
    } else if (res != RES_OK) {
        printf("f_mount error:%d!\r\n", res);
    } else {
        printf("f_mount successful!\r\n");
    }

    f_setlabel((const TCHAR *)"0:Nuclei"); 

    copyfile("0:/fatfs.txt", "0:/cp.txt"); 
    simulation_check();
}
