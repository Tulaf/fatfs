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

#ifndef _NS_SDIO_WIFI_H
#define _NS_SDIO_WIFI_H

#include "ns_sdio.h"
#include "ns_sdmmc.h"
#include <stdio.h>

/* SDIO FUNCTION*/
#define SDIO_FUNC_0 0
#define SDIO_FUNC_1 1
#define SDIO_FUNC_2 2
#define SDIO_FUNC_3 3
#define SDIO_FUNC_4 4
#define SDIO_FUNC_5 5
#define SDIO_FUNC_6 6
#define SDIO_FUNC_7 7
#define SDIO_FUNC_MAX 7

/* CCCR */
#define SDIO_CCCR_SDIO_VERSION 0x0
#define SDIO_CCCR_SD_VERSION 0x1
#define SDIO_CCCR_IO_ENABLE 0x2
#define SDIO_CCCR_IO_READY 0x3
#define SDIO_CCCR_INT_ENABLE 0x4
#define SDIO_CCCR_INT_PENDING 0x5
#define SDIO_CCCR_IO_ABORT 0x6
#define SDIO_CCCR_BUS_CONTROL 0x7
#define SDIO_CCCR_CARD_CAP 0x8
#define SDIO_CCCR_CIS_PTR 0x9
#define SDIO_CCCR_BUS_SUSPEND 0xc
#define SDIO_CCCR_FUNC_SEL 0xd
#define SDIO_CCCR_EXEC_FLAG 0xe
#define SDIO_CCCR_READY_FLG 0xf
#define SDIO_CCCR_BLK_SIZE 0x10
#define SDIO_CCCR_PWR_CONTROL 0x12
#define SDIO_CCCR_BUS_SPEED_SEL 0x13
#define SDIO_CCCR_UHSI_SUPPORT 0x14
#define SDIO_CCCR_DRV_STRENGTH 0x15
#define SDIO_CCCR_INT_EXTERN 0x16

/* SDIO block size */
#define SDIO_DEFAULT_BLK_SIZE 256
/* SDIO */
#define SDIO_RETRY_MAX 100
/* SDIO */
#define SDIO_24M_DATATIMEOUT ((uint32_t)2400000)
#define SDIO_1M_DATATIMEOUT ((uint32_t)100000)
#define SDIO_FLAG_ALL ((uint32_t)0x00FFFFFF)

/* SDIO */
#define SDIO_EXCU_READ 0
#define SDIO_EXCU_WRITE 1

/* SDIO CLK */
#define SDIO_CLK_400KHZ 0x76
#define SDIO_CLK_1MHZ 0x2E
#define SDIO_CLK_24MHZ 0x00

/* R4 response */
#define OCR_IN_R4(x) (x & 0x00ffffff)
#define S18A_IN_R4(x) ((x >> 24) & 0x1)
#define MEM_IN_R4(x) ((x >> 27) & 0x1)
#define FUNC_NUM_IN_R4(x) ((x >> 28) & 0x7)
#define C_IN_R4(x) ((x >> 31) & 0x1)

/* R6 response */
#define CS_IN_R6(x) (x & 0xffff)
#define RCA_IN_R6(x) ((x >> 16) & 0xffff)

#define SDIO_FBR_BASE(x) ((x)*0x100)

#define SDIO_DCTRL_DBLOCKSIZE_Pos (4U)
#define SDIO_DCTRL_DBLOCKSIZE_Msk                                                  (0xFU << SDIO_DCTRL_DBLOCKSIZE_Pos) /*!< 0x000000F0 */
#define SDIO_DCTRL_DBLOCKSIZE                                                      SDIO_DCTRL_DBLOCKSIZE_Msk /*!<DBLOCKSIZE[3:0] bits (Data block size) */
#define SDIO_DCTRL_DBLOCKSIZE_0                                                    (0x1U << SDIO_DCTRL_DBLOCKSIZE_Pos) /*!< 0x0010 */
#define SDIO_DCTRL_DBLOCKSIZE_1                                                    (0x2U << SDIO_DCTRL_DBLOCKSIZE_Pos) /*!< 0x0020 */
#define SDIO_DCTRL_DBLOCKSIZE_2                                                    (0x4U << SDIO_DCTRL_DBLOCKSIZE_Pos) /*!< 0x0040 */
#define SDIO_DCTRL_DBLOCKSIZE_3                                                    (0x8U << SDIO_DCTRL_DBLOCKSIZE_Pos) /*!< 0x0080 */

