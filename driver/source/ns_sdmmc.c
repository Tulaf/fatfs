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
#include "ns.h"
#include "ns_sdio.h"
#include "ns_sdmmc.h"
#include <string.h>

uint8_t CardType;
uint32_t DeviceMode;
uint32_t BusWidth;
uint32_t BusMode;

volatile SDMMC_Error TransferError = SDMMC_OK;
SDMMC_CardInfo SDCardInfo;
EmmcCardInfo MyEmmcCardInfo;
uint32_t RCA;
uint32_t CardClass;
uint32_t CSD_Tab[4];
uint32_t CID_Tab[4];
SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
SDIO_DataSetupTypeDef SDIO_DataSetupStruct;
SDIO_DmaCfgTypeDef SDIO_DmaCfgStruct;

static SDMMC_Error CmdError(SDIO_TypeDef *SDIOx);
static SDMMC_Error CmdResp1Error(SDIO_TypeDef *SDIOx, uint32_t cmd);
static SDMMC_Error CmdResp7Error(SDIO_TypeDef *SDIOx);
static SDMMC_Error CmdResp3Error(SDIO_TypeDef *SDIOx);
static SDMMC_Error CmdResp2Error(SDIO_TypeDef *SDIOx);
static SDMMC_Error CmdResp6Error(SDIO_TypeDef *SDIOx, uint32_t cmd, uint16_t *prca);
static SDMMC_Error SDEnWideBus(SDIO_TypeDef *SDIOx, uint8_t enx);
static SDMMC_Error IsCardProgramming(SDIO_TypeDef *SDIOx, uint8_t *pstatus);
static SDMMC_Error FindSCR(SDIO_TypeDef *SDIOx, uint16_t rca, uint32_t *pscr);

static SDMMC_Error CmdError(SDIO_TypeDef *SDIOx)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t timeout = SDIO_CMD0TIMEOUT;
    while(timeout--)
    {
        if(SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT) != RESET) break;
    }
    if(timeout == 0) return SDMMC_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    return errorstatus;
}

static SDMMC_Error CmdResp7Error(SDIO_TypeDef *SDIOx)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t status;
    uint32_t timeout = SDIO_CMD0TIMEOUT;
    while(timeout--)
    {
        status = SDIO_GET_STATUS(SDIOx);
        if(status & ((1 << 0) | (1 << 1) | (BITS(16, 21)) | (BITS(24, 29)))) break;
    }
    if((timeout == 0) || (status & SDIO_CMD0TIMEOUT))
    {
        errorstatus = SDMMC_CMD_RSP_TIMEOUT;
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return errorstatus;
    }
    if(status & 1 << 0)
    {
        errorstatus = SDMMC_OK;
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_EOT);
    }
    return errorstatus;
}

static SDMMC_Error CmdResp1Error(SDIO_TypeDef *SDIOx, uint32_t cmd)
{
    uint32_t status;
    while(1)
    {
        status = SDIO_GET_STATUS(SDIOx);
        if(status & ((1 << 0) | (1 << 1) | (BITS(16, 21)) | (BITS(24, 29)))) break;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMD0TIMEOUT) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_RSP_TIMEOUT;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDWrongDirection) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDBusyTimeout) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    return (SDMMC_Error)(SDIO_GET_STATUS(SDIOx) & (BITS(21, 16)));
}

static SDMMC_Error CmdResp3Error(SDIO_TypeDef *SDIOx)
{
    uint32_t status;
    while(1)
    {
        status = SDIO_GET_STATUS(SDIOx);
        if(status & ((1 << 0) | (1 << 1) | (BITS(16, 21)) | (BITS(24, 29)))) break;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMD0TIMEOUT) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_RSP_TIMEOUT;
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    return SDMMC_OK;
}

static SDMMC_Error CmdResp6Error(SDIO_TypeDef *SDIOx, uint32_t cmd, uint16_t*prca)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t status;
    uint32_t rspr1;
    while(1)
    {
        status = SDIO_GET_STATUS(SDIOx);
        if(status & ((1 << 0) | (1 << 1) | (BITS(16, 21)) | (BITS(24, 29)))) break;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMD0TIMEOUT) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_RSP_TIMEOUT;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDWrongDirection) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDBusyTimeout) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    if(((SDIOx->RSP1 & 0x3f) << 8) != cmd)
    {
        return SDMMC_ILLEGAL_CMD;
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    rspr1 = SDIOx->RSP0;

    *prca = (uint16_t)(rspr1  >>  16);
    return errorstatus;
    if(rspr1 & SDMMC_R6_GENERAL_UNKNOWN_ERROR) return SDMMC_GENERAL_UNKNOWN_ERROR;
    if(rspr1 & SDMMC_R6_ILLEGAL_CMD) return SDMMC_ILLEGAL_CMD;
    if(rspr1 & SDMMC_R6_COM_CRC_FAILED) return SDMMC_COM_CRC_FAILED;
    return errorstatus;
}

static SDMMC_Error CmdResp2Error(SDIO_TypeDef *SDIOx)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t status;
    uint32_t timeout = SDIO_CMD0TIMEOUT;
    while(timeout--)
    {
        status = SDIO_GET_STATUS(SDIOx);
        if(status & ((1 << 0) | (1 << 1) | (BITS(16, 21)) | (BITS(24, 29)))) break;
    }
    if((timeout == 0) || (status & SDIO_CMD0TIMEOUT))
    {
        errorstatus = SDMMC_CMD_RSP_TIMEOUT;
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return errorstatus;
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    return errorstatus;
}

static SDMMC_Error IsCardProgramming(SDIO_TypeDef *SDIOx, uint8_t *pstatus)
{
    uint32_t respR1 = 0, status = 0;
    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = ADDR32(RCA << 16);
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
    status = SDIO_GET_STATUS(SDIOx);

    while(!(status & ((1 << 0) | (1 << 1))))status = SDIO_GET_STATUS(SDIOx);

    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMD0TIMEOUT) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_RSP_TIMEOUT;
    }
    if(SDIOx->RSP0 != SDMMC_CMD_SEND_STATUS) return SDMMC_ILLEGAL_CMD;
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    respR1 = SDIOx->RSP0;
    *pstatus = ADDR8(((respR1  >>  9) & 0x0000000F));
    return SDMMC_OK;
}

SDMMC_Error SDMMC_SelectDeselect(SDIO_TypeDef *SDIOx, uint32_t addr)
{
    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = addr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SEL_DESEL_CARD;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
    return CmdResp1Error(SDIOx, SDMMC_CMD_SEL_DESEL_CARD);
}

