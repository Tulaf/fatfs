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

/* Includes ------------------------------------------------------------------*/
#include "ns_sdio_wifi.h"
#include "ns_sdk_hal.h"
#include <string.h>

sdio_func_t hw_sdio_func[SDIO_FUNC_MAX];
sdio_core_t hw_sdio_core;
psdio_core_t phw_sdio_core = &hw_sdio_core;

static uint8_t hw_sdio_core_init(void);
static uint8_t hw_sdio_cmd3(uint32_t para, uint32_t *resp);
static uint8_t hw_sdio_cmd5(uint32_t para, uint32_t *resp, uint32_t retry_max);
static uint8_t hw_sdio_cmd7(uint32_t para, uint32_t *resp);
static uint8_t hw_sdio_parse_r4(uint32_t r4);
static uint8_t hw_sdio_parse_r6(uint32_t r6, uint32_t *rca);
static uint8_t hw_sdio_cis_read_parse(uint8_t func_num, uint32_t cis_ptr);
uint8_t hw_sdio_set_blk_size(uint8_t func_num, uint16_t blk_size);
static uint8_t hw_sdio_cmd53_read(uint8_t func_num, uint32_t address, uint8_t incr_addr, uint8_t *buf, uint32_t size, uint16_t cur_blk_size);
static uint8_t hw_sdio_cmd53_write(uint8_t func_num, uint32_t address, uint8_t incr_addr, uint8_t *buf, uint32_t size, uint16_t cur_blk_size);
uint8_t hw_sdio_enable_func(uint8_t func_num);
uint8_t hw_sdio_enable_mgr_int(void);
uint8_t hw_sdio_enable_func_int(uint8_t func_num);
uint8_t hw_sdio_get_cis_ptr(uint8_t func_num, uint32_t *ptr_address);
uint8_t hw_sdio_set_bus_width(uint8_t bus_width);
uint8_t hw_sdio_get_cccr_version(uint8_t *cccr_version);
uint8_t hw_sdio_get_sdio_version(uint8_t *sdio_version);

