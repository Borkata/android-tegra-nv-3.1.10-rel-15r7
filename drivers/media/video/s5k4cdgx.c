/*
 * Driver for Samsung S5K4CDGX QXGA 1/4" 3.2M CMOS Image Sensor
 * with embedded SoC ISP.
 *
 * Copyright (C) 2013
 * Boris Popov <bvpopov<at>gmail.com>
 *
 * Based on s5k6aa
 * Copyright (C) 2011, Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define DEBUG 1

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/media.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>

#include <media/soc_camera.h>
#include <media/media-entity.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-mediabus.h>
#include <media/s5k4cdgx.h>

#include "s5k4cdgx_regs.h"


static int debug = 2;
module_param(debug, int, 0644);

#define DRIVER_NAME			"S5K4CDGX"

/* The token to indicate array termination */
#define S5K4CDGX_TERM			0xffffffff
#define S5K4CDGX_OUT_WIDTH_DEF		640
#define S5K4CDGX_OUT_HEIGHT_DEF		480
#define S5K4CDGX_WIN_WIDTH_MAX		2048
#define S5K4CDGX_WIN_HEIGHT_MAX		1536
#define S5K4CDGX_WIN_WIDTH_MIN		8
#define S5K4CDGX_WIN_HEIGHT_MIN		8

/*
 * H/W register Interface (0xD0000000 - 0xD0000FFF)
 */
#define AHB_MSB_ADDR_PTR		0xfcfc
#define GEN_REG_OFFSH			0xd000
#define REG_CMDWR_ADDRH			0x0028
#define REG_CMDWR_ADDRL			0x002a
#define REG_CMDRD_ADDRH			0x002c
#define REG_CMDRD_ADDRL			0x002e
#define REG_CMDBUF0_ADDR		0x0f12
#define REG_CMDBUF1_ADDR		0x0f10

/*
 * Host S/W Register interface (0x70000000 - 0x70002000)
 * The value of the two most significant address bytes is 0x7000,
 * (HOST_SWIF_OFFS_H). The register addresses below specify 2 LSBs.
 */
#define HOST_SWIF_OFFSH			0x7000

/* Initialization parameters */
/* Master clock frequency in KHz */
#define REG_I_INCLK_FREQ_L		0x7000023c
#define REG_I_INCLK_FREQ_H		0x7000023e
#define  MIN_MCLK_FREQ_KHZ		6000U
#define  MAX_MCLK_FREQ_KHZ		27000U
#define REG_I_USE_NPVI_CLOCKS		0x70000256
#define REG_I_USE_NMIPI_CLOCKS		0x70000258

/* Clock configurations, n = 0..2. REG_I_* frequency unit is 4 kHz. */
#define REG_I_OPCLK_4KHZ(n)		((n) * 6 + 0x7000025e)
#define REG_I_MIN_OUTRATE_4KHZ(n)	((n) * 6 + 0x70000260)
#define REG_I_MAX_OUTRATE_4KHZ(n)	((n) * 6 + 0x70000262)
#define  SYS_PLL_OUT_FREQ		(48000000 / 4000)
#define  PCLK_FREQ_MIN			(24000000 / 4000)
#define  PCLK_FREQ_MAX			(48000000 / 4000)
#define REG_I_INIT_PARAMS_UPDATED	0x70000272
#define REG_I_ERROR_INFO		0x70000274//?
#define REG_I_DBG_REINITCMD		0x70000532

/* General purpose parameters */
#define REG_USER_BRIGHTNESS		0x70000276//?
#define REG_USER_CONTRAST		0x70000278//?
#define REG_USER_SATURATION		0x7000027a//?
#define REG_USER_SHARPBLUR		0x7000027c//?

#define REG_G_SPEC_EFFECTS		0x70000286
#define REG_G_ENABLE_PREV		0x70000288
#define REG_G_ENABLE_PREV_CHG		0x7000028a
#define REG_G_NEW_CFG_SYNC		0x70000298
#define REG_G_PREVZOOM_IN_WIDTH		0x7000029a//?
#define REG_G_PREVZOOM_IN_HEIGHT	0x7000029c//?
#define REG_G_PREVZOOM_IN_XOFFS		0x7000029e//?
#define REG_G_PREVZOOM_IN_YOFFS		0x700002a0//?
#define REG_G_INPUTS_CHANGE_REQ		0x700002aa//?
#define REG_G_ACTIVE_PREV_CFG		0x700002ac
#define REG_G_ACTIVE_CAP_CFG            0x700002b4 //not used yet
#define REG_G_PREV_CFG_CHG		0x700002ae
#define REG_G_PREV_OPEN_AFTER_CH	0x700002b0
#define REG_G_PREV_CFG_ERROR		0x700002b2//?

/* Preview control section. n = 0...4. */
#define PREG(n, x)			((n) * 0x26 + x) //??
#define REG_P_OUT_WIDTH(n)		PREG(n, 0x700002e6)
#define REG_P_OUT_HEIGHT(n)		PREG(n, 0x700002e8)
#define REG_P_FMT(n)			PREG(n, 0x700002ea)
#define REG_P_MAX_OUT_RATE(n)		PREG(n, 0x700002ec)
#define REG_P_MIN_OUT_RATE(n)		PREG(n, 0x700002ee)
#define REG_P_PVI_MASK(n)		PREG(n, 0x700002f4)
#define REG_P_CLK_INDEX(n)		PREG(n, 0x700002fc)
#define REG_P_FR_RATE_TYPE(n)		PREG(n, 0x700002fe)
#define  FR_RATE_DYNAMIC		0
#define  FR_RATE_FIXED			1
#define  FR_RATE_FIXED_ACCURATE		2
#define REG_P_FR_RATE_Q_TYPE(n)		PREG(n, 0x70000300)
#define  FR_RATE_Q_BEST_FRRATE		1 /* Binning enabled */
#define  FR_RATE_Q_BEST_QUALITY		2 /* Binning disabled */
/* Frame period in 0.1 ms units */
#define REG_P_MAX_FR_TIME(n)		PREG(n, 0x70000302)
#define REG_P_MIN_FR_TIME(n)		PREG(n, 0x70000304)
/* Conversion to REG_P_[MAX/MIN]_FR_TIME value; __t: time in us */
#define  US_TO_FR_TIME(__t)		((__t) / 100)
#define  S5K4CDGX_MIN_FR_TIME		33300  /* us */
#define  S5K4CDGX_MAX_FR_TIME		650000 /* us */
#define  S5K4CDGX_MAX_HIGHRES_FR_TIME	666    /* x100 us */
/* The below 5 registers are for "device correction" values */
#define REG_P_COLORTEMP(n)		PREG(n, 0x7000030c) //?
#define REG_P_PREV_MIRROR(n)		PREG(n, 0x70000310)
#define REG_P_CAP_MIRROR(n)		PREG(n, 0x70000312) //not used yet

/* Extended image property controls */
/* Exposure time in 10 us units */
#define REG_SF_USR_EXPOSURE_L		0x700004f0//?
#define REG_SF_USR_EXPOSURE_H		0x700004f2//?
#define REG_SF_USR_EXPOSURE_CHG		0x700004f4//?
#define REG_SF_USR_TOT_GAIN		0x700004f6//?
#define REG_SF_USR_TOT_GAIN_CHG		0x700004f8//?
#define REG_SF_RGAIN			0x700004fa
#define REG_SF_RGAIN_CHG		0x700004fc
#define REG_SF_GGAIN			0x700004fe
#define REG_SF_GGAIN_CHG		0x70000500
#define REG_SF_BGAIN			0x70000502
#define REG_SF_BGAIN_CHG		0x70000504
#define REG_SF_FLICKER_QUANT		0x70000514
#define REG_SF_FLICKER_QUANT_CHG	0x70000516

/* Auto-algorithms enable mask */
#define REG_DBG_AUTOALG_EN		0x7000052e
#define  AALG_ALL_EN_MASK		(1 << 0)
#define  AALG_AE_EN_MASK		(1 << 1)
#define  AALG_DIVLEI_EN_MASK		(1 << 2)
#define  AALG_WB_EN_MASK		(1 << 3)
#define  AALG_FLICKER_EN_MASK		(1 << 5)
#define  AALG_FIT_EN_MASK		(1 << 6)
#define  AALG_WRHW_EN_MASK		(1 << 7)

/* Firmware revision information */
#define S5K4CDGX_CHIP_ID		0x4cd
#define REG_CHIP_ID			0x00000040
#define REG_MODEL_ID			0xD000100C
#define REG_GPIO_MODE_SEL		0xD000108E
#define REG_GPIO_FUNC_SEL		0xD0001090

/* For now we use only one user configuration register set */
#define S5K4CDGX_MAX_PRESETS		1

static const char * const s5k4cdgx_supply_names[] = {
	"vdd_core",	/* Digital core supply 1.5V (1.4V to 1.6V) */
	"vdda",		/* Analog power supply 2.8V (2.6V to 3.0V) */
	"vdd_reg",	/* Regulator input power 1.8V (1.7V to 1.9V)
			   or 2.8V (2.6V to 3.0) */
	"vddio",	/* I/O supply 1.8V (1.65V to 1.95V)
			   or 2.8V (2.5V to 3.1V) */
};
#define S5K4CDGX_NUM_SUPPLIES ARRAY_SIZE(s5k4cdgx_supply_names)

enum s5k4cdgx_gpio_id {
	STBY,
	RST,
	GPIO_NUM,
};

struct s5k4cdgx_regval {
	u16 addr;
	u16 val;
};

struct s5k4cdgx_pixfmt {
	enum v4l2_mbus_pixelcode code;
	u32 colorspace;
	/* REG_P_FMT(x) register value */
	u16 reg_p_fmt;
};

