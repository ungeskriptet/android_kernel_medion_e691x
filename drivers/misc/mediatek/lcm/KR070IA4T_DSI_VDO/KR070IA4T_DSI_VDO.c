#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#include <platform/upmu_common.h>
#include "ddp_hal.h"
#else
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of_gpio.h>
#include <mt-plat/mt_gpio.h>
#include <mt-plat/upmu_common.h>
#endif

#include "lcm_drv.h"
#include "KR070IA4T_DSI_VDO.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

#define REGFLAG_DELAY             							0xFE
#define REGFLAG_END_OF_TABLE      							0xFD   // END OF REGISTERS MARKER



// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static LCM_UTIL_FUNCS lcm_util = {
    .set_gpio_out = NULL,
};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))

//#define GPIO_OUT_ONE 1
struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};
//#define GPIO_OUT_ZERO 0

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    



static unsigned int GPIO_LCD_RST_PIN = 0;
static unsigned int GPIO_LCD_DRV_EN_PIN = 0;

static struct LCM_setting_table lcm_initialization_setting[] =
{                                                       
#if 0
	{0xff, 3, {0x98,0x81,0x03}},
	{0x01, 1, {0x00}},
	{0x02, 1, {0x00}},
	{0x03, 1, {0x55}},
	{0x04, 1, {0x13}},
	{0x05, 1, {0x00}},
	{0x06, 1, {0x06}},//{0x66}},
	{0x07, 1, {0x01}},
	{0x08, 1, {0x00}},
	{0x09, 1, {0x01}},
	{0x0a, 1, {0x01}},
	{0x0b, 1, {0x00}},
	{0x0c, 1, {0x00}},
	{0x0d, 1, {0x00}},
	{0x0e, 1, {0x00}},
	{0x0f, 1, {0x18}},
	{0x10, 1, {0x18}},
	{0x11, 1, {0x00}},
	{0x12, 1, {0x00}},
	{0x13, 1, {0x00}},
	{0x14, 1, {0x00}},
	{0x15, 1, {0x00}},
	{0x16, 1, {0x00}},
	{0x17, 1, {0x00}},
	{0x18, 1, {0x00}},
	{0x19, 1, {0x00}},
	{0x1a, 1, {0x00}},
	{0x1b, 1, {0x00}},
	{0x1c, 1, {0x00}},
	{0x1d, 1, {0x00}},
	{0x1e, 1, {0x44}},
	{0x1f, 1, {0x80}},
	{0x20, 1, {0x02}},
	{0x21, 1, {0x03}},
	{0x22, 1, {0x00}},
	{0x23, 1, {0x00}},
	{0x24, 1, {0x00}},
	{0x25, 1, {0x00}},
	{0x26, 1, {0x00}},
	{0x27, 1, {0x00}},
	{0x28, 1, {0x33}},
	{0x29, 1, {0x03}},
	{0x2a, 1, {0x00}},
	{0x2b, 1, {0x00}},
	{0x2c, 1, {0x00}},
	{0x2d, 1, {0x00}},
	{0x2e, 1, {0x00}},
	{0x2f, 1, {0x00}},
	{0x30, 1, {0x00}},
	{0x31, 1, {0x00}},
	{0x32, 1, {0x00}},
	{0x33, 1, {0x00}},
	{0x34, 1, {0x04}},
	{0x35, 1, {0x00}},
	{0x36, 1, {0x00}},
	{0x37, 1, {0x00}},
	{0x38, 1, {0x01}},
	{0x39, 1, {0x00}},
	{0x3a, 1, {0x00}},
	{0x3b, 1, {0x00}},
	{0x3c, 1, {0x00}},
	{0x3d, 1, {0x00}},
	{0x3e, 1, {0x00}},
	{0x3f, 1, {0x00}},
	{0x40, 1, {0x00}},
	{0x41, 1, {0x00}},
	{0x42, 1, {0x00}},
	{0x43, 1, {0x00}},
	{0x44, 1, {0x00}},
	{0x50, 1, {0x01}},
	{0x51, 1, {0x23}},
	{0x52, 1, {0x45}},
	{0x53, 1, {0x67}},
	{0x54, 1, {0x89}},
	{0x55, 1, {0xab}},
	{0x56, 1, {0x01}},
	{0x57, 1, {0x23}},
	{0x58, 1, {0x45}},
	{0x59, 1, {0x67}},
	{0x5a, 1, {0x89}},
	{0x5b, 1, {0xab}},
	{0x5c, 1, {0xcd}},
	{0x5d, 1, {0xef}},
	{0x5e, 1, {0x11}},
	{0x5f, 1, {0x14}},
	{0x60, 1, {0x15}},
	{0x61, 1, {0x0f}},
	{0x62, 1, {0x0d}},
	{0x63, 1, {0x0e}},
	{0x64, 1, {0x0c}},
	{0x65, 1, {0x06}},
	{0x66, 1, {0x02}},
	{0x66, 1, {0x02}},
	{0x67, 1, {0x02}},
	{0x68, 1, {0x02}},
	{0x69, 1, {0x02}},
	{0x6a, 1, {0x02}},
	{0x6b, 1, {0x02}},
	{0x6c, 1, {0x3A}},//{0x02}},
	{0x6d, 1, {0x02}},
	{0x6e, 1, {0x02}},
	{0x6f, 1, {0x02}},
	{0x70, 1, {0x02}},
	{0x71, 1, {0x00}},
	{0x72, 1, {0x01}},
	{0x73, 1, {0x08}},
	{0x74, 1, {0x02}},
	{0x75, 1, {0x14}},
	{0x76, 1, {0x15}},
	{0x77, 1, {0x0f}},
	{0x78, 1, {0x0d}},
	{0x79, 1, {0x0e}},
	{0x7a, 1, {0x0c}},
	{0x7b, 1, {0x08}},
	{0x7c, 1, {0x02}},
	{0x7d, 1, {0x02}},
	{0x7e, 1, {0x02}},
	{0x7f, 1, {0x02}},
	{0x80, 1, {0x02}},
	{0x81, 1, {0x02}},
	{0x82, 1, {0x02}},
	{0x83, 1, {0x02}},
	{0x84, 1, {0x02}},
	{0x85, 1, {0x02}},
	{0x86, 1, {0x02}},
	{0x87, 1, {0x00}},
	{0x88, 1, {0x01}},
	{0x89, 1, {0x06}},
	{0x8a, 1, {0x02}},
	{0xff, 3, {0x98,0x81,0x04}},
	{0x00, 1, {0x00}},
	{0x6c, 1, {0x15}},
	{0x6e, 1, {0x2b}},
	{0x6f, 1, {0x55}},//{0x33}},
	{0x3a, 1, {0xa4}},
	{0x8d, 1, {0x15}},
	{0x87, 1, {0xba}},
	{0x26, 1, {0x76}},
	{0xb2, 1, {0xd1}},
	{0xff, 3, {0x98,0x81,0x01}},
	{0x22, 1, {0x0a}},
	{0x53, 1, {0x6d}},
	{0x55, 1, {0x77}},
	{0x50, 1, {0xa7}},
	{0x51, 1, {0xa6}},
	{0x22, 1, {0x0a}},
	{0x53, 1, {0x6d}},
	{0x55, 1, {0x77}},
	{0x50, 1, {0xa7}},
	{0x51, 1, {0xa6}},
	{0x31, 1, {0x00}},
	{0x60, 1, {0x20}},
	{0xa0, 1, {0x08}},
	{0xa1, 1, {0x2e}},
	{0xa2, 1, {0x3c}},
	{0xa3, 1, {0x14}},
	{0xa4, 1, {0x17}},
	{0xa5, 1, {0x2b}},
	{0xa6, 1, {0x1c}},
	{0xa7, 1, {0x1e}},
	{0xa8, 1, {0xa5}},
	{0xa9, 1, {0x1c}},
	{0xaa, 1, {0x29}},
	{0xab, 1, {0x94}},
	{0xac, 1, {0x1d}},
	{0xad, 1, {0x1b}},
	{0xae, 1, {0x4f}},
	{0xaf, 1, {0x23}},
	{0xb0, 1, {0x29}},
	{0xb1, 1, {0x56}},
	{0xb2, 1, {0x66}},
	{0xb3, 1, {0x39}},
	{0xc0, 1, {0x08}},
	{0xc1, 1, {0x2c}},
	{0xc2, 1, {0x3c}},
	{0xc3, 1, {0x15}},
	{0xc4, 1, {0x17}},
	{0xc5, 1, {0x29}},
	{0xc6, 1, {0x1d}},
	{0xc7, 1, {0x1d}},
	{0xc8, 1, {0xa6}},
	{0xc9, 1, {0x1d}},
	{0xca, 1, {0x29}},
	{0xcb, 1, {0x94}},
	{0xcc, 1, {0x1d}},
	{0xcd, 1, {0x1b}},
	{0xce, 1, {0x4f}},
	{0xcf, 1, {0x24}},
	{0xd0, 1, {0x29}},
	{0xd1, 1, {0x53}},
	{0xd2, 1, {0x66}},
	{0xd3, 1, {0x39}},
	{0xff, 3, {0x98,0x81,0x00}},

