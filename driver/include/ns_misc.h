#ifndef _NS_MISC_H
#define _NS_MISC_H
/*!
 * \file     ns_misc.h
 * \brief    This file contains all the functions prototypes for the MISC firmware
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"
        
/* ========== Register List ========== */
#define  RESET_CTRL0_OFS                        0x0UL
#define  RESET_CTRL1_OFS                        0x4UL
#define  RESET_CTRL2_OFS                        0x8UL
#define  RESET_CTRL3_OFS                        0xcUL
#define  SUBM_RESET_CTRL0_OFS                   0x20UL
#define  SUBM_RESET_CTRL1_OFS                   0x24UL
#define  SUBM_RESET_CTRL2_OFS                   0x28UL
#define  SUBM_RESET_CTRL3_OFS                   0x2cUL
#define  SUBM_RESET_CTRL4_OFS                   0x30UL
#define  SUBM_RESET_CTRL5_OFS                   0x34UL
#define  SUBM_RESET_CTRL6_OFS                   0x38UL
#define  SUBM_RESET_CTRL7_OFS                   0x3cUL
#define  SUBM_CLK_CTRL0_OFS                     0x40UL
#define  SUBM_CLK_CTRL1_OFS                     0x44UL
#define  SUBM_CLK_CTRL2_OFS                     0x48UL
#define  SUBM_CLK_CTRL3_OFS                     0x4cUL
#define  SUBM_CLK_CTRL4_OFS                     0x50UL
#define  SUBM_CLK_CTRL5_OFS                     0x54UL
#define  SUBM_CLK_CTRL6_OFS                     0x58UL
#define  SUBM_CLK_CTRL7_OFS                     0x5cUL
#define  CLK_CTRL_UDMA0_CLK_I_OFS               0x100UL
#define  CLK_CTRL_USART0_CLK_I_OFS              0x104UL
#define  CLK_CTRL_I3C0_CLK_I_OFS                0x108UL
#define  CLK_CTRL_QSPI1_CLK_I_OFS               0x10cUL
#define  CLK_CTRL_QSPI_XIP0_CLK_I_OFS           0x110UL
#define  CLK_CTRL_SDIO0_CLK_I_OFS               0x114UL
#define  CLK_CTRL_SDIO_DATA_CLK_OFS             0x118UL
#define  CLK_CTRL_SRAM0_CLK_I_OFS               0x11cUL
#define  CLK_CTRL_TEST_FAB_CLK_I_OFS            0x120UL
#define  CLK_CTRL_IOMUX_CLK_I_OFS               0x124UL
#define  XTL_CTRL_SYS_CLK_IN_OFS                0xa0UL
#define  PLL_CTRL_SYS_CLK_PLL_OFS               0x78UL
#define  PLL_CTRL_SYS_ACLK_PLL_OFS              0x7cUL
#define  XTL_CTRL_RTC_CLK_I_OFS                 0xa4UL
#define  MISC_CTRL3_OFS                         0xc8cUL
            
/* ==========reset_ctrl0========== */
                
#define RESET_CTRL0_CORE0_SYSRSTREQ                    BIT(2)                    /* core0 */ 
                
#define RESET_CTRL0_POR_RST_N                          BIT(1)                          /* por_rst */ 
                
#define RESET_CTRL0_SYS_RST_N_IN                       BIT(0)                       /* sys_rst_n */ 
            
/* ==========reset_ctrl1========== */
            
/* ==========reset_ctrl2========== */
            
/* ==========reset_ctrl3========== */
                
#define RESET_CTRL3_CLEAR_RESET_STATUS                 BIT(31)                 /* clear_reset */ 
            
/* ==========subm_reset_ctrl0========== */
                
#define SUBM_RESET_CTRL0_IOMUX_18                           BIT(18)                           /* iomux */ 
                
#define SUBM_RESET_CTRL0_IDU_17                             BIT(17)                             /* idu */ 
                
#define SUBM_RESET_CTRL0_ICB_SLV0_SYNC_16                   BIT(16)                   /* icb_slv0_sync */ 
                
#define SUBM_RESET_CTRL0_APB_SLV0_SYNC_15                   BIT(15)                   /* apb_slv0_sync */ 
                
#define SUBM_RESET_CTRL0_AHB_SLV0_SYNC_14                   BIT(14)                   /* ahb_slv0_sync */ 
                
#define SUBM_RESET_CTRL0_AXI_SLV0_SYNC_13                   BIT(13)                   /* axi_slv0_sync */ 
                
#define SUBM_RESET_CTRL0_ICB_MST0_SYNC_12                   BIT(12)                   /* icb_mst0_sync */ 
                