SDMMC_Error SDMMC_PowerON(SDIO_TypeDef *SDIOx)
{
    uint32_t i = 0;
    uint32_t SDType = SDMMC_STD_CAPACITY;
    uint32_t emmcType;
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t response = 0, count = 0, validvoltage = 0, status = 0;

    SDIO_Clock_Set(SDIOx, 0x31);
    for(i = 0;i<74;i++){
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument= 0x00;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_GO_IDLE_STATE;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_NO;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_CmdInitStructure.SDIO_Power_Up = SDIO_CMD_OP_POWER_UP_ENABLE;

        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdError(SDIOx);
        if(errorstatus == SDMMC_OK) break;
    }
    if(errorstatus) return errorstatus;
    if (SDIO_MULTIMEDIA_CARD == CardType) {
        /* CDM0 cfg for entering into idle state */
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = 0x0;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_GO_IDLE_STATE;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_NO;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        do {
            status = SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT);
        } while (status == 0);
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_EOT);

        while (validvoltage == 0U) {
            /* SEND CMD1 APP_CMD with MMC_HIGH_VOLTAGE_RANGE(0xC0FF8080) as
             * argument */
            do {
                SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
                SDIO_CmdInitStructure.SDIO_Argument = 0xc0ff8080;
                SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SEND_OP_COND;
                SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
                SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
                SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
                SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
                errorstatus = CmdResp3Error(SDIOx);

            } while (errorstatus != SDMMC_OK);

            if (errorstatus != SDMMC_OK) {
                printf("rsp3 = %d\n", errorstatus);
                return errorstatus;
            }

            /* Get command response */
            response = SDIOx->RSP0;

            /* Get operating voltage*/
            validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);

            if (count++ == SDMMC_MAX_VOLT_TRIAL) {
                return SDMMC_INVALID_VOLTRANGE;
            }
        }
        /* When power routine finished & command returns valid voltage */
        if (((response & (0xFF000000U)) >> 24) == 0xC0U) {
            
            emmcType = MMC_HIGH_CAPACITY_CARD;
        } else {
            
            emmcType = MMC_LOW_CAPACITY_CARD;
        }
    } else {
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_HS_SEND_EXT_CSD;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_CmdInitStructure.SDIO_Argument = SDMMC_CHECK_PATTERN;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

        errorstatus = CmdResp7Error(SDIOx);
        if(errorstatus == SDMMC_OK)
        {
            CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;
            SDType = SDMMC_HIGH_CAPACITY;
        }

        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_CMD;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

        errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_CMD);

        if(errorstatus == SDMMC_OK)
        {
            while((!validvoltage) && (count<SDMMC_MAX_VOLT_TRIAL))
            {
                SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
                SDIO_CmdInitStructure.SDIO_Argument = 0x00;
                SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_CMD;
                SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
                SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
                SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
                SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
                errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_CMD);
                if(errorstatus != SDMMC_OK) return errorstatus;
                SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
                SDIO_CmdInitStructure.SDIO_Argument = SDMMC_VOLTAGE_WINDOW_SD | SDType;
                SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SD_APP_OP_COND;
                SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
                SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
                SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
                SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

                errorstatus = CmdResp3Error(SDIOx);
                if(errorstatus != SDMMC_OK) return errorstatus;
                response = SDIOx->RSP0;
                validvoltage = (((response  >>  31) == 1)?1:0);
                count++;
            }
            if(count >= SDMMC_MAX_VOLT_TRIAL)
            {
                errorstatus = SDMMC_INVALID_VOLTRANGE;
                return errorstatus;
            }
            if(response &= SDMMC_HIGH_CAPACITY)
            {
                CardType = SDIO_HIGH_CAPACITY_SD_CARD;
            }
        }
    }
    return errorstatus;
}

SDMMC_Error SDMMC_InitializeCards(SDIO_TypeDef *SDIOx)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint16_t rca = 0x01;

    if(SDIO_SECURE_DIGITAL_IO_CARD != CardType)
    {
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = 0x0;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_ALL_SEND_CID;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_136;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

        errorstatus = CmdResp2Error(SDIOx);
        if(errorstatus != SDMMC_OK) return errorstatus;

        /* Get Card identification number data */
        CID_Tab[3] = SDIOx->RSP0;
        CID_Tab[2] = SDIOx->RSP1;
        CID_Tab[1] = SDIOx->RSP2;
        CID_Tab[0] = SDIOx->RSP3;
    }
    if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
    {
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = 0x0;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_REL_ADDR;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp6Error(SDIOx, SDMMC_CMD_SET_REL_ADDR, & rca);
        if(errorstatus != SDMMC_OK) return errorstatus;
    }
    if (SDIO_MULTIMEDIA_CARD == CardType)
    {

        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = 0x00;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_REL_ADDR;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp6Error(SDIOx, SDMMC_CMD_SET_REL_ADDR, &rca);
        if(errorstatus != SDMMC_OK) return errorstatus;
    }
    if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
    {
        RCA = rca;
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = ADDR32((RCA << 16));
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SEND_CSD;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_136;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp2Error(SDIOx);
        if(errorstatus != SDMMC_OK) return errorstatus;

        /* Get Card Specific Data */
        CSD_Tab[3] = SDIOx->RSP0;
        CSD_Tab[2] = SDIOx->RSP1;
        CSD_Tab[1] = SDIOx->RSP2;
        CSD_Tab[0] = SDIOx->RSP3;

        CardClass = (SDIOx->RSP0 + 0x04) >> 20;
    }
    return SDMMC_OK;
}

SDMMC_Error EmmcReadExtCsd(SDIO_TypeDef *SDIOx, EmmcCardInfo *E)
{
    SDMMC_Error Result = SDMMC_OK;
    uint32_t count = 0;
    uint32_t *ExtCsdBuf;
    ExtCsdBuf = (uint32_t *)(&(E->EmmcExtCsd.CsdBuf[0]));

    SDIO_DataSetupStructInit(SDIOx, &SDIO_DataSetupStruct);
    SDIO_DataSetupStruct.Data_en = SDIO_DATA_SETUP_CHANNEL_ENABLE;
    SDIO_DataSetupStruct.Data_rwn = SDIO_DATA_SETUP_RWN_READ;
    SDIO_DataSetupStruct.Data_mode = SDIO_DATA_SETUP_MODE_SINGLE;
    SDIO_DataSetupStruct.Block_num = SDIO_DATA_SETUP_BLOCK_NUM(0);
    SDIO_DataSetupStruct.Block_size = SDIO_DATA_SETUP_BLOCK_SIZE(512 - 1);
    SDIO_DataSetup(SDIOx, &SDIO_DataSetupStruct);
    SDIO_SetDateTimeout(SDIOx, 0xffffffff);
    /* CMD8 */
    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = 0x00;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_HS_SEND_EXT_CSD;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_NO;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_DISABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
    
    while (count < (512 / 4)) {
        while (SDIO_GET_IP_FLAG(SDIOx, SDIO_IP_RXEMPTY)) {
        }
        *(ExtCsdBuf + count) = SDIO_ReadData(SDIOx);
        count++;
    }

    while (!SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT)) {
    };

    /*!< Clear all the static flags */
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    SDIO_ClearDataSetup(SDIOx);

    return Result;
}