/** @defgroup SDIO_LL_Data_Block_Size  Data Block Size
 * @{
 */
#define SDIO_DATABLOCK_SIZE_1B 0x00000000U
#define SDIO_DATABLOCK_SIZE_2B SDIO_DCTRL_DBLOCKSIZE_0
#define SDIO_DATABLOCK_SIZE_4B SDIO_DCTRL_DBLOCKSIZE_1
#define SDIO_DATABLOCK_SIZE_8B                                                     (SDIO_DCTRL_DBLOCKSIZE_0 | SDIO_DCTRL_DBLOCKSIZE_1)
#define SDIO_DATABLOCK_SIZE_16B SDIO_DCTRL_DBLOCKSIZE_2
#define SDIO_DATABLOCK_SIZE_32B                                                    (SDIO_DCTRL_DBLOCKSIZE_0 | SDIO_DCTRL_DBLOCKSIZE_2)
#define SDIO_DATABLOCK_SIZE_64B                                                    (SDIO_DCTRL_DBLOCKSIZE_1 | SDIO_DCTRL_DBLOCKSIZE_2)
#define SDIO_DATABLOCK_SIZE_128B                                                   (SDIO_DCTRL_DBLOCKSIZE_0 | SDIO_DCTRL_DBLOCKSIZE_1 |                            SDIO_DCTRL_DBLOCKSIZE_2)
#define SDIO_DATABLOCK_SIZE_256B SDIO_DCTRL_DBLOCKSIZE_3
#define SDIO_DATABLOCK_SIZE_512B                                                   (SDIO_DCTRL_DBLOCKSIZE_0 | SDIO_DCTRL_DBLOCKSIZE_3)
#define SDIO_DATABLOCK_SIZE_1024B                                                  (SDIO_DCTRL_DBLOCKSIZE_1 | SDIO_DCTRL_DBLOCKSIZE_3)
#define SDIO_DATABLOCK_SIZE_2048B                                                  (SDIO_DCTRL_DBLOCKSIZE_0 | SDIO_DCTRL_DBLOCKSIZE_1 |                            SDIO_DCTRL_DBLOCKSIZE_3)
#define SDIO_DATABLOCK_SIZE_4096B                                                  (SDIO_DCTRL_DBLOCKSIZE_2 | SDIO_DCTRL_DBLOCKSIZE_3)
#define SDIO_DATABLOCK_SIZE_8192B                                                  (SDIO_DCTRL_DBLOCKSIZE_0 | SDIO_DCTRL_DBLOCKSIZE_2 |                            SDIO_DCTRL_DBLOCKSIZE_3)
#define SDIO_DATABLOCK_SIZE_16384B                                                 (SDIO_DCTRL_DBLOCKSIZE_1 | SDIO_DCTRL_DBLOCKSIZE_2 |                            SDIO_DCTRL_DBLOCKSIZE_3)

#define IS_SDIO_BLOCK_SIZE(SIZE)                                                   (((SIZE) == SDIO_DATABLOCK_SIZE_1B) ||                                          ((SIZE) == SDIO_DATABLOCK_SIZE_2B) ||                                          ((SIZE) == SDIO_DATABLOCK_SIZE_4B) ||                                          ((SIZE) == SDIO_DATABLOCK_SIZE_8B) ||                                          ((SIZE) == SDIO_DATABLOCK_SIZE_16B) ||                                         ((SIZE) == SDIO_DATABLOCK_SIZE_32B) ||                                         ((SIZE) == SDIO_DATABLOCK_SIZE_64B) ||                                         ((SIZE) == SDIO_DATABLOCK_SIZE_128B) ||                                        ((SIZE) == SDIO_DATABLOCK_SIZE_256B) ||                                        ((SIZE) == SDIO_DATABLOCK_SIZE_512B) ||                                        ((SIZE) == SDIO_DATABLOCK_SIZE_1024B) ||                                       ((SIZE) == SDIO_DATABLOCK_SIZE_2048B) ||                                       ((SIZE) == SDIO_DATABLOCK_SIZE_4096B) ||                                       ((SIZE) == SDIO_DATABLOCK_SIZE_8192B) ||                                       ((SIZE) == SDIO_DATABLOCK_SIZE_16384B))