#ifdef HAS_LGPIO
uint8_t hw_chip_reset(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE, LGPIO0_IO_PORT_PINS_1_IOF_OVAL, 13, LGPIO0_IO_PORT_PINS_1_HS_SEL, 0, 0);
    
    LGPIO_Mode(LGPIO0, 0x2, PP);

    LGPIO_WriteBit(LGPIO0, 0x2, 0);
    delay_1ms(10);
    LGPIO_WriteBit(LGPIO0, 0x2, 1);
    delay_1ms(15);

    return SDMMC_OK;
}
#else
uint8_t hw_chip_reset(void){}
#endif
/******************************************************************************
 * function name:	hw_sdio_init
 * para:  		NULL
 * return: 	execution result
 * description:		SDIO init
******************************************************************************/
uint8_t hw_sdio_init()
{
    uint32_t rca;
    uint8_t func_index;
    uint32_t cmd3_para;
    uint32_t cmd3_resp;
    uint32_t cmd5_para;
    uint32_t cmd5_resp;
    uint32_t cmd7_para;
    uint32_t cmd7_resp;

    SDIO_DataSetupTypeDef SDIO_InitStruct = {0};
    printf("Enter: %s\n", __func__);
    hw_chip_reset();
    hw_sdio_core_init();

    SDIO_InitStruct.Data_mode = SDIO_DATA_SETUP_MODE_SINGLE;

    SDIO_DataSetup(SDIO0, &SDIO_InitStruct); 
    SDIO_SetDateTimeout(SDIO0, 0xffffffff);
    SDIO_Clock_Set(SDIO0, 0x31); 

    /* send cmd5 */
    cmd5_para = 0;
    if (hw_sdio_cmd5(cmd5_para, &cmd5_resp, SDIO_RETRY_MAX)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_ILLEGAL_CMD;
    }

    /* ocr 3.2V~3.4V*/
    cmd5_para = 0x300000;
    if (hw_sdio_cmd5(cmd5_para, &cmd5_resp, SDIO_RETRY_MAX)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_ILLEGAL_CMD;
    }

    /* parse out R4 */
    hw_sdio_parse_r4(cmd5_resp);

    /* send cmd3 for getting addr */
    cmd3_para = 0;
    if (hw_sdio_cmd3(cmd3_para, &cmd3_resp)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_ILLEGAL_CMD;
    }

    hw_sdio_parse_r6(cmd3_resp, &rca);

    /* send cmd7 for reative addr */
    cmd7_para = rca << 16;
    if (hw_sdio_cmd7(cmd7_para, &cmd7_resp)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_ILLEGAL_CMD;
    }

    /* get CCCR version and SDIO version*/
    hw_sdio_get_cccr_version(&phw_sdio_core->cccr_version);
    hw_sdio_get_sdio_version(&phw_sdio_core->sdio_version);

    /* chenge to 4 bus width, change to 16M clk */
    hw_sdio_set_bus_width(SDIO_BUS_WIDTH_4);
    SDIO_InitStruct.Data_mode = SDIO_DATA_SETUP_MODE_QUAD;
    SDIO_DataSetup(SDIO0, &SDIO_InitStruct); 
    SDIO_Clock_Set(SDIO0, 0x00);             

    /* get every func_CIS pointer and parse it */
    for (func_index = 0; func_index < phw_sdio_core->func_total_num; func_index++) {
        uint32_t cis_ptr;
        hw_sdio_get_cis_ptr(func_index, &cis_ptr);
        hw_sdio_cis_read_parse(func_index, cis_ptr);
    }

    /* enable Func */
    for (func_index = SDIO_FUNC_1; func_index < phw_sdio_core->func_total_num; func_index++) {
        hw_sdio_enable_func(func_index);
    }

    /* enable interrupt */
    hw_sdio_enable_mgr_int();
    for (func_index = SDIO_FUNC_1; func_index < phw_sdio_core->func_total_num; func_index++) {
        hw_sdio_enable_func_int(func_index);
    }

    /* set block size */
    for (func_index = SDIO_FUNC_1; func_index < phw_sdio_core->func_total_num; func_index++) {
        hw_sdio_set_blk_size(func_index, SDIO_DEFAULT_BLK_SIZE);
    }
    printf("Leave: %s\n", __func__);

    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_parse_r6
 * para:  		r6(IN)			-->R6 entry para
                rca(OUT)		-->rca return
 * return: 	execution result
 * description:		parse out R6 response
******************************************************************************/
static uint8_t hw_sdio_parse_r6(uint32_t r6, uint32_t *rca)
{
    printf("Enter: %s\n", __func__);
    if (rca) {
        *rca = RCA_IN_R6(r6);
        printf("Leave: %s\n", __func__);
        return SDMMC_OK;
    }
    printf("Leave: %s\n", __func__);
    return SDMMC_INVALID_PARAMETER;
}

/******************************************************************************
 * function name:	hw_sdio_set_blk_size
 * para:  		func_num(IN)		-->func index
                blk_size(IN)        -->set block size
 * return: 	execution result
 * description:		set func block size
******************************************************************************/
uint8_t hw_sdio_set_blk_size(uint8_t func_num, uint16_t blk_size)
{
    printf("Enter: %s\n", __func__);

    /* set block size */
    hw_sdio_cmd52(SDIO_EXCU_WRITE, SDIO_FUNC_0,
                  SDIO_FBR_BASE(func_num) + SDIO_CCCR_BLK_SIZE, blk_size & 0xff,
                  NULL);
    hw_sdio_cmd52(SDIO_EXCU_WRITE, SDIO_FUNC_0,
                  SDIO_FBR_BASE(func_num) + SDIO_CCCR_BLK_SIZE + 1,
                  (blk_size >> 8) & 0xff, NULL);

    /* renew func num struct */
    if ((phw_sdio_core->func)[func_num]) {
        (phw_sdio_core->func)[func_num]->cur_blk_size = blk_size;
    } else {
        return SDMMC_ERROR;
    }
    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_enable_func_int
 * para:  		func_num(IN)		-->func num
 * return: 	execution result
 * description:		func numenable to func interrupt
 ******************************************************************************/
uint8_t hw_sdio_enable_func_int(uint8_t func_num)
{
    uint8_t enable;
    printf("Enter: %s\n", __func__);
    if (func_num > SDIO_FUNC_MAX) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INVALID_PARAMETER;
    }

    /* CMD52 read CCCR4 value */
    if (!hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_CCCR_INT_ENABLE, 0, &enable)) {
        /* or operation, do not destroy the original value */
        enable |= (1 << func_num);
    } else {
        printf("Leave: %s\n", __func__);
        return SDMMC_INTERNAL_ERROR;
    }

    /* write back to CCCR4 */
    if (hw_sdio_cmd52(SDIO_EXCU_WRITE, SDIO_FUNC_0, SDIO_CCCR_INT_ENABLE, enable, NULL)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INTERNAL_ERROR;
    }

    /* renew func interruptstatus */
    if ((phw_sdio_core->func)[func_num]) {
        (phw_sdio_core->func)[func_num]->func_int_status = FUNC_INT_ENABLE;
    }

    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_enable_mgr_int
 * para:  		NULL
 * return: 	execution result
 * description:	according to func numenableCCCR int master switch
 ******************************************************************************/
uint8_t hw_sdio_enable_mgr_int(void)
{
    uint8_t enable;
    printf("Enter: %s\n", __func__);

    /* Read back CCCR4 value */
    if (!hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_CCCR_INT_ENABLE, 0,
                       &enable)) {
        /* The interrupt master switch is at CCCR4 bit 0, so or 1 */
        enable |= 0x1;
    } else {
        printf("Leave: %s\n", __func__);
        return SDMMC_INTERNAL_ERROR;
    }

    /* renew write back */
    if (hw_sdio_cmd52(SDIO_EXCU_WRITE, SDIO_FUNC_0, SDIO_CCCR_INT_ENABLE,
                      enable, NULL)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INTERNAL_ERROR;
    }

    /* renew INT manager status */
    phw_sdio_core->sdio_int_mgr = FUNC_INT_ENABLE;

    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_enable_func
 * para:  		func_num(IN)		-->func num
 * return: 	execution result
 * description:	according to func numenable specific func
 ******************************************************************************/
uint8_t hw_sdio_enable_func(uint8_t func_num)
{
    uint8_t enable;
    uint8_t ready = 0;
    printf("Enter: %s\n", __func__);
    if (func_num > SDIO_FUNC_MAX) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INVALID_PARAMETER;
    }

    /* CMD52 read CCCR2 value */
    if (!hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_CCCR_IO_ENABLE, 0,
                       &enable)) {
        /* or operation, do not destroy the original value */
        enable |= (1 << func_num);
    } else {
        printf("Leave: %s\n", __func__);
        return SDMMC_INVALID_PARAMETER;
    }

    /* renew write CCCR2 */
    if (hw_sdio_cmd52(SDIO_EXCU_WRITE, SDIO_FUNC_0, SDIO_CCCR_IO_ENABLE, enable,
                      NULL)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INVALID_PARAMETER;
    }

    /* wait for func ready */
    while (!(ready & (1 << func_num))) {
        hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_CCCR_IO_READY, 0,
                      &ready);
    }

    /* renew func status */
    if ((phw_sdio_core->func)[func_num]) {
        (phw_sdio_core->func)[func_num]->func_status = FUNC_ENABLE;
    }

    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_cis_read_parse
 * para:  		func_num(IN)			-->func index
                                cis_ptr(IN)			-->CIS pointer
 * return: 	execution result
 * description:		get CIS data and parse out, important data store to core struct