#define SUBM_RESET_CTRL0_AHB_MST0_SYNC_11                   BIT(11)                   /* ahb_mst0_sync */ 
                
#define SUBM_RESET_CTRL0_AXI_MST0_SYNC_10                   BIT(10)                   /* axi_mst0_sync */ 
                
#define SUBM_RESET_CTRL0_SRAM1_9                            BIT(9)                            /* sram1 */ 
                
#define SUBM_RESET_CTRL0_SRAM0_8                            BIT(8)                            /* sram0 */ 
                
#define SUBM_RESET_CTRL0_SDIO0_DMA_7                        BIT(7)                        /* sdio0_dma */ 
                
#define SUBM_RESET_CTRL0_SDIO0_6                            BIT(6)                            /* sdio0 */ 
                
#define SUBM_RESET_CTRL0_QSPI1_5                            BIT(5)                            /* qspi1 */ 
                
#define SUBM_RESET_CTRL0_QSPI_XIP0_4                        BIT(4)                        /* qspi_xip0 */ 
                
#define SUBM_RESET_CTRL0_I3C0_DMA_3                         BIT(3)                         /* i3c0_dma */ 
                
#define SUBM_RESET_CTRL0_I3C0_2                             BIT(2)                             /* i3c0 */ 
                
#define SUBM_RESET_CTRL0_USART0_1                           BIT(1)                           /* usart0 */ 
                
#define SUBM_RESET_CTRL0_UDMA0_0                            BIT(0)                            /* udma0 */ 
            
/* ==========subm_reset_ctrl1========== */
            
/* ==========subm_reset_ctrl2========== */
            
/* ==========subm_reset_ctrl3========== */
            
/* ==========subm_reset_ctrl4========== */
            
/* ==========subm_reset_ctrl5========== */
            
/* ==========subm_reset_ctrl6========== */
            
/* ==========subm_reset_ctrl7========== */
            
/* ==========subm_clk_ctrl0========== */
                
#define SUBM_CLK_CTRL0_IOMUX_18                           BIT(18)                           /* iomux */ 
                
#define SUBM_CLK_CTRL0_IDU_17                             BIT(17)                             /* idu */ 
                
#define SUBM_CLK_CTRL0_ICB_SLV0_SYNC_16                   BIT(16)                   /* icb_slv0_sync */ 
                
#define SUBM_CLK_CTRL0_APB_SLV0_SYNC_15                   BIT(15)                   /* apb_slv0_sync */ 
                
#define SUBM_CLK_CTRL0_AHB_SLV0_SYNC_14                   BIT(14)                   /* ahb_slv0_sync */ 
                
#define SUBM_CLK_CTRL0_AXI_SLV0_SYNC_13                   BIT(13)                   /* axi_slv0_sync */ 
                
#define SUBM_CLK_CTRL0_ICB_MST0_SYNC_12                   BIT(12)                   /* icb_mst0_sync */ 
                
#define SUBM_CLK_CTRL0_AHB_MST0_SYNC_11                   BIT(11)                   /* ahb_mst0_sync */ 
                
#define SUBM_CLK_CTRL0_AXI_MST0_SYNC_10                   BIT(10)                   /* axi_mst0_sync */ 
                
#define SUBM_CLK_CTRL0_SRAM1_9                            BIT(9)                            /* sram1 */ 
                
#define SUBM_CLK_CTRL0_SRAM0_8                            BIT(8)                            /* sram0 */ 
                
#define SUBM_CLK_CTRL0_SDIO0_DMA_7                        BIT(7)                        /* sdio0_dma */ 
                
#define SUBM_CLK_CTRL0_SDIO0_6                            BIT(6)                            /* sdio0 */ 
                
#define SUBM_CLK_CTRL0_QSPI1_5                            BIT(5)                            /* qspi1 */ 
                
#define SUBM_CLK_CTRL0_QSPI_XIP0_4                        BIT(4)                        /* qspi_xip0 */ 
                
#define SUBM_CLK_CTRL0_I3C0_DMA_3                         BIT(3)                         /* i3c0_dma */ 
                
#define SUBM_CLK_CTRL0_I3C0_2                             BIT(2)                             /* i3c0 */ 
                
#define SUBM_CLK_CTRL0_USART0_1                           BIT(1)                           /* usart0 */ 
                
#define SUBM_CLK_CTRL0_UDMA0_0                            BIT(0)                            /* udma0 */ 
            
/* ==========subm_clk_ctrl1========== */
            
/* ==========subm_clk_ctrl2========== */
            
/* ==========subm_clk_ctrl3========== */
            
