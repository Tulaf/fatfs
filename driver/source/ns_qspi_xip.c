/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier Apache-2.0
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
#include "ns_qspi_xip.h"
#include "ns_conf.h"

/**
 * @file     ns__qspi.c
 * @brief    This file contains all the functions prototypes for the QSPI_XIP firmware
  */

/* QSPI_XIP registers Masks */
#define QSPI_XIP_CR_Clear_Mask                       (BITS(0,6))
#define QSPI_XIP_CSMODE_Clear_Mask                   (BITS(0,1))
#define QSPI_XIP_FMT_Clear_Mask                      (BITS(0,21))
#define QSPI_XIP_FORCE_Clear_Mask                    (BITS(0,4))
/* QSPI_XIP_TX_MARK */
#define QSPI_XIP_TX_Mask                             BITS(0,2)
/* QSPI_XIP_RX_MARK */
#define QSPI_XIP_RX_Mask                             BITS(0,2)
/* QSPI_XIP_DELAY0 Mask*/
#define QSPI_XIP_DELAY0_CSSCK_Mask                   BITS(0,7)
#define QSPI_XIP_DELAY0_SCKCS_Mask                   BITS(16,23)
/* QSPI_XIP_DELAY1 Mask*/
#define QSPI_XIP_DELAY1_INTERCS_Mask                 BITS(0,7)
#define QSPI_XIP_DELAY1_INTERXFR_Mask                BITS(16,23)
/* QSPI_XIP rxedge Mask*/
#define QSPI_XIP_RXEDGE_Mask                    BIT(0)

/**
  * \brief      Initialize QSPI_XIP parameter.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_Struct QSPI_XIP parameter initialization stuct members of the structure.
  */
void QSPI_XIP_Init(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_InitTypeDef* QSPI_XIP_Struct)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_SCKDIV_PRESCALER(QSPI_XIP_Struct->SCKDIV));
    assert_param(IS_QSPI_XIP_SPI_SCKMODE_CPOL(QSPI_XIP_Struct->CPOL));
    assert_param(IS_QSPI_XIP_SPI_SCKMODE_CPHA(QSPI_XIP_Struct->CPHA));
    assert_param(IS_QSPI_XIP_SPI_CSMODE_MODE(QSPI_XIP_Struct->CSMode));
    assert_param(IS_QSPI_XIP_SPI_FMT_ENDIAN(QSPI_XIP_Struct->Endian));
    assert_param(IS_QSPI_XIP_SPI_FMT_LEN(QSPI_XIP_Struct->DataSize));
    assert_param(IS_QSPI_XIP_SPI_FMT_PROTO(QSPI_XIP_Struct->ProtolMode));
    assert_param(IS_QSPI_XIP_SPI_CR_MODE(QSPI_XIP_Struct->DevMode));
    assert_param(IS_QSPI_XIP_SPI_CR_SSM(QSPI_XIP_Struct->SSM));

    uint32_t reg = 0U;
    uint32_t reg_cr = 0U;
    /* Set  SCKDIV register according to QSPI_XIP BaudRatePrescaler value */
    QSPI_XIPx->SCKDIV = QSPI_XIP_Struct->SCKDIV;
    /* Configure QSPI_XIPx  CPOL bit */
    QSPI_XIPx->SCKMODE |= QSPI_XIP_Struct->CPOL;
    /* Configure QSPI_XIPx  CPHA bit */
    QSPI_XIPx->SCKMODE |= QSPI_XIP_Struct->CPHA;

    /* Configure QSPI_XIPx CS mode */
    reg = QSPI_XIPx->CSMODE;
    reg &= ~QSPI_XIP_CSMODE_Clear_Mask;
    reg |= (QSPI_XIP_Struct->CSMode);
    QSPI_XIPx->CSMODE = reg;

    /* Configure QSPI_XIPx direction, first transmitted ,datasize ,Protol*/
    reg = 0;
    reg = QSPI_XIPx->FMT;
    reg &= ~QSPI_XIP_FMT_Clear_Mask;
    reg |= (QSPI_XIP_Struct->Endian | QSPI_XIP_Struct->DataSize | QSPI_XIP_Struct->ProtolMode);
    QSPI_XIPx->FMT = reg;

    /* Configure QSPI_XIPx force*/
    reg = 0;
    reg = QSPI_XIPx->FORCE;
    reg &= ~QSPI_XIP_FORCE_Clear_Mask;
    reg |= QSPI_XIP_Struct->Force;
    QSPI_XIPx->FORCE = reg;

    /* Configure QSPI_XIPxMSTR bits and SSM bits according to  DevMode and  SSM value*/
    reg_cr = QSPI_XIPx->CR;
    reg_cr &= ~ QSPI_XIP_CR_Clear_Mask;
    reg_cr |= (QSPI_XIP_Struct->DevMode | QSPI_XIP_Struct->SSM | QSPI_XIP_CR_CSOE_ENABLE | QSPI_XIP_CR_CSI_OFF );
    reg_cr |= (QSPI_XIP_Struct->Tx_DMA | QSPI_XIP_Struct->Rx_DMA | QSPI_XIP_Struct->TxDmaCont | QSPI_XIP_Struct->RxDmaCont);
    QSPI_XIPx->CR = reg_cr;

    QSPI_XIPx->FCTRL &=QSPI_XIP_FCTRL_FLASH_DISABLE;
}

