/*
 * Copyright (C) 2010 Google, Inc.
 * Copyright (C) 2010-2012 NVIDIA Corporation.
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

#include <linux/resource.h>
#include <linux/platform_device.h>
#include <linux/wlan_plat.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/mmc/host.h>

#include <asm/mach-types.h>
#include <mach/irqs.h>
#include <mach/iomap.h>
#include <mach/sdhci.h>

#include "gpio-names.h"
#include "devices.h"
#include "board.h"
#include "board-smba1006.h"

static void (*wifi_status_cb)(int card_present, void *dev_id);
static void *wifi_status_cb_devid;
static int smba1006_wifi_status_register(void (*callback)(int , void *), void *);
static struct clk *wifi_32k_clk;

static int smba1006_wifi_reset(int on);
static int smba1006_wifi_power(int on);
static int smba1006_wifi_set_carddetect(int val);

static struct wifi_platform_data smba1006_wifi_control = {
	.set_power	= smba1006_wifi_power,
	.set_reset	= smba1006_wifi_reset,
	.set_carddetect = smba1006_wifi_set_carddetect,
};

static struct platform_device smba1006_wifi_device = {
	.name		= "bcmdhd_wlan",
	.id		= 1,
	.dev		= {
		.platform_data = &smba1006_wifi_control,
	},
};

#ifdef CONFIG_MMC_EMBEDDED_SDIO
static struct embedded_sdio_data embedded_sdio_data0 = {
	.cccr   = {
		.sdio_vsn	= 2,
		.multi_block	= 1,
		.low_speed	= 0,
		.wide_bus	= 0,
		.high_power	= 1,
		.high_speed	= 1,
	},
	.cis  = {
		.vendor 	= 0x02d0,
		.device 	= 0x4329,
	},
};
#endif

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data1 = {
	.mmc_data = {
		.register_status_notify	= smba1006_wifi_status_register,
#ifdef CONFIG_MMC_EMBEDDED_SDIO
		.embedded_sdio = &embedded_sdio_data0,
#endif
		.built_in = 1,
		.ocr_mask = MMC_OCR_1V8_MASK,
	},
#ifndef CONFIG_MMC_EMBEDDED_SDIO
	.pm_flags = MMC_PM_KEEP_POWER,
#endif
	.cd_gpio = -1,
	.wp_gpio = -1,
	.power_gpio = -1,
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data2 = {
	.cd_gpio = -1,
	.wp_gpio = -1,
	.power_gpio = -1,
	.has_no_vreg = 1,
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data3 = {
	.cd_gpio = SMBA1006_SDHC_CD,
	.wp_gpio = SMBA1006_SDHC_WP,
	.power_gpio = SMBA1006_SDHC_POWER,
	.bus_width = 4,
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data4 = {
	.is_8bit = 1,
	.cd_gpio = -1,
	.wp_gpio = -1,
	.power_gpio = SMBA1006_SDINT_POWER,
	.max_clk_limit = 52000000,
	.bus_width = 8,
	.mmc_data = {
		.built_in = 1,
	}
};

static struct platform_device *smba1006_sdhci_devices[] __initdata = {
	&tegra_sdhci_device1,
//	&tegra_sdhci_device2,
//have to init these out of order so that the eMMC card is registered first
	&tegra_sdhci_device4,
	&tegra_sdhci_device3,
};

static int smba1006_wifi_status_register(
		void (*callback)(int card_present, void *dev_id),
		void *dev_id)
{
	if (wifi_status_cb)
		return -EAGAIN;
	wifi_status_cb = callback;
	wifi_status_cb_devid = dev_id;
	return 0;
}

static int smba1006_wifi_set_carddetect(int val)
{
	pr_debug("%s: %d\n", __func__, val);
	if (wifi_status_cb)
		wifi_status_cb(val, wifi_status_cb_devid);
	else
		pr_warning("%s: Nobody to notify\n", __func__);
	return 0;
}

static int smba1006_wifi_power(int on)
{
	pr_debug("%s: %d\n", __func__, on);
	gpio_set_value(SMBA1006_WL_BT_POWER, on);
	mdelay(100);
	gpio_set_value(SMBA1006_WLAN_RESET, on);
	mdelay(200);

	if (on)
		clk_enable(wifi_32k_clk);
	else
		clk_disable(wifi_32k_clk);

	return 0;
}

static int smba1006_wifi_reset(int on)
{
	pr_debug("%s: do nothing\n", __func__);
	return 0;
}

#ifdef CONFIG_TEGRA_PREPOWER_WIFI
static int __init smba1006_wifi_prepower(void)
{
	if (!machine_is_smba1006())
		return 0;

	smba1006_wifi_power(1);

	return 0;
}

subsys_initcall_sync(smba1006_wifi_prepower);
#endif

static int __init smba_wifi_init(void)
{
	wifi_32k_clk = clk_get_sys(NULL, "blink");
	if (IS_ERR(wifi_32k_clk)) {
		pr_err("%s: unable to get blink clock\n", __func__);
		return PTR_ERR(wifi_32k_clk);
	}

	gpio_request(SMBA1006_WL_BT_POWER, "wlan_power");
	gpio_request(SMBA1006_WLAN_RESET, "wlan_rst");

	gpio_direction_output(SMBA1006_WL_BT_POWER, 0);
	gpio_direction_output(SMBA1006_WLAN_RESET, 0);

	platform_device_register(&smba1006_wifi_device);

	device_init_wakeup(&smba1006_wifi_device.dev, 1);
	device_set_wakeup_enable(&smba1006_wifi_device.dev, 0);

	return 0;
}
int __init smba_sdhci_init(void)
{
	int ret;
        /* Plug in platform data */
        tegra_sdhci_device1.dev.platform_data = &tegra_sdhci_platform_data1;
        tegra_sdhci_device2.dev.platform_data = &tegra_sdhci_platform_data2;
        tegra_sdhci_device3.dev.platform_data = &tegra_sdhci_platform_data3;
        tegra_sdhci_device4.dev.platform_data = &tegra_sdhci_platform_data4;

	ret = platform_add_devices(smba1006_sdhci_devices, ARRAY_SIZE(smba1006_sdhci_devices));

	smba_wifi_init();
	return ret;
}