/* ==========subm_clk_ctrl4========== */
            
/* ==========subm_clk_ctrl5========== */
            
/* ==========subm_clk_ctrl6========== */
            
/* ==========subm_clk_ctrl7========== */
            
/* ==========clk_ctrl_udma0_clk_i========== */
                
#define CLK_CTRL_UDMA0_CLK_I_UDMA0_CLK_I_DIV                    BITS(0,19)                    /* udma0_clk_i */
            
/* ==========clk_ctrl_usart0_clk_i========== */
                
#define CLK_CTRL_USART0_CLK_I_USART0_CLK_I_DIV                   BITS(0,19)                   /* usart0_clk_i */
            
/* ==========clk_ctrl_i3c0_clk_i========== */
                
#define CLK_CTRL_I3C0_CLK_I_I3C0_CLK_I_DIV                     BITS(0,19)                     /* i3c0_clk_i */
            
/* ==========clk_ctrl_qspi1_clk_i========== */
                
#define CLK_CTRL_QSPI1_CLK_I_QSPI1_CLK_I_DIV                    BITS(0,19)                    /* qspi1_clk_i */
            
/* ==========clk_ctrl_qspi_xip0_clk_i========== */
                
#define CLK_CTRL_QSPI_XIP0_CLK_I_QSPI_XIP0_CLK_I_DIV                BITS(0,19)                /* qspi_xip0_clk_i */
            
/* ==========clk_ctrl_sdio0_clk_i========== */
                
#define CLK_CTRL_SDIO0_CLK_I_SDIO0_CLK_I_DIV                    BITS(0,19)                    /* sdio0_clk_i */
            
/* ==========clk_ctrl_sdio_data_clk========== */
                
#define CLK_CTRL_SDIO_DATA_CLK_SDIO_DATA_CLK_DIV                  BITS(0,19)                  /* sdio_data_clk */
            
/* ==========clk_ctrl_sram0_clk_i========== */
                
#define CLK_CTRL_SRAM0_CLK_I_SRAM0_CLK_I_DIV                    BITS(0,19)                    /* sram0_clk_i */
            
/* ==========clk_ctrl_test_fab_clk_i========== */
                
#define CLK_CTRL_TEST_FAB_CLK_I_TEST_FAB_CLK_I_DIV                 BITS(0,19)                 /* test_fab_clk_i */
            
/* ==========clk_ctrl_iomux_clk_i========== */
                
#define CLK_CTRL_IOMUX_CLK_I_IOMUX_CLK_I_DIV                    BITS(0,19)                    /* iomux_clk_i */
            
/* ==========xtl_ctrl_sys_clk_in========== */
                
#define XTL_CTRL_SYS_CLK_IN_RESTART                            BIT(0)                            /* restart */ 
            
/* ==========pll_ctrl_sys_clk_pll========== */
                
#define PLL_CTRL_SYS_CLK_PLL_N                                  BITS(0,7)                                  /* N */
                
#define PLL_CTRL_SYS_CLK_PLL_M                                  BITS(8,17)                                  /* M */
                
#define PLL_CTRL_SYS_CLK_PLL_OD                                 BITS(18,23)                                 /* od */
                
#define PLL_CTRL_SYS_CLK_PLL_BP                                 BIT(24)                                 /* bp */ 
                
#define PLL_CTRL_SYS_CLK_PLL_LOCK                               BIT(25)                               /* lock */ 
            
/* ==========pll_ctrl_sys_aclk_pll========== */
                
#define PLL_CTRL_SYS_ACLK_PLL_N                                  BITS(0,7)                                  /* N */
                
#define PLL_CTRL_SYS_ACLK_PLL_M                                  BITS(8,17)                                  /* M */
                
#define PLL_CTRL_SYS_ACLK_PLL_OD                                 BITS(18,23)                                 /* od */
                
#define PLL_CTRL_SYS_ACLK_PLL_BP                                 BIT(24)                                 /* bp */ 
                
#define PLL_CTRL_SYS_ACLK_PLL_LOCK                               BIT(25)                               /* lock */ 
            
/* ==========xtl_ctrl_rtc_clk_i========== */
                
#define XTL_CTRL_RTC_CLK_I_RESTART                            BIT(0)                            /* restart */ 
            
/* ==========misc_ctrl3========== */
                
#define MISC_CTRL3_DBG_JTAG_DWBYPASS                  BIT(2)                  /* dbg_jtag */ 
                
#define MISC_CTRL3_DBG_JTAG_DWEN                      BIT(1)                      /* dbg_jtag */ 

/* ==========reset_ctrl0========== */
                