struct s5k4cdgx_preset {
	/* output pixel format and resolution */
	struct v4l2_mbus_framefmt mbus_fmt;
	u8 clk_id;
	u8 index;
};

struct s5k4cdgx_ctrls {
	struct v4l2_ctrl_handler handler;
	/* Auto / manual white balance cluster */
	struct v4l2_ctrl *awb;
	struct v4l2_ctrl *gain_red;
	struct v4l2_ctrl *gain_blue;
	struct v4l2_ctrl *gain_green;
	/* Mirror cluster */
	struct v4l2_ctrl *hflip;
	struct v4l2_ctrl *vflip;
	/* Auto exposure / manual exposure and gain cluster */
	struct v4l2_ctrl *auto_exp;
	struct v4l2_ctrl *exposure;
	struct v4l2_ctrl *gain;
};

struct s5k4cdgx_interval {
	u16 reg_fr_time;
	u16 fr_rate;
	struct v4l2_fract interval;
	/* Maximum rectangle for the interval */
	struct v4l2_frmsize_discrete size;
};

struct s5k4cdgx {
	struct v4l2_subdev sd;
	struct media_pad pad;

	enum v4l2_mbus_type bus_type;
	u8 mipi_lanes;

	int (*s_power)(int enable);
	struct regulator_bulk_data supplies[S5K4CDGX_NUM_SUPPLIES];
	struct s5k4cdgx_gpio gpio[GPIO_NUM];

	/* external master clock frequency */
	unsigned long mclk_frequency;
	/* ISP internal master clock frequency */
	u16 clk_fop;
	/* output pixel clock frequency range */
	u16 pclk_fmin;
	u16 pclk_fmax;

	unsigned int inv_hflip:1;
	unsigned int inv_vflip:1;

	/* protects the struct members below */
	struct mutex lock;

	/* sensor matrix scan window */
	struct v4l2_rect ccd_rect;

	struct s5k4cdgx_ctrls ctrls;
	struct s5k4cdgx_preset presets[S5K4CDGX_MAX_PRESETS];
	struct s5k4cdgx_preset *preset;
	const struct s5k4cdgx_interval *fiv;

	unsigned int streaming:1;
	unsigned int apply_cfg:1;
	unsigned int apply_crop:1;
	unsigned int power;
};

/* TODO: Add RGB888 and Bayer format */
static const struct s5k4cdgx_pixfmt s5k4cdgx_formats[] = {
	{ V4L2_MBUS_FMT_YUYV8_2X8, V4L2_COLORSPACE_JPEG, 5 },
	/* range 16-240 */
//	{ V4L2_MBUS_FMT_YUYV8_2X8,	V4L2_COLORSPACE_REC709,	6 },
//	{ V4L2_MBUS_FMT_RGB565_2X8_BE,	V4L2_COLORSPACE_JPEG,	0 },
};

static const struct v4l2_frmsize_discrete s5k4cdgx_frame_sizes[] = {
	{2048, 1536},
	{1280, 1024},
	{1280, 720},
	{800, 600}, /* SVGA */
	{640, 480}, /* VGA */
	{352, 288}, /* CIF */
	{320, 240}, /* QVGA */
	{176, 144}, /* QCIF */
};

static const struct s5k4cdgx_interval s5k4cdgx_intervals[] = {
	{ 1401, 7, {7138, 1000000}, {2048, 1536} }, /*  7.138 fps */
	{ 666, 15, {15015, 1000000}, {2048, 1536} }, /* 15.015 fps */
	{ 500, 20, {20000, 1000000}, {1280, 720} },  /* 20 fps, HD720 */
	{ 500, 20, {20000, 1000000}, {800, 600} },
	{ 400, 25, {25000, 1000000}, {640, 480} },   /* 25 fps */
	{ 333, 30, {33300, 1000000}, {640, 480} }, /* 29.940 fps */
	{ 333, 30, {33300, 1000000}, {352, 288} },   /* CIF */
	{ 333, 30, {33300, 1000000}, {320, 240} },   /* QVGA */
	{ 333, 30, {33300, 1000000}, {176, 144} },   /* QCIF */
};

#define S5K4CDGX_INTERVAL_DEF_INDEX 1

static inline struct v4l2_subdev *ctrl_to_sd(struct v4l2_ctrl *ctrl)
{
	return &container_of(ctrl->handler, struct s5k4cdgx, ctrls.handler)->sd;
}

static inline struct s5k4cdgx *to_s5k4cdgx(struct v4l2_subdev *sd)
{
	return container_of(sd, struct s5k4cdgx, sd);
}

/* Set initial values for all preview presets */
static void s5k4cdgx_presets_data_init(struct s5k4cdgx *s5k4cdgx)
{
	struct s5k4cdgx_preset *preset = &s5k4cdgx->presets[0];
	int i;

	for (i = 0; i < S5K4CDGX_MAX_PRESETS; i++) {
		preset->mbus_fmt.width	= S5K4CDGX_OUT_WIDTH_DEF;
		preset->mbus_fmt.height	= S5K4CDGX_OUT_HEIGHT_DEF;
		preset->mbus_fmt.code	= s5k4cdgx_formats[0].code;
		preset->index		= i;
		preset->clk_id		= 0;
		preset++;
	}

	s5k4cdgx->fiv = &s5k4cdgx_intervals[S5K4CDGX_INTERVAL_DEF_INDEX];
	s5k4cdgx->preset = &s5k4cdgx->presets[0];
}

static int s5k4cdgx_i2c_read(struct i2c_client *client, u16 addr, u16 *val)
{
	u8 wbuf[2] = {addr >> 8, addr & 0xFF};
	struct i2c_msg msg[2];
	u8 rbuf[2];
	int ret;

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 2;
	msg[0].buf = wbuf;

	msg[1].addr = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 2;
	msg[1].buf = rbuf;

	ret = i2c_transfer(client->adapter, msg, 2);
	*val = be16_to_cpu(*((u16 *)rbuf));

	v4l2_dbg(3, debug, client, "i2c_read: 0x%04X : 0x%04x, i2ctrans_ret: %d\n", addr, *val, ret);

	return ret == 2 ? 0 : ret;
}

static int s5k4cdgx_i2c_write(struct i2c_client *client, u16 addr, u16 val)
{
	u8 buf[4] = {addr >> 8, addr & 0xFF, val >> 8, val & 0xFF};

	int ret = i2c_master_send(client, buf, 4);
	v4l2_dbg(3, debug, client, "i2c_write: 0x%04X : 0x%04x\n", addr, val);

	return ret == 4 ? 0 : ret;
}

static int s5k4cdgx_write(struct i2c_client *client, u32 addr, u16 val)
{
	u16 high = addr >> 16, low =  addr & 0xffff;
	int ret;
	
	v4l2_dbg(3, debug, client, "write: 0x%08x : 0x%04x\n", addr, val);
	
	ret = s5k4cdgx_i2c_write(client, REG_CMDWR_ADDRH, high);
	if (!ret)
		ret = s5k4cdgx_i2c_write(client, REG_CMDWR_ADDRL, low);
	if (!ret)
		ret = s5k4cdgx_i2c_write(client, REG_CMDBUF0_ADDR, val);
		
	return ret;
}

static int s5k4cdgx_write_regs(struct v4l2_subdev *sd,
					struct s5k4cdgx_request table[], int size)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
        u8 buffer[2500];
	u8 *ptr = &buffer[2];
	int err = 0;
	int i = 0;

	buffer[0] = S5K4CDGX_DATA_MAGIC >> 8;
	buffer[1] = S5K4CDGX_DATA_MAGIC & 0xff;

	for (i = 0; i < size && err >= 0; ++i) {
		switch (table[i].subaddr) {
		case S5K4CDGX_BANK_MAGIC:
		case S5K4CDGX_PAGE_MAGIC:
		case S5K4CDGX_REG_MAGIC:
			if (ptr != &buffer[2]) {
				/* write in burst mode */
				err = i2c_master_send(client,
							buffer, ptr - buffer);
				ptr = &buffer[2];
				if (err < 0)
					break;
			}
			/* Set Address */
			err = s5k4cdgx_i2c_write(client,
					table[i].subaddr, table[i].value);
			break;
		case S5K4CDGX_DATA_MAGIC:
			/* make and fill buffer for burst mode write */
			*ptr++ = table[i].value >> 8;
			*ptr++ = table[i].value & 0xff;
			break;
		case S5K4CDGX_MSLEEP_MAGIC:
			msleep(table[i].value);
			break;
		}
	}

	if (ptr != &buffer[2])
		/* write in burst mode */
		err = i2c_master_send(client, buffer, ptr - buffer);

	if (unlikely(err < 0)) {
		v4l_err(client, "%s: register set failed\n", __func__);
		return err;
	}

	return 0;
}

static int s5k4cdgx_read(struct i2c_client *client, u32 addr, u16 *val)
{
	u16 high = addr >> 16, low =  addr & 0xffff;
	int ret;
	
	ret = s5k4cdgx_i2c_write(client, REG_CMDRD_ADDRH, high);
	if (!ret)
		ret = s5k4cdgx_i2c_write(client, REG_CMDRD_ADDRL, low);
	if (!ret)
		ret = s5k4cdgx_i2c_read(client, REG_CMDBUF0_ADDR, val);
	if (ret)
		dev_err(&client->dev, "Failed to execute read command 0x%x\n",val);
		
	return ret;
}

static int s5k4cdgx_set_arm_go(struct i2c_client *client)
{
        int ret = s5k4cdgx_i2c_write(client, AHB_MSB_ADDR_PTR, GEN_REG_OFFSH);
        if (ret < 0)
                return ret;

    /*
     * sw_reset is activated to put device into idle status
     */
    ret = s5k4cdgx_i2c_write(client, 0x0010, 0x0001);
    if (ret < 0)
        return ret;

    ret = s5k4cdgx_i2c_write(client, 0x1030, 0x0000);
    if (ret < 0)
        return ret;
    /* Halt ARM CPU */
    return s5k4cdgx_i2c_write(client, 0x0014, 0x0001);

}