/**
  * \brief  Fills each QSPI_XIP_InitTypeDef member with its default value.
  * \param  QSPI_XIP_Struct pointer to a QSPI_XIP_InitTypeDef structure which will be initialized.
  */
void QSPI_XIP_StructInit(QSPI_XIP_InitTypeDef* QSPI_XIP_Struct)
{
    /*--------------- Reset QSPI_XIP init structure parameters values -----------------*/
    /* Initialize the ProtolMode member */
    QSPI_XIP_Struct->ProtolMode =QSPI_XIP_FMT_PROTO_SINGLE;

    /* initialize the DevMode member */
    QSPI_XIP_Struct->DevMode =QSPI_XIP_CR_MODE_MASTER;

    /* initialize the DataSize member */
    QSPI_XIP_Struct->DataSize =QSPI_XIP_FMT_LEN_8B;
    /* Initialize the CPOL member */
    QSPI_XIP_Struct->CPOL =QSPI_XIP_SCKMODE_CPOL_LOW ;
    /* Initialize the CPHA member */
    QSPI_XIP_Struct->CPHA =QSPI_XIP_SCKMODE_CPHA_EDGE1;
    /* Initialize the SSM member */
    QSPI_XIP_Struct->SSM =QSPI_XIP_CR_SSM_HARD;
    /* Initialize the SCKDIV member */
    QSPI_XIP_Struct->SCKDIV =QSPI_XIP_SCKDIV_PRESCALER_4;
    /* Initialize the Endian member */
    QSPI_XIP_Struct->Endian =QSPI_XIP_FMT_ENDIAN_MSB;
    /* Initialize the CSModet member */
    QSPI_XIP_Struct->CSMode =QSPI_XIP_CSMODE_MODE_AUTO;
    /* Initialize the Force member */
    QSPI_XIP_Struct->Force =QSPI_XIP_FORCE_EN;
    QSPI_XIP_Struct->Tx_DMA =QSPI_XIP_CR_DMA_TX_DISABLE;
    QSPI_XIP_Struct->Rx_DMA =QSPI_XIP_CR_DMA_RX_DISABLE;
    QSPI_XIP_Struct->TxDmaCont =QSPI_XIP_CR_DMA_TCONTINOUS_MODE_DISABLE;
    QSPI_XIP_Struct->RxDmaCont =QSPI_XIP_CR_DMA_RCONTINOUS_MODE_DISABLE;
}

