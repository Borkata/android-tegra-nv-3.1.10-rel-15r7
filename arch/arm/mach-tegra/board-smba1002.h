/*
 * arch/arm/mach-tegra/board-smba1002.h
 *
 * Copyright (C) 2011 Eduardo Jos� Tagle <ejtagle@tutopia.com>
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

#ifndef _MACH_TEGRA_BOARD_SMBA1002_H
#define _MACH_TEGRA_BOARD_SMBA1002_H

// SMBA1002 GPIOs

// Input
/* GPS and Magnetic sensor share the same enabling IO line */
#define SMBA1002_KEY_VOLUMEUP 		TEGRA_GPIO_PV4 	/* 0=pressed */
#define SMBA1002_KEY_VOLUMEDOWN 	TEGRA_GPIO_PD4 	/* 0=pressed */
#define SMBA1002_KEY_POWER 			TEGRA_GPIO_PV2 	/* 0=pressed */
#define SMBA1002_KEY_HOMEPAGE   	TEGRA_GPIO_PS2  /* 0=pressed */
#define SMBA1002_KEYBOARD			TEGRA_GPIO_PV6
#define SMBA1002_HDMI_HPD			TEGRA_GPIO_PN7  /* 1=HDMI plug detected */
#define SMBA1002_TS_IRQ				TEGRA_GPIO_PJ7
#define SMBA1002_HP_DETECT			TEGRA_GPIO_PW2 	/* HeadPhone detect for audio codec: 1=Headphone plugged */
#define SMBA1002_TEMP_ALERT			TEGRA_GPIO_PN6
#define SMBA1002_LIS3LV02D 			TEGRA_GPIO_PJ0  /*Accelerometer */
#define SMBA1002_BT_IRQ 			TEGRA_GPIO_PU6
#define SMBA1002_ISL29023  			TEGRA_GPIO_PV5  /* Light Sensor */
#define SMBA1002_AC_PRESENT    		TEGRA_GPIO_PH2
#define SMBA1002_DOCK           	TEGRA_GPIO_PH0


// Outputs
#define SMBA1002_GPSMAG_DISABLE  	TEGRA_GPIO_PV3 	/* 0=disabled */
#define SMBA1002_3G_DISABLE			TEGRA_GPIO_PJ2  /* 0=disabled */
#define SMBA1002_CAMERA_POWER 		TEGRA_GPIO_PBB5 /* 1=powered on */
#define SMBA1002_NAND_WPN			TEGRA_GPIO_PC7	/* NAND flash write protect: 0=writeprotected */
#define SMBA1002_BL_ENB				TEGRA_GPIO_PD3  /* LCD Backlight enable */
#define SMBA1002_LVDS_SHUTDOWN		TEGRA_GPIO_PB2
#define SMBA1002_EN_VDD_PANEL		TEGRA_GPIO_PC6 
#define SMBA1002_BL_VDD				TEGRA_GPIO_PW0
#define SMBA1002_BL_PWM				TEGRA_GPIO_PU3  /* PWM */
#define	SMBA1002_ENABLE_VDD_VID		TEGRA_GPIO_PD1	/* 1=enabled.  Powers HDMI. Wait 500uS to let it stabilize before returning */
#define SMBA1002_SDHC_CD			TEGRA_GPIO_PI5  /* External SD Card Detect */
#define SMBA1002_SDHC_WP			-1				/*1=Write Protected */
#define SMBA1002_WL_BT_POWER 		TEGRA_GPIO_PK5
#define SMBA1002_WLAN_RESET 		TEGRA_GPIO_PK6
#define SMBA1002_BT_RESET 			TEGRA_GPIO_PU0  /* 0=reset asserted */
#define SMBA1002_BT_WAKEUP			TEGRA_GPIO_PU5
#define SMBA1002_LOW_BATT			TEGRA_GPIO_PW3  /*(0=low battery)*/
#define SMBA1002_TS_RESET			TEGRA_GPIO_PH1
#define SMBA1002_TS_POWER			TEGRA_GPIO_PK2
#define SMBA1002_SDINT_POWER       	TEGRA_GPIO_PI6
#define SMBA1002_SDHC_POWER			TEGRA_GPIO_PD0  /* External SD Power On */
#define SMBA1002_CAMERA_RESET   	TEGRA_GPIO_PD2  /* 1=powered on */
#define SMBA1002_CHARGING_DISABLE  	TEGRA_GPIO_PK7
#define SMBA1002_INT_MIC_EN    		TEGRA_GPIO_PX0  /* 0 = disabled */
#define SMBA1002_USB0_VBUS			TEGRA_GPIO_PB1	/* 1= VBUS usb0 */