/* SDIO read-read mode */
#define SDIO_EXCU_READ 0
#define SDIO_EXCU_WRITE 1

/* CIS tuple code */
typedef enum {
    CISTPL_NULL = 0x00,   /* Null tuple */
    CISTPL_VERS_1 = 0x15, /* Level 1 version/product-information */
    CISTPL_MANFID = 0x20, /* Manufacturer Identification String Tuple */
    CISTPL_FUNCID = 0x21, /* Function Identification Tuple */
    CISTPL_FUNCE = 0x22,  /* Function Extensions */
    CISTPL_END = 0xff,    /* The End-of-chain Tuple */
} cis_tuple_code_e;

typedef enum
{
    SDIO_BUS_WIDTH_1 = 0x0,
    SDIO_BUS_WIDTH_4 = 0x2,
    SDIO_BUS_WIDTH_8 = 0x3,
} bus_width_e;

/* func status */
typedef enum {
    FUNC_DISABLE = 0,
    FUNC_ENABLE = 1,
} func_status_e;

/* func int en */
typedef enum {
    FUNC_INT_DISABLE = 0,
    FUNC_INT_ENABLE = 1,
} func_int_status_e;

/* func int status */
typedef struct {
    uint8_t func_num;        /* funcnum */
    uint8_t func_status;     /* func enable 0:disable 1:enable  */
    uint8_t func_int_status; /* func starus 0:disable 1:enable */
    uint16_t cur_blk_size;   /* func block size */
    uint16_t max_blk_size;   /* func max block size */
} sdio_func_t;
typedef sdio_func_t *psdio_func_t;

typedef struct {
    uint8_t func_total_num; /* func total num */
    uint8_t cccr_version;   /* cccr version */
    uint8_t sdio_version;   /* sdio version */
    uint8_t sdio_int_mgr; /* sdio int mgr */
    uint16_t manf_code;   /* mgr code */
    uint16_t manf_info;   /* mgr info*/
    uint8_t int_occur;
    psdio_func_t func[SDIO_FUNC_MAX]; /* func point */
} sdio_core_t;
typedef sdio_core_t *psdio_core_t;

void hw_hex_dump(uint8_t *data, int len);
uint8_t hw_sdio_init(void);
uint8_t hw_chip_reset(void);
uint8_t hw_sdio_reset(void);
static uint8_t hw_sdio_check_err();
static uint8_t hw_sdio_set_dblocksize(uint32_t *struct_dblocksize,
                                      uint32_t block_size);

static uint8_t hw_sdio_cmd3(uint32_t para, uint32_t *resp);
static uint8_t hw_sdio_cmd5(uint32_t para, uint32_t *resp, uint32_t retry_max);
static uint8_t hw_sdio_cmd7(uint32_t para, uint32_t *resp);
static uint8_t hw_sdio_parse_r4(uint32_t r4);
static uint8_t hw_sdio_parse_r6(uint32_t r6, uint32_t *rca);
static uint8_t hw_sdio_cis_read_parse(uint8_t func_num, uint32_t cis_ptr);
uint8_t hw_sdio_set_blk_size(uint8_t func_num, uint16_t blk_size);
uint8_t hw_sdio_cmd52(uint8_t write, uint8_t func_num, uint32_t address,
                      uint8_t para, uint8_t *resp);
uint8_t hw_sdio_cmd53(uint8_t write, uint8_t func_num, uint32_t address,
                      uint8_t incr_addr, uint8_t *buf, uint32_t size);
uint8_t hw_sdio_enable_func(uint8_t func_num);
uint8_t hw_sdio_enable_mgr_int(void);
uint8_t hw_sdio_enable_func_int(uint8_t func_num);
uint8_t hw_sdio_get_cis_ptr(uint8_t func_num, uint32_t *ptr_address);
uint8_t hw_sdio_set_bus_width(uint8_t bus_width);
uint8_t hw_sdio_get_cccr_version(uint8_t *cccr_version);
uint8_t hw_sdio_get_sdio_version(uint8_t *sdio_version);
#endif
/* _NS_SDIO_WIFI_H */