/* ==========reset_ctrl1========== */
            
/* ==========reset_ctrl2========== */
            
/* ==========reset_ctrl3========== */
                
/* ==========subm_reset_ctrl0========== */
                
#define MISC_HAS_IOMUX_RST
                
#define MISC_HAS_IDU_RST
                
#define MISC_HAS_ICB_SLV0_SYNC_RST
                
#define MISC_HAS_APB_SLV0_SYNC_RST
                
#define MISC_HAS_AHB_SLV0_SYNC_RST
                
#define MISC_HAS_AXI_SLV0_SYNC_RST
                
#define MISC_HAS_ICB_MST0_SYNC_RST
                
#define MISC_HAS_AHB_MST0_SYNC_RST
                
#define MISC_HAS_AXI_MST0_SYNC_RST
                
#define MISC_HAS_SRAM1_RST
                
#define MISC_HAS_SRAM0_RST
                
#define MISC_HAS_SDIO0_DMA_RST
                
#define MISC_HAS_SDIO0_RST
                
#define MISC_HAS_QSPI1_RST
                
#define MISC_HAS_QSPI_XIP0_RST
                
#define MISC_HAS_I3C0_DMA_RST
                
#define MISC_HAS_I3C0_RST
                
#define MISC_HAS_USART0_RST
                
#define MISC_HAS_UDMA0_RST
            
/* ==========subm_reset_ctrl1========== */
            
/* ==========subm_reset_ctrl2========== */
            
/* ==========subm_reset_ctrl3========== */
            
/* ==========subm_reset_ctrl4========== */
            
/* ==========subm_reset_ctrl5========== */
            
/* ==========subm_reset_ctrl6========== */
            
/* ==========subm_reset_ctrl7========== */
            
/* ==========subm_clk_ctrl0========== */
                
#define MISC_HAS_IOMUX_HAS_CLK
                
#define MISC_HAS_IDU_HAS_CLK
                
#define MISC_HAS_ICB_SLV0_SYNC_HAS_CLK
                
#define MISC_HAS_APB_SLV0_SYNC_HAS_CLK
                
#define MISC_HAS_AHB_SLV0_SYNC_HAS_CLK
                
#define MISC_HAS_AXI_SLV0_SYNC_HAS_CLK
                
#define MISC_HAS_ICB_MST0_SYNC_HAS_CLK
                
#define MISC_HAS_AHB_MST0_SYNC_HAS_CLK
                
#define MISC_HAS_AXI_MST0_SYNC_HAS_CLK
                
#define MISC_HAS_SRAM1_HAS_CLK
                
#define MISC_HAS_SRAM0_HAS_CLK
                
#define MISC_HAS_SDIO0_DMA_HAS_CLK
                
#define MISC_HAS_SDIO0_HAS_CLK
                
#define MISC_HAS_QSPI1_HAS_CLK
                
#define MISC_HAS_QSPI_XIP0_HAS_CLK
                
#define MISC_HAS_I3C0_DMA_HAS_CLK
                
#define MISC_HAS_I3C0_HAS_CLK
                
#define MISC_HAS_USART0_HAS_CLK
                
#define MISC_HAS_UDMA0_HAS_CLK
            
/* ==========subm_clk_ctrl1========== */
            
/* ==========subm_clk_ctrl2========== */
            
/* ==========subm_clk_ctrl3========== */
            
/* ==========subm_clk_ctrl4========== */
            
/* ==========subm_clk_ctrl5========== */
            
/* ==========subm_clk_ctrl6========== */
            
/* ==========subm_clk_ctrl7========== */
            
/* ==========clk_ctrl_udma0_clk_i========== */
                
#define MISC_HAS_UDMA0_CLK_DIV
            
/* ==========clk_ctrl_usart0_clk_i========== */
                
#define MISC_HAS_USART0_CLK_DIV
            
/* ==========clk_ctrl_i3c0_clk_i========== */
                
#define MISC_HAS_I3C0_CLK_DIV
            
/* ==========clk_ctrl_qspi1_clk_i========== */
                
#define MISC_HAS_QSPI1_CLK_DIV
            
/* ==========clk_ctrl_qspi_xip0_clk_i========== */
                
#define MISC_HAS_QSPI_XIP0_CLK_DIV
            
/* ==========clk_ctrl_sdio0_clk_i========== */
                
#define MISC_HAS_SDIO0_CLK_DIV
            
/* ==========clk_ctrl_sdio_data_clk========== */
                
#define MISC_HAS_SDIO_DATA_CLK_DIV
            
/* ==========clk_ctrl_sram0_clk_i========== */
                
