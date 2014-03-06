/*
 * arch/arm/mach-tegra/board-smba1006.c
 *
 * Copyright (C) 2011 Eduardo Jose Tagle <ejtagle@tutopia.com>
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

#include <linux/console.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/clk.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/dma-mapping.h>
#include <linux/fsl_devices.h>
#include <linux/platform_data/tegra_usb.h>
#include <linux/pda_power.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/i2c-tegra.h>
#include <linux/memblock.h>
#include <linux/antares_dock.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/setup.h>

#include <mach/io.h>
#include <mach/w1.h>
#include <mach/iomap.h>
#include <mach/irqs.h>
#include <mach/nand.h>
#include <mach/iomap.h>
#include <mach/sdhci.h>
#include <mach/gpio.h>
#include <mach/clk.h>
#include <mach/usb_phy.h>
#include <mach/i2s.h>
#include <mach/system.h>
#include <linux/nvmap.h>

#include "board.h"
#include "board-smba1006.h"
#include "clock.h"
#include "gpio-names.h"
#include "devices.h"
#include "pm.h"
#include "wakeups-t2.h"
#include "wdt-recovery.h"

#include <linux/rfkill-gpio.h>



#define PMC_CTRL 0x0
#define PMC_CTRL_INTR_LOW (1 << 17)

static struct rfkill_gpio_platform_data bluetooth_rfkill = {
	.name		= "bluetooth_rfkill",
	.shutdown_gpio	= -1,
	.reset_gpio	= SMBA1006_BT_RESET,	
	.type		= RFKILL_TYPE_BLUETOOTH,
};

static struct platform_device bluetooth_rfkill_device = {
	.name	= "rfkill_gpio",
	.id	= -1,
	.dev	= {
		.platform_data = &bluetooth_rfkill,
	},
};

#ifdef CONFIG_BT_BLUEDROID
extern void bluesleep_setup_uart_port(struct platform_device *uart_dev);
#endif

void __init smba_setup_bluesleep(void)
{
	/*Add Clock Resource*/
	clk_add_alias("bcm4329_32k_clk", bluetooth_rfkill_device.name, \
				"blink", NULL);
#ifdef CONFIG_BT_BLUEDROID
	bluesleep_setup_uart_port(&tegra_uartc_device);
#endif
	return;
}

static struct resource smba_bluesleep_resources[] = {
	[0] = {
		.name = "gpio_host_wake",
			.start  = SMBA1006_BT_IRQ,
			.end    = SMBA1006_BT_IRQ,
			.flags  = IORESOURCE_IO,
	},
	[1] = {
		.name = "gpio_ext_wake",
			.start  = SMBA1006_BT_WAKEUP,
			.end    = SMBA1006_BT_WAKEUP,
			.flags  = IORESOURCE_IO,
	},
	[2] = {
		.name = "host_wake",
			.start  = TEGRA_GPIO_TO_IRQ(SMBA1006_BT_IRQ),
			.end    = TEGRA_GPIO_TO_IRQ(SMBA1006_BT_IRQ),
			.flags  = IORESOURCE_IRQ | IORESOURCE_IRQ_LOWEDGE,
	},
};

static struct platform_device smba_bluesleep_device = {
	.name           = "bluesleep",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(smba_bluesleep_resources),
	.resource       = smba_bluesleep_resources,
};

static struct platform_device *smba_devices[] __initdata = {
	&tegra_pmu_device,
	&tegra_aes_device,
	&bluetooth_rfkill_device,
	&smba_bluesleep_device,
	&tegra_wdt_device,
};

static atomic_t smba_gps_mag_powered = ATOMIC_INIT(0);
void smba_gps_mag_poweron(void)
{
	if (atomic_inc_return(&smba_gps_mag_powered) == 1) {
		pr_info("Enabling GPS/Magnetic module\n");
		/* 3G/GPS power on sequence */
		gpio_set_value(SMBA1006_GPSMAG_DISABLE, 1); /* Enable power */
		msleep(2);
	}
}
EXPORT_SYMBOL_GPL(smba_gps_mag_poweron);

