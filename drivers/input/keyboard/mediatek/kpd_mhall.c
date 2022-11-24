/*
 * Copyright (C) 2010 MediaTek, Inc.
 *
 * Author: Terry Chang <terry.chang@mediatek.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "kpd.h"
#include <linux/wakelock.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/clk.h>

#define KPD_MHALL_NAME	"mtk-mhall"
#define KPD_MHALL_INIT_GPIO

/*----------------------------------------------------------------------------*/
static u32 irq_gpio_mhall;
static struct device_node *irq_node_mhall;
static unsigned int irq_num_mhall;
static void kpd_mhallkey_handler(unsigned long data);
static DECLARE_TASKLET(kpd_mhallkey_tasklet, kpd_mhallkey_handler, 0);

/*----------------------------------------------------------------------------*/
static int kpd_mhall_pdrv_probe(struct platform_device *pdev);
static int kpd_mhall_pdrv_remove(struct platform_device *pdev);

/*----------------------------------------------------------------------------*/
static const struct of_device_id kpd_mhall_of_match[] = {
	{.compatible = "mediatek,mhall",},
	{},
};

/*----------------------------------------------------------------------------*/
static struct platform_driver kpd_mhall_pdrv = {
	.probe = kpd_mhall_pdrv_probe,
	.remove = kpd_mhall_pdrv_remove,
	.driver = {
		   .name = KPD_MHALL_NAME,
		   .owner = THIS_MODULE,
		   .of_match_table = kpd_mhall_of_match,
		   },
};

/*----------------------------------------------------------------------------*/
static void kpd_mhallkey_handler(unsigned long data)
{
        int slid;
        slid=gpio_get_value(irq_gpio_mhall);
        kpd_hallkey_report(slid);

        irq_set_irq_type(irq_num_mhall, ((1==slid) ? IRQF_TRIGGER_LOW : IRQF_TRIGGER_HIGH));
        enable_irq(irq_num_mhall);
        enable_irq_wake(irq_num_mhall);
}

/*----------------------------------------------------------------------------*/
static irqreturn_t kpd_mhallkey_eint_handler(int irq, void *dev_id)
{
        disable_irq_nosync(irq_num_mhall);
        tasklet_schedule(&kpd_mhallkey_tasklet);
        return IRQ_HANDLED;
}

/*----------------------------------------------------------------------------*/
static int kpd_mhall_pdrv_probe(struct platform_device *pdev)
{
        u32 ints[2] = {0,0};
        long ret=0;
#ifdef KPD_MHALL_INIT_GPIO
        struct pinctrl *pinctrl;
        struct pinctrl_state *pins_default;
        struct pinctrl_state *pins_cfg;

        //gpio setting
        pinctrl = devm_pinctrl_get(&pdev->dev);
        if (IS_ERR(pinctrl)) {
                ret = PTR_ERR(pinctrl);
                kpd_print("Cannot find  pinctrl!\n");
                goto EXIT;
        }
        pins_default = pinctrl_lookup_state(pinctrl, "pin_default");
        if (IS_ERR(pins_default)) {
                kpd_print("Cannot find pinctrl default!\n");
        }
        pins_cfg = pinctrl_lookup_state(pinctrl, "pin_cfg");
        if (IS_ERR(pins_cfg)) {
                ret = PTR_ERR(pins_cfg);
                kpd_print("Cannot find pinctrl pin_cfg!\n");
                goto EXIT;
        }
        pinctrl_select_state(pinctrl, pins_cfg);
        kpd_print("MHALL-eint, GPIO init ok!\n");
#endif

        //interrupt setup
        irq_node_mhall= of_find_matching_node(NULL, kpd_mhall_of_match);
        if(irq_node_mhall){
                of_property_read_u32_array(irq_node_mhall, "debounce", ints, ARRAY_SIZE(ints));
                irq_gpio_mhall=ints[0];
                //gpio_request(ints[0], "MHALL-key");
                gpio_set_debounce(ints[0], ints[1]);
                kpd_print("MHALL-eint,debounce:ints[0] = %d, ints[1] = %d!!\n", ints[0], ints[1]);
                irq_num_mhall= irq_of_parse_and_map(irq_node_mhall, 0);
                if (irq_num_mhall < 0) {
                        kpd_print("MHALL-eint,map irq_num_hall fail!\n");
                        return -EINVAL;
                }
                if (request_irq(irq_num_mhall, kpd_mhallkey_eint_handler, IRQF_TRIGGER_LOW, "MHALL-eint", NULL)) {
                        kpd_print("MHALL-eint,request_irq IRQ NOT AVAILABLE!.");
                        return -EINVAL;
                }
        }
        else{
                kpd_print("MHALL-eint, node NOT AVAILABLE!\n");
                return -EINVAL;
        }

#ifdef KPD_MHALL_INIT_GPIO
        EXIT:
#endif
        return ret;
}

/*----------------------------------------------------------------------------*/
/* should never be called */
static int kpd_mhall_pdrv_remove(struct platform_device *pdev)
{
        return 0;
}

/*----------------------------------------------------------------------------*/
static int __init kpd_mhall_mod_init(void)
{
        int r;
        kpd_print("kpd_mhall_mod_init\n");
        r = platform_driver_register(&kpd_mhall_pdrv);
        if (r) {
                kpd_print("register driver failed (%d)\n", r);
                return r;
        }

        return 0;
}
/*----------------------------------------------------------------------------*/
/* should never be called */
static void __exit kpd_mhall_mod_exit(void)
{

}

module_init(kpd_mhall_mod_init);
module_exit(kpd_mhall_mod_exit);