SDMMC_Error EmmcGetCardInfo(EmmcCardInfo *E, uint32_t *CSD_Tab,
                            uint32_t *CID_Tab, uint16_t Rca)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint8_t tmp = 0;

    E->CardType = (uint8_t)CardType;
    E->RCA = (uint16_t)Rca;

    /*!< Byte 0 */
    tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
    E->EmmcCsd.CSDStruct = (tmp & 0xC0) >> 6;
    E->EmmcCsd.SysSpecVersion = (tmp & 0x3C) >> 2;
    E->EmmcCsd.Reserved1 = tmp & 0x03;

    /*!< Byte 1 */
    tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
    E->EmmcCsd.TAAC = tmp;

    /*!< Byte 2 */
    tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
    E->EmmcCsd.NSAC = tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
    E->EmmcCsd.MaxBusClkFrec = tmp;

    /*!< Byte 4 */
    tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
    E->EmmcCsd.CardComdClasses = tmp << 4;

    /*!< Byte 5 */
    tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
    E->EmmcCsd.CardComdClasses |= (tmp & 0xF0) >> 4;
    E->EmmcCsd.RdBlockLen = tmp & 0x0F;

    /*!< Byte 6 */
    tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
    E->EmmcCsd.PartBlockRead = (tmp & 0x80) >> 7;
    E->EmmcCsd.WrBlockMisalign = (tmp & 0x40) >> 6;
    E->EmmcCsd.RdBlockMisalign = (tmp & 0x20) >> 5;
    E->EmmcCsd.DSRImpl = (tmp & 0x10) >> 4;
    E->EmmcCsd.Reserved2 = 0; /*!< Reserved */

    E->EmmcCsd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    E->EmmcCsd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
    E->EmmcCsd.DeviceSize |= (tmp & 0xC0) >> 6;

    E->EmmcCsd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    E->EmmcCsd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
    E->EmmcCsd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    E->EmmcCsd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    E->EmmcCsd.DeviceSizeMul = (tmp & 0x03) << 1;
    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    E->EmmcCsd.DeviceSizeMul |= (tmp & 0x80) >> 7;

    E->CardBlockSize = 1 << (E->EmmcCsd.RdBlockLen);

    E->CardCapacity =
        (uint64_t)((uint64_t)(E->EmmcExtCsd.EXT_CSD.SEC_COUNT[3] << 24 |
                              E->EmmcExtCsd.EXT_CSD.SEC_COUNT[2] << 16 |
                              E->EmmcExtCsd.EXT_CSD.SEC_COUNT[1] << 8 |
                              E->EmmcExtCsd.EXT_CSD.SEC_COUNT[0]) *
                   E->CardBlockSize);

    E->EmmcCsd.EraseGrSize = (tmp & 0x40) >> 6;
    E->EmmcCsd.EraseGrMul = (tmp & 0x3F) << 1;

    /*!< Byte 11 */
    tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
    E->EmmcCsd.EraseGrMul |= (tmp & 0x80) >> 7;
    E->EmmcCsd.WrProtectGrSize = (tmp & 0x7F);

    /*!< Byte 12 */
    tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
    E->EmmcCsd.WrProtectGrEnable = (tmp & 0x80) >> 7;
    E->EmmcCsd.ManDeflECC = (tmp & 0x60) >> 5;
    E->EmmcCsd.WrSpeedFact = (tmp & 0x1C) >> 2;
    E->EmmcCsd.MaxWrBlockLen = (tmp & 0x03) << 2;

    /*!< Byte 13 */
    tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
    E->EmmcCsd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
    E->EmmcCsd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
    E->EmmcCsd.Reserved3 = 0;
    E->EmmcCsd.ContentProtectAppli = (tmp & 0x01);

    /*!< Byte 14 */
    tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
    E->EmmcCsd.FileFormatGrouop = (tmp & 0x80) >> 7;
    E->EmmcCsd.CopyFlag = (tmp & 0x40) >> 6;
    E->EmmcCsd.PermWrProtect = (tmp & 0x20) >> 5;
    E->EmmcCsd.TempWrProtect = (tmp & 0x10) >> 4;
    E->EmmcCsd.FileFormat = (tmp & 0x0C) >> 2;
    E->EmmcCsd.ECC = (tmp & 0x03);

    /*!< Byte 15 */
    tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
    E->EmmcCsd.CSD_CRC = (tmp & 0xFE) >> 1;
    E->EmmcCsd.Reserved4 = 1;

    /*!< Byte 0 */
    tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
    E->EmmcCid.ManufacturerID = tmp;

    /*!< Byte 1 */
    tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
    E->EmmcCid.OEM_AppliID = tmp << 8;

    /*!< Byte 2 */
    tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
    E->EmmcCid.OEM_AppliID |= tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
    E->EmmcCid.ProdName1 = tmp << 24;

    /*!< Byte 4 */
    tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
    E->EmmcCid.ProdName1 |= tmp << 16;

    /*!< Byte 5 */
    tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
    E->EmmcCid.ProdName1 |= tmp << 8;

    /*!< Byte 6 */
    tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
    E->EmmcCid.ProdName1 |= tmp;

    /*!< Byte 7 */
    tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
    E->EmmcCid.ProdName2 = tmp;

    /*!< Byte 8 */
    tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
    E->EmmcCid.ProdRev = tmp;

    /*!< Byte 9 */
    tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
    E->EmmcCid.ProdSN = tmp << 24;

    /*!< Byte 10 */
    tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
    E->EmmcCid.ProdSN |= tmp << 16;

    /*!< Byte 11 */
    tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
    E->EmmcCid.ProdSN |= tmp << 8;

    /*!< Byte 12 */
    tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
    E->EmmcCid.ProdSN |= tmp;

    /*!< Byte 13 */
    tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
    E->EmmcCid.Reserved1 |= (tmp & 0xF0) >> 4;
    E->EmmcCid.ManufactDate = (tmp & 0x0F) << 8;

    /*!< Byte 14 */
    tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
    E->EmmcCid.ManufactDate |= tmp;

    /*!< Byte 15 */
    tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
    E->EmmcCid.CID_CRC = (tmp & 0xFE) >> 1;
    E->EmmcCid.Reserved2 = 1;

    return (errorstatus);
}

SDMMC_Error SDMMC_Init(SDIO_TypeDef *SDIOx)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint8_t clkdiv = 0;
    errorstatus = SDMMC_PowerON(SDIOx);
    if(errorstatus == SDMMC_OK) errorstatus = SDMMC_InitializeCards(SDIOx);
    if(errorstatus == SDMMC_OK) errorstatus = SDMMC_GetCardInfo( & SDCardInfo);
    if(errorstatus == SDMMC_OK) errorstatus = SDMMC_SelectDeselect(SDIOx, (uint32_t)(SDCardInfo.RCA << 16));
    if (errorstatus == SDMMC_OK) {
        if (SDIO_MULTIMEDIA_CARD == CardType) {
            errorstatus = EmmcReadExtCsd(SDIOx, &MyEmmcCardInfo);
            if (errorstatus == SDMMC_OK) errorstatus = EmmcGetCardInfo(&MyEmmcCardInfo, &CSD_Tab[0], &CID_Tab[0], RCA);
        }
    }
    if (errorstatus == SDMMC_OK) errorstatus = SDMMC_EnableWideBusOperation(SDIOx, BusWidth);
    if((errorstatus == SDMMC_OK) || (SDIO_MULTIMEDIA_CARD == CardType))
    {
        /*set clock divider here , use API: SDIO_Clock_Set(SDIOx, clkdiv); */
    }
    return errorstatus;
}