******************************************************************************/
static uint8_t hw_sdio_cis_read_parse(uint8_t func_num, uint32_t cis_ptr)
{
    /* There is such a sentence in the SDIO agreement:No SDIO card tuple can be longer than 257 bytes
     * 1 byte TPL_CODE + 1 byte TPL_LINK + FFh byte tuple body (and this 257 bytetuple ends the chain)
     * so we defined  data 255 bytes
     */
    uint8_t data[255];
    uint8_t index, len;
    uint8_t tpl_code = CISTPL_NULL;
    uint8_t tpl_link;
    uint32_t cis_ptr_temp = cis_ptr;

    printf("Enter: %s\n", __func__);
    while (tpl_code != CISTPL_END) {
        hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, cis_ptr_temp++, 0, &tpl_code);
        if (tpl_code == CISTPL_NULL)
            continue;

        /* this node data size */
        hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, cis_ptr_temp++, 0, &tpl_link);

        for (index = 0; index < tpl_link; index++)
            hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, cis_ptr_temp + index, 0, &data[index]);

        switch (tpl_code) {
            case CISTPL_VERS_1:
                printf("Product Information:");
                for (index = 2; data[index] != 0xff; index += len + 1) {
                    
                    len = strlen((char *)data + index);
                    if (len != 0)
                        printf(" %s", data + index);
                }
                printf("\n");
                break;
            case CISTPL_MANFID:
                
                printf("Manufacturer Code: 0x%04x\n",*(uint16_t *)data); 
                printf("Manufacturer Information: 0x%04x\n",*(uint16_t *)(data + 2)); 
                phw_sdio_core->manf_code = *(uint16_t *)data;
                phw_sdio_core->manf_info = *(uint16_t *)(data + 2);
                break;
            case CISTPL_FUNCID:
                
                printf("Card Function Code: 0x%02x\n",data[0]); 
                printf("System Initialization Bit Mask: 0x%02x\n",data[1]); 
                break;
            case CISTPL_FUNCE:
                
                if (data[0] == 0) {
                    
                    printf("Maximum Block Size case1: func: %d,size %d\n", func_num, *(uint16_t *)(data + 1));
                    printf("Maximum Transfer Rate Code: 0x%02x\n", data[3]);
                    if ((phw_sdio_core->func)[func_num]) {
                        (phw_sdio_core->func)[func_num]->max_blk_size = *(uint16_t *)(data + 1);
                    }
                } else {
                    
                    printf("Maximum Block Size case2 func: %d,size %d\n", func_num, *(uint16_t *)(data + 0x0c)); 
                    if ((phw_sdio_core->func)[func_num]) {
                        (phw_sdio_core->func)[func_num]->max_blk_size = *(uint16_t *)(data + 0x0c);
                    }
                }
                break;
            default:
                printf("[CIS Tuple 0x%02x] addr=0x%08x size=%d\n", tpl_code, cis_ptr_temp - 2, tpl_link);
#if HW_DEBUG_ENABLE > 0
                hw_hex_dump(data, tpl_link);
#endif
        }

        if (tpl_link == 0xff)
            break; 
        cis_ptr_temp += tpl_link;
    }

    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_set_bus_width
 * para:  		bus_width(IN)			--> data width
 * return: 	execution result
 * description:		set SDIO  data width
                                00b: 1-bit
                                01b: Reserved
                                10b: 4-bit bus
                                11b: 8-bit bus (only for embedded SDIO)
