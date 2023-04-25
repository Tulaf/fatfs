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

#include "ns.h"
#include "ns_misc.h"

uint32_t misc_reset_flag(addr_xlen MISC_BASE)
{
    uint32_t regtmp;
    regtmp = REG32(MISC_BASE + RESET_CTRL0_OFS);
    return regtmp;
}

/**
  * \brief       Enables or disables the clock.
  * \param[in]   MISC_BASE: mis control based adress.
  * \param[in]   Status: new state of the the specified peripheral clock.
  *              This parameter can be: ENABLE or DISABLE.
  */
void misc_clk_cfg(addr_xlen MISC_BASE, uint32_t rcc_bit,  ControlStatus status) 
{
    if(status == ENABLE)
        REG32(MISC_BASE + SUBM_CLK_CTRL0_OFS + (rcc_bit/32)*4 ) |= BIT((rcc_bit%32));
    else
        REG32(MISC_BASE + SUBM_CLK_CTRL0_OFS + (rcc_bit/32)*4 )  &= ~BIT((rcc_bit%32));
}

 static inline void misc_clk_cfg1(uint32_t reg_base, uint32_t offset, uint32_t bit, ControlStatus status) 
{
    if(status == ENABLE)
        REG32(reg_base + offset) |= BIT(bit);
    else
        REG32(reg_base + offset) &= ~BIT(bit);
}

/**
  * \brief     Configurates the frequency division of clock.
  * \param[in]  MISC_BASE: mis control based adress.
  * \param[in]  CLK_CTRLx_OFS: reg offset addr.
  * \param[in]  div_val: frequency division coefficient.
  */
void  misc_clk_div(addr_xlen MISC_BASE, uint32_t CLK_CTRLx_OFS ,uint8_t div_val)
{
    REG32(MISC_BASE + CLK_CTRLx_OFS  ) = div_val;
}

 static inline  void misc_clk_div1(uint32_t reg_base, uint32_t CLK_CTRLx_OFS ,uint8_t div_val ,uint32_t start,uint32_t end) 
{
    REG32(reg_base + CLK_CTRLx_OFS  ) &= ~BITS(start,end);
    REG32(reg_base + CLK_CTRLx_OFS  ) = div_val<<start;
}

/**
  * \brief      Configurates the specified peripheral Reset.
  * \param[in]  MISC_BASE: mis control based adress.
  * \param[in]  reset_bit: Configurates the new state of the peripheral.   
  * \param[in]  status: frequency division coefficient.
  *             This parameter can be: ENABLE or DISABLE.
  */
 static inline  void misc_reset_cfg(uint32_t reg_base, uint32_t offset, uint32_t bit, ControlStatus status)
{
    if(status == ENABLE)
        REG32(reg_base + offset) |= BIT(bit);
    else
        REG32(reg_base + offset) &= ~BIT(bit);
}

/**
  * \brief      Get the status_ of core0_sysrstreq.
  * \retval     Val: the status_ of core0_sysrstreq.
  */
uint32_t Get_status_core0_sysrstreq(void)
{
    return REG32(0x18100000+0x0) & BIT(2);
}

/**
  * \brief      Get the status_ of por_rst_n.
  * \retval     Val: the status_ of por_rst_n.
  */
uint32_t Get_status_por_rst_n(void)
{
    return REG32(0x18100000+0x0) & BIT(1);
}

/**
  * \brief      Get the status_ of sys_rst_n_in.
  * \retval     Val: the status_ of sys_rst_n_in.
  */
uint32_t Get_status_sys_rst_n_in(void)
{
    return REG32(0x18100000+0x0) & BIT(0);
}

/**
  * \brief      clear_reset_status.
  */
void clear_reset_status(void)
{
    REG32(0x18100000+0xc) |=BIT(31);
}

/*******************************subm_reset_ctrl0 rst ctrl******************************************/