SDMMC_Error SDMMC_WriteBlock(SDIO_TypeDef *SDIOx, uint8_t *buf, long long addr,  uint16_t blksize)
{

    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t status = 0;
    uint8_t power = 0, cardstate = 0;
    uint32_t timeout = 0, bytestransferred = 0;
    uint32_t cardstatus = 0, count = 0, restwords = 0;
    uint32_t    tlen = blksize;
    uint32_t*tempbuff = (uint32_t*)(buf);

    if(buf == NULL) return SDMMC_INVALID_PARAMETER;
    SDIO_ClearDataSetup(SDIOx);
    if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        blksize = 512;
        addr >>= 9;
    }

    timeout = SDMMC_DATATIMEOUT;
    while(((cardstatus & 0x00000100) == 0) && (timeout>0))
    {
        timeout--;
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = ADDR32(RCA << 16);
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SEND_STATUS;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SEND_STATUS);
        if(errorstatus != SDMMC_OK) return errorstatus;
        cardstatus = SDIOx->RSP0;
    }
    if(timeout == 0) return SDMMC_ERROR;

    SDIO_DataSetupStructInit(SDIOx, &SDIO_DataSetupStruct);
    SDIO_DataSetupStruct.Data_en = SDIO_DATA_SETUP_CHANNEL_ENABLE;
    SDIO_DataSetupStruct.Data_rwn = SDIO_DATA_SETUP_RWN_WRITE;
    if (CardType == SDIO_MULTIMEDIA_CARD) {
        SDIO_DataSetupStruct.Data_mode = BusWidth;
    } else {
        SDIO_DataSetupStruct.Data_mode = BusWidth;
    }

    SDIO_DataSetupStruct.Block_num = SDIO_DATA_SETUP_BLOCK_NUM(0);
    SDIO_DataSetupStruct.Block_size =
        SDIO_DATA_SETUP_BLOCK_SIZE(blksize - 1);

    SDIO_DataSetup(SDIOx, &SDIO_DataSetupStruct);
    SDIO_SetDateTimeout(SDIOx, 0xffffffff);
    SDIO_Clock_Set(SDIOx, 0x00);

    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = addr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_WRITE_SINGLE_BLOCK;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_CmdInitStructure.SDIO_Busy = 1 << 3;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

    int n = 0;
    if(DeviceMode  == SD_POLLING_MODE )
    {
        while (n < (tlen / 4)) {
            while(SDIO_GET_IP_FLAG(SDIOx, SDIO_IP_TXFULL));
            SDIOx->TX_DATA = tempbuff[n];
            n++;
        }
    }
    else if(DeviceMode == SD_DMA_MODE)
    {
        SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
        SDIO_DmaCfgStruct.Dma_en = SDIO_CR_DMA_ENABLE;
        SDIO_DmaCfgStruct.Tx_en = SDIO_TX_CFG_EN_ENABLE;
        SDIO_DmaCfgStruct.Tx_addr = ADDR32(tempbuff);
        SDIO_DmaCfgStruct.Tx_size = blksize;
        SDIO_DmaCfgStruct.Tx_datasize = SDIO_TX_CFG_DATASIZE_WORD;
        SDIO_DMA_Config(SDIOx, &SDIO_DmaCfgStruct);

        while(!SDIO_DmaGetIntStat(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_STAT));
        SDIO_DmaInterruptClr(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_CLR);
    }

    while(!SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT)){};
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMD0TIMEOUT) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_RSP_TIMEOUT;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDWrongDirection) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDBusyTimeout) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATUS_EOT);
    SDIO_DmaEn(SDIOx, DISABLE);
    SDIO_ClearDataSetup(SDIOx);
    return 0;
}