#define SMBA1002_WAKE_KEY_POWER 	TEGRA_WAKE_GPIO_PV2
#define SMBA1002_WAKE_KEY_RESUME	TEGRA_WAKE_GPIO_PV2

#define SMBA1002_FB_PAGES			2				/* At least, 2 video pages */
#define SMBA1002_FB_HDMI_PAGES		2				/* At least, 2 video pages for HDMI */

// SMBA1002 memory is 1xSZ_512M
#define SMBA1002_MEM_SIZE 			SZ_512M			/* Total memory */
#define SMBA1002_MEM_BANKS			1

#define SMBA1002_GPU_MEM_SIZE  			(SZ_1M*128)			/* Memory reserved for GPU */

#define SMBA1002_FB1_MEM_SIZE 	SZ_16M			/* Memory reserved for Framebuffer 1: LCD */
#define SMBA1002_FB2_MEM_SIZE 	SZ_8M			/* Memory reserved for Framebuffer 2: HDMI out */
	
#define DYNAMIC_GPU_MEM 0							/* use dynamic memory for GPU */

#define SMBA1002_48KHZ_AUDIO 						/* <- define this if you want 48khz audio sampling rate instead of 44100Hz */
/*#define USE_ORG_DAS 1*/

// TPS6586x GPIOs as registered 
#define PMU_GPIO_BASE				(TEGRA_NR_GPIOS) 
#define PMU_GPIO0 					(PMU_GPIO_BASE)
#define PMU_GPIO1 					(PMU_GPIO_BASE + 1) 
#define PMU_GPIO2 					(PMU_GPIO_BASE + 2)
#define PMU_GPIO3 					(PMU_GPIO_BASE + 3)

#define ALC5623_GPIO_BASE			(TEGRA_NR_GPIOS + 16)
#define ALC5623_GP0					(ALC5623_GPIO_BASE)

#define TPS6586X_GPIO_BASE      	TEGRA_NR_GPIOS

#define PMU_IRQ_BASE				(TEGRA_NR_IRQS)
//#define PMU_IRQ_RTC_ALM1 			(TPS6586X_INT_BASE + TPS6586X_INT_RTC_ALM1)






/* The switch used to indicate rotation lock */
//#define SW_ROTATION_LOCK 	(SW_MAX-1)

extern int smba_bt_wifi_gpio_set(bool on);
extern int smba_bt_wifi_gpio_init(void);

extern void smba_wifi_set_cd(int val);

extern int  smba_pinmux_init(void);
extern void smba_clks_init(void);

extern int smba_usb_register_devices(void);
extern int smba_audio_register_devices(void);
extern int smba_panel_init(void);
extern int smba_uart_register_devices(void);
extern int smba_spi_register_devices(void);
extern int smba_i2c_register_devices(void);
extern int smba_power_register_devices(void);
extern int smba_keys_init(void);
extern int smba_touch_register_devices(void);
extern int smba_sdhci_init(void);
extern int smba_sensors_register_devices(void);
extern int smba_wlan_pm_register_devices(void);
extern void smba_setup_bluesleep(void);
extern void smba_bt_rfkill(void);
extern int smba_nand_register_devices(void);
extern int smba_camera_register_devices(void);
extern int smba_charge_init(void);
extern int smba_regulator_init(void);
extern int smba_s5k6aa_set_power(int enable);

/* Autocalculate framebuffer sizes */

#define TEGRA_ROUND_ALLOC(x) (((x) + 4095) & ((unsigned)(-4096)))
/*Framebuffer Size for default Gtablet Panel*/
#define SMBA1002_FB_SIZE TEGRA_ROUND_ALLOC(1024*600*(32/8)*SMBA1002_FB_PAGES)

#if defined(SMBA1002_1920x1080HDMI)
#define SMBA1002_FB_HDMI_SIZE TEGRA_ROUND_ALLOC(1920*1080*(16/8)*2)
#else
#define SMBA1002_FB_HDMI_SIZE TEGRA_ROUND_ALLOC(1280*720*(16/8)*2)
#endif


#endif