/* Configure the AHB high address bytes for GTG registers access */
static int s5k4cdgx_set_ahb_address(struct i2c_client *client)
{
	int ret = s5k4cdgx_i2c_write(client, AHB_MSB_ADDR_PTR, GEN_REG_OFFSH);
	if (ret < 0)
		return ret;
	
	ret = s5k4cdgx_i2c_write(client, REG_CMDRD_ADDRH, HOST_SWIF_OFFSH);
	if (ret)
		return ret;
	return s5k4cdgx_i2c_write(client, REG_CMDWR_ADDRH, HOST_SWIF_OFFSH);
}

/**
 * s5k4cdgx_configure_pixel_clock - apply ISP main clock/PLL configuration
 *
 * Configure the internal ISP PLL for the required output frequency.
 * Locking: called with s5k4cdgx.lock mutex held.
 */
static int s5k4cdgx_configure_pixel_clocks(struct s5k4cdgx *s5k4cdgx)
{
	struct i2c_client *c = v4l2_get_subdevdata(&s5k4cdgx->sd);
	unsigned long fmclk = s5k4cdgx->mclk_frequency / 1000;
	u16 status;
	int ret;

	if (WARN(fmclk < MIN_MCLK_FREQ_KHZ || fmclk > MAX_MCLK_FREQ_KHZ,
		 "Invalid clock frequency: %ld\n", fmclk))
		return -EINVAL;

	s5k4cdgx->pclk_fmin = PCLK_FREQ_MIN;
	s5k4cdgx->pclk_fmax = PCLK_FREQ_MAX;
	s5k4cdgx->clk_fop = SYS_PLL_OUT_FREQ;

	/* External input clock frequency in kHz */
	ret = s5k4cdgx_write(c, REG_I_INCLK_FREQ_H, fmclk >> 16);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_I_INCLK_FREQ_L, fmclk & 0xFFFF);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_I_USE_NPVI_CLOCKS, 1);
	/* Internal PLL frequency */
	if (!ret)
		ret = s5k4cdgx_write(c, REG_I_OPCLK_4KHZ(0), s5k4cdgx->clk_fop);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_I_MIN_OUTRATE_4KHZ(0),
				   s5k4cdgx->pclk_fmin);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_I_MAX_OUTRATE_4KHZ(0),
				   s5k4cdgx->pclk_fmax);
/*        if (!ret)
                ret = s5k4cdgx_write(c, REG_I_OPCLK_4KHZ(1), s5k4cdgx->clk_fop);
        if (!ret)
                ret = s5k4cdgx_write(c, REG_I_MIN_OUTRATE_4KHZ(1),
                                   s5k4cdgx->pclk_fmin);
        if (!ret)
                ret = s5k4cdgx_write(c, REG_I_MAX_OUTRATE_4KHZ(1),
                                   s5k4cdgx->pclk_fmax);*/
	if (!ret)
		ret = s5k4cdgx_write(c, REG_I_INIT_PARAMS_UPDATED, 1);

	v4l2_info(&s5k4cdgx->sd, "Call read reg from s5k4cdgx_configure_pixel_clocks");
	if (!ret)
		ret = s5k4cdgx_read(c, REG_I_ERROR_INFO, &status);

	if(status)
		pr_err("%s: firmware indicated a clocking error (%d), try power-cycling.\n",
		       __func__, status);

	//return ret ? ret : (status ? -EINVAL : 0);
	return 0;
}

/* Set horizontal and vertical image flipping */
static int s5k4cdgx_set_mirror(struct s5k4cdgx *s5k4cdgx, int horiz_flip)
{
	struct i2c_client *client = v4l2_get_subdevdata(&s5k4cdgx->sd);
	int index = s5k4cdgx->preset->index;

	unsigned int vflip = s5k4cdgx->ctrls.vflip->val ^ s5k4cdgx->inv_vflip;
	unsigned int flip = (horiz_flip ^ s5k4cdgx->inv_hflip) | (vflip << 1);

	return s5k4cdgx_write(client, REG_P_PREV_MIRROR(index), flip);
}

/* Configure auto/manual white balance and R/G/B gains */
static int s5k4cdgx_set_awb(struct s5k4cdgx *s5k4cdgx, int awb)
{
	struct i2c_client *c = v4l2_get_subdevdata(&s5k4cdgx->sd);
	struct s5k4cdgx_ctrls *ctrls = &s5k4cdgx->ctrls;
	u16 reg;

	v4l2_info(&s5k4cdgx->sd, "Call read reg from set_awb");
	int ret = s5k4cdgx_read(c, REG_DBG_AUTOALG_EN, &reg);

	if (!ret && !awb) {
		ret = s5k4cdgx_write(c, REG_SF_RGAIN, ctrls->gain_red->val);
		if (!ret)
			ret = s5k4cdgx_write(c, REG_SF_RGAIN_CHG, 1);
		if (ret)
			return ret;

		ret = s5k4cdgx_write(c, REG_SF_GGAIN, ctrls->gain_green->val);
		if (!ret)
			ret = s5k4cdgx_write(c, REG_SF_GGAIN_CHG, 1);
		if (ret)
			return ret;

		ret = s5k4cdgx_write(c, REG_SF_BGAIN, ctrls->gain_blue->val);
		if (!ret)
			ret = s5k4cdgx_write(c, REG_SF_BGAIN_CHG, 1);
	}
	if (!ret) {
		reg = awb ? reg | AALG_WB_EN_MASK : reg & ~AALG_WB_EN_MASK;
		ret = s5k4cdgx_write(c, REG_DBG_AUTOALG_EN, reg);
	}

	return ret;
}

/* Program FW with exposure time, 'exposure' in us units */
static int s5k4cdgx_set_user_exposure(struct i2c_client *client, int exposure)
{
	unsigned int time = exposure / 10;

	int ret = s5k4cdgx_write(client, REG_SF_USR_EXPOSURE_L, time & 0xffff);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_SF_USR_EXPOSURE_H, time >> 16);
	if (ret)
		return ret;
	return s5k4cdgx_write(client, REG_SF_USR_EXPOSURE_CHG, 1);
}

static int s5k4cdgx_set_user_gain(struct i2c_client *client, int gain)
{
	int ret = s5k4cdgx_write(client, REG_SF_USR_TOT_GAIN, gain);
	if (ret)
		return ret;
	return s5k4cdgx_write(client, REG_SF_USR_TOT_GAIN_CHG, 1);
}

/* Set auto/manual exposure and total gain */
static int s5k4cdgx_set_auto_exposure(struct s5k4cdgx *s5k4cdgx, int value)
{
	struct i2c_client *c = v4l2_get_subdevdata(&s5k4cdgx->sd);
	unsigned int exp_time = s5k4cdgx->ctrls.exposure->val;
	u16 auto_alg;

	v4l2_info(&s5k4cdgx->sd, "Call read reg from set_auto_exposure");
	int ret = s5k4cdgx_read(c, REG_DBG_AUTOALG_EN, &auto_alg);
	if (ret)
		return ret;

	v4l2_dbg(1, debug, c, "man_exp: %d, auto_exp: %d, a_alg: 0x%x\n",
		 exp_time, value, auto_alg);

	if (value == V4L2_EXPOSURE_AUTO) {
		auto_alg |= AALG_AE_EN_MASK | AALG_DIVLEI_EN_MASK;
	} else {
		ret = s5k4cdgx_set_user_exposure(c, exp_time);
		if (ret)
			return ret;
		ret = s5k4cdgx_set_user_gain(c, s5k4cdgx->ctrls.gain->val);
		if (ret)
			return ret;
		auto_alg &= ~(AALG_AE_EN_MASK | AALG_DIVLEI_EN_MASK);
	}

	return s5k4cdgx_write(c, REG_DBG_AUTOALG_EN, auto_alg);
}

static int s5k4cdgx_set_anti_flicker(struct s5k4cdgx *s5k4cdgx, int value)
{
	struct i2c_client *client = v4l2_get_subdevdata(&s5k4cdgx->sd);
	u16 auto_alg;
	int ret;

	v4l2_info(&s5k4cdgx->sd, "Call read reg from set_anti_flicker");
	ret = s5k4cdgx_read(client, REG_DBG_AUTOALG_EN, &auto_alg);
	if (ret) {
		v4l2_err(&s5k4cdgx->sd, "[S5K4CDGX] cannot read REG_DBG_AUTOALG_EN");
		//return ret;
	}

	if (value == V4L2_CID_POWER_LINE_FREQUENCY_AUTO) {
		auto_alg |= AALG_FLICKER_EN_MASK;
	} else {
		auto_alg &= ~AALG_FLICKER_EN_MASK;
		/* The V4L2_CID_LINE_FREQUENCY control values match
		 * the register values */
		ret = s5k4cdgx_write(client, REG_SF_FLICKER_QUANT, value);
		if (ret)
			return ret;
		ret = s5k4cdgx_write(client, REG_SF_FLICKER_QUANT_CHG, 1);
		if (ret)
			return ret;
	}

	return s5k4cdgx_write(client, REG_DBG_AUTOALG_EN, auto_alg);
}