void smba_gps_mag_poweroff(void)
{
	if (atomic_dec_return(&smba_gps_mag_powered) == 0) {
		pr_info("Disabling GPS/Magnetic module\n");
		/* 3G/GPS power on sequence */
		gpio_set_value(SMBA1006_GPSMAG_DISABLE, 0); /* Disable power */
		msleep(2);
	}
}
EXPORT_SYMBOL_GPL(smba_gps_mag_poweroff);

static atomic_t smba_gps_mag_inited = ATOMIC_INIT(0);
void smba_gps_mag_init(void)
{
	if (atomic_inc_return(&smba_gps_mag_inited) == 1) {
		gpio_request(SMBA1006_GPSMAG_DISABLE, "gps_disable");
		gpio_direction_output(SMBA1006_GPSMAG_DISABLE, 0);
	}
}
EXPORT_SYMBOL_GPL(smba_gps_mag_init);

void smba_gps_mag_deinit(void)
{
	atomic_dec(&smba_gps_mag_inited);
}
EXPORT_SYMBOL_GPL(smba_gps_mag_deinit);

static void __init tegra_smba_init(void)
{
	/* Initialize the pinmux */
	smba_pinmux_init();

	/* Initialize the clocks - clocks require the pinmux to be initialized first */
	smba_clks_init();

	platform_add_devices(smba_devices,ARRAY_SIZE(smba_devices));
	/* Register i2c devices - required for Power management and MUST be done before the power register */
	smba_i2c_register_devices();

	/* Register the power subsystem - Including the poweroff handler - Required by all the others */
	smba_charge_init();
	smba_regulator_init();

	/* Register the USB device */
	smba_usb_register_devices();

	/* Register UART devices */
	smba_uart_register_devices();

	/* Register RAM Console */
	tegra_ram_console_debug_init();

	/* Register GPU devices */
	smba_panel_init();

	/* Register Audio devices */
	smba_audio_register_devices();

	/* Register all the keyboard devices */
	smba_keys_init();

	/* Register touchscreen devices */
	smba_touch_register_devices();

	/* Register accelerometer device */
	smba_sensors_register_devices();
	
	/* Register GPS power management */
	smba_gps_pm_register_devices();

	/* Register Camera powermanagement devices */
	smba_camera_register_devices();

	/* Register NAND flash devices */
	smba_nand_register_devices();
	
	/* Register SDHCI devices */
	smba_sdhci_init();	
	
	/* Register Bluetooth powermanagement devices */
	smba_setup_bluesleep();

	/* GPS Setup */
	smba_gps_mag_init();
	//smba_gps_mag_poweron();

	/* Release the tegra bootloader framebuffer */
	tegra_release_bootloader_fb();
}

static void __init tegra_smba_reserve(void)
{
	if (memblock_reserve(0x0, 4096) < 0)
		pr_warn("Cannot reserve first 4K of memory for safety\n");

	/* Reserve the graphics memory */
	tegra_reserve(SMBA1006_GPU_MEM_SIZE, SMBA1006_FB1_MEM_SIZE, SMBA1006_FB2_MEM_SIZE);
	tegra_ram_console_debug_reserve(SZ_1M);
}

static void __init tegra_smba_fixup(struct machine_desc *desc,
	struct tag *tags, char **cmdline, struct meminfo *mi)
{
	mi->nr_banks = SMBA1006_MEM_BANKS;
	mi->bank[0].start = PHYS_OFFSET;
	mi->bank[0].size  = SMBA1006_MEM_SIZE - SMBA1006_TOTAL_GPU_MEM_SIZE;
	mi->bank[1].start = SMBA1006_MEM_SIZE;
	mi->bank[1].size  = SMBA1006_MEM_SIZE;
} 

MACHINE_START(HARMONY, "harmony")
.boot_params = 0x00000100,
.fixup	= tegra_smba_fixup,
.map_io = tegra_map_common_io,
.reserve = tegra_smba_reserve,
.init_early	= tegra_init_early,
.init_irq = tegra_init_irq,
.timer = &tegra_timer,
.init_machine = tegra_smba_init,
MACHINE_END