	//======Normal Display======
	{0x11, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},

	{0x29, 1, {0x00}},                                        
	{REGFLAG_DELAY, 120, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
#else //20160607 Optimize gamma
	{0xff, 3, {0x98,0x81,0x03}},
	{0x01, 1, {0x00}},
	{0x02, 1, {0x00}},
	{0x03, 1, {0x55}},
	{0x04, 1, {0x13}},
	{0x05, 1, {0x00}},
	{0x06, 1, {0x06}},//{0x66}},
	{0x07, 1, {0x01}},
	{0x08, 1, {0x00}},
	{0x09, 1, {0x01}},
	{0x0a, 1, {0x01}},
	{0x0b, 1, {0x00}},
	{0x0c, 1, {0x00}},
	{0x0d, 1, {0x00}},
	{0x0e, 1, {0x00}},
	{0x0f, 1, {0x18}},
	{0x10, 1, {0x18}},
	{0x11, 1, {0x00}},
	{0x12, 1, {0x00}},
	{0x13, 1, {0x00}},
	{0x14, 1, {0x00}},
	{0x15, 1, {0x00}},
	{0x16, 1, {0x00}},
	{0x17, 1, {0x00}},
	{0x18, 1, {0x00}},
	{0x19, 1, {0x00}},
	{0x1a, 1, {0x00}},
	{0x1b, 1, {0x00}},
	{0x1c, 1, {0x00}},
	{0x1d, 1, {0x00}},
	{0x1e, 1, {0x44}},
	{0x1f, 1, {0x80}},
	{0x20, 1, {0x02}},
	{0x21, 1, {0x03}},
	{0x22, 1, {0x00}},
	{0x23, 1, {0x00}},
	{0x24, 1, {0x00}},
	{0x25, 1, {0x00}},
	{0x26, 1, {0x00}},
	{0x27, 1, {0x00}},
	{0x28, 1, {0x33}},
	{0x29, 1, {0x03}},
	{0x2a, 1, {0x00}},
	{0x2b, 1, {0x00}},
	{0x2c, 1, {0x00}},
	{0x2d, 1, {0x00}},
	{0x2e, 1, {0x00}},
	{0x2f, 1, {0x00}},
	{0x30, 1, {0x00}},
	{0x31, 1, {0x00}},
	{0x32, 1, {0x00}},
	{0x33, 1, {0x00}},
	{0x34, 1, {0x04}},
	{0x35, 1, {0x00}},
	{0x36, 1, {0x00}},
	{0x37, 1, {0x00}},
	{0x38, 1, {0x01}},
	{0x39, 1, {0x00}},
	{0x3a, 1, {0x00}},
	{0x3b, 1, {0x00}},
	{0x3c, 1, {0x00}},
	{0x3d, 1, {0x00}},
	{0x3e, 1, {0x00}},
	{0x3f, 1, {0x00}},
	{0x40, 1, {0x00}},
	{0x41, 1, {0x00}},
	{0x42, 1, {0x00}},
	{0x43, 1, {0x00}},
	{0x44, 1, {0x00}},
	{0x50, 1, {0x01}},
	{0x51, 1, {0x23}},
	{0x52, 1, {0x45}},
	{0x53, 1, {0x67}},
	{0x54, 1, {0x89}},
	{0x55, 1, {0xab}},
	{0x56, 1, {0x01}},
	{0x57, 1, {0x23}},
	{0x58, 1, {0x45}},
	{0x59, 1, {0x67}},
	{0x5a, 1, {0x89}},
	{0x5b, 1, {0xab}},
	{0x5c, 1, {0xcd}},
	{0x5d, 1, {0xef}},
	{0x5e, 1, {0x11}},
	{0x5f, 1, {0x14}},
	{0x60, 1, {0x15}},
	{0x61, 1, {0x0f}},
	{0x62, 1, {0x0d}},
	{0x63, 1, {0x0e}},
	{0x64, 1, {0x0c}},
	{0x65, 1, {0x06}},
	{0x66, 1, {0x02}},
	{0x66, 1, {0x02}},
	{0x67, 1, {0x02}},
	{0x68, 1, {0x02}},
	{0x69, 1, {0x02}},
	{0x6a, 1, {0x02}},
	{0x6b, 1, {0x02}},
	{0x6c, 1, {0x02}},//{0x3A}},
	{0x6d, 1, {0x02}},
	{0x6e, 1, {0x02}},
	{0x6f, 1, {0x02}},
	{0x70, 1, {0x02}},
	{0x71, 1, {0x00}},
	{0x72, 1, {0x01}},
	{0x73, 1, {0x08}},
	{0x74, 1, {0x02}},
	{0x75, 1, {0x14}},
	{0x76, 1, {0x15}},
	{0x77, 1, {0x0f}},
	{0x78, 1, {0x0d}},
	{0x79, 1, {0x0e}},
	{0x7a, 1, {0x0c}},
	{0x7b, 1, {0x08}},
	{0x7c, 1, {0x02}},
	{0x7d, 1, {0x02}},
	{0x7e, 1, {0x02}},
	{0x7f, 1, {0x02}},
	{0x80, 1, {0x02}},
	{0x81, 1, {0x02}},
	{0x82, 1, {0x02}},
	{0x83, 1, {0x02}},
	{0x84, 1, {0x02}},
	{0x85, 1, {0x02}},
	{0x86, 1, {0x02}},
	{0x87, 1, {0x00}},
	{0x88, 1, {0x01}},
	{0x89, 1, {0x06}},
	{0x8a, 1, {0x02}},
	{0xff, 3, {0x98,0x81,0x04}},
	{0x00, 1, {0x00}},
	{0x6c, 1, {0x15}},
	{0x6e, 1, {0x6b}},//{0x2b}}, //[M]Fix some module can not work sometimes.ArthurLin.20160823
	{0x6f, 1, {0x55}},//{0x33}},
	{0x3a, 1, {0xa4}},
	{0x8d, 1, {0x15}},
	{0x87, 1, {0xba}},
	{0x26, 1, {0x76}},
	{0xb2, 1, {0xd1}},
	{0xff, 3, {0x98,0x81,0x01}},
	{0x22, 1, {0x0a}},
	{0x53, 1, {0x72}},//{0x6d}},
	{0x55, 1, {0x77}},
	{0x50, 1, {0xa6}},//{0xa7}},
	{0x51, 1, {0xa6}},
	{0x31, 1, {0x00}},
	{0x60, 1, {0x20}},
	{0xa0, 1, {0x08}},
	{0xa1, 1, {0x1a}},
	{0xa2, 1, {0x2a}},//{0x3c}},
	{0xa3, 1, {0x14}},
	{0xa4, 1, {0x17}},
	{0xa5, 1, {0x2b}},
	{0xa6, 1, {0x1d}},
	{0xa7, 1, {0x20}},//{0x1e}},
	{0xa8, 1, {0x9d}},//{0xa5}},
	{0xa9, 1, {0x1c}},
	{0xaa, 1, {0x29}},
	{0xab, 1, {0x8f}},//{0x94}},
	{0xac, 1, {0x20}},//{0x1d}},
	{0xad, 1, {0x1f}},//{0x1b}},
	{0xae, 1, {0x4f}},
	{0xaf, 1, {0x23}},
	{0xb0, 1, {0x29}},
	{0xb1, 1, {0x56}},
	{0xb2, 1, {0x66}},
	{0xb3, 1, {0x39}},
	{0xc0, 1, {0x08}},
	{0xc1, 1, {0x1a}},//{0x2c}},
	{0xc2, 1, {0x2a}},//{0x3c}},
	{0xc3, 1, {0x15}},
	{0xc4, 1, {0x17}},
	{0xc5, 1, {0x2b}},//{0x29}},
	{0xc6, 1, {0x1d}},
	{0xc7, 1, {0x20}},//{0x1d}},
	{0xc8, 1, {0x9d}},//{0xa6}},
	{0xc9, 1, {0x1d}},
	{0xca, 1, {0x29}},
	{0xcb, 1, {0x8f}},//{0x94}},
	{0xcc, 1, {0x20}},//{0x1d}},
	{0xcd, 1, {0x1f}},//{0x1b}},
	{0xce, 1, {0x4f}},
	{0xcf, 1, {0x24}},
	{0xd0, 1, {0x29}},
	{0xd1, 1, {0x56}},//{0x53}},
	{0xd2, 1, {0x66}},
	{0xd3, 1, {0x39}},
	{0xff, 3, {0x98,0x81,0x00}},

	//======Normal Display======
	{0x11, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},

	{0x29, 1, {0x00}},                                        
	{REGFLAG_DELAY, 20, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}

#endif
};                                                      

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;
	unsigned cmd;