/**
  * \brief      Enables or disables iomux reset.
  * \param[in]  Status: new state of the iomux reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void iomux_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,18,status);
        
}

/**
  * \brief      Enables or disables idu reset.
  * \param[in]  Status: new state of the idu reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void idu_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,17,status);
        
}

/**
  * \brief      Enables or disables icb_slv0_sync reset.
  * \param[in]  Status: new state of the icb_slv0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void icb_slv0_sync_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,16,status);
        
}

/**
  * \brief      Enables or disables apb_slv0_sync reset.
  * \param[in]  Status: new state of the apb_slv0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv0_sync_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,15,status);
        
}

/**
  * \brief      Enables or disables ahb_slv0_sync reset.
  * \param[in]  Status: new state of the ahb_slv0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv0_sync_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,14,status);
        
}

/**
  * \brief      Enables or disables axi_slv0_sync reset.
  * \param[in]  Status: new state of the axi_slv0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void axi_slv0_sync_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,13,status);
        
}

/**
  * \brief      Enables or disables icb_mst0_sync reset.
  * \param[in]  Status: new state of the icb_mst0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void icb_mst0_sync_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,12,status);
        
}

/**
  * \brief      Enables or disables ahb_mst0_sync reset.
  * \param[in]  Status: new state of the ahb_mst0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_mst0_sync_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,11,status);
        
}

/**
  * \brief      Enables or disables axi_mst0_sync reset.
  * \param[in]  Status: new state of the axi_mst0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void axi_mst0_sync_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,10,status);
        
}

/**
  * \brief      Enables or disables sram1 reset.
  * \param[in]  Status: new state of the sram1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram1_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,9,status);
        
}

/**
  * \brief      Enables or disables sram0 reset.
  * \param[in]  Status: new state of the sram0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram0_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,8,status);
        
}

/**
  * \brief      Enables or disables sdio0_dma reset.
  * \param[in]  Status: new state of the sdio0_dma reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sdio0_dma_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,7,status);
        
}

/**
  * \brief      Enables or disables sdio0 reset.
  * \param[in]  Status: new state of the sdio0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sdio0_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,6,status);
        
}

/**
  * \brief      Enables or disables qspi1 reset.
  * \param[in]  Status: new state of the qspi1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi1_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,5,status);
        
}

/**
  * \brief      Enables or disables qspi_xip0 reset.
  * \param[in]  Status: new state of the qspi_xip0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi_xip0_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,4,status);
        
}

/**
  * \brief      Enables or disables i3c0_dma reset.
  * \param[in]  Status: new state of the i3c0_dma reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i3c0_dma_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,3,status);
        
}

/**
  * \brief      Enables or disables i3c0 reset.
  * \param[in]  Status: new state of the i3c0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i3c0_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,2,status);
        
}

/**
  * \brief      Enables or disables usart0 reset.
  * \param[in]  Status: new state of the usart0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart0_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,1,status);
        
}

/**
  * \brief      Enables or disables udma0 reset.
  * \param[in]  Status: new state of the udma0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void udma0_set_rst(ControlStatus status)
{
    misc_reset_cfg(0x18100000,0x20,0,status);
        
}

/*******************************subm_reset_ctrl1 rst ctrl******************************************/

/*******************************subm_reset_ctrl2 rst ctrl******************************************/

/*******************************subm_reset_ctrl3 rst ctrl******************************************/

/*******************************subm_reset_ctrl4 rst ctrl******************************************/

/*******************************subm_reset_ctrl5 rst ctrl******************************************/

/*******************************subm_reset_ctrl6 rst ctrl******************************************/

/*******************************subm_reset_ctrl7 rst ctrl******************************************/

/*******************************subm_clk_ctrl0 clk ctrl******************************************/

