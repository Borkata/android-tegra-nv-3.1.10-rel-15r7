/* OK */
/*
 * arch/arm/mach-tegra/board-smba1006-keyboard.c
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

#include <linux/platform_device.h>
#include <linux/input.h>

#include <linux/gpio_keys.h>
#include <linux/leds.h>
#include <linux/leds_pwm.h>
#include <mach/iomap.h>
#include <mach/io.h>
#include <linux/io.h>
#include <linux/pm.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include <linux/gpio.h>
#include <asm/mach-types.h>

#include "board-smba1006.h"
#include "gpio-names.h"
#include "wakeups-t2.h"
#include "fuse.h"

static struct gpio_keys_button smba_keys[] = {
	[0] = {
		.gpio = SMBA1006_KEY_VOLUMEUP,
		.active_low = true,
		.debounce_interval = 10,
		.wakeup = false,		
		.code = KEY_VOLUMEUP,
		.type = EV_KEY,		
		.desc = "volume up",
	},
	[1] = {
		.gpio = SMBA1006_KEY_VOLUMEDOWN,
		.active_low = true,
		.debounce_interval = 10,
		.wakeup = false,		
		.code = KEY_VOLUMEDOWN,
		.type = EV_KEY,		
		.desc = "volume down",
	},
	[2] = {
		.gpio = SMBA1006_KEY_POWER,
		.active_low = true,
		.debounce_interval = 10,
		.wakeup = false,
		.code = KEY_POWER,
		.type = EV_KEY,		
		.desc = "power",
	},
	[3] = {
		.gpio = SMBA1006_KEY_BACK,
		.active_low = true,
		.debounce_interval = 10,
		.wakeup = true,		
		.code = KEY_BACK,
		.type = EV_KEY,		
		.desc = "back",
	},
};

#define PMC_WAKE_STATUS 0x14

static int smba_wakeup_key(void)
{
	unsigned long status =
		readl(IO_ADDRESS(TEGRA_PMC_BASE) + PMC_WAKE_STATUS);

	return (status & SMBA1006_WAKE_KEY_BACK) ?
		KEY_POWER : KEY_RESERVED;	
}

static struct gpio_keys_platform_data smba_keys_platform_data = {
	.buttons	= smba_keys,
	.nbuttons	= ARRAY_SIZE(smba_keys),
	.wakeup_key	= smba_wakeup_key,
};

static struct platform_device smba_keys_device = {
	.name	= "gpio-keys",
	.id	= 0,
	.dev	= {
		.platform_data	= &smba_keys_platform_data,
	},
};

static struct gpio_led smba1006_gpio_leds[] = {
	{
                .name   = "cpu",
                .gpio   = TEGRA_GPIO_PI3,
		.default_trigger = "heartbeat",
		.active_low = 0,
                .retain_state_suspended = 0,
        },
	{
                .name = "cpu-busy",
                .gpio = TEGRA_GPIO_PI4,
                .active_low = 0,
                .retain_state_suspended = 0,
                .default_state = LEDS_GPIO_DEFSTATE_OFF,
        },
};

static struct gpio_led_platform_data smba1006_led_data = {
        .leds   = smba1006_gpio_leds,
        .num_leds       = ARRAY_SIZE(smba1006_gpio_leds),
};

static struct platform_device smba1006_leds_gpio = {
        .name   = "leds-gpio",
        .id     = -1,
        .dev    = {
                .platform_data = &smba1006_led_data,
        },
};

static struct platform_device *smba_pmu_devices[] __initdata = {
	&smba_keys_device,
	&smba1006_leds_gpio,
};

/* Register all keyboard devices */
int __init smba_keys_init(void)
{
	return platform_add_devices(smba_pmu_devices, ARRAY_SIZE(smba_pmu_devices));
}