static int s5k4cdgx_set_colorfx(struct s5k4cdgx *s5k4cdgx, int val)
{
	struct i2c_client *client = v4l2_get_subdevdata(&s5k4cdgx->sd);
	static const struct v4l2_control colorfx[] = {
		{ V4L2_COLORFX_NONE,	 0 },
		{ V4L2_COLORFX_BW,	 1 },
		{ V4L2_COLORFX_NEGATIVE, 2 },
		{ V4L2_COLORFX_SEPIA,	 3 },
		{ V4L2_COLORFX_SKY_BLUE, 4 },
		{ V4L2_COLORFX_SKETCH,	 5 },
	};
	int i;

	for (i = 0; i < ARRAY_SIZE(colorfx); i++) {
		if (colorfx[i].id == val)
			return s5k4cdgx_write(client, REG_G_SPEC_EFFECTS,
					    colorfx[i].value);
	}
	return -EINVAL;
}

static int s5k4cdgx_preview_config_status(struct i2c_client *client)
{
	int ret;
	u16 error = 0;

	ret = s5k4cdgx_write(client, REG_I_DBG_REINITCMD, 1);
	if (!ret)
	    ret = s5k4cdgx_write(client, REG_I_DBG_REINITCMD, 1);
	if (!ret)
	    ret = s5k4cdgx_write(client, REG_I_DBG_REINITCMD, 1);

	v4l2_info(client, "Call read reg from preview_config_status");
	ret = s5k4cdgx_read(client, REG_G_PREV_CFG_ERROR, &error);

	v4l2_dbg(1, debug, client, "preview config status error: 0x%x (%d)\n", error, ret);
	return ret ? ret : (error ? -EINVAL : 0);
}

static int s5k4cdgx_get_pixfmt_index(struct s5k4cdgx *s5k4cdgx,
				   struct v4l2_mbus_framefmt *mf)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(s5k4cdgx_formats); i++)
		if (mf->colorspace == s5k4cdgx_formats[i].colorspace &&
		    mf->code == s5k4cdgx_formats[i].code)
			return i;
	return 0;
}

static int s5k4cdgx_set_output_framefmt(struct s5k4cdgx *s5k4cdgx,
				      struct s5k4cdgx_preset *preset)
{
	struct i2c_client *client = v4l2_get_subdevdata(&s5k4cdgx->sd);
	int fmt_index = s5k4cdgx_get_pixfmt_index(s5k4cdgx, &preset->mbus_fmt);
	int ret;

	ret = s5k4cdgx_write(client, REG_P_OUT_WIDTH(preset->index),
			   preset->mbus_fmt.width);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_OUT_HEIGHT(preset->index),
				   preset->mbus_fmt.height);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_FMT(preset->index),
				   s5k4cdgx_formats[fmt_index].reg_p_fmt);
        if (!ret)
                ret = s5k4cdgx_write(client, REG_I_INIT_PARAMS_UPDATED, 1);

	return ret;
}

static int s5k4cdgx_set_input_params(struct s5k4cdgx *s5k4cdgx)
{
	struct i2c_client *c = v4l2_get_subdevdata(&s5k4cdgx->sd);
	struct v4l2_rect *r = &s5k4cdgx->ccd_rect;
	int ret;

	ret = s5k4cdgx_write(c, REG_G_PREVZOOM_IN_WIDTH, r->width);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_G_PREVZOOM_IN_HEIGHT, r->height);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_G_PREVZOOM_IN_XOFFS, r->left);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_G_PREVZOOM_IN_YOFFS, r->top);
	if (!ret)
		ret = s5k4cdgx_write(c, REG_G_INPUTS_CHANGE_REQ, 1);
	if (!ret)
		s5k4cdgx->apply_crop = 0;

	return ret;
}

/* This function should be called when switching to new user configuration set*/
static int s5k4cdgx_new_config_sync(struct i2c_client *client, int timeout,
				  int cid)
{
	unsigned long end = jiffies + msecs_to_jiffies(timeout);
	u16 reg = 1;
	int ret;

	ret = s5k4cdgx_write(client, REG_G_ACTIVE_PREV_CFG, cid);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_G_PREV_CFG_CHG, 1);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_G_NEW_CFG_SYNC, 1);
	if (timeout == 0)
		return ret;

	while (ret >= 0 && time_is_after_jiffies(end)) {
		v4l2_info(client, "Call read reg from new_config_sync");
		ret = s5k4cdgx_read(client, REG_G_NEW_CFG_SYNC, &reg);
		if (!reg)
			return 0;
		usleep_range(1000, 5000);
	}
	return ret ? ret : -ETIMEDOUT;
}

/**
 * s5k4cdgx_set_prev_config - write user preview register set
 *
 * Configure output resolution and color fromat, pixel clock
 * frequency range, device frame rate type and frame period range.
 */
static int s5k4cdgx_set_prev_config(struct s5k4cdgx *s5k4cdgx,
				  struct s5k4cdgx_preset *preset)
{
	struct i2c_client *client = v4l2_get_subdevdata(&s5k4cdgx->sd);
	int idx = preset->index;
	u16 frame_rate_q;
	int ret;

	if (s5k4cdgx->fiv->reg_fr_time >= S5K4CDGX_MAX_HIGHRES_FR_TIME)
		frame_rate_q = FR_RATE_Q_BEST_FRRATE;
	else
		frame_rate_q = FR_RATE_Q_BEST_QUALITY;

	ret = s5k4cdgx_set_output_framefmt(s5k4cdgx, preset);
	if (!ret)
        ret = s5k4cdgx_write(client, REG_P_PVI_MASK(idx),
					0x42);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_MAX_OUT_RATE(idx),
				   s5k4cdgx->pclk_fmax);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_MIN_OUT_RATE(idx),
				   s5k4cdgx->pclk_fmin);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_CLK_INDEX(idx),
				   preset->clk_id);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_FR_RATE_TYPE(idx),
				   FR_RATE_DYNAMIC);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_FR_RATE_Q_TYPE(idx),
				   frame_rate_q);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_MAX_FR_TIME(idx),
				   s5k4cdgx->fiv->reg_fr_time + 33);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_P_MIN_FR_TIME(idx),
				   s5k4cdgx->fiv->reg_fr_time - 33);
	if (!ret)
		ret = s5k4cdgx_new_config_sync(client, 250, idx);
	if (!ret)
		ret = s5k4cdgx_preview_config_status(client);
	if (!ret)
		s5k4cdgx->apply_cfg = 0;

	v4l2_dbg(1, debug, client, "Frame interval: %d +/- 3.3ms. (%d)\n",
		 s5k4cdgx->fiv->reg_fr_time, ret);
	return ret;
}

/**
 * s5k4cdgx_initialize_isp - basic ISP MCU initialization
 *
 * Configure AHB addresses for registers read/write; configure PLLs for
 * required output pixel clock. The ISP power supply needs to be already
 * enabled, with an optional H/W reset.
 * Locking: called with s5k4cdgx.lock mutex held.
 */
static int s5k4cdgx_initialize_isp(struct v4l2_subdev *sd)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	int ret;

	s5k4cdgx->apply_crop = 1;
	s5k4cdgx->apply_cfg = 1;

	ret = s5k4cdgx_set_arm_go(client);
	if (ret)
		return ret;

	ret = s5k4cdgx_set_ahb_address(client);
	if (ret)
		return ret;

	msleep(100);
 
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_io_driving_current, ARRAY_SIZE(s5k4cdgx_init_reg_io_driving_current));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for IO Driving Current\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_trap_and_patch, ARRAY_SIZE(s5k4cdgx_init_reg_trap_and_patch));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for Trap&Patch\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_af, ARRAY_SIZE(s5k4cdgx_init_reg_af));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for AF\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_factory, ARRAY_SIZE(s5k4cdgx_init_reg_factory));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for CIS/APS/Analog\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_frame_rate, ARRAY_SIZE(s5k4cdgx_init_reg_frame_rate));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for Frame Rate\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_pll, ARRAY_SIZE(s5k4cdgx_init_reg_pll));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for PLL\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_preview_conf0, ARRAY_SIZE(s5k4cdgx_init_preview_conf0));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for Preview Configuration 0\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_capture_conf0, ARRAY_SIZE(s5k4cdgx_init_capture_conf0));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for Capture Configuration 0\n", __func__);
		return ret;
	}	
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_config1, ARRAY_SIZE(s5k4cdgx_init_reg_config1));
	if (ret) {
		v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg 1\n", __func__);
		return ret;
	}

	msleep(10);	
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_jpeg, ARRAY_SIZE(s5k4cdgx_init_reg_jpeg));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for JPEG\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_gas, ARRAY_SIZE(s5k4cdgx_init_reg_gas));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for GAS (Grid Anti-shading)\n", __func__);
		return ret;
	}

	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_ccm, ARRAY_SIZE(s5k4cdgx_init_reg_ccm));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for CCM\n", __func__);
		return ret;
	}	

	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_awb, ARRAY_SIZE(s5k4cdgx_init_reg_awb));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for AWB\n", __func__);
		return ret;
	}	
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_ae, ARRAY_SIZE(s5k4cdgx_init_reg_ae));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for AE\n", __func__);
		return ret;
	}	
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_gamma_lut, ARRAY_SIZE(s5k4cdgx_init_reg_gamma_lut));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for Gamma LUT\n", __func__);
		return ret;
	}
	
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_afit, ARRAY_SIZE(s5k4cdgx_init_reg_afit));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg for AFIT table\n", __func__);
		return ret;
	}
	
	// Update changed registers in init reg config2 sequence
	ret = s5k4cdgx_write_regs(sd, s5k4cdgx_init_reg_config2, ARRAY_SIZE(s5k4cdgx_init_reg_config2));
	if (ret) {
        v4l2_err(sd, "[S5K4CDGX] %s function err in writing init reg 2\n", __func__);
		return ret;
	}
	
	

	msleep(20);

	//ret = s5k4cdgx_configure_pixel_clocks(s5k4cdgx);
	//if (ret) {
        //v4l2_err(sd, "[S5K4CDGX] %s function err in configure_pixel_clocks\n", __func__);
	//	return ret;
	//}
	
	return 0;
}