SDMMC_Error SDMMC_GetCardInfo(SDMMC_CardInfo *cardinfo)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint8_t tmp = 0;
    cardinfo->CardType = ADDR32(CardType);
    cardinfo->RCA = (uint16_t)RCA;
    tmp = ADDR8(((CSD_Tab[0] & 0xFF000000) >> 24));
    cardinfo->SDMMC_csd.CSDStruct = (tmp & 0xC0) >> 6;
    cardinfo->SDMMC_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
    cardinfo->SDMMC_csd.Reserved1 = tmp & 0x03;
    tmp = ADDR8((CSD_Tab[0] & 0x00FF0000) >> 16);
    cardinfo->SDMMC_csd.TAAC = tmp;
    tmp = ADDR8((CSD_Tab[0] & 0x0000FF00) >> 8);
    cardinfo->SDMMC_csd.NSAC = tmp;
    tmp = ADDR8(CSD_Tab[0] & 0x000000FF);
    cardinfo->SDMMC_csd.MaxBusClkFrec = tmp;
    tmp = ADDR8((CSD_Tab[1] & 0xFF000000) >> 24);
    cardinfo->SDMMC_csd.CardComdClasses = tmp << 4;
    tmp = ADDR8((CSD_Tab[1] & 0x00FF0000) >> 16);
    cardinfo->SDMMC_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
    cardinfo->SDMMC_csd.RdBlockLen = tmp & 0x0F;
    tmp = ADDR8((CSD_Tab[1] & 0x0000FF00) >> 8);
    cardinfo->SDMMC_csd.PartBlockRead = (tmp & 0x80) >> 7;
    cardinfo->SDMMC_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
    cardinfo->SDMMC_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
    cardinfo->SDMMC_csd.DSRImpl = (tmp & 0x10) >> 4;
    cardinfo->SDMMC_csd.Reserved2 = 0;
    if((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0) || (SDIO_MULTIMEDIA_CARD == CardType))
    {
        cardinfo->SDMMC_csd.DeviceSize = (tmp & 0x03) << 10;
        tmp = ADDR8(CSD_Tab[1] & 0x000000FF);
        cardinfo->SDMMC_csd.DeviceSize |= (tmp) << 2;
        tmp = ADDR8((CSD_Tab[2] & 0xFF000000) >> 24);
        cardinfo->SDMMC_csd.DeviceSize |= (tmp & 0xC0) >> 6;
        cardinfo->SDMMC_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
        cardinfo->SDMMC_csd.MaxRdCurrentVDDMax = (tmp & 0x07);
        tmp = ADDR8((CSD_Tab[2] & 0x00FF0000) >> 16);
        cardinfo->SDMMC_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
        cardinfo->SDMMC_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
        cardinfo->SDMMC_csd.DeviceSizeMul = (tmp & 0x03) << 1;
        tmp = ADDR8((CSD_Tab[2] & 0x0000FF00) >> 8);
        cardinfo->SDMMC_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
        cardinfo->CardCapacity = (cardinfo->SDMMC_csd.DeviceSize+1);
        cardinfo->CardCapacity *= (1 << (cardinfo->SDMMC_csd.DeviceSizeMul+2));
        cardinfo->CardBlockSize = 1 << (cardinfo->SDMMC_csd.RdBlockLen);
        cardinfo->CardCapacity *= cardinfo->CardBlockSize;
    }else if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        tmp = ADDR8(CSD_Tab[1] & 0x000000FF);
        cardinfo->SDMMC_csd.DeviceSize = (tmp & 0x3F) << 16;
        tmp = ADDR8((CSD_Tab[2] & 0xFF000000) >> 24);
        cardinfo->SDMMC_csd.DeviceSize |= (tmp << 8);
        tmp = ADDR8((CSD_Tab[2] & 0x00FF0000) >> 16);
        cardinfo->SDMMC_csd.DeviceSize |= (tmp);
        tmp = ADDR8((CSD_Tab[2] & 0x0000FF00) >> 8);
        cardinfo->CardCapacity = (long long)(cardinfo->SDMMC_csd.DeviceSize+1)*512*1024;
        cardinfo->CardBlockSize = 512;
    }

    cardinfo->SDMMC_csd.EraseGrSize = (tmp & 0x40) >> 6;
    cardinfo->SDMMC_csd.EraseGrMul = (tmp & 0x3F) << 1;
    tmp = ADDR8(CSD_Tab[2] & 0x000000FF);
    cardinfo->SDMMC_csd.EraseGrMul |= (tmp & 0x80) >> 7;
    cardinfo->SDMMC_csd.WrProtectGrSize = (tmp & 0x7F);
    tmp = ADDR8((CSD_Tab[3] & 0xFF000000) >> 24);
    cardinfo->SDMMC_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
    cardinfo->SDMMC_csd.ManDeflECC = (tmp & 0x60) >> 5;
    cardinfo->SDMMC_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
    cardinfo->SDMMC_csd.MaxWrBlockLen = (tmp & 0x03) << 2;
    tmp = ADDR8((CSD_Tab[3] & 0x00FF0000) >> 16);
    cardinfo->SDMMC_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
    cardinfo->SDMMC_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
    cardinfo->SDMMC_csd.Reserved3 = 0;
    cardinfo->SDMMC_csd.ContentProtectAppli = (tmp & 0x01);
    tmp = ADDR8((CSD_Tab[3] & 0x0000FF00) >> 8);
    cardinfo->SDMMC_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
    cardinfo->SDMMC_csd.CopyFlag = (tmp & 0x40) >> 6;
    cardinfo->SDMMC_csd.PermWrProtect = (tmp & 0x20) >> 5;
    cardinfo->SDMMC_csd.TempWrProtect = (tmp & 0x10) >> 4;
    cardinfo->SDMMC_csd.FileFormat = (tmp & 0x0C) >> 2;
    cardinfo->SDMMC_csd.ECC = (tmp & 0x03);
    tmp = ADDR8(CSD_Tab[3] & 0x000000FF);
    cardinfo->SDMMC_csd.CSD_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SDMMC_csd.Reserved4 = 1;
    tmp = ADDR8((CID_Tab[0] & 0xFF000000) >> 24);
    cardinfo->SDMMC_cid.ManufacturerID = tmp;
    tmp = ADDR8((CID_Tab[0] & 0x00FF0000) >> 16);
    cardinfo->SDMMC_cid.OEM_AppliID = tmp << 8;
    tmp = ADDR8((CID_Tab[0] & 0x000000FF00) >> 8);
    cardinfo->SDMMC_cid.OEM_AppliID |= tmp;
    tmp = ADDR8(CID_Tab[0] & 0x000000FF);
    cardinfo->SDMMC_cid.ProdName1 = tmp << 24;
    tmp = ADDR8((CID_Tab[1] & 0xFF000000) >> 24);
    cardinfo->SDMMC_cid.ProdName1 |= tmp << 16;
    tmp = ADDR8((CID_Tab[1] & 0x00FF0000) >> 16);
    cardinfo->SDMMC_cid.ProdName1 |= tmp << 8;
    tmp = ADDR8((CID_Tab[1] & 0x0000FF00) >> 8);
    cardinfo->SDMMC_cid.ProdName1 |= tmp;
    tmp = ADDR8(CID_Tab[1] & 0x000000FF);
    cardinfo->SDMMC_cid.ProdName2 = tmp;
    tmp = ADDR8((CID_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SDMMC_cid.ProdRev = tmp;
    tmp = ADDR8((CID_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SDMMC_cid.ProdSN = tmp << 24;
    tmp = ADDR8((CID_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SDMMC_cid.ProdSN |= tmp << 16;
    tmp = ADDR8(CID_Tab[2] & 0x000000FF);
    cardinfo->SDMMC_cid.ProdSN |= tmp << 8;
    tmp = ADDR8((CID_Tab[3] & 0xFF000000) >> 24);
    cardinfo->SDMMC_cid.ProdSN |= tmp;
    tmp = ADDR8((CID_Tab[3] & 0x00FF0000) >> 16);
    cardinfo->SDMMC_cid.Reserved1 |= (tmp & 0xF0) >> 4;
    cardinfo->SDMMC_cid.ManufactDate = (tmp & 0x0F) << 8;
    tmp = ADDR8((CID_Tab[3] & 0x0000FF00) >> 8);
    cardinfo->SDMMC_cid.ManufactDate |= tmp;
    tmp = ADDR8(CID_Tab[3] & 0x000000FF);
    cardinfo->SDMMC_cid.CID_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SDMMC_cid.Reserved2 = 1;
    return errorstatus;
}

static SDMMC_Error FindSCR(SDIO_TypeDef *SDIOx, uint16_t rca, uint32_t *pscr)
{
    uint32_t index = 0;
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t tempscr[2] = {0, 0};

    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = ADDR32(8);
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SET_BLOCKLEN);
    if(errorstatus != SDMMC_OK) return errorstatus;

    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = ADDR32(RCA << 16);
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_CMD;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_CMD);
    if(errorstatus != SDMMC_OK) return errorstatus;

    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = 0x0;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SD_APP_SEND_SCR;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SD_APP_SEND_SCR);
    if(errorstatus != SDMMC_OK) return errorstatus;

    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMD0TIMEOUT) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_RSP_TIMEOUT;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDWrongDirection) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    if(SDIO_GetFlagStatus(SDIOx, SDIO_CMDBusyTimeout) != RESET)
    {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    *(pscr+1) = ((tempscr[0] & SDMMC_0TO7BITS) << 24) | ((tempscr[0] & SDMMC_8TO15BITS) << 8) | ((tempscr[0] & SDMMC_16TO23BITS) >> 8) | ((tempscr[0] & SDMMC_24TO31BITS) >> 24);
    *(pscr) = ((tempscr[1] & SDMMC_0TO7BITS) << 24) | ((tempscr[1] & SDMMC_8TO15BITS) << 8) | ((tempscr[1] & SDMMC_16TO23BITS) >> 8) | ((tempscr[1] & SDMMC_24TO31BITS) >> 24);
    return errorstatus;
}

static SDMMC_Error SDMMCEnWideBus(SDIO_TypeDef *SDIOx, uint32_t enx)
{
    /* SDMMC_CMD_APP_SD_SET_BUSWIDTH argument format:
    *
    *	[31:26] Always 0
    *	[25:24] Access Mode
    *	[23:16] Location of target Byte in EXT_CSD
    *	[15:08] Value Byte
    *	[07:03] Always 0
    *	[02:00] Command Set
    */
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t response = 0U;
    uint8_t WideMode = enx;
    uint32_t cmdArg;

    if (BusMode == SDIO_DDR_MODE) {
        /* if cfg busmode ddr, open DDR enable */
        SDIO_CfgDdrMode(SDIOx, ENABLE);
        /* cfg SAMPLE_DDR corresponding to CLK_DIV */
        /* SDIOx->SAMPLE_DDR |= (4 * SDIOx->CLK_DIV - 1); */

        printf("cfg ddr mode\n");

        /* send CMD6 to activate hs_timing before setting BUS_WIDTH for dual data rate operation*/
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = 0x03B90100;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_SD_SET_BUSWIDTH;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_SD_SET_BUSWIDTH);
    }

    if (WideMode == SDIO_DATA_SETUP_MODE_OCTOL) {
        if (BusMode == SDIO_DDR_MODE) {
            cmdArg = 0x03B70601U;
        } else if (BusMode == SDIO_SDR_MODE) {
            cmdArg = 0x03B70200U;
        } else{
            errorstatus = SDMMC_INVALID_PARAMETER;
            return errorstatus; 
        }
    } else if (WideMode == SDIO_DATA_SETUP_MODE_QUAD) {
        if (BusMode == SDIO_DDR_MODE) {
            cmdArg = 0x03B70501U;
        } else if (BusMode == SDIO_SDR_MODE) {
            cmdArg = 0x03B70100U;
        } else{
            errorstatus = SDMMC_INVALID_PARAMETER;
            return errorstatus; 
        }
    } else if (WideMode == SDIO_DATA_SETUP_MODE_SINGLE) {
        cmdArg = 0x03B70000U;
    } else {
        /* WideMode is not a valid argument*/
        errorstatus = SDMMC_INVALID_PARAMETER;
        return errorstatus;
    }

    /* Send CMD6 to activate SDR50 Mode and Power Limit 1.44W */
    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = cmdArg;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_SD_SET_BUSWIDTH;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
    errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_SD_SET_BUSWIDTH);

    /* Check for switch error and violation of the trial number of sending CMD 13 */
    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = RCA << 16;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
    errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SEND_STATUS);
    /* get status */
    response = SDIOx->RSP0;

    return errorstatus;
}