/**
  * \brief      Configures QSPI_XIP CS in slave mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the SPIx CS signal.
  *                     This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_SSM_Switch(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status == ENABLE) {
        /* Set cs enabled */
        QSPI_XIPx->CR &= (~QSPI_XIP_CR_CSI_OFF);
    } else {
        /* Set cs disabled */
        QSPI_XIPx->CR |=QSPI_XIP_CR_CSI_OFF;
    }
}

/**
  * \brief      Configures QSPI_XIP RxEdge.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the QSPI_XIP Edge.
  *                     This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_RxEdgeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status == ENABLE) {
        /* Set QSPI_XIP edge */
        QSPI_XIPx->RXEDGE |=QSPI_XIP_RXEDGE_Mask;
    } else {
        /* Reset QSPI_XIP edge */
        QSPI_XIPx->RXEDGE &= ~QSPI_XIP_RXEDGE_Mask;
    }
}

/**
  * \brief      Configures QSPI_XIP  TX water mark.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Mark Interruption thresholds
  */
void QSPI_XIP_TxmarkConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Mark)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_TX_RX_WaterMark(Mark));

    /* Set TX Interruption thresholds bit value */
    QSPI_XIPx->TX_MARK = Mark & QSPI_XIP_TX_Mask;
}

/**
  * \brief      Configures QSPI_XIP RX water mark.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Mark Interruption thresholds.
  */
void QSPI_XIP_RxmarkConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Mark)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_TX_RX_WaterMark(Mark));

    /* Set RX Interruption thresholds bit value */
    QSPI_XIPx->RX_MARK = Mark & QSPI_XIP_RX_Mask;
}

/**
  * \brief      Configures QSPI_XIP Direction
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Direction QSPI_XIP_FMT_DIR_TX,QSPI_XIP_FMT_DIR_RX.
  *  This parameter can be one of the following values
  *     \arg QSPI_XIP_FMT_DIR_TX Selects Tx transmission direction.
  *     \arg QSPI_XIP_FMT_DIR_RX Selects Rx receive direction.
  */
void QSPI_XIP_DirectionConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Direction)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_FMT_DIR(Direction));

    if (Direction ==QSPI_XIP_FMT_DIR_TX) {
        /* Set the Tx/Rx mode */
        QSPI_XIPx->FMT |=QSPI_XIP_FMT_DIR_TX;
    } else {
        /* Set the Rx only mode */
        QSPI_XIPx->FMT &= (~QSPI_XIP_FMT_DIR_TX);
    }
}

/**
  * \brief      Enables or disables QSPI_XIP interrupts.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_IE specifies the QSPI_XIP interrupt source to be enabled or disabled.
  *  This parameter can be one of the following values
  *     \arg QSPI_XIP_IE_TX
  *     \arg QSPI_XIP_IE_RX
  *     \arg QSPI_XIP_IE_TX_UDR
  *     \arg QSPI_XIP_IE_RX_OVR
  *     \arg QSPI_XIP_IE_RX_UDR
  *     \arg QSPI_XIP_IE_TX_OVR
  *     \arg QSPI_XIP_IE_DONE
  *     \arg QSPI_XIP_IE_TX_DONE
  *     \arg QSPI_XIP_IE_RX_DONE
  * \param[in]  Status new state of the QSPI_XIPx peripheral.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_ITConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IE, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP interrupt */
        QSPI_XIPx->IE |= QSPI_XIP_IE;
    } else {
        /* Disable the selected QSPI_XIP interrupt */
        QSPI_XIPx->IE &= ~QSPI_XIP_IE;
    }
}

/**
  * \brief      Enables or disables QSPI_XIP interrupts.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_IE specifies the QSPI_XIP interrupt source to be enabled or disabled.
  *     \arg QSPI_XIP_IE_TX
  *     \arg QSPI_XIP_IE_RX
  *     \arg QSPI_XIP_IE_TX_UDR
  *     \arg QSPI_XIP_IE_RX_OVR
  *     \arg QSPI_XIP_IE_RX_UDR
  *     \arg QSPI_XIP_IE_TX_OVR
  *     \arg QSPI_XIP_IE_DONE
  *     \arg QSPI_XIP_IE_TX_DONE
  *     \arg QSPI_XIP_IE_RX_DONE
  * \retval  The specified QSPI_XIP interrupt status, 
  *          the value can be SET or RESET.
  */
