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
#include "ns_qspi.h"
#include "ns_conf.h"

/**
 * @file     ns__qspi.c
 * @brief    This file contains all the functions prototypes for the QSPI firmware
  */

/* QSPI registers Masks */
#define QSPI_CR_Clear_Mask                       (BITS(0,6))
#define QSPI_CSMODE_Clear_Mask                   (BITS(0,1))
#define QSPI_FMT_Clear_Mask                      (BITS(0,21))
#define QSPI_FORCE_Clear_Mask                    (BITS(0,4))
/* QSPI_TX_MARK */
#define QSPI_TX_Mask                             BITS(0,2)
/* QSPI_RX_MARK */
#define QSPI_RX_Mask                             BITS(0,2)
/* QSPI_DELAY0 Mask*/
#define QSPI_DELAY0_CSSCK_Mask                   BITS(0,7)
#define QSPI_DELAY0_SCKCS_Mask                   BITS(16,23)
/* QSPI_DELAY1 Mask*/
#define QSPI_DELAY1_INTERCS_Mask                 BITS(0,7)
#define QSPI_DELAY1_INTERXFR_Mask                BITS(16,23)
/* QSPI rxedge Mask*/
#define QSPI_RXEDGE_Mask                    BIT(0)

/**
  * \brief      Initialize QSPI parameter.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  QSPI_Struct QSPI parameter initialization stuct members of the structure.
  */
void QSPI_Init(QSPI_TypeDef* QSPIx, QSPI_InitTypeDef* QSPI_Struct)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_SPI_SCKDIV_PRESCALER(QSPI_Struct->SCKDIV));
    assert_param(IS_QSPI_SPI_SCKMODE_CPOL(QSPI_Struct->CPOL));
    assert_param(IS_QSPI_SPI_SCKMODE_CPHA(QSPI_Struct->CPHA));
    assert_param(IS_QSPI_SPI_CSMODE_MODE(QSPI_Struct->CSMode));
    assert_param(IS_QSPI_SPI_FMT_ENDIAN(QSPI_Struct->Endian));
    assert_param(IS_QSPI_SPI_FMT_LEN(QSPI_Struct->DataSize));
    assert_param(IS_QSPI_SPI_FMT_PROTO(QSPI_Struct->ProtolMode));
    assert_param(IS_QSPI_SPI_CR_MODE(QSPI_Struct->DevMode));
    assert_param(IS_QSPI_SPI_CR_SSM(QSPI_Struct->SSM));

    uint32_t reg = 0U;
    uint32_t reg_cr = 0U;
    /* Set  SCKDIV register according to QSPI BaudRatePrescaler value */
    QSPIx->SCKDIV = QSPI_Struct->SCKDIV;
    /* Configure QSPIx  CPOL bit */
    QSPIx->SCKMODE |= QSPI_Struct->CPOL;
    /* Configure QSPIx  CPHA bit */
    QSPIx->SCKMODE |= QSPI_Struct->CPHA;

    /* Configure QSPIx CS mode */
    reg = QSPIx->CSMODE;
    reg &= ~QSPI_CSMODE_Clear_Mask;
    reg |= (QSPI_Struct->CSMode);
    QSPIx->CSMODE = reg;

    /* Configure QSPIx direction, first transmitted ,datasize ,Protol*/
    reg = 0;
    reg = QSPIx->FMT;
    reg &= ~QSPI_FMT_Clear_Mask;
    reg |= (QSPI_Struct->Endian | QSPI_Struct->DataSize | QSPI_Struct->ProtolMode);
    QSPIx->FMT = reg;

    /* Configure QSPIx force*/
    reg = 0;
    reg = QSPIx->FORCE;
    reg &= ~QSPI_FORCE_Clear_Mask;
    reg |= QSPI_Struct->Force;
    QSPIx->FORCE = reg;

    /* Configure QSPIxMSTR bits and SSM bits according to  DevMode and  SSM value*/
    reg_cr = QSPIx->CR;
    reg_cr &= ~ QSPI_CR_Clear_Mask;
    reg_cr |= (QSPI_Struct->DevMode | QSPI_Struct->SSM | QSPI_CR_CSOE_ENABLE | QSPI_CR_CSI_OFF );
    reg_cr |= (QSPI_Struct->Tx_DMA | QSPI_Struct->Rx_DMA | QSPI_Struct->TxDmaCont | QSPI_Struct->RxDmaCont);
    QSPIx->CR = reg_cr;

}