static SDMMC_Error SDEnWideBus(SDIO_TypeDef *SDIOx, uint8_t enx)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint32_t scr[2] = {0, 0};
    uint8_t arg = 0X00;
    if(enx)arg = 0X02;
    else arg = 0X00;

    if(SDIOx->RSP0 & SDMMC_CARD_LOCKED) return SDMMC_LOCK_UNLOCK_FAILED;
    errorstatus = FindSCR(SDIOx, RCA, scr);
    if(errorstatus != SDMMC_OK) return errorstatus;

    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = ADDR32( RCA << 16);
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_CMD;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
    errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_CMD);
    if(errorstatus != SDMMC_OK) return errorstatus;

    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = arg;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_SD_SET_BUSWIDTH;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
    errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_SD_SET_BUSWIDTH);

    return errorstatus;
}

SDMMC_Error SDMMC_EnableWideBusOperation(SDIO_TypeDef *SDIOx, uint32_t WideMode)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    if (SDIO_MULTIMEDIA_CARD == CardType)
    {
        errorstatus = SDMMCEnWideBus(SDIOx, WideMode);
        return errorstatus;
    }
    else if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
    {
        if (SDIO_DATA_SETUP_MODE_OCTOL == WideMode)
        {
            errorstatus = SDMMC_UNSUPPORTED_FEATURE;
            return(errorstatus);
        }
        else
        {
            errorstatus = SDEnWideBus(SDIOx, WideMode);
            if (SDMMC_OK != errorstatus) {
                return errorstatus;
            }
        }
    }
    return errorstatus;
}

SDMMC_Error SDMMC_ReadBlock(SDIO_TypeDef *SDIOx, uint8_t *buf,long long addr,uint16_t blksize)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint8_t power;
    uint32_t count = 0, *tempbuff = (uint32_t *)(buf);
    uint32_t timeout = SDIO_DATATIMEOUT;

    if(NULL == buf) return SDMMC_INVALID_PARAMETER;
    if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        blksize = 512;
        addr >>= 9;
    }
    SDIO_ClearDataSetup(SDIOx);
    SDIO_DataSetupStructInit(SDIOx, &SDIO_DataSetupStruct);
    SDIO_DataSetupStruct.Data_en = SDIO_DATA_SETUP_CHANNEL_ENABLE;
    SDIO_DataSetupStruct.Data_rwn = SDIO_DATA_SETUP_RWN_READ;
    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        SDIO_DataSetupStruct.Data_mode = BusWidth;
    } else if (CardType == SDIO_MULTIMEDIA_CARD) {
        SDIO_DataSetupStruct.Data_mode = BusWidth;
    }

    SDIO_DataSetupStruct.Block_num = SDIO_DATA_SETUP_BLOCK_NUM(0);
    SDIO_DataSetupStruct.Block_size =
        SDIO_DATA_SETUP_BLOCK_SIZE(blksize - 1);
    SDIO_DataSetup(SDIOx, &SDIO_DataSetupStruct);
    SDIO_SetDateTimeout(SDIOx, 0xffffffff);
    SDIO_Clock_Set(SDIOx, 0x00);
    SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
    SDIO_CmdInitStructure.SDIO_Argument = addr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_READ_SINGLE_BLOCK;
    SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
    SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
    SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
    SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

    if (DeviceMode == SD_POLLING_MODE)
    {
        uint32_t rx_data;
        uint32_t i = 0;
        while(i<(blksize / 4)){
            while(SDIO_GET_IP_FLAG(SDIOx, SDIO_IP_RXEMPTY));
            tempbuff[i] = SDIO_ReadData(SDIOx);
            i++;
        }
    }
    else if(DeviceMode == SD_DMA_MODE)
    {
        SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
        SDIO_DmaCfgStruct.Dma_en = SDIO_CR_DMA_ENABLE;
        SDIO_DmaCfgStruct.Rx_en = SDIO_RX_CFG_EN_ENABLE;
        SDIO_DmaCfgStruct.Rx_addr = ADDR32(tempbuff);
        SDIO_DmaCfgStruct.Rx_size = blksize;
        SDIO_DmaCfgStruct.Rx_datasize = SDIO_RX_CFG_DATASIZE_WORD;
        SDIO_DMA_Config(SDIOx, &SDIO_DmaCfgStruct);

        while(!SDIO_DmaGetIntStat(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_STAT));
        SDIO_DmaInterruptClr(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_CLR);
    }

    while (!SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT)) {
    };
    if (SDIO_GetFlagStatus(SDIOx, SDIO_CMD0TIMEOUT) != RESET) {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_RSP_TIMEOUT;
    }
    if (SDIO_GetFlagStatus(SDIOx, SDIO_CMDWrongDirection) != RESET) {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    if (SDIO_GetFlagStatus(SDIOx, SDIO_CMDBusyTimeout) != RESET) {
        SDIO_ClearFlag(SDIOx, SDIO_STATUS_ERR);
        return SDMMC_CMD_CRC_FAIL;
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);

    SDIO_ClearDataSetup(SDIOx);
    SDIO_DmaEn(SDIOx, DISABLE);
    return errorstatus;
}

