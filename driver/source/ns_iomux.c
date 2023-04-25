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
#include "ns_iomux.h"

/**
 * @file     ns__iomux.c
 * @brief    This file contains all the functions prototypes for the IOMUX firmware
  */

/**
  * \brief  Configurates the specified pad oval.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  PER_iof_num: specifies the iof number from 0 to 31.
  * \param  pad_num:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  pad_grp: pad group.
  * \param  ls_per_grp: per group in ls mode.
  */

uint8_t iomux_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t per_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  pad_grp,uint8_t ls_per_grp)
{
   switch(hs_ls)
   {
       case 0:
            if(pad_grp != ls_per_grp)
                while(1);
            REG32((IO_MUX_BASE + LS_SRC_SEL_OFS    + 0x4 * pad_num))= per_iof_num ; 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS + 0x4 * pad_num))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num)) |=  DO_SEL_HS | OE_SEL ;   
            break;
        case 1:
        case 2:
        case 3: 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS + 0x4 * per_iof_num))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * per_iof_num))|=  DO_SEL_HS | OE_SEL; 
            break;    
     
        default:
            break;                                                                 
    }
}

/**
  * \brief  Configurates the specified pad ival.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  PER_iof_num: specifies the iof number from 0 to 31.
  * \param  pad_num:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  pad_grp: pad group.
  * \param  ls_per_grp: per group in ls mode.
  */
uint8_t iomux_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t per_iof_num,uint32_t pad_num, uint8_t hs_ls ,uint8_t  pad_grp,uint8_t ls_per_grp)
{
     switch(hs_ls)
    {
        case 0:
            if(pad_grp != ls_per_grp)
                while(1);
            REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS + 0x4 * per_iof_num))= pad_num ;    
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num))|= 0x1 ;  
            break;            
        case 1:
        case 2:
        case 3: 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS + 0x4 * per_iof_num))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * per_iof_num))|=  DO_SEL_HS |  IE_SEL;      
            break;
        default:
            break    ;                                                             
    }     
}

/**
  * \brief  Configurates the specified pad oval.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  PER_iof_num: specifies the iof number from 0 to 31.
  * \param  pad_num:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */

void iomux_ls_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{
   switch(hs_ls)
   {
       case 0:
            REG32((IO_MUX_BASE + LS_SRC_SEL_OFS    + 0x4 * pad_num))= PER_iof_num ; 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS + 0x4 * pad_num))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num)) |=  DO_SEL_HS | OE_SEL ;   
            break;
        case 1:
        case 2:
        case 3: 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS + 0x4 * PER_iof_num))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * PER_iof_num))|=  DO_SEL_HS | OE_SEL; 
            break;    
     
        default:
            break;                                                                 
    }
}

/**
  * \brief  Configurates the specified pad ival.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  PER_iof_num: specifies the iof number from 0 to 31.
  * \param  pad_num:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{
     switch(hs_ls)
    {
        case 0:
            REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS + 0x4 * PER_iof_num))= pad_num ;    
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num))|= 0x1 ;  
            break;            
        case 1:
        case 2:
        case 3: 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS + 0x4 * PER_iof_num))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * PER_iof_num))|=  DO_SEL_HS |  IE_SEL;      
            break;
        default:
            break    ;                                                             
    }     
}

void iomux_pad_phy_ctrl(unsigned long IO_MUX_BASE, uint32_t pad_num ,uint32_t ctrl )
{ 
    REG32((IO_MUX_BASE + PHY_CNTRL_OFS + 0x4 * pad_num))|= ctrl ;              
}

/**
  * \brief  Pull up the specified pad.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  pad_num:  specifies the pad number.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_pullup_cfg(unsigned long IO_MUX_BASE, uint32_t pad_num, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{ 
    REG32((IO_MUX_BASE + PHY_CNTRL_OFS + 0x4 * pad_num))|= PU ;              
}

/**
  * \brief  Pull down the specified pad.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  pad_num:  specifies the pad number.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_pulldown_cfg(unsigned long IO_MUX_BASE, uint32_t pad_num, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{ 
    REG32((IO_MUX_BASE + PHY_CNTRL_OFS + 0x4 * pad_num))|= PD ;              
}

/**
  * \brief  Configurates the specified pad only in low speed mode.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  PER_iof_ival_num : specified peripheral the iof number of ival.
  * \param  PER_iof_oval_num : specified peripheral the iof number of oval.
  * \param  pad_num:  specifies the pad number.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_loopback_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_ival_num ,uint32_t PER_iof_oval_num ,uint32_t pad_num, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{              
    REG32((IO_MUX_BASE + LS_SRC_SEL_OFS    + 0x4 * pad_num))= PER_iof_oval_num ; 
    REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS + 0x4 * pad_num))= hs_ls ; 
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num)) |=  DO_SEL_HS | OE_SEL ;  
    REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS + 0x4 * PER_iof_ival_num))= pad_num ;    
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num))|= 0x1 ;               
}

void iomux_pu_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_num)
{
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num)) |= PU_SEL ;
}

void iomux_pd_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_num)
{

    REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num)) |= PD_SEL ;

}

void iomux_pu_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_num)
{
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num)) &= (uint32_t)(~PU_SEL) ;
}

void iomux_pd_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_num)
{
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS + 0x4 * pad_num)) &= (uint32_t)(~PD_SEL) ;
}