static int s5k4cdgx_gpio_set_value(struct s5k4cdgx *priv, int id, u32 val)
{
	if (!gpio_is_valid(priv->gpio[id].gpio))
		return 0;
	gpio_set_value(priv->gpio[id].gpio, !!val);
	return 1;
}

static int s5k4cdgx_gpio_assert(struct s5k4cdgx *priv, int id)
{
	return s5k4cdgx_gpio_set_value(priv, id, priv->gpio[id].level);
}

static int s5k4cdgx_gpio_deassert(struct s5k4cdgx *priv, int id)
{
	return s5k4cdgx_gpio_set_value(priv, id, !priv->gpio[id].level);
}

static int __s5k4cdgx_power_on(struct s5k4cdgx *s5k4cdgx)
{
	int ret;

	if(!s5k4cdgx) {
	  pr_err("%s: missing private data\n", __func__);
	  return -EINVAL;
	}

	ret = regulator_bulk_enable(S5K4CDGX_NUM_SUPPLIES, s5k4cdgx->supplies);
	if (ret)
		return ret;

	if (s5k4cdgx_gpio_deassert(s5k4cdgx, STBY))
		usleep_range(150, 200);

	if (s5k4cdgx->s_power)
		ret = s5k4cdgx->s_power(1);
	usleep_range(4000, 4000);

	if (s5k4cdgx_gpio_deassert(s5k4cdgx, RST))
		msleep(20);

	return ret;
}

static int __s5k4cdgx_power_off(struct s5k4cdgx *s5k4cdgx)
{
	int ret;

	if(!s5k4cdgx) {
	  pr_err("%s: missing private data\n", __func__);
	  return -EINVAL;
	}

	if (s5k4cdgx_gpio_assert(s5k4cdgx, RST))
		usleep_range(100, 150);

	if (s5k4cdgx->s_power) {
		ret = s5k4cdgx->s_power(0);
		if (ret)
			return ret;
	}
	if (s5k4cdgx_gpio_assert(s5k4cdgx, STBY))
		usleep_range(50, 100);
	s5k4cdgx->streaming = 0;

	return regulator_bulk_disable(S5K4CDGX_NUM_SUPPLIES, s5k4cdgx->supplies);
}

/*
 * V4L2 subdev core and video operations
 */
static int s5k4cdgx_set_power(struct v4l2_subdev *sd, int on)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	int ret = 0;

	mutex_lock(&s5k4cdgx->lock);

	if (!on == s5k4cdgx->power) {
		if (on) {
			ret = __s5k4cdgx_power_on(s5k4cdgx);
			if (!ret)
				ret = s5k4cdgx_initialize_isp(sd);
		} else {
			ret = __s5k4cdgx_power_off(s5k4cdgx);
		}

		if (!ret)
			s5k4cdgx->power += on ? 1 : -1;
	}

	mutex_unlock(&s5k4cdgx->lock);

	if (!on || ret || s5k4cdgx->power != 1)
		return ret;

	return v4l2_ctrl_handler_setup(sd->ctrl_handler);
}

static int __s5k4cdgx_stream(struct s5k4cdgx *s5k4cdgx, int enable)
{
	struct i2c_client *client = v4l2_get_subdevdata(&s5k4cdgx->sd);
	int ret = 0;

	ret = s5k4cdgx_write(client, REG_G_ENABLE_PREV, enable);
	if (!ret)
		ret = s5k4cdgx_write(client, REG_G_ENABLE_PREV_CHG, 1);
	if (!ret)
		s5k4cdgx->streaming = enable;

	return ret;
}

static int s5k4cdgx_s_stream(struct v4l2_subdev *sd, int on)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	int ret = 0;

	mutex_lock(&s5k4cdgx->lock);

	if (s5k4cdgx->streaming == !on) {
		if (!ret && s5k4cdgx->apply_cfg)
			ret = s5k4cdgx_set_prev_config(s5k4cdgx, s5k4cdgx->preset);
		if (!ret && s5k4cdgx->apply_crop)
			ret = s5k4cdgx_set_input_params(s5k4cdgx);
		if (!ret)
			ret = __s5k4cdgx_stream(s5k4cdgx, !!on);
	}
	mutex_unlock(&s5k4cdgx->lock);

	return ret;
}

static int s5k4cdgx_g_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *fi)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);

	mutex_lock(&s5k4cdgx->lock);
	fi->interval = s5k4cdgx->fiv->interval;
	mutex_unlock(&s5k4cdgx->lock);

	return 0;
}

static int __s5k4cdgx_set_frame_interval(struct s5k4cdgx *s5k4cdgx,
				       struct v4l2_fract *f)
{
	struct v4l2_mbus_framefmt *mbus_fmt = &s5k4cdgx->preset->mbus_fmt;
	const struct s5k4cdgx_interval *fiv = &s5k4cdgx_intervals[0];
	unsigned int err, min_err = UINT_MAX;
	unsigned int i, fr_time;

	if (f->denominator == 0)
		return -EINVAL;

	fr_time = f->numerator * 10000 / f->denominator;

	for (i = 0; i < ARRAY_SIZE(s5k4cdgx_intervals); i++) {
		const struct s5k4cdgx_interval *iv = &s5k4cdgx_intervals[i];

		if (mbus_fmt->width > iv->size.width ||
		    mbus_fmt->height > iv->size.height)
			continue;

		err = abs(iv->reg_fr_time - fr_time);
		if (err < min_err) {
			fiv = iv;
			min_err = err;
		}
	}
	s5k4cdgx->fiv = fiv;

	v4l2_dbg(1, debug, &s5k4cdgx->sd, "Changed frame interval to %d us\n",
		 fiv->reg_fr_time * 100);
	return 0;
}

static int s5k4cdgx_s_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *fi)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	int ret;

	v4l2_dbg(1, debug, sd, "Setting %d/%d frame interval\n",
		 fi->interval.numerator, fi->interval.denominator);

	mutex_lock(&s5k4cdgx->lock);
	ret = __s5k4cdgx_set_frame_interval(s5k4cdgx, &fi->interval);
	s5k4cdgx->apply_cfg = 1;

	mutex_unlock(&s5k4cdgx->lock);
	return ret;
}

/*
 * V4L2 subdev pad level and video operations
 */
static int s5k4cdgx_enum_frame_interval(struct v4l2_subdev *sd,
			      struct v4l2_frmivalenum *fie)
{
	int i, index;
	int ret = 0;

	index = fie->index;
	for(i=0;i<ARRAY_SIZE(s5k4cdgx_intervals);i++) {
		if(s5k4cdgx_intervals[i].size.width == fie->width &&
		   s5k4cdgx_intervals[i].size.height == fie->height) {
			if(index-- == 0) {
				break;
			}
		}
	}
	if(index != -1) {
		/* index for this frame size not found */
		return -EINVAL;
	}

	fie->type = V4L2_FRMIVAL_TYPE_DISCRETE;
	fie->discrete.numerator = 1;
	fie->discrete.denominator = s5k4cdgx_intervals[i].fr_rate;

	return ret;
}

static int s5k4cdgx_enum_mbus_code(struct v4l2_subdev *sd,
				 struct v4l2_subdev_fh *fh,
				 struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->index >= ARRAY_SIZE(s5k4cdgx_formats))
		return -EINVAL;

	code->code = s5k4cdgx_formats[code->index].code;
	return 0;
}

static int s5k4cdgx_enum_mbus_fmt(struct v4l2_subdev *sd, unsigned int index,
			   enum v4l2_mbus_pixelcode *code)
{
	if (index >= ARRAY_SIZE(s5k4cdgx_formats))
		return -EINVAL;

	*code = s5k4cdgx_formats[index].code;
	return 0;
}

static int s5k4cdgx_enum_frame_size(struct v4l2_subdev *sd,
				  struct v4l2_frmsizeenum *fsize)
{
	if (fsize->index >= ARRAY_SIZE(s5k4cdgx_frame_sizes))
		return -EINVAL;

	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete = s5k4cdgx_frame_sizes[fsize->index];

	return 0;
}

static int s5k4cdgx_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	memset(cp, 0, sizeof(struct v4l2_captureparm));
	cp->capability = V4L2_CAP_TIMEPERFRAME;

	mutex_lock(&s5k4cdgx->lock);
	cp->timeperframe = s5k4cdgx->fiv->interval;
	mutex_unlock(&s5k4cdgx->lock);

	return 0;
}

static int s5k4cdgx_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct v4l2_fract *tpf = &cp->timeperframe;
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	int ret;

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
	if (cp->extendedmode != 0)
		return -EINVAL;

	mutex_lock(&s5k4cdgx->lock);
	ret = __s5k4cdgx_set_frame_interval(s5k4cdgx, tpf);
	mutex_unlock(&s5k4cdgx->lock);

	return ret;
}

static struct v4l2_rect *
__s5k4cdgx_get_crop_rect(struct s5k4cdgx *s5k4cdgx, struct v4l2_subdev_fh *fh,
		       enum v4l2_subdev_format_whence which)
{
	if (which == V4L2_SUBDEV_FORMAT_ACTIVE)
		return &s5k4cdgx->ccd_rect;
	if (which == V4L2_SUBDEV_FORMAT_TRY)
		return v4l2_subdev_get_try_crop(fh, 0);

	return NULL;
}

static void s5k4cdgx_try_format(struct s5k4cdgx *s5k4cdgx,
			      struct v4l2_mbus_framefmt *mf)
{
	unsigned int index;

	v4l_bound_align_image(&mf->width, S5K4CDGX_WIN_WIDTH_MIN,
			      S5K4CDGX_WIN_WIDTH_MAX, 1,
			      &mf->height, S5K4CDGX_WIN_HEIGHT_MIN,
			      S5K4CDGX_WIN_HEIGHT_MAX, 1, 0);

