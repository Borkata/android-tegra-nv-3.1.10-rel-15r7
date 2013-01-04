/*
 * arch/arm/mach-tegra/board-smba1002-sdhci.c
 *
 * Copyright (C) 2010 Google, Inc.
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
#include "board.h"
#include "board-smba1002.h"

static void (*wifi_status_cb)(int card_present, void *dev_id);
static void *wifi_status_cb_devid;
static int smba1002_wifi_status_register(void (*callback)(int , void *), void *);
static struct clk *wifi_32k_clk;

static int smba1002_wifi_reset(int on);
static int smba1002_wifi_power(int on);
static int smba1002_wifi_set_carddetect(int val);

static struct wifi_platform_data smba1002_wifi_control = {
	.set_power	= smba1002_wifi_power,
	.set_reset	= smba1002_wifi_reset,
	.set_carddetect = smba1002_wifi_set_carddetect,
};

static struct platform_device smba1002_wifi_device = {
	.name		= "bcmdhd_wlan",
	.id		= 1,
	.dev		= {
		.platform_data = &smba1002_wifi_control,
	},
};

static struct resource sdhci_resource0[] = {
	[0] = {
		.start	= INT_SDMMC1,
		.end	= INT_SDMMC1,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC1_BASE,
		.end	= TEGRA_SDMMC1_BASE + TEGRA_SDMMC1_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource sdhci_resource1[] = {
	[0] = {
		.start	= INT_SDMMC2,
		.end	= INT_SDMMC2,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC2_BASE,
		.end	= TEGRA_SDMMC2_BASE + TEGRA_SDMMC2_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource sdhci_resource2[] = {
	[0] = {
		.start	= INT_SDMMC3,
		.end	= INT_SDMMC3,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC3_BASE,
		.end	= TEGRA_SDMMC3_BASE + TEGRA_SDMMC3_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource sdhci_resource3[] = {
	[0] = {
		.start	= INT_SDMMC4,
		.end	= INT_SDMMC4,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= TEGRA_SDMMC4_BASE,
		.end	= TEGRA_SDMMC4_BASE + TEGRA_SDMMC4_SIZE-1,
		.flags	= IORESOURCE_MEM,
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

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data0 = {
	.mmc_data = {
		.register_status_notify	= smba1002_wifi_status_register,
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

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data1 = {
	.is_8bit = 1,
	.cd_gpio = -1,
	.wp_gpio = -1,
	.power_gpio = -1,
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data2 = {
	.cd_gpio = SMBA1002_SDHC_CD,
	.wp_gpio = SMBA1002_SDHC_WP,
	.power_gpio = SMBA1002_SDHC_POWER,
	.mmc_data = {
		.built_in = 1,
	}
};

static struct tegra_sdhci_platform_data tegra_sdhci_platform_data3 = {
	.is_8bit = 1,
	.cd_gpio = -1,
	.wp_gpio = -1,
	.power_gpio = -1,
	.max_clk_limit = 52000000,
	.mmc_data = {
		.built_in = 1,
	}
};

static struct platform_device tegra_sdhci_device0 = {
	.name		= "sdhci-tegra",
	.id		= 0,
	.resource	= sdhci_resource0,
	.num_resources	= ARRAY_SIZE(sdhci_resource0),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data0,
	},
};

static struct platform_device tegra_sdhci_device1 = {
	.name		= "sdhci-tegra",
	.id		= 1,
	.resource	= sdhci_resource1,
	.num_resources	= ARRAY_SIZE(sdhci_resource1),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data1,
	},
};

static struct platform_device tegra_sdhci_device2 = {
	.name		= "sdhci-tegra",
	.id		= 2,
	.resource	= sdhci_resource2,
	.num_resources	= ARRAY_SIZE(sdhci_resource2),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data2,
	},
};

static struct platform_device tegra_sdhci_device3 = {
	.name		= "sdhci-tegra",
	.id		= 3,
	.resource	= sdhci_resource3,
	.num_resources	= ARRAY_SIZE(sdhci_resource3),
	.dev = {
		.platform_data = &tegra_sdhci_platform_data3,
	},
};

static int smba1002_wifi_status_register(
		void (*callback)(int card_present, void *dev_id),
		void *dev_id)
{
	if (wifi_status_cb)
		return -EAGAIN;
	wifi_status_cb = callback;
	wifi_status_cb_devid = dev_id;
	return 0;
}

static int smba1002_wifi_set_carddetect(int val)
{
	pr_debug("%s: %d\n", __func__, val);
	if (wifi_status_cb)
		wifi_status_cb(val, wifi_status_cb_devid);
	else
		pr_warning("%s: Nobody to notify\n", __func__);
	return 0;
}

static int smba1002_wifi_power(int on)
{
	pr_debug("%s: %d\n", __func__, on);

//	gpio_set_value(SMBA1002_WLAN_POWER, on);
//	mdelay(100);
	gpio_set_value(SMBA1002_WLAN_RESET, on);
	mdelay(200);

	if (on)
		clk_enable(wifi_32k_clk);
	else
		clk_disable(wifi_32k_clk);

	return 0;
}

static int smba1002_wifi_reset(int on)
{
	pr_debug("%s: do nothing\n", __func__);
	return 0;
}

#ifdef CONFIG_TEGRA_PREPOWER_WIFI
static int __init smba1002_wifi_prepower(void)
{
	if (!machine_is_smba1002())
		return 0;

	smba1002_wifi_power(1);

	return 0;
}

subsys_initcall_sync(smba1002_wifi_prepower);
#endif

static int __init smba_wifi_init(void)
{
	wifi_32k_clk = clk_get_sys(NULL, "blink");
	if (IS_ERR(wifi_32k_clk)) {
		pr_err("%s: unable to get blink clock\n", __func__);
		return PTR_ERR(wifi_32k_clk);
	}

	
	gpio_request(SMBA1002_WLAN_RESET, "wlan_rst");

	tegra_gpio_enable(SMBA1002_WLAN_RESET);

	gpio_direction_output(SMBA1002_WLAN_RESET, 0);

	platform_device_register(&smba1002_wifi_device);

//	device_init_wakeup(&smba1002_wifi_device.dev, 1);
//	device_set_wakeup_enable(&smba1002_wifi_device.dev, 0);

	return 0;
}
int __init smba_sdhci_init(void)
{
	tegra_gpio_enable(tegra_sdhci_platform_data2.power_gpio);
	tegra_gpio_enable(tegra_sdhci_platform_data2.cd_gpio);

	tegra_gpio_enable(tegra_sdhci_platform_data3.power_gpio);

	platform_device_register(&tegra_sdhci_device0);
	platform_device_register(&tegra_sdhci_device3);
	platform_device_register(&tegra_sdhci_device2);

	smba_wifi_init();
	return 0;
}