    for(i = 0; i < count; i++) {
		
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}

static void init_lcm_registers(void)
{
    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

//#define LCD_RST_VALUE GPIO_OUT_ONE
static void lcm_get_gpio_inform(void)
{
	static struct device_node *node;
	printk("[Kernel/LCM] %s enter\n", __func__);

	node = of_find_compatible_node(NULL, NULL, "mediatek,mt6580-lcm");

	GPIO_LCD_RST_PIN = of_get_named_gpio(node, "lcm_reset_gpio", 0);
	if(GPIO_LCD_RST_PIN <= 0)
		printk("[Kernel/LCM] %s of_get_named_gpio lcm_reset_gpio failed \n", __func__);
		

	GPIO_LCD_DRV_EN_PIN = of_get_named_gpio(node, "lcm_ext_ldo_gpio", 0);
	if(GPIO_LCD_DRV_EN_PIN <= 0)
		printk("[Kernel/LCM] %s of_get_named_gpio lcm_ext_ldo_gpio failed \n", __func__);


	gpio_direction_output(GPIO_LCD_RST_PIN, GPIO_OUT_ZERO);
	gpio_direction_output(GPIO_LCD_DRV_EN_PIN, GPIO_OUT_ZERO);
}

/*
static void lcm_set_gpio_output(unsigned int GPIO, unsigned int output)
{
	if (GPIO == 0xFFFFFFFF) {
#ifdef BUILD_LK
		printf("[LK/LCM] GPIO_LCD_PWR =   0x%x\n", GPIO_LCD_PWR);
		printf("[LK/LCM] GPIO_LCD_RST =   0x%x\n", GPIO_LCD_RST);
#elif (defined BUILD_UBOOT)
#else
#endif
		return;
	}

#ifdef BUILD_LK
   mt_set_gpio_mode(GPIO, GPIO_MODE_00);
   mt_set_gpio_dir(GPIO, GPIO_DIR_OUT);
   mt_set_gpio_out(GPIO, (output>0)? GPIO_OUT_ONE: GPIO_OUT_ZERO);
#else
	gpio_set_value(GPIO, (output > 0) ? GPIO_OUT_ONE : GPIO_OUT_ZERO);
#endif
}
*/

static void lcm_reset(unsigned char enabled)
{
	printk("[Kernel/LCM] lcm_reset(%d) enter\n", enabled);
	if(GPIO_LCD_RST_PIN == 0 || GPIO_LCD_DRV_EN_PIN == 0){
		lcm_get_gpio_inform();
	}
 
	if(enabled)
	{	
		printk("[Kernel/LCM] %s to do pin(%d) reset\n", __func__, GPIO_LCD_RST_PIN);
		gpio_set_value(GPIO_LCD_RST_PIN, GPIO_OUT_ZERO);
		MDELAY(10);
		gpio_set_value(GPIO_LCD_RST_PIN, GPIO_OUT_ONE);
		MDELAY(10);
		gpio_set_value(GPIO_LCD_RST_PIN, GPIO_OUT_ZERO);
	}
	else{
		gpio_set_value(GPIO_LCD_RST_PIN, GPIO_OUT_ONE); 		
		MDELAY(1);
	}
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		params->physical_width = 89;
		params->physical_height = 152;
		
		params->dsi.mode   = SYNC_EVENT_VDO_MODE;		//SYNC_EVENT_VDO_MODE;
	
		// DSI
		/* Command mode setting */
		// Three lane or Four lane
		params->dsi.LANE_NUM								= LCM_THREE_LANE;
		
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 0;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		params->dsi.word_count=FRAME_WIDTH*3;
		
		params->dsi.vertical_sync_active				= 6;
		params->dsi.vertical_backporch					= 3;
		params->dsi.vertical_frontporch					= 5;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active			= 6;
		params->dsi.horizontal_backporch				= 48;
		params->dsi.horizontal_frontporch				= 16;
		params->dsi.horizontal_active_pixel			= FRAME_WIDTH;
		
		params->dsi.ssc_disable = 0;
		params->dsi.ssc_range = 5;
		params->dsi.PLL_CLOCK = 285;
		//params->dsi.cont_clock= 1;
		
		// Non-continuous clock
		params->dsi.cont_clock=0;
		//params->dsi.noncont_clock=1;
		params->dsi.clk_lp_per_line_enable=1;
}

static void lcm_init_power(void)
{
	if(GPIO_LCD_RST_PIN == 0 || GPIO_LCD_DRV_EN_PIN == 0){
		lcm_get_gpio_inform();
	}

	//VDD use internal ldo power on ->VGP1_PMU 2.8V
	//mt6350_upmu_set_rg_vgp1_volse(0x05);
	//mt6350_upmu_set_rg_vgp1_en(1);
	pmic_set_register_value(PMIC_RG_VGP1_VOSEL,0x05);
	pmic_set_register_value(PMIC_RG_VGP1_EN,1);

	//VDD use external power on ->VGP1_PMU 2.8V
	gpio_set_value(GPIO_LCD_DRV_EN_PIN, GPIO_OUT_ONE);	
}

static void lcm_suspend_power(void)
{
	if(GPIO_LCD_RST_PIN == 0 || GPIO_LCD_DRV_EN_PIN == 0){
		lcm_get_gpio_inform();
	}

	//VDD use internal ldo power on ->VGP1_PMU 2.8V
	//mt6350_upmu_set_rg_vgp1_volse(0x05);
	//mt6350_upmu_set_rg_vgp1_en(1);
	//pmic_set_register_value(PMIC_RG_VGP1_VOSEL,0);
	pmic_set_register_value(PMIC_RG_VGP1_EN,0);

	//VDD use external power on ->VGP1_PMU 2.8V
	gpio_set_value(GPIO_LCD_DRV_EN_PIN, GPIO_OUT_ZERO);	
}

static void lcm_init(void)
{
#ifdef BUILD_LK 
	printf("[LK/LCM] lcm_init() enter\n");
/*
	//SET_RESET_PIN(0);
	//VDD power on ->VGP1_PMU 2.8V
	mt6350_upmu_set_rg_vgp1_volse(0x05);
	mt6350_upmu_set_rg_vgp1_en(1);
	MDELAY(20);
	//SET_RESET_PIN(1);
	//MDELAY(10);
	//SET_RESET_PIN(0);
	//MDELAY(10);
	lcm_reset(1);
	MDELAY(50);
	init_lcm_registers();
	MDELAY(50);
*/
#else
	printk("[Kernel/LCM] lcm_init() enter\n");

	if(GPIO_LCD_RST_PIN == 0 || GPIO_LCD_DRV_EN_PIN == 0){
		lcm_get_gpio_inform();
	}
	//DSI_clk_HS_mode(DISP_MODULE_DSI0, NULL, 1);
#endif
}

static void lcm_suspend(void)
{

#ifdef BUILD_LK
	printf("[LK/LCM] lcm_suspend() enter\n");
/*
	//SET_RESET_PIN(1);
	MDELAY(20);
	//VDD power off ->VGP1_PMU 0V
	mt6350_upmu_set_rg_vgp1_volse(0);
	mt6350_upmu_set_rg_vgp1_en(0);
	MDELAY(20);
*/
#else
	printk("[Kernel/LCM] lcm_suspend() enter\n");
	//SET_RESET_PIN(1);
	MDELAY(5);
	lcm_reset(0);
	MDELAY(10);
	//VDD power off ->VGP1_PMU 0V
	//pmic_set_register_value(PMIC_RG_VGP1_EN,0);
	lcm_suspend_power();
	MDELAY(20);	
#endif
}

static void lcm_resume(void)
{
#ifdef BUILD_LK 
	printf("[LK/LCM] lcm_resume() enter\n");
/*
	//VDD power on ->VGP1_PMU 2.8V
	mt6350_upmu_set_rg_vgp1_volse(0x05);
	mt6350_upmu_set_rg_vgp1_en(1);
	MDELAY(10);
	//SET_RESET_PIN(1);
	//MDELAY(5);
	//SET_RESET_PIN(0);
	//MDELAY(15);
	init_lcm_registers();
	MDELAY(5);
*/
#else
	printk("[Kernel/LCM] lcm_resume() enter\n");
	lcm_reset(0);
	//VDD power on ->VGP1_PMU 2.8V
	//mt6350_upmu_set_rg_vgp1_volse(0x05);
	//mt6350_upmu_set_rg_vgp1_en(1);
	//pmic_set_register_value(PMIC_RG_VGP1_VOSEL,0x05);
	//pmic_set_register_value(PMIC_RG_VGP1_EN,1);
	lcm_init_power();
	MDELAY(20);

	lcm_reset(1);
	MDELAY(50);

	init_lcm_registers();
	MDELAY(50);
#endif
}


static unsigned int lcm_ata_check(unsigned char *buffer)
{
#ifndef BUILD_LK

#if 0
	unsigned int ret = 0;
	unsigned int x0 = FRAME_WIDTH / 4;
	unsigned int x1 = FRAME_WIDTH * 3 / 4;

	unsigned char x0_MSB = ((x0 >> 8) & 0xFF);
	unsigned char x0_LSB = (x0 & 0xFF);
	unsigned char x1_MSB = ((x1 >> 8) & 0xFF);
	unsigned char x1_LSB = (x1 & 0xFF);

	unsigned int data_array[3];
	unsigned char read_buf[4];

	printk("LCM ATA check1 size = 0x%x,0x%x,0x%x,0x%x\n", x0_MSB, x0_LSB, x1_MSB, x1_LSB);
	data_array[0] = 0x0005390A;	/* HS packet */
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00043700;	/* read id return two byte,version and id */
	dsi_set_cmdq(data_array, 1, 1);

	read_reg_v2(0x2A, read_buf, 4);
	
	printk("LCM ATA check2 size = 0x%x,0x%x,0x%x,0x%x\n", read_buf[0], read_buf[1], read_buf[2], read_buf[3]);

	if ((read_buf[0] == x0_MSB) && (read_buf[1] == x0_LSB)
	    && (read_buf[2] == x1_MSB) && (read_buf[3] == x1_LSB))
		ret = 1;
	else
		ret = 0;

	x0 = 0;
	x1 = FRAME_WIDTH - 1;

	x0_MSB = ((x0 >> 8) & 0xFF);
	x0_LSB = (x0 & 0xFF);
	x1_MSB = ((x1 >> 8) & 0xFF);
	x1_LSB = (x1 & 0xFF);

	data_array[0] = 0x0005390A;	/* HS packet */
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);
#endif
    //compare the value of ESD register 0x0A to support LCM ATA test, by xmlsz 20160721
	unsigned int ret = 0;
	unsigned char read_buf[1];

    read_reg_v2(0x0A, read_buf, 1);
    printk("LCM ATA: read_buf=0x%x\n", read_buf[0]);
    
	if( 0x9c == read_buf[0])
		ret = 1;
	//end

	return ret;
#else
	return 0;
#endif
}

LCM_DRIVER kr070ia4t_dsi_vdo_lcm_drv = 
{
  .name						= "KR070IA4T_DSI_VDO",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.ata_check      = lcm_ata_check,
};