	if (mf->colorspace != V4L2_COLORSPACE_JPEG &&
	    mf->colorspace != V4L2_COLORSPACE_REC709)
		mf->colorspace = V4L2_COLORSPACE_JPEG;

	index = s5k4cdgx_get_pixfmt_index(s5k4cdgx, mf);

	mf->colorspace	= s5k4cdgx_formats[index].colorspace;
	mf->code	= s5k4cdgx_formats[index].code;
	mf->field	= V4L2_FIELD_NONE;
}

static int s5k4cdgx_video_try_fmt(struct v4l2_subdev *sd,
				 struct v4l2_mbus_framefmt *mf)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);

	s5k4cdgx_try_format(s5k4cdgx, mf);

	return 0;
}

static int s5k4cdgx_video_get_fmt(struct v4l2_subdev *sd,
				struct v4l2_mbus_framefmt *mf)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);

	mutex_lock(&s5k4cdgx->lock);
	*mf = s5k4cdgx->preset->mbus_fmt;
	mutex_unlock(&s5k4cdgx->lock);

	return 0;
}

static int s5k4cdgx_pad_get_fmt(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
			  struct v4l2_subdev_format *fmt)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	struct v4l2_mbus_framefmt *mf;

	memset(fmt->reserved, 0, sizeof(fmt->reserved));

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
		mf = v4l2_subdev_get_try_format(fh, 0);
		fmt->format = *mf;
		return 0;
	}

	mutex_lock(&s5k4cdgx->lock);
	fmt->format = s5k4cdgx->preset->mbus_fmt;
	mutex_unlock(&s5k4cdgx->lock);

	return 0;
}

static int s5k4cdgx_video_set_fmt(struct v4l2_subdev *sd,
				struct v4l2_mbus_framefmt *mf)
{
	int ret = 0;
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	struct s5k4cdgx_preset *preset = s5k4cdgx->preset;
	struct v4l2_rect *crop = &s5k4cdgx->ccd_rect;

	mutex_lock(&s5k4cdgx->lock);
	s5k4cdgx_try_format(s5k4cdgx, mf);

	if (s5k4cdgx->streaming) {
		ret = -EBUSY;
	} else {
		struct v4l2_fract fr = {0, 1};

		s5k4cdgx->apply_cfg = 1;
		pr_info("%s: setting format: %dx%d code=%x\n", __func__, mf->width, mf->height, mf->code);
		preset->mbus_fmt = *mf;

		/*
		 * Make sure the crop window is valid, i.e. its size is
		 * greater than the output window, as the ISP supports
		 * only down-scaling.
		 */
		crop->width = clamp_t(unsigned int, crop->width, mf->width,
				      S5K4CDGX_WIN_WIDTH_MAX);
		crop->height = clamp_t(unsigned int, crop->height, mf->height,
				       S5K4CDGX_WIN_HEIGHT_MAX);
		crop->left = clamp_t(unsigned int, crop->left, 0,
				     S5K4CDGX_WIN_WIDTH_MAX - crop->width);
		crop->top  = clamp_t(unsigned int, crop->top, 0,
				     S5K4CDGX_WIN_HEIGHT_MAX - crop->height);

		/* Reset to minimum possible frame interval */
		ret = __s5k4cdgx_set_frame_interval(s5k4cdgx, &fr);
	}
	mutex_unlock(&s5k4cdgx->lock);

	return ret;
}

static int s5k4cdgx_pad_set_fmt(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
			  struct v4l2_subdev_format *fmt)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	struct s5k4cdgx_preset *preset = s5k4cdgx->preset;
	struct v4l2_mbus_framefmt *mf;
	struct v4l2_rect *crop;
	int ret = 0;

	mutex_lock(&s5k4cdgx->lock);
	s5k4cdgx_try_format(s5k4cdgx, &fmt->format);

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
		mf = v4l2_subdev_get_try_format(fh, fmt->pad);
		crop = v4l2_subdev_get_try_crop(fh, 0);
	} else {
		if (s5k4cdgx->streaming) {
			ret = -EBUSY;
		} else {
			mf = &preset->mbus_fmt;
			crop = &s5k4cdgx->ccd_rect;
			s5k4cdgx->apply_cfg = 1;
		}
	}

	if (ret == 0) {
		struct v4l2_fract fr = {0, 1};

		*mf = fmt->format;
		/*
		 * Make sure the crop window is valid, i.e. its size is
		 * greater than the output window, as the ISP supports
		 * only down-scaling.
		 */
		crop->width = clamp_t(unsigned int, crop->width, mf->width,
				      S5K4CDGX_WIN_WIDTH_MAX);
		crop->height = clamp_t(unsigned int, crop->height, mf->height,
				       S5K4CDGX_WIN_HEIGHT_MAX);
		crop->left = clamp_t(unsigned int, crop->left, 0,
				     S5K4CDGX_WIN_WIDTH_MAX - crop->width);
		crop->top  = clamp_t(unsigned int, crop->top, 0,
				     S5K4CDGX_WIN_HEIGHT_MAX - crop->height);

		/* Reset to minimum possible frame interval */
		ret = __s5k4cdgx_set_frame_interval(s5k4cdgx, &fr);
	}
	mutex_unlock(&s5k4cdgx->lock);

	return ret;
}

static int s5k4cdgx_get_crop(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
			   struct v4l2_subdev_crop *crop)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	struct v4l2_rect *rect;

	memset(crop->reserved, 0, sizeof(crop->reserved));
	mutex_lock(&s5k4cdgx->lock);

	rect = __s5k4cdgx_get_crop_rect(s5k4cdgx, fh, crop->which);
	if (rect)
		crop->rect = *rect;

	mutex_unlock(&s5k4cdgx->lock);

	v4l2_dbg(1, debug, sd, "Current crop rectangle: (%d,%d)/%dx%d\n",
		 rect->left, rect->top, rect->width, rect->height);

	return 0;
}

static int s5k4cdgx_set_crop(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh,
			   struct v4l2_subdev_crop *crop)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	struct v4l2_mbus_framefmt *mf;
	unsigned int max_x, max_y;
	struct v4l2_rect *crop_r;

	mutex_lock(&s5k4cdgx->lock);
	crop_r = __s5k4cdgx_get_crop_rect(s5k4cdgx, fh, crop->which);

	if (crop->which == V4L2_SUBDEV_FORMAT_ACTIVE) {
		mf = &s5k4cdgx->preset->mbus_fmt;
		s5k4cdgx->apply_crop = 1;
	} else {
		mf = v4l2_subdev_get_try_format(fh, 0);
	}
	v4l_bound_align_image(&crop->rect.width, mf->width,
			      S5K4CDGX_WIN_WIDTH_MAX, 1,
			      &crop->rect.height, mf->height,
			      S5K4CDGX_WIN_HEIGHT_MAX, 1, 0);

	max_x = (S5K4CDGX_WIN_WIDTH_MAX - crop->rect.width) & ~1;
	max_y = (S5K4CDGX_WIN_HEIGHT_MAX - crop->rect.height) & ~1;

	crop->rect.left = clamp_t(unsigned int, crop->rect.left, 0, max_x);
	crop->rect.top  = clamp_t(unsigned int, crop->rect.top, 0, max_y);

	*crop_r = crop->rect;

	mutex_unlock(&s5k4cdgx->lock);

	v4l2_dbg(1, debug, sd, "Set crop rectangle: (%d,%d)/%dx%d\n",
		 crop_r->left, crop_r->top, crop_r->width, crop_r->height);

	return 0;
}

static const struct v4l2_subdev_pad_ops s5k4cdgx_pad_ops = {
	.enum_mbus_code		= s5k4cdgx_enum_mbus_code,
	.get_fmt		= s5k4cdgx_pad_get_fmt,
	.set_fmt		= s5k4cdgx_pad_set_fmt,
	.get_crop		= s5k4cdgx_get_crop,
	.set_crop		= s5k4cdgx_set_crop,
};

static const struct v4l2_subdev_video_ops s5k4cdgx_video_ops = {
	.g_frame_interval	= s5k4cdgx_g_frame_interval,
	.s_frame_interval	= s5k4cdgx_s_frame_interval,
	.s_stream		= s5k4cdgx_s_stream,
	.enum_frameintervals	= s5k4cdgx_enum_frame_interval,
	.enum_framesizes	= s5k4cdgx_enum_frame_size,
	.enum_mbus_fsizes	= s5k4cdgx_enum_frame_size,
	.enum_mbus_fmt = s5k4cdgx_enum_mbus_fmt,
	.try_mbus_fmt = s5k4cdgx_video_try_fmt,
	.g_mbus_fmt = s5k4cdgx_video_get_fmt,
	.s_mbus_fmt = s5k4cdgx_video_set_fmt,
	.s_parm = s5k4cdgx_s_parm,
	.g_parm = s5k4cdgx_g_parm,
};

/*
 * V4L2 subdev controls
 */