/**
  * \brief  Fills each QSPI_InitTypeDef member with its default value.
  * \param  QSPI_Struct pointer to a QSPI_InitTypeDef structure which will be initialized.
  */
void QSPI_StructInit(QSPI_InitTypeDef* QSPI_Struct)
{
    /*--------------- Reset QSPI init structure parameters values -----------------*/
    /* Initialize the ProtolMode member */
    QSPI_Struct->ProtolMode =QSPI_FMT_PROTO_SINGLE;

    /* initialize the DevMode member */
    QSPI_Struct->DevMode =QSPI_CR_MODE_MASTER;

    /* initialize the DataSize member */
    QSPI_Struct->DataSize =QSPI_FMT_LEN_8B;
    /* Initialize the CPOL member */
    QSPI_Struct->CPOL =QSPI_SCKMODE_CPOL_LOW ;
    /* Initialize the CPHA member */
    QSPI_Struct->CPHA =QSPI_SCKMODE_CPHA_EDGE1;
    /* Initialize the SSM member */
    QSPI_Struct->SSM =QSPI_CR_SSM_HARD;
    /* Initialize the SCKDIV member */
    QSPI_Struct->SCKDIV =QSPI_SCKDIV_PRESCALER_4;
    /* Initialize the Endian member */
    QSPI_Struct->Endian =QSPI_FMT_ENDIAN_MSB;
    /* Initialize the CSModet member */
    QSPI_Struct->CSMode =QSPI_CSMODE_MODE_AUTO;
    /* Initialize the Force member */
    QSPI_Struct->Force =QSPI_FORCE_EN;
    QSPI_Struct->Tx_DMA =QSPI_CR_DMA_TX_DISABLE;
    QSPI_Struct->Rx_DMA =QSPI_CR_DMA_RX_DISABLE;
    QSPI_Struct->TxDmaCont =QSPI_CR_DMA_TCONTINOUS_MODE_DISABLE;
    QSPI_Struct->RxDmaCont =QSPI_CR_DMA_RCONTINOUS_MODE_DISABLE;
}

/**
  * \brief      Configures QSPI CS in slave mode.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Status new state of the SPIx CS signal.
  *                     This parameter can be ENABLE or DISABLE.
  */
void QSPI_SSM_Switch(QSPI_TypeDef* QSPIx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status == ENABLE) {
        /* Set cs enabled */
        QSPIx->CR &= (~QSPI_CR_CSI_OFF);
    } else {
        /* Set cs disabled */
        QSPIx->CR |=QSPI_CR_CSI_OFF;
    }
}

/**
  * \brief      Configures QSPI RxEdge.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Status new state of the QSPI Edge.
  *                     This parameter can be ENABLE or DISABLE.
  */
void QSPI_RxEdgeConfig(QSPI_TypeDef* QSPIx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status == ENABLE) {
        /* Set QSPI edge */
        QSPIx->RXEDGE |=QSPI_RXEDGE_Mask;
    } else {
        /* Reset QSPI edge */
        QSPIx->RXEDGE &= ~QSPI_RXEDGE_Mask;
    }
}

/**
  * \brief      Configures QSPI  TX water mark.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Mark Interruption thresholds
  */
void QSPI_TxmarkConfig(QSPI_TypeDef* QSPIx, uint32_t Mark)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_TX_RX_WaterMark(Mark));

    /* Set TX Interruption thresholds bit value */
    QSPIx->TX_MARK = Mark & QSPI_TX_Mask;
}

/**
  * \brief      Configures QSPI RX water mark.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Mark Interruption thresholds.
  */
void QSPI_RxmarkConfig(QSPI_TypeDef* QSPIx, uint32_t Mark)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_TX_RX_WaterMark(Mark));

    /* Set RX Interruption thresholds bit value */
    QSPIx->RX_MARK = Mark & QSPI_RX_Mask;
}

/**
  * \brief      Configures QSPI Direction
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Direction QSPI_FMT_DIR_TX,QSPI_FMT_DIR_RX.
  *  This parameter can be one of the following values
  *     \arg QSPI_FMT_DIR_TX Selects Tx transmission direction.
  *     \arg QSPI_FMT_DIR_RX Selects Rx receive direction.
  */
