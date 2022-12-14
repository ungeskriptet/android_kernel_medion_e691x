#ifndef TPD_CUSTOM_GT9XX_H__
#define TPD_CUSTOM_GT9XX_H__

#include <linux/hrtimer.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
/* #include <linux/io.h> */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/byteorder/generic.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/interrupt.h>
#include <linux/time.h>

#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

/* Pre-defined definition */

#define TPD_KEY_COUNT   4

#define TPD_KEYS        {KEY_BACK, KEY_HOME, KEY_MENU, KEY_SEARCH}
#define TPD_KEYS_DIM    {{60, 850, 50, 30}, {180, 850, 50, 30}, {300, 850, 50, 30}, {420, 850, 50, 30} }

/*
struct goodix_ts_data
{
    spinlock_t irq_lock;
    struct i2c_client *client;
    struct input_dev  *input_dev;
    struct hrtimer timer;
    struct work_struct  work;
    struct early_suspend early_suspend;
    s32 irq_is_disable;
    s32 use_irq;
    u16 abs_x_max;
    u16 abs_y_max;
    u8  max_touch_num;
    u8  int_trigger_type;
    u8  green_wake_mode;
    u8  chip_type;
    u8  enter_update;
    u8  gtp_is_suspend;
    u8  gtp_rawdiff_mode;
};
*/

extern u16 show_len;
extern u16 total_len;
extern u8 gtp_rawdiff_mode;
extern int tpd_halt;
extern int touch_irq;
extern u8 cfg_len;
extern u8 rqst_processing;
extern u8 is_950;
extern struct i2c_client *i2c_client_point;
extern s32 gtp_send_cfg(struct i2c_client *client);
extern void gtp_reset_guitar(struct i2c_client *client, s32 ms);
extern void gtp_int_sync(u32 ms);
extern u8 gup_init_update_proc(struct i2c_client *client);
extern u8 gup_init_fw_proc(struct i2c_client *client);
extern s32 gtp_i2c_read(struct i2c_client *client, u8 *buf, s32 len);
extern s32 gtp_i2c_write(struct i2c_client *client, u8 *buf, s32 len);
extern int i2c_write_bytes(struct i2c_client *client, u16 addr, u8 *txbuf, int len);
extern int i2c_read_bytes(struct i2c_client *client, u16 addr, u8 *rxbuf, int len);
extern s32 gtp_i2c_read_dbl_check(struct i2c_client *client, u16 addr, u8 *rxbuf, int len);
extern void gtp_get_chip_type(struct i2c_client *client);
s32 gup_fw_download_proc(void *dir, u8 dwn_mode);
u8 gtp_fw_startup(struct i2c_client *client);
u8 gup_check_fs_mounted(char *path_name);
u8 gup_clk_calibration(void);
extern s32 gup_enter_update_mode(struct i2c_client *client);
extern void gup_leave_update_mode(void);
extern s32 gup_update_proc(void *dir);
s32 init_wr_node(struct i2c_client *client);
void uninit_wr_node(void);
void gtp_esd_switch(struct i2c_client *client, s32 on);





/* ***************************PART1:ON/OFF define******************************* */
#define GTP_CUSTOM_CFG        0
#define GTP_DRIVER_SEND_CFG   1	/* driver send config to TP on intilization (for no config built in TP flash) */
#define GTP_HAVE_TOUCH_KEY    0
#define GTP_POWER_CTRL_SLEEP  0	/* turn off power on suspend */

#define GTP_AUTO_UPDATE       0	/* update FW to TP FLASH */
#define GTP_DEF_FW_UPDATE     0	/* auto updated by gtp_default_FW, function together with GTP_AUTO_UPDATE */

#define GTP_SUPPORT_I2C_DMA   0
#define GTP_COMPATIBLE_MODE   1	/* compatible with GT9XXF */

#define GTP_CHANGE_X2Y        0	/* set for se1 */
#define GTP_ESD_PROTECT       1
#define SWITCH_OFF                  0
#define SWITCH_ON                   1

#define GTP_CREATE_WR_NODE    1
#define GUP_USE_HEADER_FILE   0
#define GTP_FW_DOWNLOAD       0	/* update FW to TP SRAM */
/* #define GTP_CHARGER_DETECT */

/* #define TPD_PROXIMITY */
/* #define TPD_HAVE_BUTTON               //report key as coordinate,Vibration feedback */
/* #define TPD_WARP_X */
/* #define TPD_WARP_Y */

#define GTP_DEBUG_ON          0
#define GTP_DEBUG_ARRAY_ON    0
#define GTP_DEBUG_FUNC_ON     0

