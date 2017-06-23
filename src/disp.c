#include "display.h"
#include "dmd.h"
#include "glib.h"
#include <stdio.h>
#include <string.h>

#define STR_LEN 			60
static GLIB_Context_t 		glibContext;

/**************************************************************************//**
 * Floating point value to the string
 *  - one digit after the decimal point
 *  - add "-" if necessary
 *  - add "C" at the end
 *****************************************************************************/
static void convertTempToString(char *str, float temp) {
	uint8_t strCnt = 0;
	int16_t multipliedTemp = temp * 10;

	if (multipliedTemp < 0) {
		multipliedTemp = -multipliedTemp;
		str[strCnt] = '-';
		strCnt++;
	}
	if (multipliedTemp >= 1000) {
		str[strCnt] = '0' + (multipliedTemp % 10000) / 1000;
		strCnt++;
	}
	if (multipliedTemp >= 100) {
		str[strCnt] = '0' + (multipliedTemp % 1000) / 100;
		strCnt++;
	}
	if (multipliedTemp >= 10) {
		str[strCnt] = '0' + (multipliedTemp % 100) / 10;
		strCnt++;
	}
	str[strCnt] = '.';
	strCnt++;
	if (multipliedTemp >= 1) {
		str[strCnt] = '0' + (multipliedTemp % 10);
	} else {
		str[strCnt] = '0';
	}
	strCnt++;
	str[strCnt] = 'C';
	strCnt++;
	str[strCnt] = 0;
}


/**************************************************************************//**
 * Floating point value to the string
 *  - add "rH" at the end
 *****************************************************************************/
static void convertHumidityToString(char *str, float rh) {
	uint8_t strCnt = 0;
	int16_t multipliedRH = rh * 10;

	if (multipliedRH >= 1000) {
		str[strCnt] = '0' + (multipliedRH % 10000) / 1000;
		strCnt++;
	}
	if (multipliedRH >= 100) {
		str[strCnt] = '0' + (multipliedRH % 1000) / 100;
		strCnt++;
	}
	if (multipliedRH >= 10) {
		str[strCnt] = '0' + (multipliedRH % 100) / 10;
		strCnt++;
	}
	str[strCnt] = '%';
	strCnt++;
	str[strCnt] = 'r';
	strCnt++;
	str[strCnt] = 'H';
	strCnt++;
	str[strCnt] = 0;
}

/**************************************************************************//**
 * Text output functions
 *****************************************************************************/
static void Glib_ShowValues(GLIB_Context_t *pContext, float temp_value, float rh_value) {
	char string[STR_LEN];

	GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8);
	snprintf(string, STR_LEN, "Humidity = ");
	GLIB_drawString(&glibContext, string, strlen(string), 5, 100, 0);
	snprintf(string, STR_LEN, "Temperature = ");
	GLIB_drawString(&glibContext, string, strlen(string), 5, 115, 0);
	convertHumidityToString(string, rh_value);
	GLIB_drawString(&glibContext, string, strlen(string), 69, 100, 0);
	convertTempToString(string, temp_value);
	GLIB_drawString(&glibContext, string, strlen(string), 89, 115, 0);
}

static void Glib_ShowError(GLIB_Context_t *pContext) {
	char string[STR_LEN];

	GLIB_setFont(pContext, (GLIB_Font_t *) &GLIB_FontNormal8x8);
	snprintf(string, STR_LEN, "READING ERROR");
	GLIB_drawString(&glibContext, string, strlen(string), 15, 105, 0);
}

static void Glib_ShowCMode(GLIB_Context_t *pContext) {
	char string[STR_LEN];

	GLIB_setFont(pContext, (GLIB_Font_t *) &GLIB_FontNormal8x8);
	snprintf(string, STR_LEN, "COMMAND MODE");
	GLIB_drawString(&glibContext, string, strlen(string), 15, 105, 0);
}

static void Glib_ShowText(GLIB_Context_t *pContext) {
	char string[STR_LEN];

	GLIB_setFont(pContext, (GLIB_Font_t *) &GLIB_FontNormal8x8);
	snprintf(string, STR_LEN, "HYT-939 \nHUMIDITY \nSENSOR");
	GLIB_drawString(&glibContext, string, strlen(string), 15, 15, 0);

	GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8);
	snprintf(string, STR_LEN,
			"Use pushbutton #1 to\nupdate temperature \nand humidity values..");
	GLIB_drawString(&glibContext, string, strlen(string), 5, 55, 0);
}


/**************************************************************************//**
 * Auxiliary display functions
 *****************************************************************************/
void Display_Init(void) {
	EMSTATUS status;

	status = DISPLAY_Init();
	if (DISPLAY_EMSTATUS_OK != status)
		while (1) {
		}
	status = DMD_init(0);
	if (DMD_OK != status)
		while (1) {
		}
	status = GLIB_contextInit(&glibContext);
	if (GLIB_OK != status)
		while (1) {
		}
}

static void Glib_PrepareAndClear(void) {
	glibContext.backgroundColor = Black;
	glibContext.foregroundColor = White;
	GLIB_clear(&glibContext);
}


/**************************************************************************//**
 * Functions to use from main program
 *****************************************************************************/
void Display_ShowValues(float temp, float rh) {
	Glib_PrepareAndClear();
	Glib_ShowText(&glibContext);
	Glib_ShowValues(&glibContext, temp, rh);
	DMD_updateDisplay();
}

void Display_ShowError(void) {
	Glib_PrepareAndClear();
	Glib_ShowText(&glibContext);
	Glib_ShowError(&glibContext);
	DMD_updateDisplay();
}

void Display_ShowCMode(void) {
	Glib_PrepareAndClear();
	Glib_ShowText(&glibContext);
	Glib_ShowCMode(&glibContext);
	DMD_updateDisplay();
}

void Display_WellcomeScreen(void) {
	Glib_PrepareAndClear();
	Glib_ShowText(&glibContext);
	DMD_updateDisplay();
}