void QSPI_DirectionConfig(QSPI_TypeDef* QSPIx, uint32_t Direction)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_SPI_FMT_DIR(Direction));

    if (Direction ==QSPI_FMT_DIR_TX) {
        /* Set the Tx/Rx mode */
        QSPIx->FMT |=QSPI_FMT_DIR_TX;
    } else {
        /* Set the Rx only mode */
        QSPIx->FMT &= (~QSPI_FMT_DIR_TX);
    }
}

/**
  * \brief      Enables or disables QSPI interrupts.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  QSPI_IE specifies the QSPI interrupt source to be enabled or disabled.
  *  This parameter can be one of the following values
  *     \arg QSPI_IE_TX
  *     \arg QSPI_IE_RX
  *     \arg QSPI_IE_TX_UDR
  *     \arg QSPI_IE_RX_OVR
  *     \arg QSPI_IE_RX_UDR
  *     \arg QSPI_IE_TX_OVR
  *     \arg QSPI_IE_DONE
  *     \arg QSPI_IE_TX_DONE
  *     \arg QSPI_IE_RX_DONE
  * \param[in]  Status new state of the QSPIx peripheral.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_ITConfig(QSPI_TypeDef* QSPIx, uint32_t QSPI_IE, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI interrupt */
        QSPIx->IE |= QSPI_IE;
    } else {
        /* Disable the selected QSPI interrupt */
        QSPIx->IE &= ~QSPI_IE;
    }
}

/**
  * \brief      Enables or disables QSPI interrupts.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  QSPI_IE specifies the QSPI interrupt source to be enabled or disabled.
  *     \arg QSPI_IE_TX
  *     \arg QSPI_IE_RX
  *     \arg QSPI_IE_TX_UDR
  *     \arg QSPI_IE_RX_OVR
  *     \arg QSPI_IE_RX_UDR
  *     \arg QSPI_IE_TX_OVR
  *     \arg QSPI_IE_DONE
  *     \arg QSPI_IE_TX_DONE
  *     \arg QSPI_IE_RX_DONE
  * \retval  The specified QSPI interrupt status, 
  *          the value can be SET or RESET.
  */
FlagStatus QSPI_GetITConfig(QSPI_TypeDef* QSPIx, uint32_t QSPI_IE)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_CONFIG_IE(QSPI_IE));

    if (QSPIx->IE & QSPI_IE) {
        /* SPI_IT is set */
        return SET;
    } else {
        /* SPI_IT is reset */
        return RESET;
    }
}

/**
  * \brief     Enables or disables the QSPI Half-duplex mode.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Status new state of the QSPIx peripheral.
  *            This parameter can be ENABLE or DISABLE.
  */
void QSPI_HDSCmd(QSPI_TypeDef* QSPIx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable Half-duplex mode */
        QSPIx->CR |= QSPI_CR_HDSMODE_ENABLE;
    } else {
        /* Disable Half-duplex mode */
        QSPIx->CR &= QSPI_CR_HDSMODE_DISABLE;
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
FlagStatus QSPI_GetITStatus(QSPI_TypeDef* QSPIx, uint32_t QSPI_IT)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));

    uint16_t itpos = 0, itmask = 0, enablestatus = 0;
    /* Check the status of the specified SPI interrupt */
    if (QSPIx->IP & QSPI_IT) {
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
FlagStatus QSPI_GetFlag(QSPI_TypeDef* QSPIx, uint32_t QSPI_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));

    /* Check the status of the specified QSPI flag */
    if ((QSPIx->STATUS & QSPI_FLAG) != RESET) {
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
void QSPI_ClearFlag(QSPI_TypeDef* QSPIx, uint32_t QSPI_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    
    /* Clear the selected QSPI flag */
    QSPIx->STATUS = QSPI_FLAG;
}

/**
  * \brief      Configures QSPI CS mode for the selected QSPI.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  CsMode specifies the chip select signal mode.
  *     \arg AUTO CS is automatically controlled by the hardware.
  *     \arg HOLD CS is controlled by CSID¡¢CSDef¡¢CSMode and SPI_FCTRL.
  *     \arg OFF CS is controlled by CSID¡¢CSDef and CSMode.
  */
void QSPI_CSModeConfig(QSPI_TypeDef* QSPIx, uint32_t CsMode)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_SPI_CSMODE_MODE(CsMode));

    /* Set CSmode */
    QSPIx->CSMODE = CsMode;
}