static int s5k4cdgx_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct v4l2_subdev *sd = ctrl_to_sd(ctrl);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	int idx, err = 0;

	v4l2_dbg(1, debug, sd, "ctrl: 0x%x, value: %d\n", ctrl->id, ctrl->val);

	mutex_lock(&s5k4cdgx->lock);
	/*
	 * If the device is not powered up by the host driver do
	 * not apply any controls to H/W at this time. Instead
	 * the controls will be restored right after power-up.
	 */
	if (s5k4cdgx->power == 0)
		goto unlock;
	idx = s5k4cdgx->preset->index;

	switch (ctrl->id) {
	case V4L2_CID_AUTO_WHITE_BALANCE:
		err = s5k4cdgx_set_awb(s5k4cdgx, ctrl->val);
		break;

	case V4L2_CID_BRIGHTNESS:
		err = s5k4cdgx_write(client, REG_USER_BRIGHTNESS, ctrl->val);
		break;

	case V4L2_CID_COLORFX:
		err = s5k4cdgx_set_colorfx(s5k4cdgx, ctrl->val);
		break;

	case V4L2_CID_CONTRAST:
		err = s5k4cdgx_write(client, REG_USER_CONTRAST, ctrl->val);
		break;

	case V4L2_CID_EXPOSURE_AUTO:
		err = s5k4cdgx_set_auto_exposure(s5k4cdgx, ctrl->val);
		break;

	case V4L2_CID_HFLIP:
		err = s5k4cdgx_set_mirror(s5k4cdgx, ctrl->val);
		if (err)
			break;
		err = s5k4cdgx_write(client, REG_G_PREV_CFG_CHG, 1);
		break;

	case V4L2_CID_POWER_LINE_FREQUENCY:
		err = s5k4cdgx_set_anti_flicker(s5k4cdgx, ctrl->val);
		break;

	case V4L2_CID_SATURATION:
		err = s5k4cdgx_write(client, REG_USER_SATURATION, ctrl->val);
		break;

	case V4L2_CID_SHARPNESS:
		err = s5k4cdgx_write(client, REG_USER_SHARPBLUR, ctrl->val);
		break;

	case V4L2_CID_WHITE_BALANCE_TEMPERATURE:
		err = s5k4cdgx_write(client, REG_P_COLORTEMP(idx), ctrl->val);
		if (err)
			break;
		err = s5k4cdgx_write(client, REG_G_PREV_CFG_CHG, 1);
		break;
	}
unlock:
	mutex_unlock(&s5k4cdgx->lock);
	return err;
}

static const struct v4l2_ctrl_ops s5k4cdgx_ctrl_ops = {
	.s_ctrl	= s5k4cdgx_s_ctrl,
};

static int s5k4cdgx_log_status(struct v4l2_subdev *sd)
{
	v4l2_ctrl_handler_log_status(sd->ctrl_handler, sd->name);
	return 0;
}

#define V4L2_CID_RED_GAIN	(V4L2_CTRL_CLASS_CAMERA | 0x1001)
#define V4L2_CID_GREEN_GAIN	(V4L2_CTRL_CLASS_CAMERA | 0x1002)
#define V4L2_CID_BLUE_GAIN	(V4L2_CTRL_CLASS_CAMERA | 0x1003)

static const struct v4l2_ctrl_config s5k4cdgx_ctrls[] = {
	{
		.ops	= &s5k4cdgx_ctrl_ops,
		.id	= V4L2_CID_RED_GAIN,
		.type	= V4L2_CTRL_TYPE_INTEGER,
		.name	= "Gain, Red",
		.min	= 0,
		.max	= 256,
		.def	= 127,
		.step	= 1,
	}, {
		.ops	= &s5k4cdgx_ctrl_ops,
		.id	= V4L2_CID_GREEN_GAIN,
		.type	= V4L2_CTRL_TYPE_INTEGER,
		.name	= "Gain, Green",
		.min	= 0,
		.max	= 256,
		.def	= 127,
		.step	= 1,
	}, {
		.ops	= &s5k4cdgx_ctrl_ops,
		.id	= V4L2_CID_BLUE_GAIN,
		.type	= V4L2_CTRL_TYPE_INTEGER,
		.name	= "Gain, Blue",
		.min	= 0,
		.max	= 256,
		.def	= 127,
		.step	= 1,
	},
};

static int s5k4cdgx_initialize_ctrls(struct s5k4cdgx *s5k4cdgx)
{
	const struct v4l2_ctrl_ops *ops = &s5k4cdgx_ctrl_ops;
	struct s5k4cdgx_ctrls *ctrls = &s5k4cdgx->ctrls;
	struct v4l2_ctrl_handler *hdl = &ctrls->handler;

	int ret = v4l2_ctrl_handler_init(hdl, 16);
	if (ret)
		return ret;
	/* Auto white balance cluster */
	ctrls->awb = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_AUTO_WHITE_BALANCE,
				       0, 1, 1, 1);
	ctrls->gain_red = v4l2_ctrl_new_custom(hdl, &s5k4cdgx_ctrls[0], NULL);
	ctrls->gain_green = v4l2_ctrl_new_custom(hdl, &s5k4cdgx_ctrls[1], NULL);
	ctrls->gain_blue = v4l2_ctrl_new_custom(hdl, &s5k4cdgx_ctrls[2], NULL);
	v4l2_ctrl_auto_cluster(4, &ctrls->awb, 0, false);

	ctrls->hflip = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_HFLIP, 0, 1, 1, 0);
	ctrls->vflip = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_VFLIP, 0, 1, 1, 0);
	v4l2_ctrl_cluster(2, &ctrls->hflip);

	ctrls->auto_exp = v4l2_ctrl_new_std_menu(hdl, ops,
				V4L2_CID_EXPOSURE_AUTO,
				V4L2_EXPOSURE_MANUAL, 0, V4L2_EXPOSURE_AUTO);
	/* Exposure time: x 1 us */
	ctrls->exposure = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_EXPOSURE,
					    0, 6000000U, 1, 100000U);
	/* Total gain: 256 <=> 1x */
	ctrls->gain = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_GAIN,
					0, 256, 1, 256);
	v4l2_ctrl_auto_cluster(3, &ctrls->auto_exp, 0, false);

	v4l2_ctrl_new_std_menu(hdl, ops, V4L2_CID_POWER_LINE_FREQUENCY,
			       V4L2_CID_POWER_LINE_FREQUENCY_AUTO, 0,
			       V4L2_CID_POWER_LINE_FREQUENCY_AUTO);

	v4l2_ctrl_new_std_menu(hdl, ops, V4L2_CID_COLORFX,
			       V4L2_COLORFX_SKY_BLUE, ~0x6f, V4L2_COLORFX_NONE);

	v4l2_ctrl_new_std(hdl, ops, V4L2_CID_WHITE_BALANCE_TEMPERATURE,
			  0, 256, 1, 0);

	v4l2_ctrl_new_std(hdl, ops, V4L2_CID_SATURATION, -127, 127, 1, 0);
	v4l2_ctrl_new_std(hdl, ops, V4L2_CID_BRIGHTNESS, -127, 127, 1, 0);
	v4l2_ctrl_new_std(hdl, ops, V4L2_CID_CONTRAST, -127, 127, 1, 0);
	v4l2_ctrl_new_std(hdl, ops, V4L2_CID_SHARPNESS, -127, 127, 1, 0);

	if (hdl->error) {
		ret = hdl->error;
		v4l2_ctrl_handler_free(hdl);
		return ret;
	}

	s5k4cdgx->sd.ctrl_handler = hdl;
	return 0;
}

/*
 * V4L2 subdev internal operations
 */
static int s5k4cdgx_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct v4l2_mbus_framefmt *format = v4l2_subdev_get_try_format(fh, 0);
	struct v4l2_rect *crop = v4l2_subdev_get_try_crop(fh, 0);

	format->colorspace = s5k4cdgx_formats[0].colorspace;
	format->code = s5k4cdgx_formats[0].code;
	format->width = S5K4CDGX_OUT_WIDTH_DEF;
	format->height = S5K4CDGX_OUT_HEIGHT_DEF;
	format->field = V4L2_FIELD_NONE;

	crop->width = S5K4CDGX_WIN_WIDTH_MAX;
	crop->height = S5K4CDGX_WIN_HEIGHT_MAX;
	crop->left = 0;
	crop->top = 0;

	return 0;
}

int s5k4cdgx_check_fw_revision(struct s5k4cdgx *s5k4cdgx)
{
	struct i2c_client *client = v4l2_get_subdevdata(&s5k4cdgx->sd);
	struct v4l2_subdev *sd = &s5k4cdgx->sd;
	u16 chipid = 0, modelid = 0;
	int rc;

	rc = s5k4cdgx_read(client, REG_CHIP_ID, &chipid);
	if (rc < 0) {
			v4l2_info(sd, "s5k4cdgx_read chipid read failed!!! rc=%d\n", rc);
	}
	
	v4l2_info(sd, "chipid = 0x%x\n", chipid);
	
	rc = s5k4cdgx_write(client, REG_GPIO_MODE_SEL, 0x0033);
	if (rc < 0) {
			v4l2_info(sd, "s5k4cdgx_write REG_GPIO_MODE_SEL rc=%d\n", rc);
	}
	
	rc = s5k4cdgx_write(client, REG_GPIO_FUNC_SEL, 0x0066);
	if (rc < 0) {
			v4l2_info(sd, "s5k4cdgx_write REG_GPIO_FUNC_SEL rc=%d\n", rc);
	}
	
	mdelay(2);
	
	rc = s5k4cdgx_read(client, REG_MODEL_ID, &modelid);
	if (rc < 0) {
			v4l2_info(sd, "s5k4cdgx_read model_id read failed!!! rc=%d\n", rc);
	}
	
	v4l2_info(sd, "model_id = 0x%x\n", modelid);
	
	
	rc = s5k4cdgx_write(client, REG_GPIO_MODE_SEL, 0x0000);
	if (rc < 0) {
			v4l2_info(sd, "s5k4cdgx_write REG_GPIO_MODE_SEL rc=%d\n", rc);
	}
	
	rc = s5k4cdgx_write(client, REG_GPIO_FUNC_SEL, 0x0000);
	if (rc < 0) {
			v4l2_info(sd, "s5k4cdgx_write REG_GPIO_FUNC_SEL rc=%d\n", rc);
	}
	
	v4l2_info(&s5k4cdgx->sd, "ChipID: 0x%X, ModelID: 0x%x\n",
		  chipid, modelid);

	return chipid == S5K4CDGX_CHIP_ID ? 0 : -ENODEV;
}