SDMMC_Error SDMMC_ReadMultiBlocks(SDIO_TypeDef *SDIOx, uint8_t *buf,  long long addr, uint16_t blksize, uint32_t nblks)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint8_t power;
    uint32_t count = 0;
    uint32_t timeout = SDIO_DATATIMEOUT;
    uint32_t *tempbuff = (uint32_t*)buf;

    if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        blksize = 512;
        addr >>= 9;
    }
    SDIO_ClearDataSetup(SDIOx);
    if((blksize>0) && (blksize <= 2048) && ((blksize & (blksize-1)) == 0))
    {
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = blksize;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_BLOCKLEN;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SET_BLOCKLEN);
        if(errorstatus != SDMMC_OK) return errorstatus;
    }else return SDMMC_INVALID_PARAMETER;
    delay_1ms(5);
    if (nblks > 1) {
        SDIO_DataSetupStructInit(SDIOx, &SDIO_DataSetupStruct);
        SDIO_DataSetupStruct.Data_en = SDIO_DATA_SETUP_CHANNEL_ENABLE;
        SDIO_DataSetupStruct.Data_rwn = SDIO_DATA_SETUP_RWN_READ;
        SDIO_DataSetupStruct.Data_mode = BusWidth;
        SDIO_DataSetupStruct.Block_num = SDIO_DATA_SETUP_BLOCK_NUM(nblks - 1);
        SDIO_DataSetupStruct.Block_size = SDIO_DATA_SETUP_BLOCK_SIZE(blksize - 1);
        SDIO_DataSetup(SDIOx, &SDIO_DataSetupStruct);
        SDIO_SetDateTimeout(SDIOx, 0xffffffff);
        SDIO_Clock_Set(SDIOx, 0x00);

        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = addr;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_READ_MULT_BLOCK;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

        if(errorstatus != SDMMC_OK) return errorstatus;
        if(DeviceMode == SD_POLLING_MODE)
        {
            int i = 0;
            while(i<(nblks * blksize / 4)){
                while(SDIO_GET_IP_FLAG(SDIOx, SDIO_IP_RXEMPTY) != RESET);
                *tempbuff = SDIO_ReadData(SDIOx);
                tempbuff++;
                i++;
            }
        }else if(DeviceMode == SD_DMA_MODE)
        {
            SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
            SDIO_DmaCfgStruct.Dma_en = SDIO_CR_DMA_ENABLE;
            SDIO_DmaCfgStruct.Rx_en = SDIO_RX_CFG_EN_ENABLE;
            SDIO_DmaCfgStruct.Rx_addr = ADDR32(tempbuff);
            SDIO_DmaCfgStruct.Rx_size = blksize*nblks;
            SDIO_DmaCfgStruct.Rx_datasize = SDIO_RX_CFG_DATASIZE_WORD;
            SDIO_DMA_Config(SDIOx, &SDIO_DmaCfgStruct);

            while(!SDIO_DmaGetIntStat(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_STAT));
            SDIO_DmaInterruptClr(SDIO0_DMA_SDIO0_P2M_IRQ, RX_FTRANS_IRQ_CLR);
        }
        if(SDIO_GetFlagStatus(SDIOx, SDIO_FLAG_DTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIOx, SDIO_FLAG_DTIMEOUT);
            return SDMMC_DATA_TIMEOUT;
        }

        while(SDIO_GET_IP_FLAG(SDIOx, SDIO_IP_RXEMPTY) == RESET)
        {
            *tempbuff = SDIO_ReadData(SDIOx);
            tempbuff++;
        }
        while(SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT) == RESET)
        {
            timeout--;
            if(timeout == 0) return SDMMC_DATA_TIMEOUT;
        }
        SDIO_ClearDataSetup(SDIOx);
        SDIO_DmaEn(SDIOx, DISABLE);

        SDIO_ClearFlag(SDIOx, SDIO_STATUS_EOT | SDIO_STATUS_ERR);
        if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
        {
            SDIO_Clock_Set(SDIOx, 0x31);
            SDIO_ClearDataSetup(SDIOx);
        }
        SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    }
    return errorstatus;
}

SDMMC_Error SDMMC_WriteMultiBlocks(SDIO_TypeDef *SDIOx, uint8_t *buf,  long long addr, uint16_t blksize, uint32_t nblks)
{
    SDMMC_Error errorstatus = SDMMC_OK;
    uint8_t  power = 0, cardstate = 0;
    uint32_t timeout = SDIO_CMD0TIMEOUT, bytestransferred = 0;
    uint32_t count = 0, restwords = 0;
    uint32_t tlen = nblks*blksize;
    uint32_t *tempbuff = (uint32_t *)(buf);

    if(buf == NULL) return SDMMC_INVALID_PARAMETER;
    SDIO_ClearDataSetup(SDIOx);
    SDIO_Clock_Set(SDIOx, 0x31);
    if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        blksize = 512;
        addr >>= 9;
    }
    if((blksize > 0) && (blksize <= 2048) && ((blksize & (blksize-1)) == 0))
    {
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = blksize;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_BLOCKLEN;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SET_BLOCKLEN);
        if(errorstatus != SDMMC_OK) return errorstatus;

    }else return SDMMC_INVALID_PARAMETER;

    if(nblks > 1)
    {
        if(nblks*blksize>SDMMC_MAX_DATA_LENGTH) return SDMMC_INVALID_PARAMETER;
        if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
        {
            SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
            SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)(RCA << 16);
            SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_CMD;
            SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
            SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
            SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
            SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

            errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_CMD);
            if(errorstatus != SDMMC_OK) return errorstatus;
            SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
            SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)nblks;
            SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_BLOCK_COUNT;
            SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
            SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
            SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
            SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
            errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SET_BLOCK_COUNT);
            if(errorstatus != SDMMC_OK) return errorstatus;

        } else if (SDIO_MULTIMEDIA_CARD == CardType) {
            SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
            SDIO_CmdInitStructure.SDIO_Argument = nblks;
            SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_BLOCK_COUNT;
            SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
            SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
            SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
            SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
            errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SET_BLOCK_COUNT);
            if (errorstatus != SDMMC_OK)
                return errorstatus;
        }

        SDIO_Clock_Set(SDIOx, 0);
        SDIO_SetDateTimeout(SDIOx, 0xffffffff);
        SDIOx->DATA_SETUP =
            0x1 | 0x0 << 1 | BusWidth | (nblks - 1) << 4 | (blksize - 1) << 20;
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = addr;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_WRITE_MULT_BLOCK;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_CmdInitStructure.SDIO_Busy = 1 << 3;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        int n = 0;
        if(DeviceMode == SD_POLLING_MODE)
        {
            while (n < (tlen / 4)) {
                while (SDIO_GET_IP_FLAG(SDIOx, SDIO_IP_TXFULL))
                    ;
                SDIO_SendData(SDIOx, tempbuff[n]);
                n++;
            }

        }else if(DeviceMode == SD_DMA_MODE)
        {
            SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
            SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
            SDIO_DmaCfgStruct.Dma_en = SDIO_CR_DMA_ENABLE;
            SDIO_DmaCfgStruct.Tx_en = SDIO_TX_CFG_EN_ENABLE;
            SDIO_DmaCfgStruct.Tx_addr = ADDR32(tempbuff);
            SDIO_DmaCfgStruct.Tx_size = tlen;
            SDIO_DmaCfgStruct.Tx_datasize = SDIO_TX_CFG_DATASIZE_WORD;
            SDIO_DMA_Config(SDIOx, &SDIO_DmaCfgStruct);

            while(!SDIO_DmaGetIntStat(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_STAT));
            SDIO_DmaInterruptClr(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_CLR);
        }
        if(SDIO_GetFlagStatus(SDIOx, SDIO_FLAG_DTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIOx, SDIO_FLAG_DTIMEOUT);
            return SDMMC_DATA_TIMEOUT;
        }
        while(SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT) == RESET)
        {
            timeout--;
            if(timeout == 0)
            {
                return SDMMC_DATA_TIMEOUT;
            }
        }
        if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) ||
            (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||
            (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
            SDIO_ClearDataSetup(SDIOx);
            SDIO_Clock_Set(SDIOx, 0x31);
            SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
            SDIO_CmdInitStructure.SDIO_Argument = 0x00;
            SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_STOP_TRANSMISSION;
            SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
            SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
            SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
            SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

            errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_STOP_TRANSMISSION);
            if(errorstatus != SDMMC_OK) return errorstatus;
        }
        SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    }
    SDIO_ClearDataSetup(SDIOx);
    SDIO_DmaEn(SDIOx, DISABLE);
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    return errorstatus;
}