/* ***************************PART2:TODO define********************************** */
/* STEP_1(REQUIRED):Change config table. */
/*TODO: puts the config info corresponded to your TP here, the following is just
a sample config, send this config should cause the chip cannot work normally*/
#define CTP_CFG_GROUP1 {\
0x00,0xD0,0x02,0x00,0x05,0x05,0x35,0x00,0x01,0xC8,\
0x14,0x0F,0x50,0x32,0x03,0x03,0x00,0x00,0x00,0x00,\
0x00,0x00,0x04,0x17,0x19,0x1C,0x14,0x8C,0x2B,0x0E,\
0x4E,0x50,0xD3,0x07,0x00,0x00,0x00,0xD2,0x02,0x1D,\
0x3C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x4A,0x7D,0x94,0xC5,0x02,0x08,0x0F,0x00,0x04,\
0x80,0x4E,0x00,0x78,0x56,0x00,0x71,0x60,0x00,0x6C,\
0x6A,0x00,0x67,0x76,0x00,0x67,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x02,0x10,0x04,0x12,0x06,0x14,0x08,0x16,\
0x0A,0x18,0x0C,0x1A,0x0E,0x1C,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x26,0x1E,0x24,0x1D,0x22,0x1C,0x21,0x18,\
0x20,0x16,0x1F,0x0A,0x14,0x08,0x13,0x06,0x12,0x04,\
0x10,0x02,0x0F,0x00,0x0C,0xFF,0xFF,0xFF,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x01\
}

#define CTP_CFG_GROUP1_CHARGER {}

/* TODO puts your group2 config info here,if need. */
#define CTP_CFG_GROUP2 {}

/* TODO puts your group2 config info here,if need. */
#define CTP_CFG_GROUP2_CHARGER {}

/* TODO puts your group3 config info here,if need. */
#define CTP_CFG_GROUP3 {}

/* TODO puts your group3 config info here,if need. */
#define CTP_CFG_GROUP3_CHARGER {}

#define GTP_GPIO_GET_VALUE(pin)         mt_get_gpio_in(pin)
#define GTP_GPIO_REQUEST(pin, label)    gpio_request(pin, label)
#define GTP_GPIO_FREE(pin)              gpio_free(pin)
#define GTP_IRQ_TAB                     {IRQ_TYPE_EDGE_RISING, IRQ_TYPE_EDGE_FALLING,\
									IRQ_TYPE_LEVEL_LOW, IRQ_TYPE_LEVEL_HIGH}

/* STEP_3(optional):Custom set some config by themself,if need. */
#if GTP_CUSTOM_CFG
#define GTP_INT_TRIGGER  0	/* 0:Rising 1:Falling */
#else
#define GTP_INT_TRIGGER  1
#endif
#define GTP_MAX_TOUCH      5
#define GTP_ESD_CHECK_CIRCLE  2000
#define TPD_POWER_SOURCE_CUSTOM	MT6328_POWER_LDO_VGP2
#define VELOCITY_CUSTOM
#define TPD_VELOCITY_CUSTOM_X 20
#define TPD_VELOCITY_CUSTOM_Y 20

/* STEP_4(optional):If this project have touch key,Set touch key config. */
#if GTP_HAVE_TOUCH_KEY
#define GTP_KEY_TAB	 {KEY_MENU, KEY_HOME, KEY_BACK, KEY_SEND}
#endif

/* ***************************PART3:OTHER define********************************* */
#define GTP_DRIVER_VERSION          "V1.0<2012/05/01>"
#define GTP_I2C_NAME                "Goodix-TS"
#define GT91XX_CONFIG_PROC_FILE     "gt9xx_config"
#define GTP_POLL_TIME               10
#define GTP_ADDR_LENGTH             2
#define GTP_CONFIG_MIN_LENGTH       186
#define GTP_CONFIG_MAX_LENGTH       240
#define FAIL                        0
#define SUCCESS                     1

/* ******************** For GT9XXF Start **********************/
#if GTP_COMPATIBLE_MODE
enum CHIP_TYPE_T {
	CHIP_TYPE_GT9 = 0,
	CHIP_TYPE_GT9F = 1,
};
#endif
extern enum CHIP_TYPE_T gtp_chip_type;


#define GTP_REG_MATRIX_DRVNUM           0x8069
#define GTP_REG_MATRIX_SENNUM           0x806A
#define GTP_REG_RQST                    0x8043
#define GTP_REG_BAK_REF                 0x99D0
#define GTP_REG_MAIN_CLK                0x8020
#define GTP_REG_CHIP_TYPE               0x8000
#define GTP_REG_HAVE_KEY                0x804E

#define GTP_FL_FW_BURN              0x00
#define GTP_FL_ESD_RECOVERY         0x01
#define GTP_FL_READ_REPAIR          0x02

#define GTP_BAK_REF_SEND                0
#define GTP_BAK_REF_STORE               1
#define CFG_LOC_DRVA_NUM                29
#define CFG_LOC_DRVB_NUM                30
#define CFG_LOC_SENS_NUM                31

#define GTP_CHK_FW_MAX                  1000
#define GTP_CHK_FS_MNT_MAX              300
#define GTP_BAK_REF_PATH                "/data/gtp_ref.bin"
#define GTP_MAIN_CLK_PATH               "/data/gtp_clk.bin"
#define GTP_RQST_CONFIG                 0x01
#define GTP_RQST_BAK_REF                0x02
#define GTP_RQST_RESET                  0x03
#define GTP_RQST_MAIN_CLOCK             0x04
#define GTP_RQST_RESPONDED              0x00
#define GTP_RQST_IDLE                   0xFF