/**
  * \brief      Configures QSPI CSID value.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  CsID configures the value of CSID.
  */
void QSPI_CSIDConfig(QSPI_TypeDef* QSPIx, uint32_t CsID)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_SPI_CSID_NUM(CsID));

    /* Set new CSID bit value */
    QSPIx->CSID = CsID;
}

/**
  * \brief      Configures QSPI delay0
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Sckcs Specifies how many cycles are still valid at least after the last SCK clock edge, after the end of sending data.
  * \param[in]  Cssck Specifies how many cycles are still valid at least after the last SCK clock edge, before the end of sending data.
*/
void QSPI_Delay0Config(QSPI_TypeDef* QSPIx, uint32_t Sckcs, uint32_t Cssck)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));

    tmp |= ((Sckcs << 16) & QSPI_DELAY0_SCKCS_MASK);
    tmp |= (Cssck & QSPI_DELAY0_CSSCK_MASK);
    QSPIx->DELAY0 = tmp;
}

/**
  * \brief      Configures QSPI delay1
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Intercs Specifies Minimum CS inactive time.
  * \param[in]  Interxfr Specifies Maximum interframe delay.
  * \note       This only happen if SPI_SCKMODE is configured to HOLD or OFF modes.
  */
void QSPI_Delay1Config(QSPI_TypeDef* QSPIx, uint32_t Intercs, uint32_t Interxfr)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));

    tmp |= (Interxfr << 16);
    tmp |= Intercs;
    QSPIx->DELAY1 = tmp;
}

/**
  * \brief      Transmits a Data through the QSPIx peripheral.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \param[in]  Data Data to be transmitted.
  */
void QSPI_SendData(QSPI_TypeDef* QSPIx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    assert_param(IS_QSPI_DATA(Data));

    /* Write in the TXDATA register the data to be sent */
    QSPIx->TXDATA = Data;
}

/**
  * \brief      QSPI receive data function.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \retval     The value of the received data.
  */
uint32_t QSPI_ReceiveData(QSPI_TypeDef* QSPIx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));

    /* Return the data in the RXDATA register */
    return (uint32_t)(QSPIx->RXDATA);
}

/**
  * \brief      Read Tx FIFO Entry.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \retval     the num of fifo entry.
  */
uint32_t QSPI_TxReadEntry(QSPI_TypeDef* QSPIx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    return QSPIx->FIFO_NUM & BITS(0, 15);
}

/**
  * \brief      Read Rx FIFO Entry.
  * \param[in]  QSPIx where x can be to select the QSPI peripheral number.
  * \retval     the num of fifo entry.
  */
uint32_t QSPI_RxReadEntry(QSPI_TypeDef* QSPIx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_ALL_PERIPH(QSPIx));
    return (QSPIx->FIFO_NUM & BITS(16, 31)) >> 16;
}

/**
  * \brief  Transmit and Receive an amount of data in blocking mode.
  * \param  QSPIx: pointer to a QSPI_HandleTypeDef structure that contains
  *                the configuration information for QSPI module.
  * \param  pTxData: pointer to transmission data buffer
  * \param  pRxData: pointer to reception data buffer
  * \retval FlagStatus
  */
FlagStatus QSPI_TransmitReceive(QSPI_TypeDef *QSPIx, uint8_t *pTxData, uint8_t *pRxData)
{
    while(SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_TX_FULL)){}
    QSPIx->TXDATA = *pTxData;
    while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_BUSY)){}

    while(SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_RX_EMPTY)){}
    *pRxData = (uint32_t)(QSPIx->RXDATA);
}

/**
  * \brief  Configure Chip Select
  * \param  QSPIx: pointer to a QSPI_HandleTypeDef structure that contains
  *                the configuration information for QSPI module.
  * \param  csid: Chip select data line
  * \param  Status new state of the QSPIx peripheral.
  *            This parameter can be ENABLE or DISABLE.
  * \retval None
  */
void QSPI_CS_Enable(QSPI_TypeDef* QSPIx, uint8_t csid, ControlStatus Status)
{
    if(ENABLE == Status){
        QSPIx->CSID |= csid;
    } else {
        QSPIx->CSID &= (~csid);
    }
}