FlagStatus QSPI_XIP_GetITConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IE)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_CONFIG_IE(QSPI_XIP_IE));

    if (QSPI_XIPx->IE & QSPI_XIP_IE) {
        /* SPI_IT is set */
        return SET;
    } else {
        /* SPI_IT is reset */
        return RESET;
    }
}

/**
  * \brief     Enables or disables the QSPI_XIP Half-duplex mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the QSPI_XIPx peripheral.
  *            This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_HDSCmd(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable Half-duplex mode */
        QSPI_XIPx->CR |= QSPI_XIP_CR_HDSMODE_ENABLE;
    } else {
        /* Disable Half-duplex mode */
        QSPI_XIPx->CR &= QSPI_XIP_CR_HDSMODE_DISABLE;
    }
}

/**
  * \brief     Checks whether the specified QSPI interrupt has occurred or not.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  SPI_IP specifies the QSPI interrupt source to check.
  *     \arg QSPI_IP_TX
  *     \arg QSPI_IP_RX
  * \retval  The specified QSPI interrupt status, 
  *          the value can be SET or RESET.
  */
FlagStatus QSPI_XIP_GetITStatus(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IT)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    uint16_t itpos = 0, itmask = 0, enablestatus = 0;
    /* Check the status of the specified SPI interrupt */
    if (QSPI_XIPx->IP & QSPI_XIP_IT) {
        /* SPI_IT is set */
        return SET;
    } else {
        /* SPI_IT is reset */
        return RESET;
    }
}

/**
  * \brief      Checks whether the specified QSPI flag is set or not.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  SPI_FLAG specifies the SPI flag to check.
  *    \arg QSPI_STATUS_BUSY
  *    \arg QSPI_STATUS_RX_OVR
  *    \arg QSPI_STATUS_TX_UDR
  *    \arg QSPI_STATUS_TX_FULL
  *    \arg QSPI_STATUS_RX_EMPTY
  *    \arg QSPI_STATUS_RX_UDR
  *    \arg QSPI_STATUS_TX_OVR
  *    \arg QSPI_STATUS_TX_EMPTY
  *    \arg QSPI_STATUS_RX_FULL
  *    \arg QSPI_STATUS_DONE
  *    \arg QSPI_STATUS_TX_DONE
  *    \arg QSPI_STATUS_RX_DONE
  * \retval  The specified QSPI flag status, 
  *          the value can be SET or RESET.
  */
FlagStatus QSPI_XIP_GetFlag(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    /* Check the status of the specified QSPI_XIP flag */
    if ((QSPI_XIPx->STATUS & QSPI_XIP_FLAG) != RESET) {
        /* SPI_FLAG is set */
        return SET;
    } else {
        /* SPI_FLAG is reset */
        return RESET;
    }
}

/**
  * \brief      Clears the specified QSPI flag.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  QSPI_CLEAR_FLAG specifies the QSPI flag to clears.
  *    \arg QSPI_STATUS_RX_OVR
  *    \arg QSPI_STATUS_TX_UDR
  *    \arg QSPI_STATUS_RX_UDR
  *    \arg QSPI_STATUS_TX_OVR
  *    \arg QSPI_STATUS_DONE
  *    \arg QSPI_STATUS_TX_DONE
  *    \arg QSPI_STATUS_RX_DONE
  */
void QSPI_XIP_ClearFlag(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    
    /* Clear the selected QSPI_XIP flag */
    QSPI_XIPx->STATUS = QSPI_XIP_FLAG;
}

/**
  * \brief      Configures QSPI_XIP CS mode for the selected QSPI_XIP.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  CsMode specifies the chip select signal mode.
  *     \arg AUTO CS is automatically controlled by the hardware.
  *     \arg HOLD CS is controlled by CSID¡¢CSDef¡¢CSMode and SPI_FCTRL.
  *     \arg OFF CS is controlled by CSID¡¢CSDef and CSMode.
  */
