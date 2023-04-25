#include "nuclei_sdk_hal.h"

#include "diskio.h"
#include "ff.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t n = 0, j = 0;
uint8_t clkdiv = 0;
uint8_t state = 0;
int32_t status = 0;
SDMMC_Error errorstatus = SDMMC_OK;

extern SDMMC_CardInfo SDCardInfo;
extern uint8_t CardType;
extern uint32_t DeviceMode;
extern uint32_t BusWidth;
extern uint32_t BusMode;

#define FATFS_WR_SIZE 1024 * 8

void show_sdcard_info(void)
{
    switch (SDCardInfo.CardType) {
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
    printf("Card ManufacturerID:%d\r\n", SDCardInfo.SDMMC_cid.ManufacturerID);
    printf("Card RCA:%d\r\n", SDCardInfo.RCA);
    printf("Card Capacity:%d MB\r\n",
           (uint32_t)(SDCardInfo.CardCapacity >> 20));
    printf("Card BlockSize:%d\r\n\r\n", SDCardInfo.CardBlockSize);
}

__attribute__((aligned(4))) uint8_t buffer_all[FATFS_WR_SIZE];
void copyfile(uint8_t *srcfilename, uint8_t *destfilename)
{

    UINT br, bw;
    uint32_t f_res;
    FIL file_src;
    FIL file_dest;

    f_res = f_open(&file_src, srcfilename,
                   FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
    if (f_res == FR_OK) {
        f_res = f_open(&file_dest, destfilename,
                       FA_CREATE_ALWAYS | FA_WRITE | FA_READ | FA_OPEN_ALWAYS);
        if (f_res != FR_OK) {
            return;
        }
    }
    printf("start copy file.... \r\n");
    while (f_res == 0) {
        f_res = f_read(&file_src, buffer_all, FATFS_WR_SIZE, &br);
        printf("f_res = %d,br= %d   ", f_res, br);
        if (f_res || br == 0)
            break;
        f_res = f_write(&file_dest, buffer_all, br, &bw);
        printf("f_res = %d,br= %d,bw=%d \r\n", f_res, br, bw);
        if (f_res || bw < br)
            break;
    }
    f_close(&file_src);
    f_close(&file_dest);
    printf("\r\ncopyfile finish\r\n");
}

void sdio_config(void)
{
    SDIO_SetDateTimeout(SDIO0, 0xFFFFFFFF);
    SDIO_Clock_Set(SDIO0, 0x31);
    SDIO_StopClkEn(SDIO0, ENABLE);
    SDIO_DmaInterruptEn(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_EN, ENABLE);
    SDIO_DmaInterruptEn(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_EN, ENABLE);
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
    
    sdio_config();

    /* default:dma 8 line mode */
    CardType = SDIO_MULTIMEDIA_CARD;
    DeviceMode = SD_DMA_MODE;
    BusWidth = SDIO_DATA_SETUP_MODE_OCTOL;
    BusMode = SDIO_SDR_MODE;

    int i, j;
    FIL file;
    FIL file1;
    FATFS fatfs;
    static FRESULT res;
    FILINFO fno;
    unsigned int counter;

    BYTE work[FF_MAX_SS]; /* Working buffer */

    /* filesystem parameter: format = FAT32, other use default val */
    MKFS_PARM fs_parm = {
        .fmt = FM_FAT32,
        .n_fat = 0,
        .au_size = 0,
        .align = 0,
        .n_root = 0,
    };

    res = f_mount(&fatfs, "0:", 1); 

    if (res == FR_NO_FILESYSTEM) {
        printf("emmc has no file system, start make filesystem...\n");
        res = f_mkfs((const TCHAR *)"0:", &fs_parm, work,
                     sizeof(work)); 
        if (res != FR_OK) {
            printf("f_mkfs fail : %d!\r\n", res);
            simulation_fail();
        } else {
            printf("f_mkfs successful!\n");
            res = f_mount(&fatfs, "0:", 1); 
        }
    }

    f_setlabel((const TCHAR *)"0:Nuclei"); 

    copyfile("0:/fatfs.txt", "0:/cp.txt"); 
    simulation_pass();
}