******************************************************************************/
uint8_t hw_sdio_set_bus_width(uint8_t bus_width)
{
    uint8_t width;
    printf("Enter: %s\n", __func__);

    if (!hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_CCCR_BUS_CONTROL, 0, &width)) {
        /* clean and set data width */
        width &= ~0x3;
        width |= bus_width;
    } else {
        printf("Leave: %s\n", __func__);
        return SDMMC_SWITCH_ERROR;
    }

    if (hw_sdio_cmd52(SDIO_EXCU_WRITE, SDIO_FUNC_0, SDIO_CCCR_BUS_CONTROL,
                      width, NULL)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_SWITCH_ERROR;
    }

    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_get_cis_ptr
 * para:  		func_num(IN)				-->func index
                                bus_width(OUT)			--> data width
 * return: 	execution result
 * description:		Obtain CIS pointer according to func index
******************************************************************************/
uint8_t hw_sdio_get_cis_ptr(uint8_t func_num, uint32_t *ptr_address)
{
    uint8_t index;
    printf("Enter: %s\n", __func__);

    uint32_t prt_temp = 0;
    if (func_num > SDIO_FUNC_MAX || (!ptr_address)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INVALID_PARAMETER;
    }

    /* The CIS pointers are: 0x9, 0xa, 0xb, and the obtained combination is the CIS pointer */
    for (index = 0; index < 3; index++) {
        uint8_t x;
        hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_FBR_BASE(func_num) + SDIO_CCCR_CIS_PTR + index, 0, &x);
        prt_temp |= x << (index * 8);
    }

    *ptr_address = prt_temp;
    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_get_sdio_version
 * para:  		sdio_version(OUT)		-->SDIO version
 * return: 	execution result
 * description:	according to get CCCR register, and SDIO version
                                SDIO version register address is 0x0
                                format is :
                                |-7-|-6-|-5-|-4-|-3-|-2-|-1-|-0-|
                                |--SDIO version-| CCCR version  |
                                Value SDIO Specification
                                00h SDIO Specification Version 1.00
                                01h SDIO Specification Version 1.10
                                02h SDIO Specification Version 1.20 (unreleased)
                                03h SDIO Specification Version 2.00
                                04h SDIO Specification Version 3.00
                                05h-0Fh Reserved for Future Use
******************************************************************************/
uint8_t hw_sdio_get_sdio_version(uint8_t *sdio_version)
{
    uint8_t version;
    printf("Enter: %s\n", __func__);
    if (!sdio_version) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INVALID_PARAMETER;
    }

    /* CMD52 read CCCR0 value */
    if (hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_CCCR_SDIO_VERSION, 0, &version)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_ERROR;
    }

    *sdio_version = phw_sdio_core->sdio_version = (version >> 4) & 0xf;
    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_get_cccr_version
 * para:  		cccr_version(OUT)		-->CCCR version
 * return: 	execution result
 * description:		according to get CCCRregister, and CCCRregister version
                                CCCR version register address is 0x0
                                format is :
                                |-7-|-6-|-5-|-4-|-3-|-2-|-1-|-0-|
                                |--SDIO version-| CCCR version  |
                                Value CCCR/FBR Format Version
                                00h CCCR/FBR defined in SDIO Version 1.00
                                01h CCCR/FBR defined in SDIO Version 1.10
                                02h CCCR/FBR defined in SDIO Version 2.00
                                03h CCCR/FBR defined in SDIO Version 3.00
                                04h-0Fh Reserved for Future Use