void QSPI_XIP_CSModeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t CsMode)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_CSMODE_MODE(CsMode));

    /* Set CSmode */
    QSPI_XIPx->CSMODE = CsMode;
}

/**
  * \brief      Configures QSPI_XIP CSID value.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  CsID configures the value of CSID.
  */
void QSPI_XIP_CSIDConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t CsID)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_CSID_NUM(CsID));

    /* Set new CSID bit value */
    QSPI_XIPx->CSID = CsID;
}

/**
  * \brief      Configures QSPI_XIP delay0
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Sckcs Specifies how many cycles are still valid at least after the last SCK clock edge, after the end of sending data.
  * \param[in]  Cssck Specifies how many cycles are still valid at least after the last SCK clock edge, before the end of sending data.
*/
void QSPI_XIP_Delay0Config(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Sckcs, uint32_t Cssck)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    tmp |= ((Sckcs << 16) & QSPI_XIP_DELAY0_SCKCS_MASK);
    tmp |= (Cssck & QSPI_XIP_DELAY0_CSSCK_MASK);
    QSPI_XIPx->DELAY0 = tmp;
}

/**
  * \brief      Configures QSPI_XIP delay1
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Intercs Specifies Minimum CS inactive time.
  * \param[in]  Interxfr Specifies Maximum interframe delay.
  * \note       This only happen if SPI_SCKMODE is configured to HOLD or OFF modes.
  */
void QSPI_XIP_Delay1Config(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Intercs, uint32_t Interxfr)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    tmp |= (Interxfr << 16);
    tmp |= Intercs;
    QSPI_XIPx->DELAY1 = tmp;
}

/**
  * \brief      Transmits a Data through the QSPI_XIPx peripheral.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Data Data to be transmitted.
  */
void QSPI_XIP_SendData(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_DATA(Data));

    /* Write in the TXDATA register the data to be sent */
    QSPI_XIPx->TXDATA = Data;
}

/**
  * \brief      QSPI_XIP receive data function.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \retval     The value of the received data.
  */
uint32_t QSPI_XIP_ReceiveData(QSPI_XIP_TypeDef* QSPI_XIPx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    /* Return the data in the RXDATA register */
    return (uint32_t)(QSPI_XIPx->RXDATA);
}

/**
  * \brief      Enables/Disables flashxip mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the selected QSPI_XIP XIP mode.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_Enable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP XIP mode */
        QSPI_XIPx->FCTRL |= (uint32_t)QSPI_XIP_FCTRL_FLASH_ENABLE;
    } else {
        /* Disable the selected QSPI_XIP mode */
        QSPI_XIPx->FCTRL &= (~(uint32_t)QSPI_XIP_FCTRL_FLASH_ENABLE);
    }
}

/**
  * \brief      the format of date to send in flashxip read mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_RDFmt pointer to a QSPI_XIP_ReadStruct structure that
  *             contains the configuration information for the specified QSPI_XIP peripheral number in FLASHXIP read mode.
  */