/* ******************** For GT9XXF End **********************/

/* Register define */
#define GTP_READ_COOR_ADDR          0x814E
#define GTP_REG_SLEEP               0x8040
#define GTP_REG_SENSOR_ID           0x814A
#define GTP_REG_CONFIG_DATA         0x8047
#define GTP_REG_VERSION             0x8140
#define GTP_REG_HW_INFO             0x4220
#define GTP_REG_REFRESH_RATE		0x8056

#define RESOLUTION_LOC              3
#define TRIGGER_LOC                 8


#define MAX_TRANSACTION_LENGTH        8
#define TPD_I2C_NUMBER				1
#define GTP_DMA_MAX_TRANSACTION_LENGTH  255	/* for DMA mode */
#define GTP_DMA_MAX_I2C_TRANSFER_SIZE   (GTP_DMA_MAX_TRANSACTION_LENGTH - GTP_ADDR_LENGTH)
#define I2C_MASTER_CLOCK              300
#define MAX_I2C_TRANSFER_SIZE         (MAX_TRANSACTION_LENGTH - GTP_ADDR_LENGTH)
#define TPD_MAX_RESET_COUNT           3
/* #define TPD_CALIBRATION_MATRIX        {962,0,0,0,1600,0,0,0}; */
#define TPD_CALIBRATION_MATRIX_ROTATION_NORMAL  {4096, 0, 0, 0, -4096, 4194304, 0, 0}
#define TPD_CALIBRATION_MATRIX_ROTATION_FACTORY {4096, 0, 0, 0, -4096, 4194304, 0, 0}
/* #define TPD_CALIBRATION_MATRIX_ROTATION_NORMAL_800  {0, -6832, 3272704, 2455, 0, 0, 0, 0}; */
/* #define TPD_CALIBRATION_MATRIX_ROTATION_FACTORY_800 {0, -4096, 1961984, 4096, 0, 0, 0, 0}; */

#define TPD_RESET_ISSUE_WORKAROUND
#define TPD_HAVE_CALIBRATION
#define TPD_NO_GPIO
#define TPD_RESET_ISSUE_WORKAROUND

#ifdef TPD_WARP_X
#undef TPD_WARP_X
#define TPD_WARP_X(x_max, x) (x_max - 1 - x)
#else
#define TPD_WARP_X(x_max, x) x
#endif

#ifdef TPD_WARP_Y
#undef TPD_WARP_Y
#define TPD_WARP_Y(y_max, y) (y_max - 1 - y)
#else
#define TPD_WARP_Y(y_max, y) y
#endif

/* Log define */
#define GTP_INFO(fmt, arg...)           pr_debug("<<-GTP-INFO->> "fmt"\n", ##arg)
#define GTP_ERROR(fmt, arg...)          pr_err("<<-GTP-ERROR->> "fmt"\n", ##arg)
#define GTP_DEBUG(fmt, arg...)    do {\
if (GTP_DEBUG_ON)\
	pr_debug("<<-GTP-DEBUG->> [%d]"fmt"\n", __LINE__, ##arg);\
} while (0)
#define GTP_DEBUG_ARRAY(array, num)    do {\
s32 i;\
u8 *a = array;\
if (GTP_DEBUG_ARRAY_ON) {\
	pr_debug("<<-GTP-DEBUG-ARRAY->>\n");\
	for (i = 0; i < (num); i++) {\
		pr_debug("%02x   ", (a)[i]);\
		if ((i + 1) % 10 == 0) {\
			pr_debug("\n");\
		} \
		} \
	pr_debug("\n");\
} \
} while (0)
#define GTP_DEBUG_FUNC()	do {\
if (GTP_DEBUG_FUNC_ON)\
	pr_debug("<<-GTP-FUNC->> Func:%s@Line:%d\n", __func__, __LINE__);\
} while (0)
#define GTP_SWAP(x, y)                 do {\
					 typeof(x) z = x;\
					 x = y;\
					 y = z;\
				       } while (0)

/* ****************************PART4:UPDATE define******************************* */
/* Error no */
#define ERROR_NO_FILE           2	/* ENOENT */
#define ERROR_FILE_READ         23	/* ENFILE */
#define ERROR_FILE_TYPE         21	/* EISDIR */
#define ERROR_GPIO_REQUEST      4	/* EINTR */
#define ERROR_I2C_TRANSFER      5	/* EIO */
#define ERROR_NO_RESPONSE       16	/* EBUSY */
#define ERROR_TIMEOUT           110	/* ETIMEDOUT */

/* *****************************End of Part III******************************** */

#if GTP_DEF_FW_UPDATE
unsigned char gtp_default_FW[] = 
{
            //TODO:Puts your update firmware data here!
};
#endif

#endif				/* TOUCHPANEL_H__ */
