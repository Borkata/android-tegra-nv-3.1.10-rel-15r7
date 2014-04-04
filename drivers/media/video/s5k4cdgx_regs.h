/*
 * Driver for S5K4CDGX (QXGA camera) from Samsung Electronics
 *
 * 1/4" 3.2Mp CMOS Image Sensor SoC with an Embedded Image Processor
 *
 * Copyright (C)	2013, Boris Popov <bvpopov at gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef __S5K4CDGX_H__
#define __S5K4CDGX_H__

#define S5K4CDGX_BANK_MAGIC	(0xfcfc)
#define S5K4CDGX_PAGE_MAGIC	(0x0028)
#define S5K4CDGX_REG_MAGIC	(0x002a)
#define S5K4CDGX_DATA_MAGIC	(0x0f12)
#define S5K4CDGX_MSLEEP_MAGIC	(0xffff)

#define S5K4CDGX_BANK(bank)	{ S5K4CDGX_BANK_MAGIC, bank }
#define S5K4CDGX_PAGE(page)	{ S5K4CDGX_PAGE_MAGIC, page }
#define S5K4CDGX_REG(reg)		{ S5K4CDGX_REG_MAGIC, reg }
#define S5K4CDGX_DATA(data)	{ S5K4CDGX_DATA_MAGIC, data }
#define S5K4CDGX_MSLEEP(msecs)	{ S5K4CDGX_MSLEEP_MAGIC, msecs }

struct s5k4cdgx_request {
	u16 subaddr;
	u16 value;
};

//VERSION 4
//=================================================================================================
//	* Name		:	4CDGX EVT1.0 Initial Setfile
//	* PLL mode	:	MCLK=24MHz / SYSCLK=30MHz / PCLK=60MHz
//	* FPS		:	Preview YUV 640X480 15~7.5fps Capture YUV 2048x1536 7.5fps
//	* Made by	:	SYS.LSI Sang-il Park
//	* Date		:	2009.11.03
//	* History
//						: 09.11.03	Initial draft (based LG-SB210 tuning value)
//						: 09.11.06	Changed GAS LUT & Alpha & AFIT for Shading compensation
//=================================================================================================
//out from config1 - step 1
static struct s5k4cdgx_request s5k4cdgx_init_reg_io_driving_current[] ={
//p20								
// Set IO driving current
S5K4CDGX_PAGE(0xD000),
S5K4CDGX_REG(0x1082),
S5K4CDGX_DATA(0x0155), // [9:8] D4, [7:6] D3, [5:4] D2, [3:2] D1, [1:0] D0
S5K4CDGX_DATA(0x0155), // [9:8] D9, [7:6] D8, [5:4] D7, [3:2] D6, [1:0] D5
S5K4CDGX_DATA(0x00A9), // [5:4] GPIO3, [3:2] GPIO2, [1:0] GPIO1
S5K4CDGX_DATA(0x0555), // [11:10] SDA, [9:8] SCA, [7:6] PCLK, [3:2] VSYNC, [1:0] HSYNC
};

//out from config1 - step 2 //checked wirh orig
static struct s5k4cdgx_request s5k4cdgx_init_reg_trap_and_patch[] ={
//=================================================================================================
//	Trap & Patch
//	(need in case of using standby mode)
//=================================================================================================
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x28C8),
S5K4CDGX_DATA(0xB570),
S5K4CDGX_DATA(0x4C15),
S5K4CDGX_DATA(0x2607),
S5K4CDGX_DATA(0x6821),
S5K4CDGX_DATA(0x0736),
S5K4CDGX_DATA(0x42B1),
S5K4CDGX_DATA(0xDA05),
S5K4CDGX_DATA(0x4813),
S5K4CDGX_DATA(0x22D8),
S5K4CDGX_DATA(0x1C05),
S5K4CDGX_DATA(0xF000),
S5K4CDGX_DATA(0xF854),
S5K4CDGX_DATA(0x6025),
S5K4CDGX_DATA(0x68A1),
S5K4CDGX_DATA(0x42B1),
S5K4CDGX_DATA(0xDA07),
S5K4CDGX_DATA(0x480E),
S5K4CDGX_DATA(0x2224),
S5K4CDGX_DATA(0x3824),
S5K4CDGX_DATA(0xF000),
S5K4CDGX_DATA(0xF84B),
S5K4CDGX_DATA(0x480C),
S5K4CDGX_DATA(0x3824),
S5K4CDGX_DATA(0x60A0),
S5K4CDGX_DATA(0x4C0B),
S5K4CDGX_DATA(0x6961),
S5K4CDGX_DATA(0x42B1),
S5K4CDGX_DATA(0xDA07),
S5K4CDGX_DATA(0x4808),
S5K4CDGX_DATA(0x228F),
S5K4CDGX_DATA(0x00D2),
S5K4CDGX_DATA(0x30D8),
S5K4CDGX_DATA(0x1C05),
S5K4CDGX_DATA(0xF000),
S5K4CDGX_DATA(0xF83D),
S5K4CDGX_DATA(0x6165),
S5K4CDGX_DATA(0x4906),
S5K4CDGX_DATA(0x4807),
S5K4CDGX_DATA(0x2200),
S5K4CDGX_DATA(0xF000),
S5K4CDGX_DATA(0xF83D),
S5K4CDGX_DATA(0xBC70),
S5K4CDGX_DATA(0xBC08),
S5K4CDGX_DATA(0x4718),
S5K4CDGX_DATA(0x06D8),
S5K4CDGX_DATA(0x7000),
S5K4CDGX_DATA(0x33A4),
S5K4CDGX_DATA(0x7000),
S5K4CDGX_DATA(0x0778),
S5K4CDGX_DATA(0x7000),
S5K4CDGX_DATA(0x2935),
S5K4CDGX_DATA(0x7000),
S5K4CDGX_DATA(0xF00F),
S5K4CDGX_DATA(0x0000),
S5K4CDGX_DATA(0xB5F8),
S5K4CDGX_DATA(0x1C04),
S5K4CDGX_DATA(0x2001),
S5K4CDGX_DATA(0x1C05),
S5K4CDGX_DATA(0x1C21),
S5K4CDGX_DATA(0x3910),
S5K4CDGX_DATA(0x4088),
S5K4CDGX_DATA(0x1C06),
S5K4CDGX_DATA(0x40A5),
S5K4CDGX_DATA(0x4F0E),
S5K4CDGX_DATA(0x2C10),
S5K4CDGX_DATA(0xDA03),
S5K4CDGX_DATA(0x8838),
S5K4CDGX_DATA(0x43A8),
S5K4CDGX_DATA(0x8038),
S5K4CDGX_DATA(0xE002),
S5K4CDGX_DATA(0x8878),
S5K4CDGX_DATA(0x43B0),
S5K4CDGX_DATA(0x8078),
S5K4CDGX_DATA(0xF000),
S5K4CDGX_DATA(0xF823),
S5K4CDGX_DATA(0x4909),
S5K4CDGX_DATA(0x2000),
S5K4CDGX_DATA(0x8188),
S5K4CDGX_DATA(0x80C8),
S5K4CDGX_DATA(0x2C10),
S5K4CDGX_DATA(0xDA05),
S5K4CDGX_DATA(0x8838),
S5K4CDGX_DATA(0x4328),
S5K4CDGX_DATA(0x8038),
S5K4CDGX_DATA(0xBCF8),
S5K4CDGX_DATA(0xBC08),
S5K4CDGX_DATA(0x4718),
S5K4CDGX_DATA(0x8878),
S5K4CDGX_DATA(0x4330),
S5K4CDGX_DATA(0x8078),
S5K4CDGX_DATA(0xE7F8),
S5K4CDGX_DATA(0x0000),
S5K4CDGX_DATA(0x1100),
S5K4CDGX_DATA(0xD000),
S5K4CDGX_DATA(0x0060),
S5K4CDGX_DATA(0xD000),
S5K4CDGX_DATA(0x4778),
S5K4CDGX_DATA(0x46C0),
S5K4CDGX_DATA(0xF004),
S5K4CDGX_DATA(0xE51F),
S5K4CDGX_DATA(0xFA28),
S5K4CDGX_DATA(0x0000),
S5K4CDGX_DATA(0x4778),
S5K4CDGX_DATA(0x46C0),
S5K4CDGX_DATA(0xC000),
S5K4CDGX_DATA(0xE59F),
S5K4CDGX_DATA(0xFF1C),
S5K4CDGX_DATA(0xE12F),
S5K4CDGX_DATA(0xF563),
S5K4CDGX_DATA(0x0000),
S5K4CDGX_DATA(0x4778),
S5K4CDGX_DATA(0x46C0),
S5K4CDGX_DATA(0xC000),
S5K4CDGX_DATA(0xE59F),
S5K4CDGX_DATA(0xFF1C),
S5K4CDGX_DATA(0xE12F),
S5K4CDGX_DATA(0xF00F),
S5K4CDGX_DATA(0x0000),
};

//out from config1 - step 3 
static struct s5k4cdgx_request s5k4cdgx_init_reg_af[] ={
//================================================================================================
//	AF Initialize
//================================================================================================
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x0242),//#REG_TC_IPRM_CM_Init_AfModeType   
S5K4CDGX_DATA(0x0003),
S5K4CDGX_REG(0x0248),// #REG_TC_IPRM_CM_Init_GpioConfig1    
S5K4CDGX_DATA(0x0021),
S5K4CDGX_REG(0x0250),//#REG_TC_IPRM_CM_Init_Mi2cBits         
S5K4CDGX_DATA(0x450C),
S5K4CDGX_DATA(0x0190),////#REG_TC_IPRM_CM_Init_Mi2cRateKhz

//delete focus window setting
//S5K4CDGX_REG(0x02CA),
//S5K4CDGX_DATA(0x0100),
//S5K4CDGX_DATA(0x00E3),
//S5K4CDGX_DATA(0x0200),
//S5K4CDGX_DATA(0x0238),
//S5K4CDGX_DATA(0x018C),
//S5K4CDGX_DATA(0x0166),
//S5K4CDGX_DATA(0x00E6),
//S5K4CDGX_DATA(0x0132),
//S5K4CDGX_DATA(0x0001),

// AF searching position table
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x0F44),
S5K4CDGX_DATA(0x000C),

// AF searching position table
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x0F46),
S5K4CDGX_DATA(0x0000),	// index 0
S5K4CDGX_DATA(0x0038),	// index 1
S5K4CDGX_DATA(0x0060),	// index 2
S5K4CDGX_DATA(0x0068),	// index 3
S5K4CDGX_DATA(0x006E),	// index 4
S5K4CDGX_DATA(0x0074),	// index 5
S5K4CDGX_DATA(0x007A),	// index 6
S5K4CDGX_DATA(0x0080),	// index 7
S5K4CDGX_DATA(0x0086),	// index 8
S5K4CDGX_DATA(0x008C),	// index 9
S5K4CDGX_DATA(0x0092),	// index 10
S5K4CDGX_DATA(0x0098),	// index 11
S5K4CDGX_DATA(0x00A0),	// index 12
S5K4CDGX_DATA(0x0000),	// index 13
S5K4CDGX_DATA(0x0000),	// index 14
S5K4CDGX_DATA(0x0000),	// index 15
S5K4CDGX_DATA(0x0000),	// index 16

// Min. peak detection number
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x0FA8),
S5K4CDGX_DATA(0x0003),
//
S5K4CDGX_REG(0x0F30),
S5K4CDGX_DATA(0x0003),
S5K4CDGX_DATA(0x0003),
//add focus windows
S5K4CDGX_REG(0x1084),
S5K4CDGX_DATA(0x03B8),
S5K4CDGX_DATA(0x03CC),
S5K4CDGX_DATA(0x03AA),
S5K4CDGX_REG(0x1036),
S5K4CDGX_DATA(0x4040),
S5K4CDGX_DATA(0x9090),
};

//out from config1 - step 4 //checked wirh orig
static struct s5k4cdgx_request s5k4cdgx_init_reg_factory[]={
//=================================================================================================
//	Set CIS/APS/Analog
//=================================================================================================
// This registers are for FACTORY ONLY. If you change it without prior notification 
// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
//=================================================================================================
S5K4CDGX_PAGE(0xD000),
S5K4CDGX_REG(0xF52E),	// aig_gain_offset_3 (ADC_SAT level)
S5K4CDGX_DATA(0x0023),              
S5K4CDGX_REG(0xF536),	// aig_ref_ramp_1
S5K4CDGX_DATA(0x0639),//S5K4CDGX_DATA(0x0619),  //modified by zj.for vertical bar
S5K4CDGX_PAGE(0x7000),	// #start add MSW
S5K4CDGX_REG(0x1166),	// #start add LSW of senHal_ContPtrs_pSenModesRegsArray
S5K4CDGX_DATA(0x0006),	// #senHal_ContPtrs_pSenModesRegsArray[0][0]
S5K4CDGX_DATA(0x0006),	// #senHal_ContPtrs_pSenModesRegsArray[0][1]
S5K4CDGX_DATA(0x0866),	// #senHal_ContPtrs_pSenModesRegsArray[1][0]
S5K4CDGX_DATA(0x0866),	// #senHal_ContPtrs_pSenModesRegsArray[1][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[2][0]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[2][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[3][0]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[3][1]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[4][0]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[4][1]
S5K4CDGX_DATA(0x084C),	// #senHal_ContPtrs_pSenModesRegsArray[5][0]
S5K4CDGX_DATA(0x0368),	// #senHal_ContPtrs_pSenModesRegsArray[5][1]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[6][0]
S5K4CDGX_DATA(0x0364),	// #senHal_ContPtrs_pSenModesRegsArray[6][1]
S5K4CDGX_DATA(0x084C),	// #senHal_ContPtrs_pSenModesRegsArray[7][0]
S5K4CDGX_DATA(0x06C4),	// #senHal_ContPtrs_pSenModesRegsArray[7][1]
S5K4CDGX_DATA(0x001E),	// #senHal_ContPtrs_pSenModesRegsArray[8][0]
S5K4CDGX_DATA(0x001E),	// #senHal_ContPtrs_pSenModesRegsArray[8][1]
S5K4CDGX_DATA(0x084C),	// #senHal_ContPtrs_pSenModesRegsArray[9][0]
S5K4CDGX_DATA(0x0364),	// #senHal_ContPtrs_pSenModesRegsArray[9][1]
S5K4CDGX_DATA(0x001E),	// #senHal_ContPtrs_pSenModesRegsArray[10][0]
S5K4CDGX_DATA(0x037E),	// #senHal_ContPtrs_pSenModesRegsArray[10][1]
S5K4CDGX_DATA(0x084C),	// #senHal_ContPtrs_pSenModesRegsArray[11][0]
S5K4CDGX_DATA(0x06C4),	// #senHal_ContPtrs_pSenModesRegsArray[11][1]
S5K4CDGX_DATA(0x01FC),	// #senHal_ContPtrs_pSenModesRegsArray[12][0]
S5K4CDGX_DATA(0x0138),	// #senHal_ContPtrs_pSenModesRegsArray[12][1]
S5K4CDGX_DATA(0x0256),	// #senHal_ContPtrs_pSenModesRegsArray[13][0]
S5K4CDGX_DATA(0x0192),	// #senHal_ContPtrs_pSenModesRegsArray[13][1]
S5K4CDGX_DATA(0x01FC),	// #senHal_ContPtrs_pSenModesRegsArray[14][0]
S5K4CDGX_DATA(0x0498),	// #senHal_ContPtrs_pSenModesRegsArray[14][1]
S5K4CDGX_DATA(0x0256),	// #senHal_ContPtrs_pSenModesRegsArray[15][0]
S5K4CDGX_DATA(0x04F2),	// #senHal_ContPtrs_pSenModesRegsArray[15][1]
S5K4CDGX_DATA(0x0256),	// #senHal_ContPtrs_pSenModesRegsArray[16][0]
S5K4CDGX_DATA(0x0192),	// #senHal_ContPtrs_pSenModesRegsArray[16][1]
S5K4CDGX_DATA(0x084A),	// #senHal_ContPtrs_pSenModesRegsArray[17][0]
S5K4CDGX_DATA(0x0362),	// #senHal_ContPtrs_pSenModesRegsArray[17][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[18][0]
S5K4CDGX_DATA(0x04F2),	// #senHal_ContPtrs_pSenModesRegsArray[18][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[19][0]
S5K4CDGX_DATA(0x06C2),	// #senHal_ContPtrs_pSenModesRegsArray[19][1]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[20][0]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[20][1]
S5K4CDGX_DATA(0x01F8),	// #senHal_ContPtrs_pSenModesRegsArray[21][0]
S5K4CDGX_DATA(0x0134),	// #senHal_ContPtrs_pSenModesRegsArray[21][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[22][0]
S5K4CDGX_DATA(0x0366),	// #senHal_ContPtrs_pSenModesRegsArray[22][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[23][0]
S5K4CDGX_DATA(0x0494),	// #senHal_ContPtrs_pSenModesRegsArray[23][1]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[24][0]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[24][1]
S5K4CDGX_DATA(0x00DC),	// #senHal_ContPtrs_pSenModesRegsArray[25][0]
S5K4CDGX_DATA(0x00DC),	// #senHal_ContPtrs_pSenModesRegsArray[25][1]
S5K4CDGX_DATA(0x025E),	// #senHal_ContPtrs_pSenModesRegsArray[26][0]
S5K4CDGX_DATA(0x0192),	// #senHal_ContPtrs_pSenModesRegsArray[26][1]
S5K4CDGX_DATA(0x029A),	// #senHal_ContPtrs_pSenModesRegsArray[27][0]
S5K4CDGX_DATA(0x01D6),	// #senHal_ContPtrs_pSenModesRegsArray[27][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[28][0]
S5K4CDGX_DATA(0x0380),	// #senHal_ContPtrs_pSenModesRegsArray[28][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[29][0]
S5K4CDGX_DATA(0x0402),	// #senHal_ContPtrs_pSenModesRegsArray[29][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[30][0]
S5K4CDGX_DATA(0x04FA),	// #senHal_ContPtrs_pSenModesRegsArray[30][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[31][0]
S5K4CDGX_DATA(0x0536),	// #senHal_ContPtrs_pSenModesRegsArray[31][1]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[32][0]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[32][1]
S5K4CDGX_DATA(0x007A),	// #senHal_ContPtrs_pSenModesRegsArray[33][0]
S5K4CDGX_DATA(0x007A),	// #senHal_ContPtrs_pSenModesRegsArray[33][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[34][0]
S5K4CDGX_DATA(0x0380),	// #senHal_ContPtrs_pSenModesRegsArray[34][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[35][0]
S5K4CDGX_DATA(0x03DA),	// #senHal_ContPtrs_pSenModesRegsArray[35][1]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[36][0]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[36][1]
S5K4CDGX_DATA(0x00DC),	// #senHal_ContPtrs_pSenModesRegsArray[37][0]
S5K4CDGX_DATA(0x00DC),	// #senHal_ContPtrs_pSenModesRegsArray[37][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[38][0]
S5K4CDGX_DATA(0x0380),	// #senHal_ContPtrs_pSenModesRegsArray[38][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[39][0]
S5K4CDGX_DATA(0x0402),	// #senHal_ContPtrs_pSenModesRegsArray[39][1]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[40][0]
S5K4CDGX_DATA(0x005A),	// #senHal_ContPtrs_pSenModesRegsArray[40][1]
S5K4CDGX_DATA(0x01F8),	// #senHal_ContPtrs_pSenModesRegsArray[41][0]
S5K4CDGX_DATA(0x0134),	// #senHal_ContPtrs_pSenModesRegsArray[41][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[42][0]
S5K4CDGX_DATA(0x0380),	// #senHal_ContPtrs_pSenModesRegsArray[42][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[43][0]
S5K4CDGX_DATA(0x0494),	// #senHal_ContPtrs_pSenModesRegsArray[43][1]
S5K4CDGX_DATA(0x02A2),	// #senHal_ContPtrs_pSenModesRegsArray[44][0]
S5K4CDGX_DATA(0x01DE),	// #senHal_ContPtrs_pSenModesRegsArray[44][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[45][0]
S5K4CDGX_DATA(0x0366),	// #senHal_ContPtrs_pSenModesRegsArray[45][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[46][0]
S5K4CDGX_DATA(0x053E),	// #senHal_ContPtrs_pSenModesRegsArray[46][1]
S5K4CDGX_DATA(0x00E4),	// #senHal_ContPtrs_pSenModesRegsArray[47][0]
S5K4CDGX_DATA(0x00E4),	// #senHal_ContPtrs_pSenModesRegsArray[47][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[48][0]
S5K4CDGX_DATA(0x0366),	// #senHal_ContPtrs_pSenModesRegsArray[48][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[49][0]
S5K4CDGX_DATA(0x040A),	// #senHal_ContPtrs_pSenModesRegsArray[49][1]
S5K4CDGX_DATA(0x00E4),	// #senHal_ContPtrs_pSenModesRegsArray[50][0]
S5K4CDGX_DATA(0x00E4),	// #senHal_ContPtrs_pSenModesRegsArray[50][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[51][0]
S5K4CDGX_DATA(0x0366),	// #senHal_ContPtrs_pSenModesRegsArray[51][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[52][0]
S5K4CDGX_DATA(0x040A),	// #senHal_ContPtrs_pSenModesRegsArray[52][1]
S5K4CDGX_DATA(0x00F0),	// #senHal_ContPtrs_pSenModesRegsArray[53][0]
S5K4CDGX_DATA(0x00EF),	// #senHal_ContPtrs_pSenModesRegsArray[53][1]
S5K4CDGX_DATA(0x01F0),	// #senHal_ContPtrs_pSenModesRegsArray[54][0]
S5K4CDGX_DATA(0x012F),	// #senHal_ContPtrs_pSenModesRegsArray[54][1]
S5K4CDGX_DATA(0x02C2),	// #senHal_ContPtrs_pSenModesRegsArray[55][0]
S5K4CDGX_DATA(0x01FD),	// #senHal_ContPtrs_pSenModesRegsArray[55][1]
S5K4CDGX_DATA(0x0842),	// #senHal_ContPtrs_pSenModesRegsArray[56][0]
S5K4CDGX_DATA(0x035D),	// #senHal_ContPtrs_pSenModesRegsArray[56][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[57][0]
S5K4CDGX_DATA(0x044F),	// #senHal_ContPtrs_pSenModesRegsArray[57][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[58][0]
S5K4CDGX_DATA(0x048F),	// #senHal_ContPtrs_pSenModesRegsArray[58][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[59][0]
S5K4CDGX_DATA(0x055D),	// #senHal_ContPtrs_pSenModesRegsArray[59][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[60][0]
S5K4CDGX_DATA(0x06BD),	// #senHal_ContPtrs_pSenModesRegsArray[60][1]
S5K4CDGX_DATA(0x01FE),	// #senHal_ContPtrs_pSenModesRegsArray[61][0]
S5K4CDGX_DATA(0x013A),	// #senHal_ContPtrs_pSenModesRegsArray[61][1]
S5K4CDGX_DATA(0x022A),	// #senHal_ContPtrs_pSenModesRegsArray[62][0]
S5K4CDGX_DATA(0x0166),	// #senHal_ContPtrs_pSenModesRegsArray[62][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[63][0]
S5K4CDGX_DATA(0x0362),	// #senHal_ContPtrs_pSenModesRegsArray[63][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[64][0]
S5K4CDGX_DATA(0x0378),	// #senHal_ContPtrs_pSenModesRegsArray[64][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[65][0]
S5K4CDGX_DATA(0x049A),	// #senHal_ContPtrs_pSenModesRegsArray[65][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[66][0]
S5K4CDGX_DATA(0x04C6),	// #senHal_ContPtrs_pSenModesRegsArray[66][1]
S5K4CDGX_DATA(0x020A),	// #senHal_ContPtrs_pSenModesRegsArray[67][0]
S5K4CDGX_DATA(0x0146),	// #senHal_ContPtrs_pSenModesRegsArray[67][1]
S5K4CDGX_DATA(0x023E),	// #senHal_ContPtrs_pSenModesRegsArray[68][0]
S5K4CDGX_DATA(0x017A),	// #senHal_ContPtrs_pSenModesRegsArray[68][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[69][0]
S5K4CDGX_DATA(0x0368),	// #senHal_ContPtrs_pSenModesRegsArray[69][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[70][0]
S5K4CDGX_DATA(0x037C),	// #senHal_ContPtrs_pSenModesRegsArray[70][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[71][0]
S5K4CDGX_DATA(0x04A6),	// #senHal_ContPtrs_pSenModesRegsArray[71][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[72][0]
S5K4CDGX_DATA(0x04DA),	// #senHal_ContPtrs_pSenModesRegsArray[72][1]
S5K4CDGX_DATA(0x0216),	// #senHal_ContPtrs_pSenModesRegsArray[73][0]
S5K4CDGX_DATA(0x0152),	// #senHal_ContPtrs_pSenModesRegsArray[73][1]
S5K4CDGX_DATA(0x023E),	// #senHal_ContPtrs_pSenModesRegsArray[74][0]
S5K4CDGX_DATA(0x017A),	// #senHal_ContPtrs_pSenModesRegsArray[74][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[75][0]
S5K4CDGX_DATA(0x036C),	// #senHal_ContPtrs_pSenModesRegsArray[75][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[76][0]
S5K4CDGX_DATA(0x037C),	// #senHal_ContPtrs_pSenModesRegsArray[76][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[77][0]
S5K4CDGX_DATA(0x04B2),	// #senHal_ContPtrs_pSenModesRegsArray[77][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[78][0]
S5K4CDGX_DATA(0x04DA),	// #senHal_ContPtrs_pSenModesRegsArray[78][1]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[79][0]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[79][1]
S5K4CDGX_DATA(0x0011),	// #senHal_ContPtrs_pSenModesRegsArray[80][0]
S5K4CDGX_DATA(0x0011),	// #senHal_ContPtrs_pSenModesRegsArray[80][1]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[81][0]
S5K4CDGX_DATA(0x0004),	// #senHal_ContPtrs_pSenModesRegsArray[81][1]
S5K4CDGX_DATA(0x0011),	// #senHal_ContPtrs_pSenModesRegsArray[82][0]
S5K4CDGX_DATA(0x0011),	// #senHal_ContPtrs_pSenModesRegsArray[82][1]
S5K4CDGX_DATA(0x01F8),	// #senHal_ContPtrs_pSenModesRegsArray[83][0]
S5K4CDGX_DATA(0x0134),	// #senHal_ContPtrs_pSenModesRegsArray[83][1]
S5K4CDGX_DATA(0x0200),	// #senHal_ContPtrs_pSenModesRegsArray[84][0]
S5K4CDGX_DATA(0x013C),	// #senHal_ContPtrs_pSenModesRegsArray[84][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[85][0]
S5K4CDGX_DATA(0x0362),	// #senHal_ContPtrs_pSenModesRegsArray[85][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[86][0]
S5K4CDGX_DATA(0x036A),	// #senHal_ContPtrs_pSenModesRegsArray[86][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[87][0]
S5K4CDGX_DATA(0x0494),	// #senHal_ContPtrs_pSenModesRegsArray[87][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[88][0]
S5K4CDGX_DATA(0x049C),	// #senHal_ContPtrs_pSenModesRegsArray[88][1]
S5K4CDGX_DATA(0x00EE),	// #senHal_ContPtrs_pSenModesRegsArray[89][0]
S5K4CDGX_DATA(0x00EE),	// #senHal_ContPtrs_pSenModesRegsArray[89][1]
S5K4CDGX_DATA(0x01F6),	// #senHal_ContPtrs_pSenModesRegsArray[90][0]
S5K4CDGX_DATA(0x0132),	// #senHal_ContPtrs_pSenModesRegsArray[90][1]
S5K4CDGX_DATA(0x02C0),	// #senHal_ContPtrs_pSenModesRegsArray[91][0]
S5K4CDGX_DATA(0x01FC),	// #senHal_ContPtrs_pSenModesRegsArray[91][1]
S5K4CDGX_DATA(0x0848),	// #senHal_ContPtrs_pSenModesRegsArray[92][0]
S5K4CDGX_DATA(0x0360),	// #senHal_ContPtrs_pSenModesRegsArray[92][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[93][0]
S5K4CDGX_DATA(0x044E),	// #senHal_ContPtrs_pSenModesRegsArray[93][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[94][0]
S5K4CDGX_DATA(0x0492),	// #senHal_ContPtrs_pSenModesRegsArray[94][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[95][0]
S5K4CDGX_DATA(0x055C),	// #senHal_ContPtrs_pSenModesRegsArray[95][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[96][0]
S5K4CDGX_DATA(0x06C0),	// #senHal_ContPtrs_pSenModesRegsArray[96][1]
S5K4CDGX_DATA(0x01F8),	// #senHal_ContPtrs_pSenModesRegsArray[97][0]
S5K4CDGX_DATA(0x0134),	// #senHal_ContPtrs_pSenModesRegsArray[97][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[98][0]
S5K4CDGX_DATA(0x0362),	// #senHal_ContPtrs_pSenModesRegsArray[98][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[99][0]
S5K4CDGX_DATA(0x0494),	// #senHal_ContPtrs_pSenModesRegsArray[99][1]
S5K4CDGX_DATA(0x0008),	// #senHal_ContPtrs_pSenModesRegsArray[100][0]
S5K4CDGX_DATA(0x0008),	// #senHal_ContPtrs_pSenModesRegsArray[100][1]
S5K4CDGX_DATA(0x2D90),	// #senHal_ContPtrs_pSenModesRegsArray[101][0]
S5K4CDGX_DATA(0x2D90),	// #senHal_ContPtrs_pSenModesRegsArray[101][1]
S5K4CDGX_DATA(0x6531),	// #senHal_ContPtrs_pSenModesRegsArray[102][0]
S5K4CDGX_DATA(0x6531),	// #senHal_ContPtrs_pSenModesRegsArray[102][1]
S5K4CDGX_DATA(0x3E5A),	// #senHal_ContPtrs_pSenModesRegsArray[103][0]
S5K4CDGX_DATA(0x3E5A),	// #senHal_ContPtrs_pSenModesRegsArray[103][1]
S5K4CDGX_DATA(0x1422),	// #senHal_ContPtrs_pSenModesRegsArray[104][0]
S5K4CDGX_DATA(0x1422),	// #senHal_ContPtrs_pSenModesRegsArray[104][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[105][0]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[105][1]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[106][0]
S5K4CDGX_DATA(0x0000),	// #senHal_ContPtrs_pSenModesRegsArray[106][1]
S5K4CDGX_REG(0x1320),	// #gisp_dadlc_config
S5K4CDGX_DATA(0xAAF0),

S5K4CDGX_REG(0x0B32),	// #setot_usSetRomWaitStateThreshold4KHz
S5K4CDGX_DATA(0xFFFF),
S5K4CDGX_REG(0x13EE),	// #pll_uMaxDivFreqMhz
S5K4CDGX_DATA(0x0001),
S5K4CDGX_REG(0x0B2C),	// #setot_uOnlineClocksDiv40
S5K4CDGX_DATA(0x0EA6),

S5K4CDGX_REG(0x1368),
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[0]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[1]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[2]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[3]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[4]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[5]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[6]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainNoBin[7]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainBin[0]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainBin[1]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainBin[2]
S5K4CDGX_DATA(0x2000),	// #gisp_msm_uGainBin[3]

S5K4CDGX_REG(0x1152),	// #senHal_NExpLinesCheckFine
S5K4CDGX_DATA(0x0001),
S5K4CDGX_REG(0x05B2),	// #skl_usConfigStbySettings
S5K4CDGX_DATA(0x0007),

S5K4CDGX_REG(0x323C),	// #TuneHWRegs_gtg_aig_ref_ramp_1
S5K4CDGX_DATA(0x0637),
S5K4CDGX_REG(0x114E),
S5K4CDGX_DATA(0x0AC0),	// #SenHal_ExpMinPixles : 2752 (added 0730)

// New MSM Config  for SNR
S5K4CDGX_REG(0x1390),	
S5K4CDGX_DATA(0x0100),	// #gisp_msm_NonLinearOfsInput_0_
S5K4CDGX_DATA(0x0200),	// #gisp_msm_NonLinearOfsInput_1_
S5K4CDGX_DATA(0x0300),	// #gisp_msm_NonLinearOfsInput_2_
S5K4CDGX_DATA(0x0400),	// #gisp_msm_NonLinearOfsInput_3_
S5K4CDGX_DATA(0x0500),	// #gisp_msm_NonLinearOfsInput_4_
S5K4CDGX_DATA(0x0800),	// #gisp_msm_NonLinearOfsInput_5_
S5K4CDGX_DATA(0x0000),	// #gisp_msm_NonLinearOfsOutput_0_
S5K4CDGX_DATA(0x0000),	// #gisp_msm_NonLinearOfsOutput_1_
S5K4CDGX_DATA(0x0001),	// #gisp_msm_NonLinearOfsOutput_2_
S5K4CDGX_DATA(0x0002),	// #gisp_msm_NonLinearOfsOutput_3_
S5K4CDGX_DATA(0x0003),	// #gisp_msm_NonLinearOfsOutput_4_
S5K4CDGX_DATA(0x0002),	// #gisp_msm_NonLinearOfsOutput_5_
}; 

//out from config1 - step 5 //checked wirh orig - changed analog and digital gain
static struct s5k4cdgx_request s5k4cdgx_init_reg_frame_rate[] ={
//================================================================================================
//	Set Frame Rate
//================================================================================================
//	How to set
//	1. Exposure value
//			dec2hex((1 / (frame rate you want)) * 1000d * 500d)
//	2. Analog Digital gain
//			dec2hex((Analog gain you want) * 256d)
//================================================================================================
// Preview exposure time
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x0574),
S5K4CDGX_DATA(0x61A8),	// #lt_uMaxExp1		: 50ms
S5K4CDGX_DATA(0x0000),
S5K4CDGX_DATA(0xC350),	// #lt_uMaxExp2		: 100ms
S5K4CDGX_DATA(0x0000),
S5K4CDGX_REG(0x1408),
S5K4CDGX_DATA(0xC350),	// #lt_uMaxExp3		: 100ms
S5K4CDGX_DATA(0x0000),
// Capure exposure time
S5K4CDGX_REG(0x057C),
S5K4CDGX_DATA(0x5214),	// #lt_uCapMaxExp1	: 42ms
S5K4CDGX_DATA(0x0000),
S5K4CDGX_DATA(0x8280),	// #lt_uCapMaxExp2	: 66.8ms
S5K4CDGX_DATA(0x0000),
S5K4CDGX_REG(0x140C),
S5K4CDGX_DATA(0x8280),	// #lt_uCapMaxExp3	: 66.8ms
S5K4CDGX_DATA(0x0000),
// Aaxalog gain
S5K4CDGX_REG(0x0584),
S5K4CDGX_DATA(0x0200),	// #lt_uMaxAnGain1	: Analog gain1 = x2.25
S5K4CDGX_DATA(0x0400),//S5K4CDGX_DATA(0x0600),	// #lt_uMaxAnGain2	: Analog gain2 = x6 //modified by zj.for vertical bar
S5K4CDGX_REG(0x1410),
S5K4CDGX_DATA(0x0560),//S5K4CDGX_DATA(0x0600),	// #lt_uMaxAnGain3	: Analog gain3 = x6 //modified by zj.for vertical bar
// Dixgital gain
S5K4CDGX_REG(0x0588),
S5K4CDGX_DATA(0x0100),//S5K4CDGX_DATA(0x0200),	// #lt_uMaxDigGain	: Digital gain = x1 //modified by zj.for vertical bar
S5K4CDGX_REG(0x114E),
S5K4CDGX_DATA(0x0A0C),	// #SenHal_ExpMinPixels	: 2752
};

//out from config1 - step 6 //updated with orig regs - added 3 NPvi clocks
static struct s5k4cdgx_request s5k4cdgx_init_reg_pll[] ={
//================================================================================================
//	Set PLL
//================================================================================================
//	How to set
//	1. MCLK
//		dec2hex(CLK you want) * 1000)
//	2. System CLK
//		dec2hex((CLK you want) * 1000 / 4)
//	3. PCLK
//		dec2hex((CLK you want) * 1000 / 4)
//================================================================================================
// MCLK : 24MHz
S5K4CDGX_REG(0x023C),
S5K4CDGX_DATA(0x4E20),	//#input clock=24MHz //S5K4CDGX_DATA(0x5DC0),	// #REG_TC_IPRM_InClockLSBs
S5K4CDGX_DATA(0x0000),	// #REG_TC_IPRM_InClockMSBs
S5K4CDGX_REG(0x0256),
S5K4CDGX_DATA(0x0003), //S5K4CDGX_DATA(0x0002),	// #REG_TC_IPRM_UseNPviClocks
// System CLK 
S5K4CDGX_REG(0x025E),
S5K4CDGX_DATA(0x3A98),	//#REG_TC_IPRM_OpClk4KHz_0		//1st system CLK 60MHz 
S5K4CDGX_DATA(0x1308),	//#REG_TC_IPRM_MinOutRate4KHz_0		//24MHz
S5K4CDGX_DATA(0x1388),	//#REG_TC_IPRM_MaxOutRate4KHz_0	  

S5K4CDGX_DATA(0x3A98),	//#REG_TC_IPRM_OpClk4KHz_1		//2st system CLK 54MHz 
S5K4CDGX_DATA(0x2E00),	//#REG_TC_IPRM_MinOutRate4KHz_1		//48MHz
S5K4CDGX_DATA(0x2EE0),	//#REG_TC_IPRM_MaxOutRate4KHz_1	  

S5K4CDGX_DATA(0x3A98),	//#REG_TC_IPRM_OpClk4KHz_2		//3st system CLK 54MHz 
S5K4CDGX_DATA(0x4600),	//#REG_TC_IPRM_MinOutRate4KHz_2		//72MHz
S5K4CDGX_DATA(0x4650),	//#REG_TC_IPRM_MaxOutRate4KHz_2	  

S5K4CDGX_REG(0x0272),
S5K4CDGX_DATA(0x0001),	 //#REG_TC_IPRM_InitParamsUpdated
//// Manual 60Hz flicker
//002a 0b76
//0f12 0001
//002A 052E
//0F12 075F
//002A 0514
//0F12 0002
//0F12 0001
S5K4CDGX_REG(0x052E),
S5K4CDGX_DATA(0x075F),    //#REG_TC_DBG_AutoAlgEnBits 
S5K4CDGX_REG(0x0514),
S5K4CDGX_DATA(0x0001),    //#REG_SF_USER_FlickerQuant 
S5K4CDGX_DATA(0x0001),    //#REG_SF_USER_FlickerQuantChanged /S5K4CDGX_PAGE(0x7000),
};

//out from config1 - step 7 //updated with orig regs with two preview configs
static struct s5k4cdgx_request s5k4cdgx_init_preview_conf0[] ={
//==,==0x====),==========================================================================                                 
// p,re0xview), configuration
//==,==0x====),==========================================================================      
//PR,EV0xIEW ),CONFIGURATION 0 (VGA, YUV, 30fps)
S5K4CDGX_REG(0x02E6),
S5K4CDGX_DATA(0x0280),  //#REG_0TC_PCFG_usWidth                  640
S5K4CDGX_DATA(0x01e0),  //#REG_0TC_PCFG_usHeight                 480
S5K4CDGX_DATA(0x0005),  //#REG_0TC_PCFG_Format                   YUV
S5K4CDGX_DATA(0x1388),  //#REG_0TC_PCFG_usMaxOut4KHzRate               PCLK max
S5K4CDGX_DATA(0x1308),  //#REG_0TC_PCFG_usMinOut4KHzRate                     PCLK min
S5K4CDGX_DATA(0x0100),  //#REG_0TC_PCFG_outclkpre88
S5K4CDGX_DATA(0x0300),  //#REG_0TC_PCFG_ubpp88
S5K4CDGX_DATA(0x0042),  //#REG_0TC_PCFG_PVIMask
S5K4CDGX_DATA(0x0000),  //#REG_0TC_PCFG_OIFMask
S5K4CDGX_DATA(0x01E0),  //#REG_0TC_PCFG_usjepg pak
S5K4CDGX_DATA(0xC69C),  //#REG_0TC_PCFG_usjepg total
S5K4CDGX_DATA(0x0000),  //#REG_0TC_PCFG_uClockInd
S5K4CDGX_DATA(0x0000),  //#REG_0TC_PCFG_usFrTimeType
S5K4CDGX_DATA(0x0001),  //#REG_0TC_PCFG_FrRateQualityType 1b: FR (bin) 2b: Quality (no-bin)
S5K4CDGX_DATA(0x0535),  //#REG_0TC_PCFG_usMaxFrTimeMsecMult10
S5K4CDGX_DATA(0x029a),  //#REG_0TC_PCFG_usMinFrTimeMsecMult10
//==,==0x====),==========================================================================                                 
// p,re0xview), configuration
//==,==0x====),==========================================================================      
//PR,EV0xIEW ),CONFIGURATION 1 (800 600, YUV, 30fps)  
//S5K4CDGX_REG(0x0316),
//S5K4CDGX_DATA(0x0320),	//#REG_2TC_PCFG_usWidth	                 800	    					
//S5K4CDGX_DATA(0x0258),	//#REG_2TC_PCFG_usHeight                 600	
//S5K4CDGX_DATA(0x0005),	//#REG_2TC_PCFG_Format	                 YUV	
//S5K4CDGX_DATA(0x1388),	//#REG_2TC_PCFG_usMaxOut4KHzRate	       PCLK max
//S5K4CDGX_DATA(0x1308),	//#REG_2TC_PCFG_usMinOut4KHzRate		     PCLK min
//S5K4CDGX_DATA(0x0100),  //#REG_2TC_PCFG_outclkpre88
//S5K4CDGX_DATA(0x0300),	//#REG_2TC_PCFG_ubpp88
//S5K4CDGX_DATA(0x0042),	//#REG_2TC_PCFG_PVIMask 
//S5K4CDGX_DATA(0x0000),  //#REG_2TC_PCFG_OIFMask
//S5K4CDGX_DATA(0x01E0),  //#REG_2TC_PCFG_usjepg pak
//S5K4CDGX_DATA(0xC69C),  //#REG_2TC_PCFG_usjepg total
//S5K4CDGX_DATA(0x0000),	//#REG_2TC_PCFG_uClockInd         
//S5K4CDGX_DATA(0x0000),	//#REG_2TC_PCFG_usFrTimeType			
//S5K4CDGX_DATA(0x0001),	//#REG_2TC_PCFG_FrRateQualityType 1b: FR (bin) 2b: Quality (no-bin)		     
//S5K4CDGX_DATA(0x0535),	//#REG_2TC_PCFG_usMaxFrTimeMsecMult10	  
//S5K4CDGX_DATA(0x029a),	//#REG_2TC_PCFG_usMinFrTimeMsecMult10	
//==========================================================================    

//renn add start for test
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x0310), 
S5K4CDGX_DATA(0x0001),//S5K4CDGX_DATA(0x0000),   //normal  0x0000, mirror 0x0001. //modified by zj.for hmirror.
S5K4CDGX_DATA(0x0001),//S5K4CDGX_DATA(0x0000), //normal  0x0000, mirror 0x0001. //modified by zj.for hmirror.
S5K4CDGX_REG(0x0340), 
S5K4CDGX_DATA(0x0001),//S5K4CDGX_DATA(0x0000),  //normal  0x0000, mirror 0x0001. //modified by zj.for hmirror.
S5K4CDGX_DATA(0x0001),//S5K4CDGX_DATA(0x0000), //normal  0x0000, mirror 0x0001. //modified by zj.for hmirror.
};

//out from config1 - step 8 //updated with orig regs
static struct s5k4cdgx_request s5k4cdgx_init_capture_conf0[] ={
//CAPTURE,CONFIGURATION 0 (2048x1536, YUV, 20fps)  
S5K4CDGX_REG(0x03D8),  
S5K4CDGX_DATA(0x0800),	//#REG_0TC_CCFG_usWidth
S5K4CDGX_DATA(0x0600),	//#REG_0TC_CCFG_usHeight
S5K4CDGX_DATA(0x0005),	//#REG_0TC_CCFG_Format YUV
S5K4CDGX_REG(0x03EE), 
S5K4CDGX_DATA(0x0000),	//#REG_0TC_CCFG_uClockInd PLL config
S5K4CDGX_REG(0x03DE),
S5K4CDGX_DATA(0x1388),	//0x2904 //#REG_0TC_CCFG_usMaxOut4KHzRate	          PCLK max	                                                   
S5K4CDGX_DATA(0x1308),	//0x2904 //#REG_0TC_CCFG_usMinOut4KHzRate	          PCLK min                                                        
S5K4CDGX_REG(0x03E6),
S5K4CDGX_DATA(0x0042),	//#REG_0TC_CCFG_PVIMask 
S5K4CDGX_REG(0x03F2),
S5K4CDGX_DATA(0x0002),	 //#REG_0TC_CCFG_FrRateQualityType	          1b: FR (bin) 2b: Quality (no-bin)
S5K4CDGX_REG(0x03F0),
S5K4CDGX_DATA(0x0000),	// #REG_0TC_CCFG_usFrTimeType
S5K4CDGX_REG(0x03F4),
S5K4CDGX_DATA(0x1200),	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10: 30 fps(0x014D)  25fps(0x0190)  20fps(0x01F4) 15fps(0x029A)  10fps(0x03E8)  7.5fps(0x0535)  4.0fps(0x09C4)
S5K4CDGX_DATA(0x0000),	//#REG_0TC_CCFG_usMinFrTimeMsecMult10
};

// Update preview & capture configurations, run preview and set host interrupt //updated with orig
static struct s5k4cdgx_request s5k4cdgx_init_reg_config1[] ={
// Update preview & capture configuration
S5K4CDGX_REG(0x02AC),
S5K4CDGX_DATA(0x0001),	//#REG_TC_GP_ActivePrevConfig
S5K4CDGX_REG(0x02B0),
S5K4CDGX_DATA(0x0001),	//#REG_TC_GP_PrevOpenAfterChanged
S5K4CDGX_REG(0x0298),
S5K4CDGX_DATA(0x0001),	//#REG_TC_GP_NewConfigSync
S5K4CDGX_REG(0x02AE),
S5K4CDGX_DATA(0x0001),	//#REG_TC_GP_PrevConfigChange
S5K4CDGX_REG(0x0288),
S5K4CDGX_DATA(0x0001),	//#REG_TC_GP_EnablePreview
S5K4CDGX_DATA(0x0001),	//#REG_TC_GP_EnablePreviewChanged

// Run preivew
S5K4CDGX_REG(0x0288),
S5K4CDGX_DATA(0x0001),	// #REG_TC_GP_EnablePreview
S5K4CDGX_REG(0x028A),
S5K4CDGX_DATA(0x0001),	// #REG_TC_GP_EnablePreviewChanged

// Set host interrupt so main start run
S5K4CDGX_PAGE(0xD000),
S5K4CDGX_REG(0x1000),
S5K4CDGX_DATA(0x0001),
};

//out from config2 - step 1 //this is different from orig and have to be revised
static struct s5k4cdgx_request s5k4cdgx_init_reg_jpeg[] ={
//p10

//0028 D000  // color bar
//002A 3100
//0F12 0002

//================================================================================================
//	Set JPEG option
//================================================================================================
// Set Q-value
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x04B6),
S5K4CDGX_DATA(0x0000),	// #REG_RC_BRC_BRC_type
S5K4CDGX_DATA(0x005A),	// #Preview_BRC : Super Fine (90d)
S5K4CDGX_DATA(0x005A),	// #Capture_BRC : Super Fine (90d)
//// Set thumbnail
//002A 04BC
//0F12 0001	// #REG_TC_THUMB_Thumb_bActive
//0F12 0140 // 0280	// #REG_TC_THUMB_Thumb_uWidth
//0F12 00F0 // 01E0	// #REG_TC_THUMB_Thumb_uHeight
//0F12 0000	// #REG_TC_THUMB_Thumb_Format
//// Set spoof size
//002A 03E8
//0F12 0050	// #REG_0TC_CCFG_OIFMask : SPOOF_EN + JPEG8
//0F12 03C0	// #REG_0TC_CCFG_usJpegPacketSize
//0F12 08FC	// #REG_0TC_CCFG_usJpegTotalPackets
// c,ha0xnge ),the R or B gain  by yourself
// e,xp0x: if), yellownish , add some b gain-> Change 055c to 057c
S5K4CDGX_PAGE(0x7000),
S5K4CDGX_REG(0x0e72),
S5K4CDGX_DATA(0x053c),    //r
S5K4CDGX_DATA(0x0400),    //g this can not change
S5K4CDGX_DATA(0x055c),    //b
};

//out from config2 - step 2 //this one is completely deferent from orig. Changed!
static struct s5k4cdgx_request s5k4cdgx_init_reg_gas[] ={
//================================================================================================
//	Set GAS (Grid Anti-shading)
//================================================================================================
// S,et0x GAS), LUT
S5K4CDGX_REG(0x347C),  
S5K4CDGX_DATA(0x00D6),	// #TVAR_ash_pGAS[0]
S5K4CDGX_DATA(0x00A2),	// #TVAR_ash_pGAS[1]
S5K4CDGX_DATA(0x0085),	// #TVAR_ash_pGAS[2]
S5K4CDGX_DATA(0x0086),	// #TVAR_ash_pGAS[3]
S5K4CDGX_DATA(0x0076),	// #TVAR_ash_pGAS[4]
S5K4CDGX_DATA(0x006F),	// #TVAR_ash_pGAS[5]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[6]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[7]
S5K4CDGX_DATA(0x0081),	// #TVAR_ash_pGAS[8]
S5K4CDGX_DATA(0x0088),	// #TVAR_ash_pGAS[9]
S5K4CDGX_DATA(0x0096),	// #TVAR_ash_pGAS[10]
S5K4CDGX_DATA(0x00B1),	// #TVAR_ash_pGAS[11]
S5K4CDGX_DATA(0x00E4),	// #TVAR_ash_pGAS[12]
S5K4CDGX_DATA(0x00B0),	// #TVAR_ash_pGAS[13]
S5K4CDGX_DATA(0x0080),	// #TVAR_ash_pGAS[14]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[15]
S5K4CDGX_DATA(0x006D),	// #TVAR_ash_pGAS[16]
S5K4CDGX_DATA(0x005E),	// #TVAR_ash_pGAS[17]
S5K4CDGX_DATA(0x0051),	// #TVAR_ash_pGAS[18]
S5K4CDGX_DATA(0x004F),	// #TVAR_ash_pGAS[19]
S5K4CDGX_DATA(0x0050),	// #TVAR_ash_pGAS[20]
S5K4CDGX_DATA(0x005D),	// #TVAR_ash_pGAS[21]
S5K4CDGX_DATA(0x0073),	// #TVAR_ash_pGAS[22]
S5K4CDGX_DATA(0x0086),	// #TVAR_ash_pGAS[23]
S5K4CDGX_DATA(0x0097),	// #TVAR_ash_pGAS[24]
S5K4CDGX_DATA(0x00BB),	// #TVAR_ash_pGAS[25]
S5K4CDGX_DATA(0x0083),	// #TVAR_ash_pGAS[26]
S5K4CDGX_DATA(0x0075),	// #TVAR_ash_pGAS[27]
S5K4CDGX_DATA(0x0068),	// #TVAR_ash_pGAS[28]
S5K4CDGX_DATA(0x0053),	// #TVAR_ash_pGAS[29]
S5K4CDGX_DATA(0x0040),	// #TVAR_ash_pGAS[30]
S5K4CDGX_DATA(0x0031),	// #TVAR_ash_pGAS[31]
S5K4CDGX_DATA(0x002D),	// #TVAR_ash_pGAS[32]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[33]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[34]
S5K4CDGX_DATA(0x004E),	// #TVAR_ash_pGAS[35]
S5K4CDGX_DATA(0x006E),	// #TVAR_ash_pGAS[36]
S5K4CDGX_DATA(0x0083),	// #TVAR_ash_pGAS[37]
S5K4CDGX_DATA(0x009A),	// #TVAR_ash_pGAS[38]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[39]
S5K4CDGX_DATA(0x006C),	// #TVAR_ash_pGAS[40]
S5K4CDGX_DATA(0x0058),	// #TVAR_ash_pGAS[41]
S5K4CDGX_DATA(0x003F),	// #TVAR_ash_pGAS[42]
S5K4CDGX_DATA(0x002A),	// #TVAR_ash_pGAS[43]
S5K4CDGX_DATA(0x001C),	// #TVAR_ash_pGAS[44]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[45]
S5K4CDGX_DATA(0x0019),	// #TVAR_ash_pGAS[46]
S5K4CDGX_DATA(0x0022),	// #TVAR_ash_pGAS[47]
S5K4CDGX_DATA(0x0037),	// #TVAR_ash_pGAS[48]
S5K4CDGX_DATA(0x0054),	// #TVAR_ash_pGAS[49]
S5K4CDGX_DATA(0x007B),	// #TVAR_ash_pGAS[50]
S5K4CDGX_DATA(0x008C),	// #TVAR_ash_pGAS[51]
S5K4CDGX_DATA(0x0071),	// #TVAR_ash_pGAS[52]
S5K4CDGX_DATA(0x0064),	// #TVAR_ash_pGAS[53]
S5K4CDGX_DATA(0x004C),	// #TVAR_ash_pGAS[54]
S5K4CDGX_DATA(0x0033),	// #TVAR_ash_pGAS[55]
S5K4CDGX_DATA(0x001D),	// #TVAR_ash_pGAS[56]
S5K4CDGX_DATA(0x000C),	// #TVAR_ash_pGAS[57]
S5K4CDGX_DATA(0x0006),	// #TVAR_ash_pGAS[58]
S5K4CDGX_DATA(0x0009),	// #TVAR_ash_pGAS[59]
S5K4CDGX_DATA(0x0015),	// #TVAR_ash_pGAS[60]
S5K4CDGX_DATA(0x002A),	// #TVAR_ash_pGAS[61]
S5K4CDGX_DATA(0x0046),	// #TVAR_ash_pGAS[62]
S5K4CDGX_DATA(0x0071),	// #TVAR_ash_pGAS[63]
S5K4CDGX_DATA(0x0088),	// #TVAR_ash_pGAS[64]
S5K4CDGX_DATA(0x0068),	// #TVAR_ash_pGAS[65]
S5K4CDGX_DATA(0x0061),	// #TVAR_ash_pGAS[66]
S5K4CDGX_DATA(0x0048),	// #TVAR_ash_pGAS[67]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[68]
S5K4CDGX_DATA(0x0017),	// #TVAR_ash_pGAS[69]
S5K4CDGX_DATA(0x0007),	// #TVAR_ash_pGAS[70]
S5K4CDGX_DATA(0x0000),	// #TVAR_ash_pGAS[71]
S5K4CDGX_DATA(0x0005),	// #TVAR_ash_pGAS[72]
S5K4CDGX_DATA(0x0010),	// #TVAR_ash_pGAS[73]
S5K4CDGX_DATA(0x0025),	// #TVAR_ash_pGAS[74]
S5K4CDGX_DATA(0x0042),	// #TVAR_ash_pGAS[75]
S5K4CDGX_DATA(0x006D),	// #TVAR_ash_pGAS[76]
S5K4CDGX_DATA(0x007D),	// #TVAR_ash_pGAS[77]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[78]
S5K4CDGX_DATA(0x0063),	// #TVAR_ash_pGAS[79]
S5K4CDGX_DATA(0x004B),	// #TVAR_ash_pGAS[80]
S5K4CDGX_DATA(0x0033),	// #TVAR_ash_pGAS[81]
S5K4CDGX_DATA(0x001C),	// #TVAR_ash_pGAS[82]
S5K4CDGX_DATA(0x000E),	// #TVAR_ash_pGAS[83]
S5K4CDGX_DATA(0x0006),	// #TVAR_ash_pGAS[84]
S5K4CDGX_DATA(0x0009),	// #TVAR_ash_pGAS[85]
S5K4CDGX_DATA(0x0018),	// #TVAR_ash_pGAS[86]
S5K4CDGX_DATA(0x002C),	// #TVAR_ash_pGAS[87]
S5K4CDGX_DATA(0x004A),	// #TVAR_ash_pGAS[88]
S5K4CDGX_DATA(0x0076),	// #TVAR_ash_pGAS[89]
S5K4CDGX_DATA(0x008B),	// #TVAR_ash_pGAS[90]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[91]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[92]
S5K4CDGX_DATA(0x0059),	// #TVAR_ash_pGAS[93]
S5K4CDGX_DATA(0x0041),	// #TVAR_ash_pGAS[94]
S5K4CDGX_DATA(0x002C),	// #TVAR_ash_pGAS[95]
S5K4CDGX_DATA(0x001D),	// #TVAR_ash_pGAS[96]
S5K4CDGX_DATA(0x0017),	// #TVAR_ash_pGAS[97]
S5K4CDGX_DATA(0x001B),	// #TVAR_ash_pGAS[98]
S5K4CDGX_DATA(0x0027),	// #TVAR_ash_pGAS[99]
S5K4CDGX_DATA(0x003D),	// #TVAR_ash_pGAS[100]
S5K4CDGX_DATA(0x005D),	// #TVAR_ash_pGAS[101]
S5K4CDGX_DATA(0x0089),	// #TVAR_ash_pGAS[102]
S5K4CDGX_DATA(0x0097),	// #TVAR_ash_pGAS[103]
S5K4CDGX_DATA(0x0082),	// #TVAR_ash_pGAS[104]
S5K4CDGX_DATA(0x0078),	// #TVAR_ash_pGAS[105]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[106]
S5K4CDGX_DATA(0x0056),	// #TVAR_ash_pGAS[107]
S5K4CDGX_DATA(0x0042),	// #TVAR_ash_pGAS[108]
S5K4CDGX_DATA(0x0035),	// #TVAR_ash_pGAS[109]
S5K4CDGX_DATA(0x002F),	// #TVAR_ash_pGAS[110]
S5K4CDGX_DATA(0x0034),	// #TVAR_ash_pGAS[111]
S5K4CDGX_DATA(0x003E),	// #TVAR_ash_pGAS[112]
S5K4CDGX_DATA(0x0055),	// #TVAR_ash_pGAS[113]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[114]
S5K4CDGX_DATA(0x0095),	// #TVAR_ash_pGAS[115]
S5K4CDGX_DATA(0x00B1),	// #TVAR_ash_pGAS[116]
S5K4CDGX_DATA(0x00AC),	// #TVAR_ash_pGAS[117]
S5K4CDGX_DATA(0x008A),	// #TVAR_ash_pGAS[118]
S5K4CDGX_DATA(0x0083),	// #TVAR_ash_pGAS[119]
S5K4CDGX_DATA(0x0077),	// #TVAR_ash_pGAS[120]
S5K4CDGX_DATA(0x0065),	// #TVAR_ash_pGAS[121]
S5K4CDGX_DATA(0x0057),	// #TVAR_ash_pGAS[122]
S5K4CDGX_DATA(0x0054),	// #TVAR_ash_pGAS[123]
S5K4CDGX_DATA(0x0056),	// #TVAR_ash_pGAS[124]
S5K4CDGX_DATA(0x0062),	// #TVAR_ash_pGAS[125]
S5K4CDGX_DATA(0x007A),	// #TVAR_ash_pGAS[126]
S5K4CDGX_DATA(0x0097),	// #TVAR_ash_pGAS[127]
S5K4CDGX_DATA(0x00AA),	// #TVAR_ash_pGAS[128]
S5K4CDGX_DATA(0x00D1),	// #TVAR_ash_pGAS[129]
S5K4CDGX_DATA(0x00CF),	// #TVAR_ash_pGAS[130]
S5K4CDGX_DATA(0x00A2),	// #TVAR_ash_pGAS[131]
S5K4CDGX_DATA(0x0091),	// #TVAR_ash_pGAS[132]
S5K4CDGX_DATA(0x008E),	// #TVAR_ash_pGAS[133]
S5K4CDGX_DATA(0x007E),	// #TVAR_ash_pGAS[134]
S5K4CDGX_DATA(0x0074),	// #TVAR_ash_pGAS[135]
S5K4CDGX_DATA(0x0071),	// #TVAR_ash_pGAS[136]
S5K4CDGX_DATA(0x0074),	// #TVAR_ash_pGAS[137]
S5K4CDGX_DATA(0x0082),	// #TVAR_ash_pGAS[138]
S5K4CDGX_DATA(0x0097),	// #TVAR_ash_pGAS[139]
S5K4CDGX_DATA(0x00A3),	// #TVAR_ash_pGAS[140]
S5K4CDGX_DATA(0x00C0),	// #TVAR_ash_pGAS[141]
S5K4CDGX_DATA(0x00F8),	// #TVAR_ash_pGAS[142]
S5K4CDGX_DATA(0x00B6),	// #TVAR_ash_pGAS[143]
S5K4CDGX_DATA(0x0080),	// #TVAR_ash_pGAS[144]
S5K4CDGX_DATA(0x006A),	// #TVAR_ash_pGAS[145]
S5K4CDGX_DATA(0x006D),	// #TVAR_ash_pGAS[146]
S5K4CDGX_DATA(0x0066),	// #TVAR_ash_pGAS[147]
S5K4CDGX_DATA(0x0062),	// #TVAR_ash_pGAS[148]
S5K4CDGX_DATA(0x0062),	// #TVAR_ash_pGAS[149]
S5K4CDGX_DATA(0x0069),	// #TVAR_ash_pGAS[150]
S5K4CDGX_DATA(0x006F),	// #TVAR_ash_pGAS[151]
S5K4CDGX_DATA(0x007D),	// #TVAR_ash_pGAS[152]
S5K4CDGX_DATA(0x0082),	// #TVAR_ash_pGAS[153]
S5K4CDGX_DATA(0x0093),	// #TVAR_ash_pGAS[154]
S5K4CDGX_DATA(0x00D0),	// #TVAR_ash_pGAS[155]
S5K4CDGX_DATA(0x008B),	// #TVAR_ash_pGAS[156]
S5K4CDGX_DATA(0x0069),	// #TVAR_ash_pGAS[157]
S5K4CDGX_DATA(0x0065),	// #TVAR_ash_pGAS[158]
S5K4CDGX_DATA(0x005E),	// #TVAR_ash_pGAS[159]
S5K4CDGX_DATA(0x004F),	// #TVAR_ash_pGAS[160]
S5K4CDGX_DATA(0x004A),	// #TVAR_ash_pGAS[161]
S5K4CDGX_DATA(0x0048),	// #TVAR_ash_pGAS[162]
S5K4CDGX_DATA(0x004C),	// #TVAR_ash_pGAS[163]
S5K4CDGX_DATA(0x0057),	// #TVAR_ash_pGAS[164]
S5K4CDGX_DATA(0x0068),	// #TVAR_ash_pGAS[165]
S5K4CDGX_DATA(0x0076),	// #TVAR_ash_pGAS[166]
S5K4CDGX_DATA(0x007F),	// #TVAR_ash_pGAS[167]
S5K4CDGX_DATA(0x00A7),	// #TVAR_ash_pGAS[168]
S5K4CDGX_DATA(0x006B),	// #TVAR_ash_pGAS[169]
S5K4CDGX_DATA(0x0060),	// #TVAR_ash_pGAS[170]
S5K4CDGX_DATA(0x0059),	// #TVAR_ash_pGAS[171]
S5K4CDGX_DATA(0x0046),	// #TVAR_ash_pGAS[172]
S5K4CDGX_DATA(0x0035),	// #TVAR_ash_pGAS[173]
S5K4CDGX_DATA(0x002B),	// #TVAR_ash_pGAS[174]
S5K4CDGX_DATA(0x0027),	// #TVAR_ash_pGAS[175]
S5K4CDGX_DATA(0x002A),	// #TVAR_ash_pGAS[176]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[177]
S5K4CDGX_DATA(0x004A),	// #TVAR_ash_pGAS[178]
S5K4CDGX_DATA(0x0065),	// #TVAR_ash_pGAS[179]
S5K4CDGX_DATA(0x0077),	// #TVAR_ash_pGAS[180]
S5K4CDGX_DATA(0x0087),	// #TVAR_ash_pGAS[181]
S5K4CDGX_DATA(0x0061),	// #TVAR_ash_pGAS[182]
S5K4CDGX_DATA(0x005E),	// #TVAR_ash_pGAS[183]
S5K4CDGX_DATA(0x004B),	// #TVAR_ash_pGAS[184]
S5K4CDGX_DATA(0x0034),	// #TVAR_ash_pGAS[185]
S5K4CDGX_DATA(0x0022),	// #TVAR_ash_pGAS[186]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[187]
S5K4CDGX_DATA(0x0012),	// #TVAR_ash_pGAS[188]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[189]
S5K4CDGX_DATA(0x0023),	// #TVAR_ash_pGAS[190]
S5K4CDGX_DATA(0x0036),	// #TVAR_ash_pGAS[191]
S5K4CDGX_DATA(0x0052),	// #TVAR_ash_pGAS[192]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[193]
S5K4CDGX_DATA(0x0083),	// #TVAR_ash_pGAS[194]
S5K4CDGX_DATA(0x005C),	// #TVAR_ash_pGAS[195]
S5K4CDGX_DATA(0x0057),	// #TVAR_ash_pGAS[196]
S5K4CDGX_DATA(0x0042),	// #TVAR_ash_pGAS[197]
S5K4CDGX_DATA(0x0029),	// #TVAR_ash_pGAS[198]
S5K4CDGX_DATA(0x0017),	// #TVAR_ash_pGAS[199]
S5K4CDGX_DATA(0x0009),	// #TVAR_ash_pGAS[200]
S5K4CDGX_DATA(0x0005),	// #TVAR_ash_pGAS[201]
S5K4CDGX_DATA(0x0009),	// #TVAR_ash_pGAS[202]
S5K4CDGX_DATA(0x0015),	// #TVAR_ash_pGAS[203]
S5K4CDGX_DATA(0x002B),	// #TVAR_ash_pGAS[204]
S5K4CDGX_DATA(0x0047),	// #TVAR_ash_pGAS[205]
S5K4CDGX_DATA(0x006B),	// #TVAR_ash_pGAS[206]
S5K4CDGX_DATA(0x007E),	// #TVAR_ash_pGAS[207]
S5K4CDGX_DATA(0x005C),	// #TVAR_ash_pGAS[208]
S5K4CDGX_DATA(0x0053),	// #TVAR_ash_pGAS[209]
S5K4CDGX_DATA(0x003B),	// #TVAR_ash_pGAS[210]
S5K4CDGX_DATA(0x0025),	// #TVAR_ash_pGAS[211]
S5K4CDGX_DATA(0x0013),	// #TVAR_ash_pGAS[212]
S5K4CDGX_DATA(0x0005),	// #TVAR_ash_pGAS[213]
S5K4CDGX_DATA(0x0001),	// #TVAR_ash_pGAS[214]
S5K4CDGX_DATA(0x0005),	// #TVAR_ash_pGAS[215]
S5K4CDGX_DATA(0x0011),	// #TVAR_ash_pGAS[216]
S5K4CDGX_DATA(0x0028),	// #TVAR_ash_pGAS[217]
S5K4CDGX_DATA(0x0043),	// #TVAR_ash_pGAS[218]
S5K4CDGX_DATA(0x0069),	// #TVAR_ash_pGAS[219]
S5K4CDGX_DATA(0x007F),	// #TVAR_ash_pGAS[220]
S5K4CDGX_DATA(0x005C),	// #TVAR_ash_pGAS[221]
S5K4CDGX_DATA(0x0056),	// #TVAR_ash_pGAS[222]
S5K4CDGX_DATA(0x0040),	// #TVAR_ash_pGAS[223]
S5K4CDGX_DATA(0x0029),	// #TVAR_ash_pGAS[224]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[225]
S5K4CDGX_DATA(0x000A),	// #TVAR_ash_pGAS[226]
S5K4CDGX_DATA(0x0005),	// #TVAR_ash_pGAS[227]
S5K4CDGX_DATA(0x000A),	// #TVAR_ash_pGAS[228]
S5K4CDGX_DATA(0x0019),	// #TVAR_ash_pGAS[229]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[230]
S5K4CDGX_DATA(0x004D),	// #TVAR_ash_pGAS[231]
S5K4CDGX_DATA(0x0073),	// #TVAR_ash_pGAS[232]
S5K4CDGX_DATA(0x0089),	// #TVAR_ash_pGAS[233]
S5K4CDGX_DATA(0x0067),	// #TVAR_ash_pGAS[234]
S5K4CDGX_DATA(0x005F),	// #TVAR_ash_pGAS[235]
S5K4CDGX_DATA(0x004F),	// #TVAR_ash_pGAS[236]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[237]
S5K4CDGX_DATA(0x0025),	// #TVAR_ash_pGAS[238]
S5K4CDGX_DATA(0x0019),	// #TVAR_ash_pGAS[239]
S5K4CDGX_DATA(0x0015),	// #TVAR_ash_pGAS[240]
S5K4CDGX_DATA(0x001B),	// #TVAR_ash_pGAS[241]
S5K4CDGX_DATA(0x0029),	// #TVAR_ash_pGAS[242]
S5K4CDGX_DATA(0x0043),	// #TVAR_ash_pGAS[243]
S5K4CDGX_DATA(0x0062),	// #TVAR_ash_pGAS[244]
S5K4CDGX_DATA(0x0084),	// #TVAR_ash_pGAS[245]
S5K4CDGX_DATA(0x0094),	// #TVAR_ash_pGAS[246]
S5K4CDGX_DATA(0x0074),	// #TVAR_ash_pGAS[247]
S5K4CDGX_DATA(0x0068),	// #TVAR_ash_pGAS[248]
S5K4CDGX_DATA(0x0060),	// #TVAR_ash_pGAS[249]
S5K4CDGX_DATA(0x004D),	// #TVAR_ash_pGAS[250]
S5K4CDGX_DATA(0x003B),	// #TVAR_ash_pGAS[251]
S5K4CDGX_DATA(0x0032),	// #TVAR_ash_pGAS[252]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[253]
S5K4CDGX_DATA(0x0036),	// #TVAR_ash_pGAS[254]
S5K4CDGX_DATA(0x0044),	// #TVAR_ash_pGAS[255]
S5K4CDGX_DATA(0x005E),	// #TVAR_ash_pGAS[256]
S5K4CDGX_DATA(0x0081),	// #TVAR_ash_pGAS[257]
S5K4CDGX_DATA(0x009B),	// #TVAR_ash_pGAS[258]
S5K4CDGX_DATA(0x00AE),	// #TVAR_ash_pGAS[259]
S5K4CDGX_DATA(0x0093),	// #TVAR_ash_pGAS[260]
S5K4CDGX_DATA(0x0076),	// #TVAR_ash_pGAS[261]
S5K4CDGX_DATA(0x0078),	// #TVAR_ash_pGAS[262]
S5K4CDGX_DATA(0x006D),	// #TVAR_ash_pGAS[263]
S5K4CDGX_DATA(0x005D),	// #TVAR_ash_pGAS[264]
S5K4CDGX_DATA(0x0057),	// #TVAR_ash_pGAS[265]
S5K4CDGX_DATA(0x0054),	// #TVAR_ash_pGAS[266]
S5K4CDGX_DATA(0x005D),	// #TVAR_ash_pGAS[267]
S5K4CDGX_DATA(0x006F),	// #TVAR_ash_pGAS[268]
S5K4CDGX_DATA(0x0087),	// #TVAR_ash_pGAS[269]
S5K4CDGX_DATA(0x00A0),	// #TVAR_ash_pGAS[270]
S5K4CDGX_DATA(0x00AD),	// #TVAR_ash_pGAS[271]
S5K4CDGX_DATA(0x00C9),	// #TVAR_ash_pGAS[272]
S5K4CDGX_DATA(0x00C2),	// #TVAR_ash_pGAS[273]
S5K4CDGX_DATA(0x008C),	// #TVAR_ash_pGAS[274]
S5K4CDGX_DATA(0x007D),	// #TVAR_ash_pGAS[275]
S5K4CDGX_DATA(0x007E),	// #TVAR_ash_pGAS[276]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[277]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[278]
S5K4CDGX_DATA(0x0071),	// #TVAR_ash_pGAS[279]
S5K4CDGX_DATA(0x007F),	// #TVAR_ash_pGAS[280]
S5K4CDGX_DATA(0x008E),	// #TVAR_ash_pGAS[281]
S5K4CDGX_DATA(0x009F),	// #TVAR_ash_pGAS[282]
S5K4CDGX_DATA(0x00AA),	// #TVAR_ash_pGAS[283]
S5K4CDGX_DATA(0x00BD),	// #TVAR_ash_pGAS[284]
S5K4CDGX_DATA(0x00F5),	// #TVAR_ash_pGAS[285]
S5K4CDGX_DATA(0x00B8),	// #TVAR_ash_pGAS[286]
S5K4CDGX_DATA(0x0081),	// #TVAR_ash_pGAS[287]
S5K4CDGX_DATA(0x006B),	// #TVAR_ash_pGAS[288]
S5K4CDGX_DATA(0x006B),	// #TVAR_ash_pGAS[289]
S5K4CDGX_DATA(0x0065),	// #TVAR_ash_pGAS[290]
S5K4CDGX_DATA(0x005F),	// #TVAR_ash_pGAS[291]
S5K4CDGX_DATA(0x005C),	// #TVAR_ash_pGAS[292]
S5K4CDGX_DATA(0x0068),	// #TVAR_ash_pGAS[293]
S5K4CDGX_DATA(0x0077),	// #TVAR_ash_pGAS[294]
S5K4CDGX_DATA(0x008B),	// #TVAR_ash_pGAS[295]
S5K4CDGX_DATA(0x0093),	// #TVAR_ash_pGAS[296]
S5K4CDGX_DATA(0x00A9),	// #TVAR_ash_pGAS[297]
S5K4CDGX_DATA(0x00EA),	// #TVAR_ash_pGAS[298]
S5K4CDGX_DATA(0x0093),	// #TVAR_ash_pGAS[299]
S5K4CDGX_DATA(0x006C),	// #TVAR_ash_pGAS[300]
S5K4CDGX_DATA(0x0068),	// #TVAR_ash_pGAS[301]
S5K4CDGX_DATA(0x005F),	// #TVAR_ash_pGAS[302]
S5K4CDGX_DATA(0x004E),	// #TVAR_ash_pGAS[303]
S5K4CDGX_DATA(0x0047),	// #TVAR_ash_pGAS[304]
S5K4CDGX_DATA(0x0047),	// #TVAR_ash_pGAS[305]
S5K4CDGX_DATA(0x004E),	// #TVAR_ash_pGAS[306]
S5K4CDGX_DATA(0x005F),	// #TVAR_ash_pGAS[307]
S5K4CDGX_DATA(0x0077),	// #TVAR_ash_pGAS[308]
S5K4CDGX_DATA(0x008E),	// #TVAR_ash_pGAS[309]
S5K4CDGX_DATA(0x0099),	// #TVAR_ash_pGAS[310]
S5K4CDGX_DATA(0x00BD),	// #TVAR_ash_pGAS[311]
S5K4CDGX_DATA(0x0075),	// #TVAR_ash_pGAS[312]
S5K4CDGX_DATA(0x0066),	// #TVAR_ash_pGAS[313]
S5K4CDGX_DATA(0x005E),	// #TVAR_ash_pGAS[314]
S5K4CDGX_DATA(0x0049),	// #TVAR_ash_pGAS[315]
S5K4CDGX_DATA(0x0036),	// #TVAR_ash_pGAS[316]
S5K4CDGX_DATA(0x002B),	// #TVAR_ash_pGAS[317]
S5K4CDGX_DATA(0x0027),	// #TVAR_ash_pGAS[318]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[319]
S5K4CDGX_DATA(0x003F),	// #TVAR_ash_pGAS[320]
S5K4CDGX_DATA(0x005A),	// #TVAR_ash_pGAS[321]
S5K4CDGX_DATA(0x007A),	// #TVAR_ash_pGAS[322]
S5K4CDGX_DATA(0x0091),	// #TVAR_ash_pGAS[323]
S5K4CDGX_DATA(0x00A5),	// #TVAR_ash_pGAS[324]
S5K4CDGX_DATA(0x006C),	// #TVAR_ash_pGAS[325]
S5K4CDGX_DATA(0x0065),	// #TVAR_ash_pGAS[326]
S5K4CDGX_DATA(0x0052),	// #TVAR_ash_pGAS[327]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[328]
S5K4CDGX_DATA(0x0024),	// #TVAR_ash_pGAS[329]
S5K4CDGX_DATA(0x0017),	// #TVAR_ash_pGAS[330]
S5K4CDGX_DATA(0x0012),	// #TVAR_ash_pGAS[331]
S5K4CDGX_DATA(0x0018),	// #TVAR_ash_pGAS[332]
S5K4CDGX_DATA(0x0029),	// #TVAR_ash_pGAS[333]
S5K4CDGX_DATA(0x0043),	// #TVAR_ash_pGAS[334]
S5K4CDGX_DATA(0x0064),	// #TVAR_ash_pGAS[335]
S5K4CDGX_DATA(0x0087),	// #TVAR_ash_pGAS[336]
S5K4CDGX_DATA(0x0095),	// #TVAR_ash_pGAS[337]
S5K4CDGX_DATA(0x0066),	// #TVAR_ash_pGAS[338]
S5K4CDGX_DATA(0x0060),	// #TVAR_ash_pGAS[339]
S5K4CDGX_DATA(0x0048),	// #TVAR_ash_pGAS[340]
S5K4CDGX_DATA(0x002D),	// #TVAR_ash_pGAS[341]
S5K4CDGX_DATA(0x0018),	// #TVAR_ash_pGAS[342]
S5K4CDGX_DATA(0x0009),	// #TVAR_ash_pGAS[343]
S5K4CDGX_DATA(0x0005),	// #TVAR_ash_pGAS[344]
S5K4CDGX_DATA(0x000A),	// #TVAR_ash_pGAS[345]
S5K4CDGX_DATA(0x001A),	// #TVAR_ash_pGAS[346]
S5K4CDGX_DATA(0x0033),	// #TVAR_ash_pGAS[347]
S5K4CDGX_DATA(0x0052),	// #TVAR_ash_pGAS[348]
S5K4CDGX_DATA(0x007B),	// #TVAR_ash_pGAS[349]
S5K4CDGX_DATA(0x008D),	// #TVAR_ash_pGAS[350]
S5K4CDGX_DATA(0x0065),	// #TVAR_ash_pGAS[351]
S5K4CDGX_DATA(0x005A),	// #TVAR_ash_pGAS[352]
S5K4CDGX_DATA(0x0041),	// #TVAR_ash_pGAS[353]
S5K4CDGX_DATA(0x0028),	// #TVAR_ash_pGAS[354]
S5K4CDGX_DATA(0x0013),	// #TVAR_ash_pGAS[355]
S5K4CDGX_DATA(0x0005),	// #TVAR_ash_pGAS[356]
S5K4CDGX_DATA(0x0000),	// #TVAR_ash_pGAS[357]
S5K4CDGX_DATA(0x0004),	// #TVAR_ash_pGAS[358]
S5K4CDGX_DATA(0x0011),	// #TVAR_ash_pGAS[359]
S5K4CDGX_DATA(0x002A),	// #TVAR_ash_pGAS[360]
S5K4CDGX_DATA(0x0048),	// #TVAR_ash_pGAS[361]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[362]
S5K4CDGX_DATA(0x0083),	// #TVAR_ash_pGAS[363]
S5K4CDGX_DATA(0x0064),	// #TVAR_ash_pGAS[364]
S5K4CDGX_DATA(0x005C),	// #TVAR_ash_pGAS[365]
S5K4CDGX_DATA(0x0044),	// #TVAR_ash_pGAS[366]
S5K4CDGX_DATA(0x002B),	// #TVAR_ash_pGAS[367]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[368]
S5K4CDGX_DATA(0x0007),	// #TVAR_ash_pGAS[369]
S5K4CDGX_DATA(0x0004),	// #TVAR_ash_pGAS[370]
S5K4CDGX_DATA(0x0007),	// #TVAR_ash_pGAS[371]
S5K4CDGX_DATA(0x0015),	// #TVAR_ash_pGAS[372]
S5K4CDGX_DATA(0x002C),	// #TVAR_ash_pGAS[373]
S5K4CDGX_DATA(0x0048),	// #TVAR_ash_pGAS[374]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[375]
S5K4CDGX_DATA(0x0081),	// #TVAR_ash_pGAS[376]
S5K4CDGX_DATA(0x006B),	// #TVAR_ash_pGAS[377]
S5K4CDGX_DATA(0x0064),	// #TVAR_ash_pGAS[378]
S5K4CDGX_DATA(0x0051),	// #TVAR_ash_pGAS[379]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[380]
S5K4CDGX_DATA(0x0025),	// #TVAR_ash_pGAS[381]
S5K4CDGX_DATA(0x0017),	// #TVAR_ash_pGAS[382]
S5K4CDGX_DATA(0x0011),	// #TVAR_ash_pGAS[383]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[384]
S5K4CDGX_DATA(0x0022),	// #TVAR_ash_pGAS[385]
S5K4CDGX_DATA(0x003A),	// #TVAR_ash_pGAS[386]
S5K4CDGX_DATA(0x0057),	// #TVAR_ash_pGAS[387]
S5K4CDGX_DATA(0x0078),	// #TVAR_ash_pGAS[388]
S5K4CDGX_DATA(0x0087),	// #TVAR_ash_pGAS[389]
S5K4CDGX_DATA(0x0076),	// #TVAR_ash_pGAS[390]
S5K4CDGX_DATA(0x006C),	// #TVAR_ash_pGAS[391]
S5K4CDGX_DATA(0x0062),	// #TVAR_ash_pGAS[392]
S5K4CDGX_DATA(0x004D),	// #TVAR_ash_pGAS[393]
S5K4CDGX_DATA(0x003A),	// #TVAR_ash_pGAS[394]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[395]
S5K4CDGX_DATA(0x0029),	// #TVAR_ash_pGAS[396]
S5K4CDGX_DATA(0x002C),	// #TVAR_ash_pGAS[397]
S5K4CDGX_DATA(0x0037),	// #TVAR_ash_pGAS[398]
S5K4CDGX_DATA(0x004D),	// #TVAR_ash_pGAS[399]
S5K4CDGX_DATA(0x006C),	// #TVAR_ash_pGAS[400]
S5K4CDGX_DATA(0x0083),	// #TVAR_ash_pGAS[401]
S5K4CDGX_DATA(0x0096),	// #TVAR_ash_pGAS[402]
S5K4CDGX_DATA(0x009D),	// #TVAR_ash_pGAS[403]
S5K4CDGX_DATA(0x007A),	// #TVAR_ash_pGAS[404]
S5K4CDGX_DATA(0x0077),	// #TVAR_ash_pGAS[405]
S5K4CDGX_DATA(0x006D),	// #TVAR_ash_pGAS[406]
S5K4CDGX_DATA(0x005C),	// #TVAR_ash_pGAS[407]
S5K4CDGX_DATA(0x0052),	// #TVAR_ash_pGAS[408]
S5K4CDGX_DATA(0x004C),	// #TVAR_ash_pGAS[409]
S5K4CDGX_DATA(0x0052),	// #TVAR_ash_pGAS[410]
S5K4CDGX_DATA(0x005F),	// #TVAR_ash_pGAS[411]
S5K4CDGX_DATA(0x0072),	// #TVAR_ash_pGAS[412]
S5K4CDGX_DATA(0x0084),	// #TVAR_ash_pGAS[413]
S5K4CDGX_DATA(0x0090),	// #TVAR_ash_pGAS[414]
S5K4CDGX_DATA(0x00B3),	// #TVAR_ash_pGAS[415]
S5K4CDGX_DATA(0x00C2),	// #TVAR_ash_pGAS[416]
S5K4CDGX_DATA(0x008E),	// #TVAR_ash_pGAS[417]
S5K4CDGX_DATA(0x007C),	// #TVAR_ash_pGAS[418]
S5K4CDGX_DATA(0x007D),	// #TVAR_ash_pGAS[419]
S5K4CDGX_DATA(0x0074),	// #TVAR_ash_pGAS[420]
S5K4CDGX_DATA(0x006D),	// #TVAR_ash_pGAS[421]
S5K4CDGX_DATA(0x006D),	// #TVAR_ash_pGAS[422]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[423]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[424]
S5K4CDGX_DATA(0x0089),	// #TVAR_ash_pGAS[425]
S5K4CDGX_DATA(0x008F),	// #TVAR_ash_pGAS[426]
S5K4CDGX_DATA(0x009C),	// #TVAR_ash_pGAS[427]
S5K4CDGX_DATA(0x00DB),	// #TVAR_ash_pGAS[428]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[429]
S5K4CDGX_DATA(0x004F),	// #TVAR_ash_pGAS[430]
S5K4CDGX_DATA(0x0045),	// #TVAR_ash_pGAS[431]
S5K4CDGX_DATA(0x0049),	// #TVAR_ash_pGAS[432]
S5K4CDGX_DATA(0x004A),	// #TVAR_ash_pGAS[433]
S5K4CDGX_DATA(0x0049),	// #TVAR_ash_pGAS[434]
S5K4CDGX_DATA(0x004A),	// #TVAR_ash_pGAS[435]
S5K4CDGX_DATA(0x0058),	// #TVAR_ash_pGAS[436]
S5K4CDGX_DATA(0x0061),	// #TVAR_ash_pGAS[437]
S5K4CDGX_DATA(0x0071),	// #TVAR_ash_pGAS[438]
S5K4CDGX_DATA(0x007D),	// #TVAR_ash_pGAS[439]
S5K4CDGX_DATA(0x008A),	// #TVAR_ash_pGAS[440]
S5K4CDGX_DATA(0x00C2),	// #TVAR_ash_pGAS[441]
S5K4CDGX_DATA(0x0056),	// #TVAR_ash_pGAS[442]
S5K4CDGX_DATA(0x003F),	// #TVAR_ash_pGAS[443]
S5K4CDGX_DATA(0x0042),	// #TVAR_ash_pGAS[444]
S5K4CDGX_DATA(0x0041),	// #TVAR_ash_pGAS[445]
S5K4CDGX_DATA(0x0039),	// #TVAR_ash_pGAS[446]
S5K4CDGX_DATA(0x0036),	// #TVAR_ash_pGAS[447]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[448]
S5K4CDGX_DATA(0x0043),	// #TVAR_ash_pGAS[449]
S5K4CDGX_DATA(0x0052),	// #TVAR_ash_pGAS[450]
S5K4CDGX_DATA(0x0063),	// #TVAR_ash_pGAS[451]
S5K4CDGX_DATA(0x0077),	// #TVAR_ash_pGAS[452]
S5K4CDGX_DATA(0x007E),	// #TVAR_ash_pGAS[453]
S5K4CDGX_DATA(0x009E),	// #TVAR_ash_pGAS[454]
S5K4CDGX_DATA(0x0040),	// #TVAR_ash_pGAS[455]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[456]
S5K4CDGX_DATA(0x003A),	// #TVAR_ash_pGAS[457]
S5K4CDGX_DATA(0x002F),	// #TVAR_ash_pGAS[458]
S5K4CDGX_DATA(0x0027),	// #TVAR_ash_pGAS[459]
S5K4CDGX_DATA(0x0023),	// #TVAR_ash_pGAS[460]
S5K4CDGX_DATA(0x0021),	// #TVAR_ash_pGAS[461]
S5K4CDGX_DATA(0x002A),	// #TVAR_ash_pGAS[462]
S5K4CDGX_DATA(0x0036),	// #TVAR_ash_pGAS[463]
S5K4CDGX_DATA(0x004D),	// #TVAR_ash_pGAS[464]
S5K4CDGX_DATA(0x0067),	// #TVAR_ash_pGAS[465]
S5K4CDGX_DATA(0x0078),	// #TVAR_ash_pGAS[466]
S5K4CDGX_DATA(0x0084),	// #TVAR_ash_pGAS[467]
S5K4CDGX_DATA(0x0035),	// #TVAR_ash_pGAS[468]
S5K4CDGX_DATA(0x0036),	// #TVAR_ash_pGAS[469]
S5K4CDGX_DATA(0x0031),	// #TVAR_ash_pGAS[470]
S5K4CDGX_DATA(0x0024),	// #TVAR_ash_pGAS[471]
S5K4CDGX_DATA(0x001A),	// #TVAR_ash_pGAS[472]
S5K4CDGX_DATA(0x0012),	// #TVAR_ash_pGAS[473]
S5K4CDGX_DATA(0x0012),	// #TVAR_ash_pGAS[474]
S5K4CDGX_DATA(0x0018),	// #TVAR_ash_pGAS[475]
S5K4CDGX_DATA(0x0025),	// #TVAR_ash_pGAS[476]
S5K4CDGX_DATA(0x0039),	// #TVAR_ash_pGAS[477]
S5K4CDGX_DATA(0x0054),	// #TVAR_ash_pGAS[478]
S5K4CDGX_DATA(0x0071),	// #TVAR_ash_pGAS[479]
S5K4CDGX_DATA(0x007B),	// #TVAR_ash_pGAS[480]
S5K4CDGX_DATA(0x0031),	// #TVAR_ash_pGAS[481]
S5K4CDGX_DATA(0x0033),	// #TVAR_ash_pGAS[482]
S5K4CDGX_DATA(0x002B),	// #TVAR_ash_pGAS[483]
S5K4CDGX_DATA(0x001C),	// #TVAR_ash_pGAS[484]
S5K4CDGX_DATA(0x0012),	// #TVAR_ash_pGAS[485]
S5K4CDGX_DATA(0x000B),	// #TVAR_ash_pGAS[486]
S5K4CDGX_DATA(0x0007),	// #TVAR_ash_pGAS[487]
S5K4CDGX_DATA(0x000F),	// #TVAR_ash_pGAS[488]
S5K4CDGX_DATA(0x0018),	// #TVAR_ash_pGAS[489]
S5K4CDGX_DATA(0x002D),	// #TVAR_ash_pGAS[490]
S5K4CDGX_DATA(0x0044),	// #TVAR_ash_pGAS[491]
S5K4CDGX_DATA(0x0066),	// #TVAR_ash_pGAS[492]
S5K4CDGX_DATA(0x0071),	// #TVAR_ash_pGAS[493]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[494]
S5K4CDGX_DATA(0x002D),	// #TVAR_ash_pGAS[495]
S5K4CDGX_DATA(0x0023),	// #TVAR_ash_pGAS[496]
S5K4CDGX_DATA(0x0017),	// #TVAR_ash_pGAS[497]
S5K4CDGX_DATA(0x000D),	// #TVAR_ash_pGAS[498]
S5K4CDGX_DATA(0x0004),	// #TVAR_ash_pGAS[499]
S5K4CDGX_DATA(0x0003),	// #TVAR_ash_pGAS[500]
S5K4CDGX_DATA(0x0006),	// #TVAR_ash_pGAS[501]
S5K4CDGX_DATA(0x0010),	// #TVAR_ash_pGAS[502]
S5K4CDGX_DATA(0x0022),	// #TVAR_ash_pGAS[503]
S5K4CDGX_DATA(0x0039),	// #TVAR_ash_pGAS[504]
S5K4CDGX_DATA(0x005B),	// #TVAR_ash_pGAS[505]
S5K4CDGX_DATA(0x0066),	// #TVAR_ash_pGAS[506]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[507]
S5K4CDGX_DATA(0x002F),	// #TVAR_ash_pGAS[508]
S5K4CDGX_DATA(0x0026),	// #TVAR_ash_pGAS[509]
S5K4CDGX_DATA(0x001A),	// #TVAR_ash_pGAS[510]
S5K4CDGX_DATA(0x0010),	// #TVAR_ash_pGAS[511]
S5K4CDGX_DATA(0x0009),	// #TVAR_ash_pGAS[512]
S5K4CDGX_DATA(0x0006),	// #TVAR_ash_pGAS[513]
S5K4CDGX_DATA(0x0009),	// #TVAR_ash_pGAS[514]
S5K4CDGX_DATA(0x0013),	// #TVAR_ash_pGAS[515]
S5K4CDGX_DATA(0x0022),	// #TVAR_ash_pGAS[516]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[517]
S5K4CDGX_DATA(0x005A),	// #TVAR_ash_pGAS[518]
S5K4CDGX_DATA(0x0065),	// #TVAR_ash_pGAS[519]
S5K4CDGX_DATA(0x0034),	// #TVAR_ash_pGAS[520]
S5K4CDGX_DATA(0x0039),	// #TVAR_ash_pGAS[521]
S5K4CDGX_DATA(0x0032),	// #TVAR_ash_pGAS[522]
S5K4CDGX_DATA(0x0026),	// #TVAR_ash_pGAS[523]
S5K4CDGX_DATA(0x001D),	// #TVAR_ash_pGAS[524]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[525]
S5K4CDGX_DATA(0x0013),	// #TVAR_ash_pGAS[526]
S5K4CDGX_DATA(0x0016),	// #TVAR_ash_pGAS[527]
S5K4CDGX_DATA(0x001D),	// #TVAR_ash_pGAS[528]
S5K4CDGX_DATA(0x002F),	// #TVAR_ash_pGAS[529]
S5K4CDGX_DATA(0x0046),	// #TVAR_ash_pGAS[530]
S5K4CDGX_DATA(0x0062),	// #TVAR_ash_pGAS[531]
S5K4CDGX_DATA(0x006E),	// #TVAR_ash_pGAS[532]
S5K4CDGX_DATA(0x0041),	// #TVAR_ash_pGAS[533]
S5K4CDGX_DATA(0x003E),	// #TVAR_ash_pGAS[534]
S5K4CDGX_DATA(0x0043),	// #TVAR_ash_pGAS[535]
S5K4CDGX_DATA(0x0038),	// #TVAR_ash_pGAS[536]
S5K4CDGX_DATA(0x002E),	// #TVAR_ash_pGAS[537]
S5K4CDGX_DATA(0x0029),	// #TVAR_ash_pGAS[538]
S5K4CDGX_DATA(0x0026),	// #TVAR_ash_pGAS[539]
S5K4CDGX_DATA(0x002A),	// #TVAR_ash_pGAS[540]
S5K4CDGX_DATA(0x0031),	// #TVAR_ash_pGAS[541]
S5K4CDGX_DATA(0x003F),	// #TVAR_ash_pGAS[542]
S5K4CDGX_DATA(0x0057),	// #TVAR_ash_pGAS[543]
S5K4CDGX_DATA(0x006A),	// #TVAR_ash_pGAS[544]
S5K4CDGX_DATA(0x0077),	// #TVAR_ash_pGAS[545]
S5K4CDGX_DATA(0x005F),	// #TVAR_ash_pGAS[546]
S5K4CDGX_DATA(0x004B),	// #TVAR_ash_pGAS[547]
S5K4CDGX_DATA(0x0054),	// #TVAR_ash_pGAS[548]
S5K4CDGX_DATA(0x0054),	// #TVAR_ash_pGAS[549]
S5K4CDGX_DATA(0x004F),	// #TVAR_ash_pGAS[550]
S5K4CDGX_DATA(0x004A),	// #TVAR_ash_pGAS[551]
S5K4CDGX_DATA(0x0046),	// #TVAR_ash_pGAS[552]
S5K4CDGX_DATA(0x004A),	// #TVAR_ash_pGAS[553]
S5K4CDGX_DATA(0x0050),	// #TVAR_ash_pGAS[554]
S5K4CDGX_DATA(0x005E),	// #TVAR_ash_pGAS[555]
S5K4CDGX_DATA(0x0070),	// #TVAR_ash_pGAS[556]
S5K4CDGX_DATA(0x0079),	// #TVAR_ash_pGAS[557]
S5K4CDGX_DATA(0x0090),	// #TVAR_ash_pGAS[558]
S5K4CDGX_DATA(0x007F),	// #TVAR_ash_pGAS[559]
S5K4CDGX_DATA(0x005B),	// #TVAR_ash_pGAS[560]
S5K4CDGX_DATA(0x0055),	// #TVAR_ash_pGAS[561]
S5K4CDGX_DATA(0x0060),	// #TVAR_ash_pGAS[562]
S5K4CDGX_DATA(0x005E),	// #TVAR_ash_pGAS[563]
S5K4CDGX_DATA(0x005B),	// #TVAR_ash_pGAS[564]
S5K4CDGX_DATA(0x0060),	// #TVAR_ash_pGAS[565]
S5K4CDGX_DATA(0x0061),	// #TVAR_ash_pGAS[566]
S5K4CDGX_DATA(0x0067),	// #TVAR_ash_pGAS[567]
S5K4CDGX_DATA(0x0074),	// #TVAR_ash_pGAS[568]
S5K4CDGX_DATA(0x0073),	// #TVAR_ash_pGAS[569]
S5K4CDGX_DATA(0x0080),	// #TVAR_ash_pGAS[570]
S5K4CDGX_DATA(0x00AC),	// #TVAR_ash_pGAS[571]  
S5K4CDGX_REG(0x0786),
S5K4CDGX_DATA(0x0001),	// #awbb_GLocusR                 
S5K4CDGX_REG(0x0D5C),
S5K4CDGX_DATA(0x02A7),	// #awbb_GLocusR
S5K4CDGX_DATA(0x0343),	// #awbb_GLocusB                        
S5K4CDGX_REG(0x06F8),
S5K4CDGX_DATA(0x00AA),	// #TVAR_ash_AwbAshCord[0]
S5K4CDGX_DATA(0x00B5),	// #TVAR_ash_AwbAshCord[1]
S5K4CDGX_DATA(0x00BE),	// #TVAR_ash_AwbAshCord[2]
S5K4CDGX_DATA(0x011D),	// #TVAR_ash_AwbAshCord[3]
S5K4CDGX_DATA(0x0144),	// #TVAR_ash_AwbAshCord[4]
S5K4CDGX_DATA(0x0173),	// #TVAR_ash_AwbAshCord[5]
S5K4CDGX_DATA(0x0180),	// #TVAR_ash_AwbAshCord[6]                                       
S5K4CDGX_REG(0x078C),
S5K4CDGX_DATA(0x347C),	// #TVAR_ash_pGAS : 7000_347C
S5K4CDGX_DATA(0x7000),
};

//out from config2 - step 2 //this one is completely different from orig. Changed!
static struct s5k4cdgx_request s5k4cdgx_init_reg_ccm[] ={
//================================================================================================
//	Set CCM
//================================================================================================
S5K4CDGX_REG(0x33A4),
S5K4CDGX_DATA(0x0148),	// #TVAR_wbt_pBaseCcms[0]	// R
S5K4CDGX_DATA(0xFFD9),	// #TVAR_wbt_pBaseCcms[1]
S5K4CDGX_DATA(0x000A),	// #TVAR_wbt_pBaseCcms[2]
S5K4CDGX_DATA(0xFF94),	// #TVAR_wbt_pBaseCcms[3] 	// G
S5K4CDGX_DATA(0x0145),	// #TVAR_wbt_pBaseCcms[4]
S5K4CDGX_DATA(0xFF72),	// #TVAR_wbt_pBaseCcms[5]
S5K4CDGX_DATA(0xFFE9),	// #TVAR_wbt_pBaseCcms[6] 	// B
S5K4CDGX_DATA(0x000C),	// #TVAR_wbt_pBaseCcms[7]
S5K4CDGX_DATA(0x0194),	// #TVAR_wbt_pBaseCcms[8]
S5K4CDGX_DATA(0x00F1),	// #TVAR_wbt_pBaseCcms[9] 	// Y
S5K4CDGX_DATA(0x00FE),	// #TVAR_wbt_pBaseCcms[10]
S5K4CDGX_DATA(0xFF87),	// #TVAR_wbt_pBaseCcms[11]
S5K4CDGX_DATA(0x015D),	// #TVAR_wbt_pBaseCcms[12]	// M
S5K4CDGX_DATA(0xFF97),	// #TVAR_wbt_pBaseCcms[13]
S5K4CDGX_DATA(0x01C0),	// #TVAR_wbt_pBaseCcms[14]
S5K4CDGX_DATA(0xFF86),	// #TVAR_wbt_pBaseCcms[15]	// C
S5K4CDGX_DATA(0x014D),	// #TVAR_wbt_pBaseCcms[16]
S5K4CDGX_DATA(0x00BB),	// #TVAR_wbt_pBaseCcms[17]
// I,nc0xaA  ),	    	
S5K4CDGX_DATA(0x0148),	// #TVAR_wbt_pBaseCcms[18]	// R
S5K4CDGX_DATA(0xFFD9),	// #TVAR_wbt_pBaseCcms[19]
S5K4CDGX_DATA(0x000A),	// #TVAR_wbt_pBaseCcms[20]
S5K4CDGX_DATA(0xFF94),	// #TVAR_wbt_pBaseCcms[21]      // G
S5K4CDGX_DATA(0x0145),	// #TVAR_wbt_pBaseCcms[22]
S5K4CDGX_DATA(0xFF72),	// #TVAR_wbt_pBaseCcms[23]
S5K4CDGX_DATA(0xFFE9),	// #TVAR_wbt_pBaseCcms[24]      // B
S5K4CDGX_DATA(0x000C),	// #TVAR_wbt_pBaseCcms[25]
S5K4CDGX_DATA(0x0194),	// #TVAR_wbt_pBaseCcms[26]
S5K4CDGX_DATA(0x00F1),	// #TVAR_wbt_pBaseCcms[27]      // Y
S5K4CDGX_DATA(0x00FE),	// #TVAR_wbt_pBaseCcms[28]
S5K4CDGX_DATA(0xFF87),	// #TVAR_wbt_pBaseCcms[29]
S5K4CDGX_DATA(0x015D),	// #TVAR_wbt_pBaseCcms[30]      // M
S5K4CDGX_DATA(0xFF97),	// #TVAR_wbt_pBaseCcms[31]
S5K4CDGX_DATA(0x01C0),	// #TVAR_wbt_pBaseCcms[32]
S5K4CDGX_DATA(0xFF86),	// #TVAR_wbt_pBaseCcms[33]      // C
S5K4CDGX_DATA(0x014D),	// #TVAR_wbt_pBaseCcms[34]
S5K4CDGX_DATA(0x00BB),	// #TVAR_wbt_pBaseCcms[35]
// W,W 0x    ),    	            
S5K4CDGX_DATA(0x0148),	// #TVAR_wbt_pBaseCcms[36]	// R
S5K4CDGX_DATA(0xFFD9),	// #TVAR_wbt_pBaseCcms[37]
S5K4CDGX_DATA(0x000A),	// #TVAR_wbt_pBaseCcms[38]
S5K4CDGX_DATA(0xFF94),	// #TVAR_wbt_pBaseCcms[39]      // G
S5K4CDGX_DATA(0x0145),	// #TVAR_wbt_pBaseCcms[40]
S5K4CDGX_DATA(0xFF72),	// #TVAR_wbt_pBaseCcms[41]
S5K4CDGX_DATA(0xFFE9),	// #TVAR_wbt_pBaseCcms[42]      // B
S5K4CDGX_DATA(0x000C),	// #TVAR_wbt_pBaseCcms[43]
S5K4CDGX_DATA(0x0194),	// #TVAR_wbt_pBaseCcms[44]
S5K4CDGX_DATA(0x00F1),	// #TVAR_wbt_pBaseCcms[45]      // Y
S5K4CDGX_DATA(0x00FE),	// #TVAR_wbt_pBaseCcms[46]
S5K4CDGX_DATA(0xFF87),	// #TVAR_wbt_pBaseCcms[47]
S5K4CDGX_DATA(0x015D),	// #TVAR_wbt_pBaseCcms[48]      // M
S5K4CDGX_DATA(0xFF97),	// #TVAR_wbt_pBaseCcms[49]
S5K4CDGX_DATA(0x01C0),	// #TVAR_wbt_pBaseCcms[50]
S5K4CDGX_DATA(0xFF86),	// #TVAR_wbt_pBaseCcms[51]      // C
S5K4CDGX_DATA(0x014D),	// #TVAR_wbt_pBaseCcms[52]
S5K4CDGX_DATA(0x00BB),	// #TVAR_wbt_pBaseCcms[53]
// C,WF0x    ),	    	
S5K4CDGX_DATA(0x0148),	// #TVAR_wbt_pBaseCcms[54]	// R
S5K4CDGX_DATA(0xFFD9),	// #TVAR_wbt_pBaseCcms[55]
S5K4CDGX_DATA(0x000A),	// #TVAR_wbt_pBaseCcms[56]
S5K4CDGX_DATA(0xFF94),	// #TVAR_wbt_pBaseCcms[57]	// G
S5K4CDGX_DATA(0x0145),	// #TVAR_wbt_pBaseCcms[58]
S5K4CDGX_DATA(0xFF72),	// #TVAR_wbt_pBaseCcms[59]
S5K4CDGX_DATA(0xFFE9),	// #TVAR_wbt_pBaseCcms[60]	// B
S5K4CDGX_DATA(0x000C),	// #TVAR_wbt_pBaseCcms[61]
S5K4CDGX_DATA(0x0194),	// #TVAR_wbt_pBaseCcms[62]
S5K4CDGX_DATA(0x00F1),	// #TVAR_wbt_pBaseCcms[63]	// Y
S5K4CDGX_DATA(0x00FE),	// #TVAR_wbt_pBaseCcms[64]
S5K4CDGX_DATA(0xFF87),	// #TVAR_wbt_pBaseCcms[65]
S5K4CDGX_DATA(0x015D),	// #TVAR_wbt_pBaseCcms[66]	// M
S5K4CDGX_DATA(0xFF97),	// #TVAR_wbt_pBaseCcms[67]
S5K4CDGX_DATA(0x01C0),	// #TVAR_wbt_pBaseCcms[68]
S5K4CDGX_DATA(0xFF86),	// #TVAR_wbt_pBaseCcms[69]	// C
S5K4CDGX_DATA(0x014D),	// #TVAR_wbt_pBaseCcms[70]
S5K4CDGX_DATA(0x00BB),	// #TVAR_wbt_pBaseCcms[71]
// D,500x    ),    	
S5K4CDGX_DATA(0x0148),	// #TVAR_wbt_pBaseCcms[72]
S5K4CDGX_DATA(0xFFD9),	// #TVAR_wbt_pBaseCcms[73]
S5K4CDGX_DATA(0x000A),	// #TVAR_wbt_pBaseCcms[74]
S5K4CDGX_DATA(0xFF94),	// #TVAR_wbt_pBaseCcms[75]
S5K4CDGX_DATA(0x0145),	// #TVAR_wbt_pBaseCcms[76]
S5K4CDGX_DATA(0xFF72),	// #TVAR_wbt_pBaseCcms[77]
S5K4CDGX_DATA(0xFFE9),	// #TVAR_wbt_pBaseCcms[78]
S5K4CDGX_DATA(0x000C),	// #TVAR_wbt_pBaseCcms[79]
S5K4CDGX_DATA(0x0194),	// #TVAR_wbt_pBaseCcms[80]
S5K4CDGX_DATA(0x00F1),	// #TVAR_wbt_pBaseCcms[81]
S5K4CDGX_DATA(0x00FE),	// #TVAR_wbt_pBaseCcms[82]
S5K4CDGX_DATA(0xFF87),	// #TVAR_wbt_pBaseCcms[83]
S5K4CDGX_DATA(0x015D),	// #TVAR_wbt_pBaseCcms[84]
S5K4CDGX_DATA(0xFF97),	// #TVAR_wbt_pBaseCcms[85]
S5K4CDGX_DATA(0x01C0),	// #TVAR_wbt_pBaseCcms[86]
S5K4CDGX_DATA(0xFF86),	// #TVAR_wbt_pBaseCcms[87]
S5K4CDGX_DATA(0x014D),	// #TVAR_wbt_pBaseCcms[88]
S5K4CDGX_DATA(0x00BB),	// #TVAR_wbt_pBaseCcms[89]
// D,650x    ),	    	                                                                                 
S5K4CDGX_DATA(0x0148),	// #TVAR_wbt_pBaseCcms[90]	// R
S5K4CDGX_DATA(0xFFD9),	// #TVAR_wbt_pBaseCcms[91]
S5K4CDGX_DATA(0x000A),	// #TVAR_wbt_pBaseCcms[92]
S5K4CDGX_DATA(0xFF94),	// #TVAR_wbt_pBaseCcms[93]	// G
S5K4CDGX_DATA(0x0145),	// #TVAR_wbt_pBaseCcms[94]
S5K4CDGX_DATA(0xFF72),	// #TVAR_wbt_pBaseCcms[95]
S5K4CDGX_DATA(0xFFE9),	// #TVAR_wbt_pBaseCcms[96]	// B
S5K4CDGX_DATA(0x000C),	// #TVAR_wbt_pBaseCcms[97]
S5K4CDGX_DATA(0x0194),	// #TVAR_wbt_pBaseCcms[98]
S5K4CDGX_DATA(0x00F1),	// #TVAR_wbt_pBaseCcms[99]	// Y
S5K4CDGX_DATA(0x00FE),	// #TVAR_wbt_pBaseCcms[100]
S5K4CDGX_DATA(0xFF87),	// #TVAR_wbt_pBaseCcms[101]
S5K4CDGX_DATA(0x015D),	// #TVAR_wbt_pBaseCcms[102]	// M
S5K4CDGX_DATA(0xFF97),	// #TVAR_wbt_pBaseCcms[103]
S5K4CDGX_DATA(0x01C0),	// #TVAR_wbt_pBaseCcms[104]
S5K4CDGX_DATA(0xFF86),	// #TVAR_wbt_pBaseCcms[105]	// C
S5K4CDGX_DATA(0x014D),	// #TVAR_wbt_pBaseCcms[106]
S5K4CDGX_DATA(0x00BB),	// #TVAR_wbt_pBaseCcms[107]

S5K4CDGX_REG(0x06D8),
S5K4CDGX_DATA(0x33A4),	// #TVAR_wbt_pBaseCcms 700033A4
S5K4CDGX_DATA(0x7000),
// O,ut0xdoor),
S5K4CDGX_REG(0x3380),
S5K4CDGX_DATA(0x017A),	// #TVAR_wbt_pOutdoorCcm[0]
S5K4CDGX_DATA(0xFFBB),	// #TVAR_wbt_pOutdoorCcm[1]
S5K4CDGX_DATA(0xFFF6),	// #TVAR_wbt_pOutdoorCcm[2]
S5K4CDGX_DATA(0xFF90),	// #TVAR_wbt_pOutdoorCcm[3]
S5K4CDGX_DATA(0x014E),	// #TVAR_wbt_pOutdoorCcm[4]
S5K4CDGX_DATA(0xFF6D),	// #TVAR_wbt_pOutdoorCcm[5]
S5K4CDGX_DATA(0xFFE6),	// #TVAR_wbt_pOutdoorCcm[6]
S5K4CDGX_DATA(0x000A),	// #TVAR_wbt_pOutdoorCcm[7]
S5K4CDGX_DATA(0x019A),	// #TVAR_wbt_pOutdoorCcm[8]
S5K4CDGX_DATA(0x00F1),	// #TVAR_wbt_pOutdoorCcm[9]
S5K4CDGX_DATA(0x00FE),	// #TVAR_wbt_pOutdoorCcm[10]
S5K4CDGX_DATA(0xFF87),	// #TVAR_wbt_pOutdoorCcm[11]
S5K4CDGX_DATA(0x0162),	// #TVAR_wbt_pOutdoorCcm[12]
S5K4CDGX_DATA(0xFF88),	// #TVAR_wbt_pOutdoorCcm[13]
S5K4CDGX_DATA(0x01CA),	// #TVAR_wbt_pOutdoorCcm[14]
S5K4CDGX_DATA(0xFF86),	// #TVAR_wbt_pOutdoorCcm[15]
S5K4CDGX_DATA(0x014D),	// #TVAR_wbt_pOutdoorCcm[16]
S5K4CDGX_DATA(0x00BB),	// #TVAR_wbt_pOutdoorCcm[17]              

S5K4CDGX_REG(0x06E0),
S5K4CDGX_DATA(0x3380),	// #TVAR_wbt_pOutdoorCcm
S5K4CDGX_DATA(0x7000),

S5K4CDGX_REG(0x1388),
S5K4CDGX_DATA(0x0200),
S5K4CDGX_DATA(0x0200),
S5K4CDGX_DATA(0x0200),
S5K4CDGX_DATA(0x0200),              
// r,eI0xnitC),ore  
S5K4CDGX_REG(0x0532),
S5K4CDGX_DATA(0x0001),	// #REG_TC_DBG_ReInitCmd
};

//out from config2 - step 3 //this one is completely different from orig. Changed!
static struct s5k4cdgx_request s5k4cdgx_init_reg_awb[] ={
//================================================================================================
//	Set AWB
//================================================================================================
// I,nd0xoor ),     
S5K4CDGX_REG(0x0C74),
S5K4CDGX_DATA(0x043F),	// #awbb_IndoorGrZones_m_BGrid[0]
S5K4CDGX_DATA(0x046A),	// #awbb_IndoorGrZones_m_BGrid[1]
S5K4CDGX_DATA(0x03E9),	// #awbb_IndoorGrZones_m_BGrid[2]
S5K4CDGX_DATA(0x047A),	// #awbb_IndoorGrZones_m_BGrid[3]
S5K4CDGX_DATA(0x03A5),	// #awbb_IndoorGrZones_m_BGrid[4]
S5K4CDGX_DATA(0x0449),	// #awbb_IndoorGrZones_m_BGrid[5]
S5K4CDGX_DATA(0x035C),	// #awbb_IndoorGrZones_m_BGrid[6]
S5K4CDGX_DATA(0x040E),	// #awbb_IndoorGrZones_m_BGrid[7]
S5K4CDGX_DATA(0x031E),	// #awbb_IndoorGrZones_m_BGrid[8]
S5K4CDGX_DATA(0x03D2),	// #awbb_IndoorGrZones_m_BGrid[9]
S5K4CDGX_DATA(0x02E5),	// #awbb_IndoorGrZones_m_BGrid[10]
S5K4CDGX_DATA(0x038C),	// #awbb_IndoorGrZones_m_BGrid[11]
S5K4CDGX_DATA(0x02BE),	// #awbb_IndoorGrZones_m_BGrid[12]
S5K4CDGX_DATA(0x033B),	// #awbb_IndoorGrZones_m_BGrid[13]
S5K4CDGX_DATA(0x0295),	// #awbb_IndoorGrZones_m_BGrid[14]
S5K4CDGX_DATA(0x0305),	// #awbb_IndoorGrZones_m_BGrid[15]
S5K4CDGX_DATA(0x027F),	// #awbb_IndoorGrZones_m_BGrid[16]
S5K4CDGX_DATA(0x02DB),	// #awbb_IndoorGrZones_m_BGrid[17]
S5K4CDGX_DATA(0x0265),	// #awbb_IndoorGrZones_m_BGrid[18]
S5K4CDGX_DATA(0x02BC),	// #awbb_IndoorGrZones_m_BGrid[19]
S5K4CDGX_DATA(0x024E),	// #awbb_IndoorGrZones_m_BGrid[20]
S5K4CDGX_DATA(0x02A9),	// #awbb_IndoorGrZones_m_BGrid[21]
S5K4CDGX_DATA(0x0234),	// #awbb_IndoorGrZones_m_BGrid[22]
S5K4CDGX_DATA(0x029F),	// #awbb_IndoorGrZones_m_BGrid[23]
S5K4CDGX_DATA(0x021D),	// #awbb_IndoorGrZones_m_BGrid[24]
S5K4CDGX_DATA(0x0297),	// #awbb_IndoorGrZones_m_BGrid[25]
S5K4CDGX_DATA(0x020C),	// #awbb_IndoorGrZones_m_BGrid[26]
S5K4CDGX_DATA(0x028D),	// #awbb_IndoorGrZones_m_BGrid[27]
S5K4CDGX_DATA(0x01FE),	// #awbb_IndoorGrZones_m_BGrid[28]
S5K4CDGX_DATA(0x027D),	// #awbb_IndoorGrZones_m_BGrid[29]
S5K4CDGX_DATA(0x01F6),	// #awbb_IndoorGrZones_m_BGrid[30]
S5K4CDGX_DATA(0x0263),	// #awbb_IndoorGrZones_m_BGrid[31]
S5K4CDGX_DATA(0x0213),	// #awbb_IndoorGrZones_m_BGrid[32]
S5K4CDGX_DATA(0x0242),	// #awbb_IndoorGrZones_m_BGrid[33]
S5K4CDGX_DATA(0x0000),	// #awbb_IndoorGrZones_m_BGrid[34]
S5K4CDGX_DATA(0x0000),	// #awbb_IndoorGrZones_m_BGrid[35]
S5K4CDGX_DATA(0x0000),	// #awbb_IndoorGrZones_m_BGrid[36]
S5K4CDGX_DATA(0x0000),	// #awbb_IndoorGrZones_m_BGrid[37]
S5K4CDGX_DATA(0x0000),	// #awbb_IndoorGrZones_m_BGrid[38]
S5K4CDGX_DATA(0x0000),	// #awbb_IndoorGrZones_m_BGrid[39]
S5K4CDGX_DATA(0x0005),	// #awbb_IndoorGrZones_m_GridStep
S5K4CDGX_REG(0x0CC8),
S5K4CDGX_DATA(0x0011),
S5K4CDGX_REG(0x0CCC),
S5K4CDGX_DATA(0x00ac),	// #awbb_IndoorGrZones_m_Boffs               
// L,ow0xBr  ),     
S5K4CDGX_REG(0x0D0C),
S5K4CDGX_DATA(0x03B7),	// #awbb_LowBrGrZones_m_BGrid[0]
S5K4CDGX_DATA(0x0491),	// #awbb_LowBrGrZones_m_BGrid[1]
S5K4CDGX_DATA(0x0323),	// #awbb_LowBrGrZones_m_BGrid[2]
S5K4CDGX_DATA(0x04B2),	// #awbb_LowBrGrZones_m_BGrid[3]
S5K4CDGX_DATA(0x02B6),	// #awbb_LowBrGrZones_m_BGrid[4]
S5K4CDGX_DATA(0x0464),	// #awbb_LowBrGrZones_m_BGrid[5]
S5K4CDGX_DATA(0x0275),	// #awbb_LowBrGrZones_m_BGrid[6]
S5K4CDGX_DATA(0x040B),	// #awbb_LowBrGrZones_m_BGrid[7]
S5K4CDGX_DATA(0x023C),	// #awbb_LowBrGrZones_m_BGrid[8]
S5K4CDGX_DATA(0x03AB),	// #awbb_LowBrGrZones_m_BGrid[9]
S5K4CDGX_DATA(0x0208),	// #awbb_LowBrGrZones_m_BGrid[10]
S5K4CDGX_DATA(0x0360),	// #awbb_LowBrGrZones_m_BGrid[11]
S5K4CDGX_DATA(0x01E1),	// #awbb_LowBrGrZones_m_BGrid[12]
S5K4CDGX_DATA(0x0320),	// #awbb_LowBrGrZones_m_BGrid[13]
S5K4CDGX_DATA(0x01CF),	// #awbb_LowBrGrZones_m_BGrid[14]
S5K4CDGX_DATA(0x02F4),	// #awbb_LowBrGrZones_m_BGrid[15]
S5K4CDGX_DATA(0x01D6),	// #awbb_LowBrGrZones_m_BGrid[16]
S5K4CDGX_DATA(0x02BD),	// #awbb_LowBrGrZones_m_BGrid[17]
S5K4CDGX_DATA(0x0211),	// #awbb_LowBrGrZones_m_BGrid[18]
S5K4CDGX_DATA(0x026B),	// #awbb_LowBrGrZones_m_BGrid[19]
S5K4CDGX_DATA(0x0000),	// #awbb_LowBrGrZones_m_BGrid[20]
S5K4CDGX_DATA(0x0000),	// #awbb_LowBrGrZones_m_BGrid[21]
S5K4CDGX_DATA(0x0000),	// #awbb_LowBrGrZones_m_BGrid[22]
S5K4CDGX_DATA(0x0000),	// #awbb_LowBrGrZones_m_BGrid[23]
S5K4CDGX_DATA(0x0006),	// #awbb_LowBrGrZones_m_GridStep
S5K4CDGX_REG(0x0D40),
S5K4CDGX_DATA(0x000a),	 
S5K4CDGX_REG(0x0D44),
S5K4CDGX_DATA(0x0081),	// #awbb_LowBrGrZones_m_Boffs                       
// O,ut0xdoor),         
S5K4CDGX_REG(0x0CD0),
S5K4CDGX_DATA(0x029C),	// #awbb_OutdoorGrZones_m_BGrid[0]
S5K4CDGX_DATA(0x02C3),	// #awbb_OutdoorGrZones_m_BGrid[1]
S5K4CDGX_DATA(0x0283),	// #awbb_OutdoorGrZones_m_BGrid[2]
S5K4CDGX_DATA(0x02D0),	// #awbb_OutdoorGrZones_m_BGrid[3]
S5K4CDGX_DATA(0x026A),	// #awbb_OutdoorGrZones_m_BGrid[4]
S5K4CDGX_DATA(0x02D1),	// #awbb_OutdoorGrZones_m_BGrid[5]
S5K4CDGX_DATA(0x024C),	// #awbb_OutdoorGrZones_m_BGrid[6]
S5K4CDGX_DATA(0x02CE),	// #awbb_OutdoorGrZones_m_BGrid[7]
S5K4CDGX_DATA(0x0234),	// #awbb_OutdoorGrZones_m_BGrid[8]
S5K4CDGX_DATA(0x02BD),	// #awbb_OutdoorGrZones_m_BGrid[9]
S5K4CDGX_DATA(0x021D),	// #awbb_OutdoorGrZones_m_BGrid[10]
S5K4CDGX_DATA(0x02A4),	// #awbb_OutdoorGrZones_m_BGrid[11]
S5K4CDGX_DATA(0x020F),	// #awbb_OutdoorGrZones_m_BGrid[12]
S5K4CDGX_DATA(0x028B),	// #awbb_OutdoorGrZones_m_BGrid[13]
S5K4CDGX_DATA(0x020C),	// #awbb_OutdoorGrZones_m_BGrid[14]
S5K4CDGX_DATA(0x0271),	// #awbb_OutdoorGrZones_m_BGrid[15]
S5K4CDGX_DATA(0x0220),	// #awbb_OutdoorGrZones_m_BGrid[16]
S5K4CDGX_DATA(0x024E),	// #awbb_OutdoorGrZones_m_BGrid[17]
S5K4CDGX_DATA(0x0000),	// #awbb_OutdoorGrZones_m_BGrid[18]
S5K4CDGX_DATA(0x0000),	// #awbb_OutdoorGrZones_m_BGrid[19]
S5K4CDGX_DATA(0x0000),	// #awbb_OutdoorGrZones_m_BGrid[20]
S5K4CDGX_DATA(0x0000),	// #awbb_OutdoorGrZones_m_BGrid[21]
S5K4CDGX_DATA(0x0000),	// #awbb_OutdoorGrZones_m_BGrid[22]
S5K4CDGX_DATA(0x0000),	// #awbb_OutdoorGrZones_m_BGrid[23]
S5K4CDGX_DATA(0x0005),	// #awbb_OutdoorGrZones_m_GridStep
S5K4CDGX_REG(0x0D04),
S5K4CDGX_DATA(0x0009),	// #awbb_OutdoorGrZones_m_Boffs  
S5K4CDGX_REG(0x0D08),
S5K4CDGX_DATA(0x0191),	// #awbb_OutdoorGrZones_m_Boffs                      
// L,ow0x tem),perature   
S5K4CDGX_REG(0x0D48),
S5K4CDGX_DATA(0x03DB),	// #awbb_CrclLowT_R_c
S5K4CDGX_REG(0x0D4C),
S5K4CDGX_DATA(0x00E3),	// #awbb_CrclLowT_B_c
S5K4CDGX_REG(0x0D50),
S5K4CDGX_DATA(0x6EC6),	// #awbb_CrclLowT_Rad_c                  

S5K4CDGX_REG(0x202A),
S5K4CDGX_DATA(0x0004),	// #Mon_awb_ByPassMode : LowTemp bypass                                      
// W,hi0xte l),ocus        
S5K4CDGX_REG(0x0D58),
S5K4CDGX_DATA(0x0130),//0111	// #awbb_IntcR
S5K4CDGX_DATA(0x011F),//0138	// #awbb_IntcB                     
// G,am0xut t),hreshold   
S5K4CDGX_REG(0x0D78),
S5K4CDGX_DATA(0x0187),	//#awbb_GamutWidthThr1
S5K4CDGX_DATA(0x00CF),	//#awbb_GamutHeighhr1
S5K4CDGX_DATA(0x000D),	//#awbb_GamutWidthThr2
S5K4CDGX_DATA(0x000A),	//#awbb_GamutHeighhr2                      
// S,et0x sce),ne threshold        
S5K4CDGX_REG(0x0D88),
S5K4CDGX_DATA(0x05AA),	// #awbb_LowTempRB
S5K4CDGX_DATA(0x0050),	// #awbb_LowTemp_RBzone
S5K4CDGX_REG(0x0D72),
S5K4CDGX_DATA(0x011C),	// #awbb_MvEq_RBthresh
S5K4CDGX_REG(0x0DD4),       
S5K4CDGX_DATA(0x05dc),	////awbb_OutdoorDetectionZone_ZInfo_m_MaxNB                     

S5K4CDGX_REG(0x0E6A),
S5K4CDGX_DATA(0x0000),	// #awbb_rpl_InvalidOutdoor off
S5K4CDGX_REG(0x0B96),
S5K4CDGX_DATA(0x0002),	// #THSTAT_Mon_u16_StartX
S5K4CDGX_DATA(0x0001),	// #THSTAT_Mon_u16_StartY
S5K4CDGX_DATA(0x0006),	// #THSTAT_Mon_u16_StepX
S5K4CDGX_DATA(0x0004),	// #THSTAT_Mon_u16_StepY
S5K4CDGX_REG(0x0B94),
S5K4CDGX_DATA(0x0002),	// #THSTAT_Mon_u16_CaptureType
S5K4CDGX_REG(0x0B92),
S5K4CDGX_DATA(0x0001),	// #THSTAT_Mon_u16_CaptureFrameRequest  
};

//out from config2 - step 4 //checked - same as orig
static struct s5k4cdgx_request s5k4cdgx_init_reg_ae[] ={
//================================================================================================
//	Set AE 
//================================================================================================
// AE Target   
S5K4CDGX_REG(0x0E88),
S5K4CDGX_DATA(0x003C),	// #TVAR_ae_BrAve

// AE State Mode
S5K4CDGX_REG(0x0E8E),
S5K4CDGX_DATA(0x000F),	// #Ae_StatMode

// AE Weight   
S5K4CDGX_REG(0x0E96),
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_0_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_1_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_2_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_3_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_4_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_5_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_6_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_7_
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_8_
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_9_
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_10
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_11
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_12
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_13
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_14
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_15
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_16
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_17
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_18
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_19
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_20
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_21
S5K4CDGX_DATA(0x0303),	// #ae_WeightTbl_16_22
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_23
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_24
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_25
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_26
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_27
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_28
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_29
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_30
S5K4CDGX_DATA(0x0101),	// #ae_WeightTbl_16_31
};

//out from config2 - step 5 //this one has some differenies from orig. Changed!
static struct s5k4cdgx_request s5k4cdgx_init_reg_gamma_lut[] ={
//================================================================================================
//	Set Gamma LUT
//================================================================================================
S5K4CDGX_REG(0x05DC),
S5K4CDGX_DATA(0x0000),	// #SARR_usGammaLutRGBIndoor[0]
S5K4CDGX_DATA(0x0005),	// #SARR_usGammaLutRGBIndoor[1]
S5K4CDGX_DATA(0x000D),	// #SARR_usGammaLutRGBIndoor[2]
S5K4CDGX_DATA(0x0044),	// #SARR_usGammaLutRGBIndoor[3]
S5K4CDGX_DATA(0x009C),	// #SARR_usGammaLutRGBIndoor[4]
S5K4CDGX_DATA(0x00F9),	// #SARR_usGammaLutRGBIndoor[5]
S5K4CDGX_DATA(0x013F),	// #SARR_usGammaLutRGBIndoor[6]
S5K4CDGX_DATA(0x0162),	// #SARR_usGammaLutRGBIndoor[7]
S5K4CDGX_DATA(0x017C),	// #SARR_usGammaLutRGBIndoor[8]
S5K4CDGX_DATA(0x01B0),	// #SARR_usGammaLutRGBIndoor[9]
S5K4CDGX_DATA(0x01DF),	// #SARR_usGammaLutRGBIndoor[10]
S5K4CDGX_DATA(0x0209),	// #SARR_usGammaLutRGBIndoor[11]
S5K4CDGX_DATA(0x022F),	// #SARR_usGammaLutRGBIndoor[12]
S5K4CDGX_DATA(0x0280),	// #SARR_usGammaLutRGBIndoor[13]
S5K4CDGX_DATA(0x02BF),	// #SARR_usGammaLutRGBIndoor[14]
S5K4CDGX_DATA(0x032C),	// #SARR_usGammaLutRGBIndoor[15]
S5K4CDGX_DATA(0x037E),	// #SARR_usGammaLutRGBIndoor[16]
S5K4CDGX_DATA(0x03BC),	// #SARR_usGammaLutRGBIndoor[17]
S5K4CDGX_DATA(0x03E5),	// #SARR_usGammaLutRGBIndoor[18]
S5K4CDGX_DATA(0x03FF),	// #SARR_usGammaLutRGBIndoor[19]
S5K4CDGX_DATA(0x0000),	// #SARR_usGammaLutRGBIndoor[20]
S5K4CDGX_DATA(0x0005),	// #SARR_usGammaLutRGBIndoor[21]
S5K4CDGX_DATA(0x000D),	// #SARR_usGammaLutRGBIndoor[22]
S5K4CDGX_DATA(0x0044),	// #SARR_usGammaLutRGBIndoor[23]
S5K4CDGX_DATA(0x009C),	// #SARR_usGammaLutRGBIndoor[24]
S5K4CDGX_DATA(0x00F9),	// #SARR_usGammaLutRGBIndoor[25]
S5K4CDGX_DATA(0x013F),	// #SARR_usGammaLutRGBIndoor[26]
S5K4CDGX_DATA(0x0162),	// #SARR_usGammaLutRGBIndoor[27]
S5K4CDGX_DATA(0x017C),	// #SARR_usGammaLutRGBIndoor[28]
S5K4CDGX_DATA(0x01B0),	// #SARR_usGammaLutRGBIndoor[29]
S5K4CDGX_DATA(0x01DF),	// #SARR_usGammaLutRGBIndoor[30]
S5K4CDGX_DATA(0x0209),	// #SARR_usGammaLutRGBIndoor[31]
S5K4CDGX_DATA(0x022F),	// #SARR_usGammaLutRGBIndoor[32]
S5K4CDGX_DATA(0x0280),	// #SARR_usGammaLutRGBIndoor[33]
S5K4CDGX_DATA(0x02BF),	// #SARR_usGammaLutRGBIndoor[34]
S5K4CDGX_DATA(0x032C),	// #SARR_usGammaLutRGBIndoor[35]
S5K4CDGX_DATA(0x037E),	// #SARR_usGammaLutRGBIndoor[36]
S5K4CDGX_DATA(0x03BC),	// #SARR_usGammaLutRGBIndoor[37]
S5K4CDGX_DATA(0x03E5),	// #SARR_usGammaLutRGBIndoor[38]
S5K4CDGX_DATA(0x03FF),	// #SARR_usGammaLutRGBIndoor[39]
S5K4CDGX_DATA(0x0000),	// #SARR_usGammaLutRGBIndoor[40]
S5K4CDGX_DATA(0x0005),	// #SARR_usGammaLutRGBIndoor[41]
S5K4CDGX_DATA(0x000D),	// #SARR_usGammaLutRGBIndoor[42]
S5K4CDGX_DATA(0x0044),	// #SARR_usGammaLutRGBIndoor[43]
S5K4CDGX_DATA(0x009C),	// #SARR_usGammaLutRGBIndoor[44]
S5K4CDGX_DATA(0x00F9),	// #SARR_usGammaLutRGBIndoor[45]
S5K4CDGX_DATA(0x013F),	// #SARR_usGammaLutRGBIndoor[46]
S5K4CDGX_DATA(0x0162),	// #SARR_usGammaLutRGBIndoor[47]
S5K4CDGX_DATA(0x017C),	// #SARR_usGammaLutRGBIndoor[48]
S5K4CDGX_DATA(0x01B0),	// #SARR_usGammaLutRGBIndoor[49]
S5K4CDGX_DATA(0x01DF),	// #SARR_usGammaLutRGBIndoor[50]
S5K4CDGX_DATA(0x0209),	// #SARR_usGammaLutRGBIndoor[51]
S5K4CDGX_DATA(0x022F),	// #SARR_usGammaLutRGBIndoor[52]
S5K4CDGX_DATA(0x0280),	// #SARR_usGammaLutRGBIndoor[53]
S5K4CDGX_DATA(0x02BF),	// #SARR_usGammaLutRGBIndoor[54]
S5K4CDGX_DATA(0x032C),	// #SARR_usGammaLutRGBIndoor[55]
S5K4CDGX_DATA(0x037E),	// #SARR_usGammaLutRGBIndoor[56]
S5K4CDGX_DATA(0x03BC),	// #SARR_usGammaLutRGBIndoor[57]
S5K4CDGX_DATA(0x03E5),	// #SARR_usGammaLutRGBIndoor[58]
S5K4CDGX_DATA(0x03FF),	// #SARR_usGammaLutRGBIndoor[59]   

S5K4CDGX_REG(0x06E6),
S5K4CDGX_DATA(0x00AD),	// #SARR_AwbCcmCord[0]
S5K4CDGX_DATA(0x00BA),	// #SARR_AwbCcmCord[1]
S5K4CDGX_DATA(0x00D4),	// #SARR_AwbCcmCord[2]
S5K4CDGX_DATA(0x0104),	// #SARR_AwbCcmCord[3]
S5K4CDGX_DATA(0x013B),	// #SARR_AwbCcmCord[4]
S5K4CDGX_DATA(0x016D),	// #SARR_AwbCcmCord[5]    

S5K4CDGX_REG(0x0F06),
S5K4CDGX_DATA(0x00AD),	// #SARR_IllumType[0]
S5K4CDGX_DATA(0x00BA),	// #SARR_IllumType[1]
S5K4CDGX_DATA(0x00D4),	// #SARR_IllumType[2]
S5K4CDGX_DATA(0x0104),	// #SARR_IllumType[3]
S5K4CDGX_DATA(0x013B),	// #SARR_IllumType[4]
S5K4CDGX_DATA(0x016D),	// #SARR_IllumType[5]
S5K4CDGX_DATA(0x0185),	// #SARR_IllumType[6]  

S5K4CDGX_DATA(0x01D6),	// #SARR_IllumTypeF[0]
S5K4CDGX_DATA(0x0100),	// #SARR_IllumTypeF[1]
S5K4CDGX_DATA(0x0122),	// #SARR_IllumTypeF[2]
S5K4CDGX_DATA(0x00F1),	// #SARR_IllumTypeF[3]
S5K4CDGX_DATA(0x0100),	// #SARR_IllumTypeF[4]
S5K4CDGX_DATA(0x0100),	// #SARR_IllumTypeF[5]
S5K4CDGX_DATA(0x0100),	// #SARR_IllumTypeF[6]
};

//out from config2 - step 6 //this one has some differenies from orig. Changed!
static struct s5k4cdgx_request s5k4cdgx_init_reg_afit[] ={
//==, 0x====),=========================================================================================
//	 0xSet ),AFIT table
//==, 0x====),=========================================================================================
//	 0xpara),m_start	afit_uNoiseIndInDoor
S5K4CDGX_REG(0x07A8),
S5K4CDGX_DATA(0x000A),	// #afit_uNoiseIndInDoor[0]      //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
S5K4CDGX_DATA(0x0019),	// #afit_uNoiseIndInDoor[1]
S5K4CDGX_DATA(0x007D),	// #afit_uNoiseIndInDoor[2]
S5K4CDGX_DATA(0x01F4),	// #afit_uNoiseIndInDoor[3]
S5K4CDGX_DATA(0x1388),	// #afit_uNoiseIndInDoor[4]

S5K4CDGX_REG(0x07EC),	
S5K4CDGX_DATA(0x0005),	// #TVAR_afit_pBaseVals[0]	// brightness
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[1]	// contrast
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[2]	// saturation
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[3]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[4]
S5K4CDGX_DATA(0x0032),	// #TVAR_afit_pBaseVals[5]
S5K4CDGX_DATA(0x03FF),	// #TVAR_afit_pBaseVals[6]
S5K4CDGX_DATA(0x007A),	// #TVAR_afit_pBaseVals[7]
S5K4CDGX_DATA(0x012C),	// #TVAR_afit_pBaseVals[8]
S5K4CDGX_DATA(0x002A),	// #TVAR_afit_pBaseVals[9]
S5K4CDGX_DATA(0x0078),	// #TVAR_afit_pBaseVals[10]
S5K4CDGX_DATA(0x0019),	// #TVAR_afit_pBaseVals[11]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[12]
S5K4CDGX_DATA(0x01F4),	// #TVAR_afit_pBaseVals[13]
S5K4CDGX_DATA(0x00D5),	// #TVAR_afit_pBaseVals[14]
S5K4CDGX_DATA(0x00DE),	// #TVAR_afit_pBaseVals[15]
S5K4CDGX_DATA(0x0100),	// #TVAR_afit_pBaseVals[16]
S5K4CDGX_DATA(0x00F3),	// #TVAR_afit_pBaseVals[17]
S5K4CDGX_DATA(0x0020),	// #TVAR_afit_pBaseVals[18]
S5K4CDGX_DATA(0x0078),	// #TVAR_afit_pBaseVals[19]
S5K4CDGX_DATA(0x0070),	// #TVAR_afit_pBaseVals[20]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[21]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[22]
S5K4CDGX_DATA(0x01CE),	// #TVAR_afit_pBaseVals[23]
S5K4CDGX_DATA(0x003A),	// #TVAR_afit_pBaseVals[24]
S5K4CDGX_DATA(0xF804),	// #TVAR_afit_pBaseVals[25]
S5K4CDGX_DATA(0x010C),	// #TVAR_afit_pBaseVals[26]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[27]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[28]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[29]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[30]
S5K4CDGX_DATA(0x0200),	// #TVAR_afit_pBaseVals[31]
S5K4CDGX_DATA(0x3202),	// #TVAR_afit_pBaseVals[32]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[33]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[34]
S5K4CDGX_DATA(0x8E1E),	// #TVAR_afit_pBaseVals[35]
S5K4CDGX_DATA(0x481E),	// #TVAR_afit_pBaseVals[36]
S5K4CDGX_DATA(0x4C33),	// #TVAR_afit_pBaseVals[37]
S5K4CDGX_DATA(0x323C),	// #TVAR_afit_pBaseVals[38]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[39]
S5K4CDGX_DATA(0x00FF),	// #TVAR_afit_pBaseVals[40]
S5K4CDGX_DATA(0x0F32),	// #TVAR_afit_pBaseVals[41]
S5K4CDGX_DATA(0x1414),	// #TVAR_afit_pBaseVals[42]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[43]
S5K4CDGX_DATA(0x8002),	// #TVAR_afit_pBaseVals[44]
S5K4CDGX_DATA(0x061E),	// #TVAR_afit_pBaseVals[45]
S5K4CDGX_DATA(0x0A1E),	// #TVAR_afit_pBaseVals[46]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[47]
S5K4CDGX_DATA(0x0505),	// #TVAR_afit_pBaseVals[48]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[49]
S5K4CDGX_DATA(0x0A0A),	// #TVAR_afit_pBaseVals[50]
S5K4CDGX_DATA(0x0404),	// #TVAR_afit_pBaseVals[51]
S5K4CDGX_DATA(0x0007),	// #TVAR_afit_pBaseVals[52]
S5K4CDGX_DATA(0x0500),	// #TVAR_afit_pBaseVals[53]
S5K4CDGX_DATA(0xB200),	// #TVAR_afit_pBaseVals[54]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[55]
S5K4CDGX_DATA(0x0808),	// #TVAR_afit_pBaseVals[56]
S5K4CDGX_DATA(0x1008),	// #TVAR_afit_pBaseVals[57]
S5K4CDGX_DATA(0xFF91),	// #TVAR_afit_pBaseVals[58]
S5K4CDGX_DATA(0xFF9B),	// #TVAR_afit_pBaseVals[59]
S5K4CDGX_DATA(0x0606),	// #TVAR_afit_pBaseVals[60]
S5K4CDGX_DATA(0x4123),	// #TVAR_afit_pBaseVals[61]
S5K4CDGX_DATA(0x4123),	// #TVAR_afit_pBaseVals[62]
S5K4CDGX_DATA(0x0A90),	// #TVAR_afit_pBaseVals[63]
S5K4CDGX_DATA(0x0A23),	// #TVAR_afit_pBaseVals[64]
S5K4CDGX_DATA(0x2C00),	// #TVAR_afit_pBaseVals[65]
S5K4CDGX_DATA(0x2E00),	// #TVAR_afit_pBaseVals[66]
S5K4CDGX_DATA(0x0200),	// #TVAR_afit_pBaseVals[67]
S5K4CDGX_DATA(0x40FF),	// #TVAR_afit_pBaseVals[68]
S5K4CDGX_DATA(0x0F40),	// #TVAR_afit_pBaseVals[69]
S5K4CDGX_DATA(0x100F),	// #TVAR_afit_pBaseVals[70]
S5K4CDGX_DATA(0x0F00),	// #TVAR_afit_pBaseVals[71]
S5K4CDGX_DATA(0x0018),	// #TVAR_afit_pBaseVals[72]
S5K4CDGX_DATA(0x0900),	// #TVAR_afit_pBaseVals[73]
S5K4CDGX_DATA(0x0902),	// #TVAR_afit_pBaseVals[74]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[75]
S5K4CDGX_DATA(0x0600),	// #TVAR_afit_pBaseVals[76]
S5K4CDGX_DATA(0x0201),	// #TVAR_afit_pBaseVals[77]
S5K4CDGX_DATA(0x3203),	// #TVAR_afit_pBaseVals[78]
S5K4CDGX_DATA(0x005A),	// #TVAR_afit_pBaseVals[79]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[80]
S5K4CDGX_DATA(0x8080),	// #TVAR_afit_pBaseVals[81]
S5K4CDGX_DATA(0x0005),	// #TVAR_afit_pBaseVals[82]		// brightness
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[83]     	// contrast  
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[84]     	// saturation
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[85]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[86]
S5K4CDGX_DATA(0x0032),	// #TVAR_afit_pBaseVals[87]
S5K4CDGX_DATA(0x03FF),	// #TVAR_afit_pBaseVals[88]
S5K4CDGX_DATA(0x00DF),	// #TVAR_afit_pBaseVals[89]
S5K4CDGX_DATA(0x012C),	// #TVAR_afit_pBaseVals[90]
S5K4CDGX_DATA(0x004B),	// #TVAR_afit_pBaseVals[91]
S5K4CDGX_DATA(0x0078),	// #TVAR_afit_pBaseVals[92]
S5K4CDGX_DATA(0x0019),	// #TVAR_afit_pBaseVals[93]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[94]
S5K4CDGX_DATA(0x01F4),	// #TVAR_afit_pBaseVals[95]
S5K4CDGX_DATA(0x00D5),	// #TVAR_afit_pBaseVals[96]
S5K4CDGX_DATA(0x00DE),	// #TVAR_afit_pBaseVals[97]
S5K4CDGX_DATA(0x0100),	// #TVAR_afit_pBaseVals[98]
S5K4CDGX_DATA(0x00F3),	// #TVAR_afit_pBaseVals[99]
S5K4CDGX_DATA(0x0020),	// #TVAR_afit_pBaseVals[100]
S5K4CDGX_DATA(0x0078),	// #TVAR_afit_pBaseVals[101]
S5K4CDGX_DATA(0x0070),	// #TVAR_afit_pBaseVals[102]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[103]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[104]
S5K4CDGX_DATA(0x01CE),	// #TVAR_afit_pBaseVals[105]
S5K4CDGX_DATA(0x003A),	// #TVAR_afit_pBaseVals[106]
S5K4CDGX_DATA(0xF804),	// #TVAR_afit_pBaseVals[107]
S5K4CDGX_DATA(0x010C),	// #TVAR_afit_pBaseVals[108]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[109]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[110]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[111]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[112]
S5K4CDGX_DATA(0x0200),	// #TVAR_afit_pBaseVals[113]
S5K4CDGX_DATA(0x3202),	// #TVAR_afit_pBaseVals[114]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[115]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[116]
S5K4CDGX_DATA(0x4C1E),	// #TVAR_afit_pBaseVals[117]
S5K4CDGX_DATA(0x271D),	// #TVAR_afit_pBaseVals[118]
S5K4CDGX_DATA(0x282A),	// #TVAR_afit_pBaseVals[119]
S5K4CDGX_DATA(0x2233),	// #TVAR_afit_pBaseVals[120]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[121]
S5K4CDGX_DATA(0x00FF),	// #TVAR_afit_pBaseVals[122]
S5K4CDGX_DATA(0x0F32),	// #TVAR_afit_pBaseVals[123]
S5K4CDGX_DATA(0x1414),	// #TVAR_afit_pBaseVals[124]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[125]
S5K4CDGX_DATA(0x8002),	// #TVAR_afit_pBaseVals[126]
S5K4CDGX_DATA(0x061E),	// #TVAR_afit_pBaseVals[127]
S5K4CDGX_DATA(0x0A1E),	// #TVAR_afit_pBaseVals[128]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[129]
S5K4CDGX_DATA(0x0505),	// #TVAR_afit_pBaseVals[130]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[131]
S5K4CDGX_DATA(0x0A0A),	// #TVAR_afit_pBaseVals[132]
S5K4CDGX_DATA(0x0404),	// #TVAR_afit_pBaseVals[133]
S5K4CDGX_DATA(0x0006),	// #TVAR_afit_pBaseVals[134]
S5K4CDGX_DATA(0x0408),	// #TVAR_afit_pBaseVals[135]
S5K4CDGX_DATA(0x7010),	// #TVAR_afit_pBaseVals[136]
S5K4CDGX_DATA(0x0880),	// #TVAR_afit_pBaseVals[137]
S5K4CDGX_DATA(0x0808),	// #TVAR_afit_pBaseVals[138]
S5K4CDGX_DATA(0x1008),	// #TVAR_afit_pBaseVals[139]
S5K4CDGX_DATA(0xFF91),	// #TVAR_afit_pBaseVals[140]
S5K4CDGX_DATA(0xFF9B),	// #TVAR_afit_pBaseVals[141]
S5K4CDGX_DATA(0x0606),	// #TVAR_afit_pBaseVals[142]
S5K4CDGX_DATA(0x4B32),	// #TVAR_afit_pBaseVals[143]
S5K4CDGX_DATA(0x4B32),	// #TVAR_afit_pBaseVals[144]
S5K4CDGX_DATA(0x0590),	// #TVAR_afit_pBaseVals[145]
S5K4CDGX_DATA(0x0523),	// #TVAR_afit_pBaseVals[146]
S5K4CDGX_DATA(0x2700),	// #TVAR_afit_pBaseVals[147]
S5K4CDGX_DATA(0x2700),	// #TVAR_afit_pBaseVals[148]
S5K4CDGX_DATA(0x0200),	// #TVAR_afit_pBaseVals[149]
S5K4CDGX_DATA(0x40FF),	// #TVAR_afit_pBaseVals[150]
S5K4CDGX_DATA(0x0F40),	// #TVAR_afit_pBaseVals[151]
S5K4CDGX_DATA(0x100F),	// #TVAR_afit_pBaseVals[152]
S5K4CDGX_DATA(0x0F00),	// #TVAR_afit_pBaseVals[153]
S5K4CDGX_DATA(0x0018),	// #TVAR_afit_pBaseVals[154]
S5K4CDGX_DATA(0x0900),	// #TVAR_afit_pBaseVals[155]
S5K4CDGX_DATA(0x0902),	// #TVAR_afit_pBaseVals[156]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[157]
S5K4CDGX_DATA(0x0600),	// #TVAR_afit_pBaseVals[158]
S5K4CDGX_DATA(0x0201),	// #TVAR_afit_pBaseVals[159]
S5K4CDGX_DATA(0x3203),	// #TVAR_afit_pBaseVals[160]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[161]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[162]
S5K4CDGX_DATA(0x8080),	// #TVAR_afit_pBaseVals[163]
S5K4CDGX_DATA(0x0005),	// #TVAR_afit_pBaseVals[164]		// brightness
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[165]    	// contrast  
S5K4CDGX_DATA(0x001D),	// #TVAR_afit_pBaseVals[166]    	// saturation
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[167]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[168]
S5K4CDGX_DATA(0x0032),	// #TVAR_afit_pBaseVals[169]
S5K4CDGX_DATA(0x03FF),	// #TVAR_afit_pBaseVals[170]
S5K4CDGX_DATA(0x0130),	// #TVAR_afit_pBaseVals[171]
S5K4CDGX_DATA(0x012C),	// #TVAR_afit_pBaseVals[172]
S5K4CDGX_DATA(0x0074),	// #TVAR_afit_pBaseVals[173]
S5K4CDGX_DATA(0x0078),	// #TVAR_afit_pBaseVals[174]
S5K4CDGX_DATA(0x0019),	// #TVAR_afit_pBaseVals[175]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[176]
S5K4CDGX_DATA(0x01F4),	// #TVAR_afit_pBaseVals[177]
S5K4CDGX_DATA(0x0064),	// #TVAR_afit_pBaseVals[178]
S5K4CDGX_DATA(0x0064),	// #TVAR_afit_pBaseVals[179]
S5K4CDGX_DATA(0x0100),	// #TVAR_afit_pBaseVals[180]
S5K4CDGX_DATA(0x00AA),	// #TVAR_afit_pBaseVals[181]
S5K4CDGX_DATA(0x0020),	// #TVAR_afit_pBaseVals[182]
S5K4CDGX_DATA(0x003C),	// #TVAR_afit_pBaseVals[183]
S5K4CDGX_DATA(0x0070),	// #TVAR_afit_pBaseVals[184]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[185]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[186]
S5K4CDGX_DATA(0x01CE),	// #TVAR_afit_pBaseVals[187]
S5K4CDGX_DATA(0x003A),	// #TVAR_afit_pBaseVals[188]
S5K4CDGX_DATA(0xF804),	// #TVAR_afit_pBaseVals[189]
S5K4CDGX_DATA(0x010C),	// #TVAR_afit_pBaseVals[190]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[191]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[192]
S5K4CDGX_DATA(0x0500),	// #TVAR_afit_pBaseVals[193]
S5K4CDGX_DATA(0x0505),	// #TVAR_afit_pBaseVals[194]
S5K4CDGX_DATA(0x0205),	// #TVAR_afit_pBaseVals[195]
S5K4CDGX_DATA(0x3202),	// #TVAR_afit_pBaseVals[196]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[197]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[198]
S5K4CDGX_DATA(0x191C),	// #TVAR_afit_pBaseVals[199]
S5K4CDGX_DATA(0x191B),	// #TVAR_afit_pBaseVals[200]
S5K4CDGX_DATA(0x1422),	// #TVAR_afit_pBaseVals[201]
S5K4CDGX_DATA(0x1428),	// #TVAR_afit_pBaseVals[202]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[203]
S5K4CDGX_DATA(0x00FF),	// #TVAR_afit_pBaseVals[204]
S5K4CDGX_DATA(0x0F32),	// #TVAR_afit_pBaseVals[205]
S5K4CDGX_DATA(0x1414),	// #TVAR_afit_pBaseVals[206]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[207]
S5K4CDGX_DATA(0x8002),	// #TVAR_afit_pBaseVals[208]
S5K4CDGX_DATA(0x061E),	// #TVAR_afit_pBaseVals[209]
S5K4CDGX_DATA(0x0A1E),	// #TVAR_afit_pBaseVals[210]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[211]
S5K4CDGX_DATA(0x0505),	// #TVAR_afit_pBaseVals[212]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[213]
S5K4CDGX_DATA(0x0A0A),	// #TVAR_afit_pBaseVals[214]
S5K4CDGX_DATA(0x0404),	// #TVAR_afit_pBaseVals[215]
S5K4CDGX_DATA(0x1106),	// #TVAR_afit_pBaseVals[216]
S5K4CDGX_DATA(0x0F08),	// #TVAR_afit_pBaseVals[217]
S5K4CDGX_DATA(0x2510),	// #TVAR_afit_pBaseVals[218]
S5K4CDGX_DATA(0x0880),	// #TVAR_afit_pBaseVals[219]
S5K4CDGX_DATA(0x0808),	// #TVAR_afit_pBaseVals[220]
S5K4CDGX_DATA(0x1008),	// #TVAR_afit_pBaseVals[221]
S5K4CDGX_DATA(0xFF33),	// #TVAR_afit_pBaseVals[222]
S5K4CDGX_DATA(0x007E),	// #TVAR_afit_pBaseVals[223]
S5K4CDGX_DATA(0x0605),	// #TVAR_afit_pBaseVals[224]
S5K4CDGX_DATA(0x6437),	// #TVAR_afit_pBaseVals[225]
S5K4CDGX_DATA(0x6437),	// #TVAR_afit_pBaseVals[226]
S5K4CDGX_DATA(0x0090),	// #TVAR_afit_pBaseVals[227]
S5K4CDGX_DATA(0x0903),	// #TVAR_afit_pBaseVals[228]
S5K4CDGX_DATA(0x1E00),	// #TVAR_afit_pBaseVals[229]
S5K4CDGX_DATA(0x1B00),	// #TVAR_afit_pBaseVals[230]
S5K4CDGX_DATA(0x0200),	// #TVAR_afit_pBaseVals[231]
S5K4CDGX_DATA(0x40FF),	// #TVAR_afit_pBaseVals[232]
S5K4CDGX_DATA(0x0F40),	// #TVAR_afit_pBaseVals[233]
S5K4CDGX_DATA(0x100F),	// #TVAR_afit_pBaseVals[234]
S5K4CDGX_DATA(0x0F00),	// #TVAR_afit_pBaseVals[235]
S5K4CDGX_DATA(0x0018),	// #TVAR_afit_pBaseVals[236]
S5K4CDGX_DATA(0x0900),	// #TVAR_afit_pBaseVals[237]
S5K4CDGX_DATA(0x0902),	// #TVAR_afit_pBaseVals[238]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[239]
S5K4CDGX_DATA(0x0600),	// #TVAR_afit_pBaseVals[240]
S5K4CDGX_DATA(0x0201),	// #TVAR_afit_pBaseVals[241]
S5K4CDGX_DATA(0x3C03),	// #TVAR_afit_pBaseVals[242]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[243]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[244]
S5K4CDGX_DATA(0x8080),	// #TVAR_afit_pBaseVals[245]
S5K4CDGX_DATA(0x0005),	// #TVAR_afit_pBaseVals[246]		// brightness
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[247]    	// contrast  
S5K4CDGX_DATA(0x002E),	// #TVAR_afit_pBaseVals[248]    	// saturation
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[249]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[250]
S5K4CDGX_DATA(0x0032),	// #TVAR_afit_pBaseVals[251]
S5K4CDGX_DATA(0x03FF),	// #TVAR_afit_pBaseVals[252]
S5K4CDGX_DATA(0x0144),	// #TVAR_afit_pBaseVals[253]
S5K4CDGX_DATA(0x012C),	// #TVAR_afit_pBaseVals[254]
S5K4CDGX_DATA(0x0078),	// #TVAR_afit_pBaseVals[255]
S5K4CDGX_DATA(0x0078),	// #TVAR_afit_pBaseVals[256]
S5K4CDGX_DATA(0x0019),	// #TVAR_afit_pBaseVals[257]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[258]
S5K4CDGX_DATA(0x01F4),	// #TVAR_afit_pBaseVals[259]
S5K4CDGX_DATA(0x0009),	// #TVAR_afit_pBaseVals[260]
S5K4CDGX_DATA(0x0012),	// #TVAR_afit_pBaseVals[261]
S5K4CDGX_DATA(0x0100),	// #TVAR_afit_pBaseVals[262]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[263]
S5K4CDGX_DATA(0x0020),	// #TVAR_afit_pBaseVals[264]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[265]
S5K4CDGX_DATA(0x0070),	// #TVAR_afit_pBaseVals[266]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[267]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[268]
S5K4CDGX_DATA(0x01CE),	// #TVAR_afit_pBaseVals[269]
S5K4CDGX_DATA(0x003A),	// #TVAR_afit_pBaseVals[270]
S5K4CDGX_DATA(0xF804),	// #TVAR_afit_pBaseVals[271]
S5K4CDGX_DATA(0x010C),	// #TVAR_afit_pBaseVals[272]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[273]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[274]
S5K4CDGX_DATA(0x0500),	// #TVAR_afit_pBaseVals[275]
S5K4CDGX_DATA(0x0505),	// #TVAR_afit_pBaseVals[276]
S5K4CDGX_DATA(0x0205),	// #TVAR_afit_pBaseVals[277]
S5K4CDGX_DATA(0x3202),	// #TVAR_afit_pBaseVals[278]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[279]
S5K4CDGX_DATA(0x0007),	// #TVAR_afit_pBaseVals[280]
S5K4CDGX_DATA(0x1913),	// #TVAR_afit_pBaseVals[281]
S5K4CDGX_DATA(0x1913),	// #TVAR_afit_pBaseVals[282]
S5K4CDGX_DATA(0x080A),	// #TVAR_afit_pBaseVals[283]
S5K4CDGX_DATA(0x080F),	// #TVAR_afit_pBaseVals[284]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[285]
S5K4CDGX_DATA(0x00FF),	// #TVAR_afit_pBaseVals[286]
S5K4CDGX_DATA(0x0F32),	// #TVAR_afit_pBaseVals[287]
S5K4CDGX_DATA(0x1414),	// #TVAR_afit_pBaseVals[288]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[289]
S5K4CDGX_DATA(0x8007),	// #TVAR_afit_pBaseVals[290]
S5K4CDGX_DATA(0x061E),	// #TVAR_afit_pBaseVals[291]
S5K4CDGX_DATA(0x0A1E),	// #TVAR_afit_pBaseVals[292]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[293]
S5K4CDGX_DATA(0x0A05),	// #TVAR_afit_pBaseVals[294]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[295]
S5K4CDGX_DATA(0x0A0A),	// #TVAR_afit_pBaseVals[296]
S5K4CDGX_DATA(0x0404),	// #TVAR_afit_pBaseVals[297]
S5K4CDGX_DATA(0x1306),	// #TVAR_afit_pBaseVals[298]
S5K4CDGX_DATA(0x1110),	// #TVAR_afit_pBaseVals[299]
S5K4CDGX_DATA(0x0F18),	// #TVAR_afit_pBaseVals[300]
S5K4CDGX_DATA(0x0880),	// #TVAR_afit_pBaseVals[301]
S5K4CDGX_DATA(0x0808),	// #TVAR_afit_pBaseVals[302]
S5K4CDGX_DATA(0x1008),	// #TVAR_afit_pBaseVals[303]
S5K4CDGX_DATA(0x001E),	// #TVAR_afit_pBaseVals[304]
S5K4CDGX_DATA(0x001E),	// #TVAR_afit_pBaseVals[305]
S5K4CDGX_DATA(0x0605),	// #TVAR_afit_pBaseVals[306]
S5K4CDGX_DATA(0x6E4C),	// #TVAR_afit_pBaseVals[307]
S5K4CDGX_DATA(0x6E48),	// #TVAR_afit_pBaseVals[308]
S5K4CDGX_DATA(0x0086),	// #TVAR_afit_pBaseVals[309]
S5K4CDGX_DATA(0x2904),	// #TVAR_afit_pBaseVals[310]
S5K4CDGX_DATA(0x0A00),	// #TVAR_afit_pBaseVals[311]
S5K4CDGX_DATA(0x0A00),	// #TVAR_afit_pBaseVals[312]
S5K4CDGX_DATA(0x0200),	// #TVAR_afit_pBaseVals[313]
S5K4CDGX_DATA(0x40FF),	// #TVAR_afit_pBaseVals[314]
S5K4CDGX_DATA(0x0F40),	// #TVAR_afit_pBaseVals[315]
S5K4CDGX_DATA(0x100F),	// #TVAR_afit_pBaseVals[316]
S5K4CDGX_DATA(0x0F00),	// #TVAR_afit_pBaseVals[317]
S5K4CDGX_DATA(0x0018),	// #TVAR_afit_pBaseVals[318]
S5K4CDGX_DATA(0x0900),	// #TVAR_afit_pBaseVals[319]
S5K4CDGX_DATA(0x0903),	// #TVAR_afit_pBaseVals[320]
S5K4CDGX_DATA(0x0002),	// #TVAR_afit_pBaseVals[321]
S5K4CDGX_DATA(0x0800),	// #TVAR_afit_pBaseVals[322]
S5K4CDGX_DATA(0x0106),	// #TVAR_afit_pBaseVals[323]
S5K4CDGX_DATA(0x5002),	// #TVAR_afit_pBaseVals[324]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[325]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[326]
S5K4CDGX_DATA(0x8080),	// #TVAR_afit_pBaseVals[327]
S5K4CDGX_DATA(0x0005),	// #TVAR_afit_pBaseVals[328]		// brightness
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[329]    	// contrast  
S5K4CDGX_DATA(0x0030),	// #TVAR_afit_pBaseVals[330]    	// saturation
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[331]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[332]
S5K4CDGX_DATA(0x0032),	// #TVAR_afit_pBaseVals[333]
S5K4CDGX_DATA(0x03FF),	// #TVAR_afit_pBaseVals[334]
S5K4CDGX_DATA(0x0158),	// #TVAR_afit_pBaseVals[335]
S5K4CDGX_DATA(0x0158),	// #TVAR_afit_pBaseVals[336]
S5K4CDGX_DATA(0x007E),	// #TVAR_afit_pBaseVals[337]
S5K4CDGX_DATA(0x007E),	// #TVAR_afit_pBaseVals[338]
S5K4CDGX_DATA(0x0019),	// #TVAR_afit_pBaseVals[339]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[340]
S5K4CDGX_DATA(0x01F4),	// #TVAR_afit_pBaseVals[341]
S5K4CDGX_DATA(0x0009),	// #TVAR_afit_pBaseVals[342]
S5K4CDGX_DATA(0x0012),	// #TVAR_afit_pBaseVals[343]
S5K4CDGX_DATA(0x0100),	// #TVAR_afit_pBaseVals[344]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[345]
S5K4CDGX_DATA(0x0020),	// #TVAR_afit_pBaseVals[346]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[347]
S5K4CDGX_DATA(0x0070),	// #TVAR_afit_pBaseVals[348]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[349]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[350]
S5K4CDGX_DATA(0x01CE),	// #TVAR_afit_pBaseVals[351]
S5K4CDGX_DATA(0x003A),	// #TVAR_afit_pBaseVals[352]
S5K4CDGX_DATA(0xF804),	// #TVAR_afit_pBaseVals[353]
S5K4CDGX_DATA(0x010C),	// #TVAR_afit_pBaseVals[354]
S5K4CDGX_DATA(0x0003),	// #TVAR_afit_pBaseVals[355]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[356]
S5K4CDGX_DATA(0x0500),	// #TVAR_afit_pBaseVals[357]
S5K4CDGX_DATA(0x0505),	// #TVAR_afit_pBaseVals[358]
S5K4CDGX_DATA(0x0205),	// #TVAR_afit_pBaseVals[359]
S5K4CDGX_DATA(0x3202),	// #TVAR_afit_pBaseVals[360]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[361]
S5K4CDGX_DATA(0x0007),	// #TVAR_afit_pBaseVals[362]
S5K4CDGX_DATA(0x190F),	// #TVAR_afit_pBaseVals[363]
S5K4CDGX_DATA(0x190F),	// #TVAR_afit_pBaseVals[364]
S5K4CDGX_DATA(0x0403),	// #TVAR_afit_pBaseVals[365]
S5K4CDGX_DATA(0x0405),	// #TVAR_afit_pBaseVals[366]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[367]
S5K4CDGX_DATA(0x00FF),	// #TVAR_afit_pBaseVals[368]
S5K4CDGX_DATA(0x0F32),	// #TVAR_afit_pBaseVals[369]
S5K4CDGX_DATA(0x1414),	// #TVAR_afit_pBaseVals[370]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[371]
S5K4CDGX_DATA(0x8007),	// #TVAR_afit_pBaseVals[372]
S5K4CDGX_DATA(0x061E),	// #TVAR_afit_pBaseVals[373]
S5K4CDGX_DATA(0x0A1E),	// #TVAR_afit_pBaseVals[374]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[375]
S5K4CDGX_DATA(0x0505),	// #TVAR_afit_pBaseVals[376]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[377]
S5K4CDGX_DATA(0x0A0A),	// #TVAR_afit_pBaseVals[378]
S5K4CDGX_DATA(0x0404),	// #TVAR_afit_pBaseVals[379]
S5K4CDGX_DATA(0x1706),	// #TVAR_afit_pBaseVals[380]
S5K4CDGX_DATA(0x1318),	// #TVAR_afit_pBaseVals[381]
S5K4CDGX_DATA(0x0F18),	// #TVAR_afit_pBaseVals[382]
S5K4CDGX_DATA(0x0880),	// #TVAR_afit_pBaseVals[383]
S5K4CDGX_DATA(0x0808),	// #TVAR_afit_pBaseVals[384]
S5K4CDGX_DATA(0x1008),	// #TVAR_afit_pBaseVals[385]
S5K4CDGX_DATA(0x000F),	// #TVAR_afit_pBaseVals[386]
S5K4CDGX_DATA(0x0000),	// #TVAR_afit_pBaseVals[387]
S5K4CDGX_DATA(0x0605),	// #TVAR_afit_pBaseVals[388]
S5K4CDGX_DATA(0x7863),	// #TVAR_afit_pBaseVals[389]
S5K4CDGX_DATA(0x785E),	// #TVAR_afit_pBaseVals[390]
S5K4CDGX_DATA(0x006B),	// #TVAR_afit_pBaseVals[391]
S5K4CDGX_DATA(0x3C04),	// #TVAR_afit_pBaseVals[392]
S5K4CDGX_DATA(0x0700),	// #TVAR_afit_pBaseVals[393]
S5K4CDGX_DATA(0x0500),	// #TVAR_afit_pBaseVals[394]
S5K4CDGX_DATA(0x0200),	// #TVAR_afit_pBaseVals[395]
S5K4CDGX_DATA(0x40FF),	// #TVAR_afit_pBaseVals[396]
S5K4CDGX_DATA(0x0F40),	// #TVAR_afit_pBaseVals[397]
S5K4CDGX_DATA(0x100F),	// #TVAR_afit_pBaseVals[398]
S5K4CDGX_DATA(0x0F00),	// #TVAR_afit_pBaseVals[399]
S5K4CDGX_DATA(0x0018),	// #TVAR_afit_pBaseVals[400]
S5K4CDGX_DATA(0x0900),	// #TVAR_afit_pBaseVals[401]
S5K4CDGX_DATA(0x0903),	// #TVAR_afit_pBaseVals[402]
S5K4CDGX_DATA(0x0002),	// #TVAR_afit_pBaseVals[403]
S5K4CDGX_DATA(0x0800),	// #TVAR_afit_pBaseVals[404]
S5K4CDGX_DATA(0x0106),	// #TVAR_afit_pBaseVals[405]
S5K4CDGX_DATA(0x5A02),	// #TVAR_afit_pBaseVals[406]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[407]
S5K4CDGX_DATA(0x0080),	// #TVAR_afit_pBaseVals[408]
S5K4CDGX_DATA(0x8080),	// #TVAR_afit_pBaseVals[409]

S5K4CDGX_DATA(0x7C7E),	// #afit_pConstBaseVals[0]
S5K4CDGX_DATA(0xF7BD),	// #afit_pConstBaseVals[1]
S5K4CDGX_DATA(0xBE7C),	// #afit_pConstBaseVals[2]
S5K4CDGX_DATA(0x95BD),	// #afit_pConstBaseVals[3]
S5K4CDGX_DATA(0x7C32),	// #afit_pConstBaseVals[4]
S5K4CDGX_DATA(0x0003),	// #afit_pConstBaseVals[5]
};

//Update changed registers in config2 seq //checked
static struct s5k4cdgx_request s5k4cdgx_init_reg_config2[] ={
// continus af
S5K4CDGX_REG(0x02c2),
S5K4CDGX_DATA(0x0003),
S5K4CDGX_MSLEEP(50),
S5K4CDGX_REG(0x02c2),
S5K4CDGX_DATA(0x0006),
// Update Changed Registers
S5K4CDGX_REG(0x0532),
S5K4CDGX_DATA(0x0001),	// #REG_TC_DBG_ReInitCmd
S5K4CDGX_REG(0x0532),
S5K4CDGX_DATA(0x0001),	// #REG_TC_DBG_ReInitCmd
S5K4CDGX_REG(0x0532),
S5K4CDGX_DATA(0x0001),	// #REG_TC_DBG_ReInitCmd
// ---------------------------------------------------------------------------- End of 4CDGX EVT1.0 initializationion
};
#endif