/**
  * \brief      Enables or disables iomux clock.
  * \param[in]  Status: new state of the iomux clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void iomux_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,18,status);
        
}

/**
  * \brief      Enables or disables idu clock.
  * \param[in]  Status: new state of the idu clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void idu_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,17,status);
        
}

/**
  * \brief      Enables or disables icb_slv0_sync clock.
  * \param[in]  Status: new state of the icb_slv0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void icb_slv0_sync_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,16,status);
        
}

/**
  * \brief      Enables or disables apb_slv0_sync clock.
  * \param[in]  Status: new state of the apb_slv0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv0_sync_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,15,status);
        
}

/**
  * \brief      Enables or disables ahb_slv0_sync clock.
  * \param[in]  Status: new state of the ahb_slv0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv0_sync_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,14,status);
        
}

/**
  * \brief      Enables or disables axi_slv0_sync clock.
  * \param[in]  Status: new state of the axi_slv0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void axi_slv0_sync_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,13,status);
        
}

/**
  * \brief      Enables or disables icb_mst0_sync clock.
  * \param[in]  Status: new state of the icb_mst0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void icb_mst0_sync_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,12,status);
        
}

/**
  * \brief      Enables or disables ahb_mst0_sync clock.
  * \param[in]  Status: new state of the ahb_mst0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_mst0_sync_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,11,status);
        
}

/**
  * \brief      Enables or disables axi_mst0_sync clock.
  * \param[in]  Status: new state of the axi_mst0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void axi_mst0_sync_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,10,status);
        
}

/**
  * \brief      Enables or disables sram1 clock.
  * \param[in]  Status: new state of the sram1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram1_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,9,status);
        
}

/**
  * \brief      Enables or disables sram0 clock.
  * \param[in]  Status: new state of the sram0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram0_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,8,status);
        
}

/**
  * \brief      Enables or disables sdio0_dma clock.
  * \param[in]  Status: new state of the sdio0_dma clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sdio0_dma_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,7,status);
        
}

/**
  * \brief      Enables or disables sdio0 clock.
  * \param[in]  Status: new state of the sdio0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sdio0_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,6,status);
        
}

/**
  * \brief      Enables or disables qspi1 clock.
  * \param[in]  Status: new state of the qspi1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi1_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,5,status);
        
}

/**
  * \brief      Enables or disables qspi_xip0 clock.
  * \param[in]  Status: new state of the qspi_xip0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi_xip0_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,4,status);
        
}

/**
  * \brief      Enables or disables i3c0_dma clock.
  * \param[in]  Status: new state of the i3c0_dma clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i3c0_dma_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,3,status);
        
}

/**
  * \brief      Enables or disables i3c0 clock.
  * \param[in]  Status: new state of the i3c0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i3c0_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,2,status);
        
}

/**
  * \brief      Enables or disables usart0 clock.
  * \param[in]  Status: new state of the usart0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart0_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,1,status);
        
}

/**
  * \brief      Enables or disables udma0 clock.
  * \param[in]  Status: new state of the udma0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void udma0_clk_en(ControlStatus status)
{
    misc_clk_cfg1(0x18100000,0x40,0,status);
        
}

/*******************************subm_clk_ctrl1 clk ctrl******************************************/

/*******************************subm_clk_ctrl2 clk ctrl******************************************/

/*******************************subm_clk_ctrl3 clk ctrl******************************************/

/*******************************subm_clk_ctrl4 clk ctrl******************************************/

/*******************************subm_clk_ctrl5 clk ctrl******************************************/

/*******************************subm_clk_ctrl6 clk ctrl******************************************/

/*******************************subm_clk_ctrl7 clk ctrl******************************************/

/**
  * \brief udma0 clk i div config
  * \param div_val: clk div number
  */
void udma0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x100,div_val,0,19);
}

/**
  * \brief usart0 clk i div config
  * \param div_val: clk div number
  */
void usart0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x104,div_val,0,19);
}

/**
  * \brief i3c0 clk i div config
  * \param div_val: clk div number
  */
void i3c0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x108,div_val,0,19);
}

/**
  * \brief qspi1 clk i div config
  * \param div_val: clk div number
  */
void qspi1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x10c,div_val,0,19);
}