******************************************************************************/
uint8_t hw_sdio_get_cccr_version(uint8_t *cccr_version)
{
    uint8_t version;
    printf("Enter: %s\n", __func__);
    if (!cccr_version) {
        printf("Leave: %s\n", __func__);
        return SDMMC_INVALID_PARAMETER;
    }

    /* CMD52 read CCCR0 value */
    if (hw_sdio_cmd52(SDIO_EXCU_READ, SDIO_FUNC_0, SDIO_CCCR_SDIO_VERSION, 0,
                      &version)) {
        printf("Leave: %s\n", __func__);
        return SDMMC_ERROR;
    }

    *cccr_version = phw_sdio_core->cccr_version = version & 0xf;
    
    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_parse_r4
 * para:  		r4(IN)			-->R4 entry para
 * return: 	execution result
 * description:		parse out R4, get func number
 ******************************************************************************/
static uint8_t hw_sdio_parse_r4(uint32_t r4)
{
    printf("Enter: %s\n", __func__);
    uint32_t index = 0;

    phw_sdio_core->func_total_num = FUNC_NUM_IN_R4(r4);
    for (index = 0; index < phw_sdio_core->func_total_num; index++) {
        (phw_sdio_core->func)[index] = &hw_sdio_func[index];
        hw_sdio_func[index].func_num = index;
    }

    printf("Leave: %s\n", __func__);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_cmd7
 * para:  		para(IN)			-->entry para
                                resp(OUT)			-->return
 * return: 	execution result
 * description:		send cmd7
******************************************************************************/
static uint8_t hw_sdio_cmd7(uint32_t para, uint32_t *resp)
{
    uint8_t error_status;
    uint32_t response;
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;

    printf("Enter: %s\n", __func__);
    /* Send CMD7 SDIO_SEL_DESEL_CARD */
    SDIO_CmdInitStructInit(SDIO0, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = para;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SEL_DESEL_CARD;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;

    SDIO_SendCommand(SDIO0, &SDIO_CmdInitStructure);

    /* wait for sendcompleted */
    while (!SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_EOT));

    SDIO_ClearFlag(SDIO0, SDIO_STATUS_EOT);
    error_status = hw_sdio_check_err();

    if (SDMMC_OK != error_status) {
        return error_status;
    }
    /* get return result */
    response = SDIO0->RSP0;
    
    printf("Leave: %s\n", __func__);
    return (error_status);
}

/******************************************************************************
 * function name:	hw_sdio_cmd5
 * para:  		para(IN)			-->entry para
                                resp(OUT)			-->return
                                retry_max(IN)		-->max retry
 * return: 	execution result
 * description:		send cmd5
******************************************************************************/
static uint8_t hw_sdio_cmd5(uint32_t para, uint32_t *resp, uint32_t retry_max)
{
    uint32_t index;
    uint32_t response;
    uint8_t error_status;
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;

    printf("Enter: %s\n", __func__);
    SDIO_CmdInitStructInit(SDIO0, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = para;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SDIO_SEN_OP_COND;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    
    for (index = 0; index < retry_max; index++) {
        SDIO_SendCommand(SDIO0, &SDIO_CmdInitStructure);
        /* wait for send completed */
        while (!SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_EOT));
        
        SDIO_ClearFlag(SDIO0, SDIO_STATUS_EOT);
        error_status = hw_sdio_check_err();
        if (SDMMC_OK != error_status) {
            continue;
        }
        response = SDIO0->RSP0;
        
        /* Judging whether it is OK */
        if (C_IN_R4(response)) {
            if (resp) {
                *resp = response;
            }
            break;
        }
    }

    printf("Leave: %s\n", __func__);
    return error_status;
}

/******************************************************************************
 * function name:	hw_sdio_cmd3
 * para:  		para(IN)		-->send cmd3 para
                                resp			-->cmd3 return
 * return: 	execution result
 * description:		send cmd3
******************************************************************************/
static uint8_t hw_sdio_cmd3(uint32_t para, uint32_t *resp)
{
    uint8_t error_status;
    uint32_t response;
    printf("Enter: %s\n", __func__);
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    SDIO_CmdInitStructInit(SDIO0, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = para;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_REL_ADDR;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIO0, &SDIO_CmdInitStructure);

    /* wait for send completed */
    while (!SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_EOT));

    SDIO_ClearFlag(SDIO0, SDIO_STATUS_EOT);
    error_status = hw_sdio_check_err();

    if (SDMMC_OK != error_status) {
        return error_status;
    }
    /* get response result */
    response = SDIO0->RSP0;
    if (resp) {
        *resp = response;
    }

    printf("Leave: %s\n", __func__);
    return (error_status);
}
/******************************************************************************
 * function name:	hw_sdio_core_init
 * para:  		NULL
 * return: 	execution result
 * description:		sdio core init
 ******************************************************************************/
static uint8_t hw_sdio_core_init(void)
{
    memset(&hw_sdio_func, 0, sizeof(sdio_func_t) * SDIO_FUNC_MAX);
    memset(&hw_sdio_core, 0, sizeof(sdio_core_t));
    hw_sdio_func[SDIO_FUNC_0].func_status = FUNC_INT_ENABLE;
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_cmd52
 * para:  	write(IN)	  -->operation: read or write
 *          func_num(IN)  -->func  index address(IN) -->address address 
 *          para(IN)      -->write para
 *          resp(OUT)     -->return  data 
 * return: 	execution result
 * description:		ִCMD52 operation
 ******************************************************************************/
uint8_t hw_sdio_cmd52(uint8_t write, uint8_t func_num, uint32_t address,
                      uint8_t para, uint8_t *resp)
{
    uint8_t error_status;
    uint8_t response;

    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    SDIO_CmdInitStructInit(SDIO0, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = write ? 0x80000000 : 0x00000000;
    SDIO_CmdInitStructure.SDIO_Argument |= func_num << 28;
    SDIO_CmdInitStructure.SDIO_Argument |= (write && resp) ? 0x08000000 : 0x00000000;
    SDIO_CmdInitStructure.SDIO_Argument |= address << 9;
    SDIO_CmdInitStructure.SDIO_Argument |= para;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SDIO_RW_DIRECT;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIO0, &SDIO_CmdInitStructure);

    /* wait for send completed */
    while (!SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_EOT));
    SDIO_ClearFlag(SDIO0, SDIO_STATUS_EOT);
    error_status = hw_sdio_check_err();

    if (SDMMC_OK != error_status) {
        return SDMMC_ERROR;
    }

    response = SDIO0->RSP0 & 0xff;
    if ((!write) && resp) {
        *resp = response;
    }
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_cmd53
 * para:  write(IN)			--> perform an operation, read or write
 *       func_num(IN)		--> func  index
 *       address(IN)        --> address address
 *       incr_addr(IN)      --> address increase or not
 *       buf(IN/OUT)        --> If the operation is to write, then this para is to write buffer If the operation is to read, then this para is to read and return buffer
 *       size(IN)           --> read or write size
 * return: 	execution result
 * description:		operation CMD53
 ******************************************************************************/
uint8_t hw_sdio_cmd53(uint8_t write, uint8_t func_num, uint32_t address,
                      uint8_t incr_addr, uint8_t *buf, uint32_t size)
{
    uint16_t func_cur_blk_size;

    if ((phw_sdio_core->func)[func_num]) {
        func_cur_blk_size = (phw_sdio_core->func)[func_num]->cur_blk_size;
        if (func_cur_blk_size == 0) {
            return SDMMC_ADDR_OUT_OF_RANGE;
        }
    } else {
        return SDMMC_SDIO_UNKNOWN_FUNCTION;
    }

    if (write) {
        /* CMD53 write */
        hw_sdio_cmd53_write(func_num, address, incr_addr, buf, size, func_cur_blk_size);
    } else {
        /* CMD53 read */
        hw_sdio_cmd53_read(func_num, address, incr_addr, buf, size, func_cur_blk_size);
    }

    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_cmd53_read
 *  para:  	func_num(IN)	--> func index
            address(IN)		--> get address
            incr_addr(IN)	--> address increase or not
            buf(OUT)        --> data
            buffer size(IN)	--> buffer size
            cur_blk_size(IN)--> current func index block size
 * return: 	execution result
 * description: operation CMD53 read block mode
******************************************************************************/
static uint8_t hw_sdio_cmd53_read(uint8_t func_num, uint32_t address,
                                  uint8_t incr_addr, uint8_t *buf,
                                  uint32_t size, uint16_t cur_blk_size)
{

    uint8_t error_status;
    uint32_t remain_size = size;
    uint32_t blk_num;
    if (size%cur_blk_size){
        blk_num = size/cur_blk_size;
    }
    else{
        blk_num = size/cur_blk_size -1;
    }

    SDIO_CmdInitTypeDef SDIO_CmdInitStructure = {0};
    SDIO_DataSetupTypeDef SDIO_DataInitStructure = {0};
    SDIO_DmaCfgTypeDef SDIO_DmaCfgStruct = {0};

    SDIO_ClearDataSetup(SDIO0);

    /* 1.send CMD53 */
    /* CMD53 commod paraformat is  */
    /* |--RW FLAG--|--FUNC NUM--|--BLK MODE--|--OP MODE--|--REGADDR--|--BYTE/BLK* CNT--| */
    /* |--1  BYTE--|--3   BYTE--|--1   BYTE--|--1  BYTE--|--17 BYTE--|--9        BYTE--| */
    SDIO_CmdInitStructure.SDIO_Argument = 0x0; /* CMD53 R/W read flag */
    SDIO_CmdInitStructure.SDIO_Argument |= func_num << 28; /* FUNC */
    SDIO_CmdInitStructure.SDIO_Argument |= 0x08000000;     /* Block mode */
    SDIO_CmdInitStructure.SDIO_Argument |= incr_addr ? 0x04000000 : 0x0; /* OP MODE :1.increase 0,fix address  */
    if (incr_addr) SDIO_CmdInitStructure.SDIO_Argument |= (address)<< 9; /* REG ADDR,address to write  */
    else SDIO_CmdInitStructure.SDIO_Argument |= address << 9; /* REG ADDR,address to write  */

    if (remain_size % cur_blk_size) {
        SDIO_CmdInitStructure.SDIO_Argument |= (remain_size / cur_blk_size + 1);
    } else {
        SDIO_CmdInitStructure.SDIO_Argument |= remain_size / cur_blk_size;
    }

    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SDIO_RW_EXTENDED;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_CmdInitStructure.SDIO_Busy = 1 << 3;

    /* sdio read setup */
    SDIO_DataInitStructure.Data_en = SDIO_DATA_SETUP_CHANNEL_ENABLE;
    SDIO_DataInitStructure.Data_rwn = SDIO_DATA_SETUP_RWN_READ;
    SDIO_DataInitStructure.Data_mode = SDIO_DATA_SETUP_MODE_QUAD;

    if (remain_size % cur_blk_size) {
        SDIO_DataInitStructure.Block_size = (remain_size / cur_blk_size + 1) * cur_blk_size;
    } else {
        SDIO_DataInitStructure.Block_size = (remain_size / cur_blk_size) * cur_blk_size;
    }
    SDIO_DataInitStructure.Block_num = SDIO_DATA_SETUP_BLOCK_NUM(blk_num);
    SDIO_DataInitStructure.Block_size = SDIO_DATA_SETUP_BLOCK_SIZE(cur_blk_size - 1); 

    SDIO_DataSetup(SDIO0, &SDIO_DataInitStructure);
    SDIO_SetDateTimeout(SDIO0, 0xffffffff);
    SDIO_Clock_Set(SDIO0, 0x00);

    /* dma cfg */
    SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
    SDIO_DmaCfgStruct.Dma_en = SDIO_CR_DMA_ENABLE;
    SDIO_DmaCfgStruct.Rx_en = SDIO_RX_CFG_EN_ENABLE;
    SDIO_DmaCfgStruct.Rx_addr = ADDR32(buf);
    if(remain_size%cur_blk_size){
        SDIO_DmaCfgStruct.Rx_size = (remain_size/cur_blk_size+1)*cur_blk_size;
    }
    else{
        SDIO_DmaCfgStruct.Rx_size = size;
    }
    SDIO_DmaCfgStruct.Rx_datasize = SDIO_RX_CFG_DATASIZE_BYTE;
    SDIO_DMA_Config(SDIO0, &SDIO_DmaCfgStruct);

    /* CMD53 */
    SDIO_SendCommand(SDIO0, &SDIO_CmdInitStructure);

    /* wait for complete transmit */
    while (!SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_EOT)){}

    error_status = hw_sdio_check_err();

    if (SDMMC_OK != error_status) {
        return error_status;
    }
    SDIO_ClearFlag(SDIO0, SDIO_STATUS_EOT);

    /* wait for dma complete transmit */
    while(!SDIO_DmaGetIntStat(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_STAT));
    SDIO_DmaInterruptClr(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_CLR);

    /* close sdio & sdio_dma */
    SDIO_ClearDataSetup(SDIO0);
    SDIO_DmaEn(SDIO0,DISABLE);

    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_cmd53_write
 *  para:  	func_num(IN)		--> func index
            address(IN)			--> get address
            incr_addr(IN)		--> address increase or not
            buf(IN)             --> data
            buffer size(IN)     --> get size
            cur_blk_size(IN)    --> current func index block size
 * return: 	execution result
 * description:	operate CMD53 write, use block mode to write
******************************************************************************/
static uint8_t hw_sdio_cmd53_write(uint8_t func_num, uint32_t address,
                                   uint8_t incr_addr, uint8_t *buf,
                                   uint32_t size, uint16_t cur_blk_size)
{
    
    uint8_t error_status = 0;
    uint32_t remain_size = size;
    uint32_t blk_num;
    if (size%cur_blk_size){
        blk_num = size/cur_blk_size;
    }
    else{
        blk_num = size/cur_blk_size -1;
    }

    SDIO_CmdInitTypeDef SDIO_CmdInitStructure = {0};
    SDIO_DataSetupTypeDef SDIO_DataSetupStruct = {0};
    SDIO_DmaCfgTypeDef SDIO_DmaCfgStruct = {0};

    SDIO_ClearDataSetup(SDIO0);

    /* 1.send CMD53 */
    /* CMD53 commod paraformat is  */
    /* |--RW FLAG--|--FUNC NUM--|--BLK MODE--|--OP MODE--|--REGADDR--|--BYTE/BLKCNT--| */
    /* |--1  BYTE--|--3   BYTE--|--1   BYTE--|--1  BYTE--|--17 BYTE--|--9      BYTE--| */
    SDIO_CmdInitStructure.SDIO_Argument = 0x80000000; /* CMD53 R/W write flag */
    SDIO_CmdInitStructure.SDIO_Argument |= func_num << 28; /* FUNC */
    SDIO_CmdInitStructure.SDIO_Argument |= 0x08000000;     /* Block mode */
    SDIO_CmdInitStructure.SDIO_Argument |= incr_addr ? 0x04000000 : 0x0; /* OP MODE :1.increase 0,fix address  */

    SDIO_CmdInitStructure.SDIO_Argument |= address << 9; /* REG ADDR,address to write  */

    if (remain_size % cur_blk_size) {
        SDIO_CmdInitStructure.SDIO_Argument |= (remain_size / cur_blk_size + 1);
    } else {
        SDIO_CmdInitStructure.SDIO_Argument |= remain_size / cur_blk_size;
    }

    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SDIO_RW_EXTENDED;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_CmdInitStructure.SDIO_Busy = 1 << 3;

    /* sdio write cfg */
    SDIO_DataSetupStructInit(SDIO0, &SDIO_DataSetupStruct);
    SDIO_DataSetupStruct.Data_en = SDIO_DATA_SETUP_CHANNEL_ENABLE;
    SDIO_DataSetupStruct.Data_rwn = SDIO_DATA_SETUP_RWN_WRITE;
    SDIO_DataSetupStruct.Data_mode = SDIO_DATA_SETUP_MODE_QUAD;
    SDIO_DataSetupStruct.Block_num = SDIO_DATA_SETUP_BLOCK_NUM(blk_num);
    SDIO_DataSetupStruct.Block_size = SDIO_DATA_SETUP_BLOCK_SIZE(cur_blk_size - 1); 

    SDIO_DataSetup(SDIO0, &SDIO_DataSetupStruct);
    SDIO_SetDateTimeout(SDIO0, 0xffffffff);
    SDIO_Clock_Set(SDIO0, 0x00);

    /* dma cfg */
    SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
    SDIO_DmaCfgStruct.Dma_en = SDIO_CR_DMA_ENABLE;
    SDIO_DmaCfgStruct.Tx_en = SDIO_TX_CFG_EN_ENABLE;
    SDIO_DmaCfgStruct.Tx_addr = ADDR32(buf);
    if(remain_size%cur_blk_size){
        SDIO_DmaCfgStruct.Tx_size = (remain_size/cur_blk_size+1)*cur_blk_size;
    }
    else{
        SDIO_DmaCfgStruct.Tx_size = size;
    }
    SDIO_DmaCfgStruct.Tx_datasize = SDIO_TX_CFG_DATASIZE_BYTE;
    SDIO_DMA_Config(SDIO0, &SDIO_DmaCfgStruct);

    /* CMD53 */
    SDIO_SendCommand(SDIO0, &SDIO_CmdInitStructure);

    /* wait for complete transmit */
    while (!SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_EOT)){}

    error_status = hw_sdio_check_err();

    if (SDMMC_OK != error_status) {
        return error_status;
    }
    SDIO_ClearFlag(SDIO0, SDIO_STATUS_EOT);

    /* wait for dma complete transmit */
    while(!SDIO_DmaGetIntStat(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_STAT));
    SDIO_DmaInterruptClr(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_CLR);

    /* close sdio & sdio_dma */
    SDIO_ClearDataSetup(SDIO0);
    SDIO_DmaEn(SDIO0,DISABLE);
    return SDMMC_OK;
}

/******************************************************************************
 * function name:	hw_sdio_check_err
 * para:  		NULL
 * return: 	execution result
 * description:	Verify that there is any error, and clean corresponding flag
 ******************************************************************************/
static uint8_t hw_sdio_check_err()
{
    uint8_t err = SDMMC_OK;
    if (SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_CMD_ERR_CRCERR)) {
        SDIO_ClearFlag(SDIO0, SDIO_STATUS_ERR);
        err++;
        printf("%s: CMD%d CRC failed!\n", __func__, (SDIO0->CMD_OP & BITS(8, 13)) >> 8);
    }
    if (SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_CMD_ERR_TIMEOUT)) {
        SDIO_ClearFlag(SDIO0, SDIO_STATUS_ERR);
        err++;
        printf("%s: CMD%d timeout!\n", __func__, (SDIO0->CMD_OP & BITS(8, 13)) >> 8);
    }
    if (SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_CMD_ERR_TIMEOUT)) {
        SDIO_ClearFlag(SDIO0, SDIO_STATUS_ERR);
        err++;
        printf("%s: data CRC failed!\n", __func__);
    }
    if (SDIO_GetFlagStatus(SDIO0, SDIO_FLAG_DTIMEOUT)) {
        SDIO_ClearFlag(SDIO0, SDIO_STATUS_ERR);
        err++;
        printf("%s: data timeout!\n", __func__);
    }
    if (SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_TXUDR_ERR)) {
        SDIO_ClearFlag(SDIO0, SDIO_STATUS_TXUDR_ERR);
        err++;
        printf("%s: data underrun!\n", __func__);
    }
    if (SDIO_GetFlagStatus(SDIO0, SDIO_STATUS_RXOVF_ERR)) {
        SDIO_ClearFlag(SDIO0, SDIO_STATUS_RXOVF_ERR);
        err++;
        printf("%s: data overrun!\n", __func__);
    }
    SDIO_ClearFlag(SDIO0, SDIO_STATIC_FLAGS);
    return err;
}