void QSPI_XIP_RDConfig(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_ReadStruct* QSPI_XIP_RDFmt)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    uint32_t reg_tmp = 0U;
    reg_tmp = QSPI_XIPx->FFMT;
    reg_tmp &= ~(QSPI_XIP_FFMT_CMD_CODE_MASK | QSPI_XIP_FFMT_CMD | QSPI_XIP_FFMT_ADDR_LEN_MASK |
                  QSPI_XIP_FFMT_PAD_CNT_MASK | QSPI_XIP_FFMT_CMD_PROTO_MASK |QSPI_XIP_FFMT_ADDR_PROTO_MASK |
                  QSPI_XIP_FFMT_DATA_PROTO_MASK | QSPI_XIP_FFMT_ENDINA_F | QSPI_XIP_FFMT_CMD_CODE_MASK);

    reg_tmp |= (QSPI_XIP_RDFmt->PAD_CODE << QSPI_XIP_FFMT_PAD_CODE_OFS)        |
                (QSPI_XIP_RDFmt->CMD_CODE << QSPI_XIP_FFMT_CMD_CODE_OFS)      |
                (QSPI_XIP_RDFmt->DATA_PROTO << QSPI_XIP_FFMT_DATA_PROTO_OFS)  |
                (QSPI_XIP_RDFmt->ADDR_PROTO << QSPI_XIP_FFMT_ADDR_PROTO_OFS)  |
                (QSPI_XIP_RDFmt->CMD_PROTO << QSPI_XIP_FFMT_CMD_PROTO_OFS)    |
                (QSPI_XIP_RDFmt->PAD_CNT << QSPI_XIP_FFMT_PAD_CNT_OFS)        |
                (QSPI_XIP_RDFmt->ADDR_LEN << QSPI_XIP_FFMT_ADDR_LEN_OFS)      |
                (QSPI_XIP_RDFmt->CMD_EN << QSPI_XIP_FFMT_CMD_OFS);

    QSPI_XIPx->FFMT = reg_tmp;
}

/**
  * \brief      the format of date to send in flashxip write mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_WRFFmt1 pointer to a QSPI_XIP_WriteStruct structure that
  *             contains the configuration information for the specified SPI peripheral in FLASHXIP write mode.
  */
void QSPI_XIP_WRConfig(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_WriteStruct* QSPI_XIP_WRFFmt1)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    QSPI_XIP_RDConfig(QSPI_XIPx, &(QSPI_XIP_WRFFmt1->XIPRX));
    uint32_t reg_tmp = 0U;
    reg_tmp = QSPI_XIPx->FFMT1;
    reg_tmp &= ~(QSPI_XIP_FFMT1_WCMD_CODE_MASK | QSPI_XIP_FFMT1_WPAD_CNT_MASK | QSPI_XIP_FFMT1_PAD_CNT_H |
                  QSPI_XIP_FFMT1_DDR_EN_MASK | QSPI_XIP_FFMT1_MODE_PROTO_MASK |QSPI_XIP_FFMT1_MODE_CODE_MASK |
                  QSPI_XIP_FFMT1_MODE_CNT_MASK);

    reg_tmp |= (QSPI_XIP_WRFFmt1->WCMD_CODE << QSPI_XIP_FFMT1_WCMD_CODE_OFS)   |
                (QSPI_XIP_WRFFmt1->WPAD_CNT << QSPI_XIP_FFMT1_WPAD_CNT_OFS)    |
                (QSPI_XIP_WRFFmt1->DDR_EN << QSPI_XIP_FFMT1_DDR_EN_OFS)        |
                (QSPI_XIP_WRFFmt1->PAD_CNT_H << QSPI_XIP_FFMT1_PAD_CNT_H_OFS)  |
                (QSPI_XIP_WRFFmt1->MODE_PROTO << QSPI_XIP_FFMT1_MODE_PROTO_OFS)|
                (QSPI_XIP_WRFFmt1->MODE_CODE << QSPI_XIP_FFMT1_MODE_CODE_OFS)  |
                (QSPI_XIP_WRFFmt1->MODE_CNT << QSPI_XIP_FFMT1_MODE_CNT_OFS);
    QSPI_XIPx->FFMT1 = reg_tmp;
}

/**
  * \brief      Read Tx FIFO Entry.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \retval     the num of fifo entry.
  */
uint32_t QSPI_XIP_TxReadEntry(QSPI_XIP_TypeDef* QSPI_XIPx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    return QSPI_XIPx->FIFO_NUM & BITS(0, 15);
}

/**
  * \brief      Read Rx FIFO Entry.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \retval     the num of fifo entry.
  */
uint32_t QSPI_XIP_RxReadEntry(QSPI_XIP_TypeDef* QSPI_XIPx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    return (QSPI_XIPx->FIFO_NUM & BITS(16, 31)) >> 16;
}