#define MISC_HAS_SRAM0_CLK_DIV
            
/* ==========clk_ctrl_test_fab_clk_i========== */
                
#define MISC_HAS_TEST_FAB_CLK_DIV
            
/* ==========clk_ctrl_iomux_clk_i========== */
                
#define MISC_HAS_IOMUX_CLK_DIV
            
/* ==========xtl_ctrl_sys_clk_in========== */
                
/* ==========pll_ctrl_sys_clk_pll========== */
                
/* ==========pll_ctrl_sys_aclk_pll========== */
                
/* ==========xtl_ctrl_rtc_clk_i========== */
                
/* ==========misc_ctrl3========== */
                
void iomux_set_rst(ControlStatus status);
                
void idu_set_rst(ControlStatus status);
                
void icb_slv0_sync_set_rst(ControlStatus status);
                
void apb_slv0_sync_set_rst(ControlStatus status);
                
void ahb_slv0_sync_set_rst(ControlStatus status);
                
void axi_slv0_sync_set_rst(ControlStatus status);
                
void icb_mst0_sync_set_rst(ControlStatus status);
                
void ahb_mst0_sync_set_rst(ControlStatus status);
                
void axi_mst0_sync_set_rst(ControlStatus status);
                
void sram1_set_rst(ControlStatus status);
                
void sram0_set_rst(ControlStatus status);
                
void sdio0_dma_set_rst(ControlStatus status);
                
void sdio0_set_rst(ControlStatus status);
                
void qspi1_set_rst(ControlStatus status);
                
void qspi_xip0_set_rst(ControlStatus status);
                
void i3c0_dma_set_rst(ControlStatus status);
                
void i3c0_set_rst(ControlStatus status);
                
void usart0_set_rst(ControlStatus status);
                
void udma0_set_rst(ControlStatus status);
            
void iomux_clk_en(ControlStatus status);
                
void idu_clk_en(ControlStatus status);
                
void icb_slv0_sync_clk_en(ControlStatus status);
                
void apb_slv0_sync_clk_en(ControlStatus status);
                
void ahb_slv0_sync_clk_en(ControlStatus status);
                
void axi_slv0_sync_clk_en(ControlStatus status);
                
void icb_mst0_sync_clk_en(ControlStatus status);
                
void ahb_mst0_sync_clk_en(ControlStatus status);
                
void axi_mst0_sync_clk_en(ControlStatus status);
                
void sram1_clk_en(ControlStatus status);
                
void sram0_clk_en(ControlStatus status);
                
void sdio0_dma_clk_en(ControlStatus status);
                
void sdio0_clk_en(ControlStatus status);
                
void qspi1_clk_en(ControlStatus status);
                
void qspi_xip0_clk_en(ControlStatus status);
                
void i3c0_dma_clk_en(ControlStatus status);
                
void i3c0_clk_en(ControlStatus status);
                
void usart0_clk_en(ControlStatus status);
                
void udma0_clk_en(ControlStatus status);
            
void udma0_clk_div(uint32_t div_val) ;
            
void usart0_clk_div(uint32_t div_val) ;
            
void i3c0_clk_div(uint32_t div_val) ;
            
void qspi1_clk_div(uint32_t div_val) ;
            
void qspi_xip0_clk_div(uint32_t div_val) ;
            
void sdio0_clk_div(uint32_t div_val) ;
            
void sdio_data_clk_div(uint32_t div_val) ;
            
void sram0_clk_div(uint32_t div_val) ;
            
void test_fab_clk_div(uint32_t div_val) ;
            
void iomux_clk_div(uint32_t div_val) ;
            
#define Get_status_CORE0_SYSRSTREQ
uint32_t Get_status_core0_sysrstreq(void);
#define Get_status_POR_RST_N
uint32_t Get_status_por_rst_n(void);
#define Get_status_SYS_RST_N_IN
uint32_t Get_status_sys_rst_n_in(void);
            
#define CLEAR_RESET_STATUS
void clear_reset_status(void);
            
void pll_ctrl0_sys_clk_pll(ControlStatus status);
uint32_t pll_ctrl0_sys_clk_pll_lock(void);
            
void pll_ctrl1_sys_aclk_pll(ControlStatus status);
uint32_t pll_ctrl1_sys_aclk_pll_lock(void);
            
#define DBG_JTAG_DWBYPASS
void dbg_jtag_dwbypass(ControlStatus status);
                    
#define DBG_JTAG_DWEN
uint32_t dbg_jtag_dwen(void);                        

void all_clk_div(uint32_t div_val);

#ifdef __cplusplus
}
#endif

#endif

/* _NS_MISC_H */