/**
  * \brief qspi xip0 clk i div config
  * \param div_val: clk div number
  */
void qspi_xip0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x110,div_val,0,19);
}

/**
  * \brief sdio0 clk i div config
  * \param div_val: clk div number
  */
void sdio0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x114,div_val,0,19);
}

/**
  * \brief sdio data clk div config
  * \param div_val: clk div number
  */
void sdio_data_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x118,div_val,0,19);
}

/**
  * \brief sram0 clk i div config
  * \param div_val: clk div number
  */
void sram0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x11c,div_val,0,19);
}

/**
  * \brief test fab clk i div config
  * \param div_val: clk div number
  */
void test_fab_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x120,div_val,0,19);
}

/**
  * \brief iomux clk i div config
  * \param div_val: clk div number
  */
void iomux_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x18100000, 0x124,div_val,0,19);
}

void all_clk_div(uint32_t div_val)
{

#ifdef MISC_HAS_UDMA0_CLK_DIV
 udma0_clk_div(div_val);
#endif

#ifdef MISC_HAS_I3C0_CLK_DIV
 i3c0_clk_div(div_val);
#endif

#ifdef MISC_HAS_QSPI1_CLK_DIV
 qspi1_clk_div(div_val);
#endif

#ifdef MISC_HAS_QSPI_XIP0_CLK_DIV
 qspi_xip0_clk_div(div_val);
#endif

#ifdef MISC_HAS_SDIO0_CLK_DIV
 sdio0_clk_div(div_val);
#endif

#ifdef MISC_HAS_SDIO_DATA_CLK_DIV
 sdio_data_clk_div(div_val);
#endif

#ifdef MISC_HAS_SRAM0_CLK_DIV
 sram0_clk_div(div_val);
#endif

#ifdef MISC_HAS_IOMUX_CLK_DIV
 iomux_clk_div(div_val);
#endif

}

/**
  * \brief      Enables or disables pll_ctrl0_sys_clk_pll.
  * \param[in]  Status: new state of the pll_ctrl0_sys_clk_pll.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void pll_ctrl0_sys_clk_pll(ControlStatus status)
{
    if(status == ENABLE)
    REG32(0x18100000 + 0x78) |= (1<<24);
    else
    REG32(0x18100000 + 0x78) &= ~(1<<24);
}

/**
  * \brief      Get pll_ctrl0_sys_clk_pll lock status.
  * \retval SET: the specified PLL is LOCK.
  * \retval RESET: the specified PLL is UNLOCK.
  */

uint32_t pll_ctrl0_sys_clk_pll_lock(void)
{
    return REG32(0x18100000 + 0x78) & (1<<25);
}

/**
  * \brief      Enables or disables pll_ctrl1_sys_aclk_pll.
  * \param[in]  Status: new state of the pll_ctrl1_sys_aclk_pll.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void pll_ctrl1_sys_aclk_pll(ControlStatus status)
{
    if(status == ENABLE)
    REG32(0x18100000 + 0x7c) |= (1<<24);
    else
    REG32(0x18100000 + 0x7c) &= ~(1<<24);
}

/**
  * \brief      Get pll_ctrl1_sys_aclk_pll lock status.
  * \retval SET: the specified PLL is LOCK.
  * \retval RESET: the specified PLL is UNLOCK.
  */

uint32_t pll_ctrl1_sys_aclk_pll_lock(void)
{
    return REG32(0x18100000 + 0x7c) & (1<<25);
}

/**
  * \brief  cfg dbg_jtag_dwbypass.
  */
void dbg_jtag_dwbypass(ControlStatus status)
{
    if(status == ENABLE)
     REG32(0x18100000 + 0xc8c) |= (1<<2);
     else
      REG32(0x18100000 + 0xc8c) &= ~(1<<2);
}

/**
  * \brief get flag Bit: dbg_jtag_dwen.
  */
uint32_t dbg_jtag_dwen(void)
{
    return REG32(0x18100000 + 0xc8c) & BIT(1);
}