static uint8_t hw_sdio_set_dblocksize(uint32_t *struct_dblocksize, uint32_t block_size)
{
    uint32_t dblock_size;
    switch (block_size) {
        case 1:
            dblock_size = SDIO_DATABLOCK_SIZE_1B;
            break;
        case 2:
            dblock_size = SDIO_DATABLOCK_SIZE_2B;
            break;
        case 4:
            dblock_size = SDIO_DATABLOCK_SIZE_4B;
            break;
        case 8:
            dblock_size = SDIO_DATABLOCK_SIZE_8B;
            break;
        case 16:
            dblock_size = SDIO_DATABLOCK_SIZE_16B;
            break;
        case 32:
            dblock_size = SDIO_DATABLOCK_SIZE_32B;
            break;
        case 64:
            dblock_size = SDIO_DATABLOCK_SIZE_64B;
            break;
        case 128:
            dblock_size = SDIO_DATABLOCK_SIZE_128B;
            break;
        case 256:
            dblock_size = SDIO_DATABLOCK_SIZE_256B;
            break;
        case 512:
            dblock_size = SDIO_DATABLOCK_SIZE_512B;
            break;
        case 1024:
            dblock_size = SDIO_DATABLOCK_SIZE_1024B;
            break;
        case 2048:
            dblock_size = SDIO_DATABLOCK_SIZE_2048B;
            break;
        case 4096:
            dblock_size = SDIO_DATABLOCK_SIZE_4096B;
            break;
        case 8192:
            dblock_size = SDIO_DATABLOCK_SIZE_8192B;
            break;
        case 16384:
            dblock_size = SDIO_DATABLOCK_SIZE_16384B;
            break;
    }
    *struct_dblocksize = dblock_size;
    return SDMMC_OK;
}

#define MAX_COL 16
#define SHOW_LINE_SIZE 16
void hw_hex_dump(uint8_t *data, int len)
{
    int line;
    int curline = 0;
    int curcol = 0;
    char showline[SHOW_LINE_SIZE];
    int data_pos = 0;

    if (len % MAX_COL) {
        line = len / MAX_COL + 1;
    } else {
        line = len / MAX_COL;
    }

    for (curline = 0; curline < line; curline++) {
        sprintf(showline, "%08xh:", curline * MAX_COL);
        printf("%s", showline);
        for (curcol = 0; curcol < MAX_COL; curcol++) {
            if (data_pos < len) {
                printf("%02x ", data[data_pos]);
                data_pos++;
                continue;
            } else {
                break;
            }
        }
        printf("\n");
    }
}