SDMMC_Error SDMMC_WriteMultiBlocks_int(SDIO_TypeDef *SDIOx, uint8_t *buf, long long addr, uint16_t blksize, uint32_t nblks)
{
    SDIO_Clock_Set(SDIOx, 0x31);
    SDMMC_Error errorstatus = SDMMC_OK;
    uint8_t  power = 0, cardstate = 0;
    uint32_t timeout = SDIO_CMD0TIMEOUT, bytestransferred = 0;
    uint32_t count = 0, restwords = 0;
    uint32_t tlen = nblks*blksize;
    uint32_t *tempbuff = (uint32_t *)(buf);

    if(buf == NULL) return SDMMC_INVALID_PARAMETER;
    SDIO_ClearDataSetup(SDIOx);

    if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        blksize = 512;
        addr >>= 9;
    }
    if((blksize > 0) && (blksize <= 2048) && ((blksize & (blksize-1)) == 0))
    {

        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = blksize;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_BLOCKLEN;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
        errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SET_BLOCKLEN);
        if(errorstatus != SDMMC_OK) return errorstatus;

    }else return SDMMC_INVALID_PARAMETER;
    if(nblks > 1)
    {
        if(nblks*blksize>SDMMC_MAX_DATA_LENGTH) return SDMMC_INVALID_PARAMETER;
        if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
        {
            SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
            SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)(RCA << 16);
            SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_APP_CMD;
            SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
            SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
            SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
            SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
            errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_APP_CMD);

            if(errorstatus != SDMMC_OK) return errorstatus;
            SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
            SDIO_CmdInitStructure.SDIO_Argument = nblks;
            SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_SET_BLOCK_COUNT;
            SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
            SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
            SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
            SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);
            errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_SET_BLOCK_COUNT);
            if(errorstatus != SDMMC_OK) return errorstatus;
        }

        SDIO_Clock_Set(SDIOx, 0);
        SDIO_SetDateTimeout(SDIOx, 0xffffffff);
        SDIOx->DATA_SETUP =
            0x1 | 0x0 << 1 | BusWidth | (nblks - 1) << 4 | (blksize - 1) << 20;
        SDIO_CmdInitStructInit(SDIOx, &SDIO_CmdInitStructure);
        SDIO_CmdInitStructure.SDIO_Argument = addr;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SDMMC_CMD_WRITE_MULT_BLOCK;
        SDIO_CmdInitStructure.SDIO_No_Rsp = SDIO_CMD_OP_RSP_YES;
        SDIO_CmdInitStructure.SDIO_No_Rsp_Len = SDIO_CMD_OP_RSP_LEN_48;
        SDIO_CmdInitStructure.SDIO_CrcEn = SDIO_CMD_OP_CRC_ENABLE;
        SDIO_SendCommand(SDIOx, &SDIO_CmdInitStructure);

        if(DeviceMode == SD_POLLING_MODE)
        {
            SDIOx->TX_MARK = 1;
            SDIOx->IE |= 1 << 0;

        }else if(DeviceMode == SD_DMA_MODE)
        {
            SDIO_DmaCfgStructInit(&SDIO_DmaCfgStruct);
            SDIO_DmaCfgStruct.Dma_en = SDIO_CR_DMA_ENABLE;
            SDIO_DmaCfgStruct.Rx_en = SDIO_RX_CFG_EN_ENABLE;
            SDIO_DmaCfgStruct.Rx_addr = ADDR32(tempbuff);
            SDIO_DmaCfgStruct.Rx_size = blksize;
            SDIO_DmaCfgStruct.Rx_datasize = SDIO_RX_CFG_DATASIZE_WORD;
            SDIO_DMA_Config(SDIOx, &SDIO_DmaCfgStruct);

            while(!SDIO_DmaGetIntStat(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_STAT));
            SDIO_DmaInterruptClr(SDIO0_DMA_SDIO0_P2M_IRQ, TX_FTRANS_IRQ_CLR);
        }
        if(SDIO_GetFlagStatus(SDIOx, SDIO_FLAG_DTIMEOUT) != RESET)
        {
            SDIO_ClearFlag(SDIOx, SDIO_FLAG_DTIMEOUT);
            return SDMMC_DATA_TIMEOUT;
        }

        while(SDIO_GetFlagStatus(SDIOx, SDIO_STATUS_EOT) == RESET)
        {
            timeout--;
            if(timeout == 0)
            {
                return SDMMC_DATA_TIMEOUT;
            }
        }

        if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
        {
            SDIO_ClearDataSetup(SDIOx);
            SDIO_Clock_Set(SDIOx, 0x31);
            errorstatus = CmdResp1Error(SDIOx, SDMMC_CMD_STOP_TRANSMISSION);
            if(errorstatus != SDMMC_OK) return errorstatus;
        }
        SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    }
    SDIO_ClearFlag(SDIOx, SDIO_STATIC_FLAGS);
    SDIO_DmaEn(SDIOx, DISABLE);
    return errorstatus;

}

__attribute__ ((aligned (4))) uint8_t SDIO_DATA_BUFFER[512];

uint8_t SDMMC_ReadDisk(SDIO_TypeDef *SDIOx, uint8_t*buf, uint32_t sector, uint8_t cnt)
{
    uint8_t sta = SDMMC_OK;
    long long lsector = sector;
    uint8_t n;
    lsector <<= 9;
    if(ADDR32(buf)%4 != 0)
    {
        for(n = 0;n<cnt;n++)
        {
            sta = SDMMC_ReadBlock(SDIOx, SDIO_DATA_BUFFER, lsector+512*n, 512);
            memcpy(buf, SDIO_DATA_BUFFER, 512);
            buf += 512;
        }
    }else
    {
        if(cnt == 1)sta = SDMMC_ReadBlock(SDIOx, buf, lsector, 512);
        else sta = SDMMC_ReadMultiBlocks(SDIOx, buf, lsector, 512, cnt);
    }
    return sta;
}

uint8_t SDMMC_WriteDisk(SDIO_TypeDef *SDIOx, uint8_t *buf, uint32_t sector, uint8_t cnt)
{
    uint8_t sta = SDMMC_OK;
    uint8_t n;
    long long lsector = sector;
    lsector <<= 9;
    if(ADDR32(buf)%4 != 0)
    {
        for(n = 0;n<cnt;n++)
        {
            memcpy(SDIO_DATA_BUFFER, buf, 512);
            sta = SDMMC_WriteBlock(SDIOx, SDIO_DATA_BUFFER, lsector+512*n, 512);
            buf += 512;
        }
    }else
    {

        if(cnt == 1)sta = SDMMC_WriteBlock(SDIOx, buf, lsector, 512);
        else sta = SDMMC_WriteMultiBlocks(SDIOx, buf, lsector, 512, cnt);
    }
    return sta;
}