static int s5k4cdgx_registered(struct v4l2_subdev *sd)
{
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int ret;

	if(!sd || !s5k4cdgx) {
	  pr_err("%s: invalid subdev %p or priv %p\n", __func__, sd, s5k4cdgx);
	  return -EINVAL;
	}

	mutex_lock(&s5k4cdgx->lock);
	ret = __s5k4cdgx_power_on(s5k4cdgx);
	if (!ret) {
		msleep(100);
		ret = s5k4cdgx_check_fw_revision(s5k4cdgx);
		__s5k4cdgx_power_off(s5k4cdgx);
	}
	mutex_unlock(&s5k4cdgx->lock);

	return ret;
}

static const struct v4l2_subdev_internal_ops s5k4cdgx_subdev_internal_ops = {
	.registered = s5k4cdgx_registered,
	.open = s5k4cdgx_open,
};

static const struct v4l2_subdev_core_ops s5k4cdgx_core_ops = {
	.s_power = s5k4cdgx_set_power,
	.log_status = s5k4cdgx_log_status,
};

static const struct v4l2_subdev_ops s5k4cdgx_subdev_ops = {
	.core = &s5k4cdgx_core_ops,
	.pad = &s5k4cdgx_pad_ops,
	.video = &s5k4cdgx_video_ops,
};

static int s5k4cdgx_set_bus_param(struct soc_camera_device *icd,
				unsigned long		  flags)
{
	return 0;
}

static unsigned long s5k4cdgx_query_bus_param(struct soc_camera_device *icd)
{
	struct soc_camera_link *icl = to_soc_camera_link(icd);
	unsigned long flags = SOCAM_PCLK_SAMPLE_RISING | SOCAM_MASTER |
		SOCAM_VSYNC_ACTIVE_HIGH | SOCAM_HSYNC_ACTIVE_HIGH |
		SOCAM_DATA_ACTIVE_HIGH | SOCAM_DATAWIDTH_8;

	return soc_camera_apply_sensor_flags(icl, flags);
}

static struct soc_camera_ops s5k4cdgx_ops = {
	.set_bus_param		= s5k4cdgx_set_bus_param,
	.query_bus_param	= s5k4cdgx_query_bus_param,
};

/*
 * GPIO setup
 */
static int s5k4cdgx_configure_gpio(int nr, int val, const char *name)
{
	unsigned long flags = val ? GPIOF_OUT_INIT_HIGH : GPIOF_OUT_INIT_LOW;
	int ret;

	if (!gpio_is_valid(nr))
		return 0;
	ret = gpio_request_one(nr, flags, name);
	if (!ret)
		gpio_export(nr, 0);
	return ret;
}

static void s5k4cdgx_free_gpios(struct s5k4cdgx *s5k4cdgx)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(s5k4cdgx->gpio); i++) {
		if (!gpio_is_valid(s5k4cdgx->gpio[i].gpio))
			continue;
		gpio_free(s5k4cdgx->gpio[i].gpio);
		s5k4cdgx->gpio[i].gpio = -EINVAL;
	}
}

static int s5k4cdgx_configure_gpios(struct s5k4cdgx *s5k4cdgx,
				  const struct s5k4cdgx_platform_data *pdata)
{
	const struct s5k4cdgx_gpio *gpio = &pdata->gpio_stby;
	int ret;

	s5k4cdgx->gpio[STBY].gpio = -EINVAL;
	s5k4cdgx->gpio[RST].gpio  = -EINVAL;

	ret = s5k4cdgx_configure_gpio(gpio->gpio, gpio->level, "S5K4CDGX_STBY");
	if (ret) {
		s5k4cdgx_free_gpios(s5k4cdgx);
		return ret;
	}
	s5k4cdgx->gpio[STBY] = *gpio;
	if (gpio_is_valid(gpio->gpio))
		gpio_set_value(gpio->gpio, 0);

	gpio = &pdata->gpio_reset;
	ret = s5k4cdgx_configure_gpio(gpio->gpio, gpio->level, "S5K4CDGX_RST");
	if (ret) {
		s5k4cdgx_free_gpios(s5k4cdgx);
		return ret;
	}
	s5k4cdgx->gpio[RST] = *gpio;
	if (gpio_is_valid(gpio->gpio))
		gpio_set_value(gpio->gpio, 0);

	return 0;
}

static int s5k4cdgx_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	/* NOTE! This changes in later kernel versions: pdata is a 
	   camera_link instead of camera_device! */
	struct soc_camera_device *icd = client->dev.platform_data;
	struct soc_camera_link *icl;
	const struct s5k4cdgx_platform_data *pdata;
	struct v4l2_subdev *sd;
	struct s5k4cdgx *s5k4cdgx;
	int i, ret;

	if (!icd) {
		dev_err(&client->dev, "Missing soc_camera_device\n");
		return -EINVAL;
	}

        icl = to_soc_camera_link(icd);
        if (!icl || !icl->priv) {
		dev_err(&client->dev, "Missing soc_camera_link\n");
                return -EINVAL;
	}

	pdata = icl->priv;

	if (pdata == NULL) {
		dev_err(&client->dev, "Platform data not specified\n");
		return -EINVAL;
	}

	if (pdata->mclk_frequency == 0) {
		dev_err(&client->dev, "MCLK frequency not specified\n");
		return -EINVAL;
	}

	s5k4cdgx = kzalloc(sizeof(*s5k4cdgx), GFP_KERNEL);
	if (!s5k4cdgx)
		return -ENOMEM;

	mutex_init(&s5k4cdgx->lock);

	s5k4cdgx->mclk_frequency = pdata->mclk_frequency;
	s5k4cdgx->bus_type = pdata->bus_type;
	s5k4cdgx->mipi_lanes = pdata->nlanes;
	s5k4cdgx->s_power	= pdata->set_power;
	s5k4cdgx->inv_hflip = pdata->horiz_flip;
	s5k4cdgx->inv_vflip = pdata->vert_flip;

	sd = &s5k4cdgx->sd;
	strlcpy(sd->name, DRIVER_NAME, sizeof(sd->name));
	v4l2_i2c_subdev_init(sd, client, &s5k4cdgx_subdev_ops);
	icd->ops = &s5k4cdgx_ops;

	sd->internal_ops = &s5k4cdgx_subdev_internal_ops;
	sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;

	s5k4cdgx->pad.flags = MEDIA_PAD_FL_SOURCE;
	sd->entity.type = MEDIA_ENT_T_V4L2_SUBDEV_SENSOR;
	ret = media_entity_init(&sd->entity, 1, &s5k4cdgx->pad, 0);
	if (ret) {
		dev_err(&client->dev, "Failed to initialize media entity\n");
		goto out_err1;
	}

	ret = s5k4cdgx_configure_gpios(s5k4cdgx, pdata);
	if (ret) {
		dev_err(&client->dev, "Failed to initialize GPIO\n");
		goto out_err2;
	}

	for (i = 0; i < S5K4CDGX_NUM_SUPPLIES; i++)
		s5k4cdgx->supplies[i].supply = s5k4cdgx_supply_names[i];

	ret = regulator_bulk_get(&client->dev, S5K4CDGX_NUM_SUPPLIES,
				 s5k4cdgx->supplies);
	if (ret) {
		dev_err(&client->dev, "Failed to get regulators\n");
		goto out_err3;
	}

	ret = s5k4cdgx_initialize_ctrls(s5k4cdgx);
	if (ret) {
		dev_err(&client->dev, "Failed to initialize v4l controls\n");
		goto out_err4;
	}

	s5k4cdgx_presets_data_init(s5k4cdgx);

	s5k4cdgx->ccd_rect.width = S5K4CDGX_WIN_WIDTH_MAX;
	s5k4cdgx->ccd_rect.height = S5K4CDGX_WIN_HEIGHT_MAX;
	s5k4cdgx->ccd_rect.left = 0;
	s5k4cdgx->ccd_rect.top = 0;

	return 0;

out_err4:
	regulator_bulk_free(S5K4CDGX_NUM_SUPPLIES, s5k4cdgx->supplies);
out_err3:
	s5k4cdgx_free_gpios(s5k4cdgx);
out_err2:
	media_entity_cleanup(&s5k4cdgx->sd.entity);
out_err1:
	kfree(s5k4cdgx);
	return ret;
}

static int s5k4cdgx_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct s5k4cdgx *s5k4cdgx = to_s5k4cdgx(sd);

	v4l2_device_unregister_subdev(sd);
	v4l2_ctrl_handler_free(sd->ctrl_handler);
	media_entity_cleanup(&sd->entity);
	regulator_bulk_free(S5K4CDGX_NUM_SUPPLIES, s5k4cdgx->supplies);
	s5k4cdgx_free_gpios(s5k4cdgx);
	kfree(s5k4cdgx);

	return 0;
}

static const struct i2c_device_id s5k4cdgx_id[] = {
	{ DRIVER_NAME, 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, s5k4cdgx_id);


static struct i2c_driver s5k4cdgx_i2c_driver = {
	.driver = {
		.name = DRIVER_NAME
	},
	.probe		= s5k4cdgx_probe,
	.remove		= s5k4cdgx_remove,
	.id_table	= s5k4cdgx_id,
};

static int __init s5k4cdgx_init(void)
{
	return i2c_add_driver(&s5k4cdgx_i2c_driver);
}

static void __exit s5k4cdgx_exit(void)
{
	i2c_del_driver(&s5k4cdgx_i2c_driver);
}

module_init(s5k4cdgx_init);
module_exit(s5k4cdgx_exit);

MODULE_DESCRIPTION("Samsung S5K4CDGX QXGA camera driver");
MODULE_AUTHOR("Boris Popov <bvpopov<at>gmail.com>");
MODULE_LICENSE("GPL");
