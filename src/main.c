/*
 * Program for reading HYT digital humidity & temperature sensors TSic206, TSic306 and TSic506
 * using Silicon Labs' EFM32ZG-STK3200 kit
 *
 * Whether the button is pressed, we receive temrerature and humidity data from the sensor.
 *
 * HYT sensor connected by 4 pins (Vdd, GND, SDA and SCL) and distribute data with I2C protocol.
 *
 * GLIB library provided by SiLabs is used to show temperature at the Sharp Memory
 * LCD on the EFM32ZG-STK3200.
 *
 * HYT Application Note: https://www.ist-ag.com/sites/default/files/AHHYTM_E.pdf
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "i2cspm.h"
#include "em_timer.h"

#include "HYT.h"

float temperature, humidity;

/* Display functions from disp.c file */
void Display_Init(void);
void Display_WellcomeScreen(void);
void Display_ShowValues(float temp, float rh);
void Display_ShowError(void);
void Display_ShowCMode(void);

/**************************************************************************//**
 * Start a measuring cycle (Measurement Request command)
 *  - [6-bit address] + [W flag]
 *  - w/o data transfer
 *****************************************************************************/
void performMRCommand(void) {
	I2C_TransferSeq_TypeDef seq;
	seq.addr = HYT_ADDR;
	seq.flags = I2C_FLAG_WRITE;
	seq.buf[0].len = 0;
	seq.buf[1].len = 0;
	I2CSPM_Transfer(I2C0, &seq);
}

/**************************************************************************//**
 * Fetch the last measured value of Humidity / Temperature (Data Fetch command)
 *  - [6-bit address] + [R flag]
 *  - w/ 4-byte data transfer: [CMode bit] + [State bit] + [14 bit humididty] +
 * 								+ [14 bit temeprature] + [2 insignificant bits]
 *****************************************************************************/
int8_t performDFCommand(void) {
	I2C_TransferSeq_TypeDef seq;
	uint8_t I2CdataToRead[4];
	unsigned int tempRawData, humididtyRawData;

	seq.addr = HYT_ADDR;
	seq.flags = I2C_FLAG_READ;
	seq.buf[0].data = I2CdataToRead;
	seq.buf[0].len = 4;
	seq.buf[1].len = 0;
	I2CSPM_Transfer(I2C0, &seq);

	uint8_t sensorCModeBit = (I2CdataToRead[0] & 0x80) >> 7;
	uint8_t sensorStateBit = (I2CdataToRead[0] & 0x40) >> 6;
	tempRawData = ((I2CdataToRead[2] << 8) | I2CdataToRead[3]) >> 2;
	humididtyRawData = ((I2CdataToRead[0] & 0x3F) << 8) | I2CdataToRead[1];

	if (sensorCModeBit == 1) {
		return HYT_COMMAND_MODE;
	}
	if (tempRawData != 0 && humididtyRawData != 0 && tempRawData < 0x3FFF
			&& humididtyRawData < 0x3FFF) {
		if (sensorStateBit != 1) {
			/* temperature as 14bit-value in the range from -40°C to +125°C */
			temperature = ((float) (tempRawData) * 165.0F / 16383.0F) - 40.0F;
			/* humidity as 14bit-value in the range from 0%rH to 100%rH */
			humidity = (float) humididtyRawData * 100.0F / 16383.0F;
		} else {
			/* No new value has been created since the last reading */
		}
		return HYT_IS_OK;
	} else {
		return HYT_ERROR;
	}
}

/**************************************************************************//**
 * Setup GPIO for both I2C wires and pushbutton #1
 *****************************************************************************/
static void gpioSetup(void) {
	GPIO_PinModeSet(HYT_SDA_PORT, HYT_SDA_PIN, gpioModeWiredAndPullUpFilter, 1);
	GPIO_PinModeSet(HYT_SCL_PORT, HYT_SCL_PIN, gpioModeWiredAndPullUpFilter, 1);

	GPIO_PinModeSet(BUTTON_PORT, BUTTON_PIN, gpioModeInput, 1);
	GPIO_IntConfig(BUTTON_PORT, BUTTON_PIN, false, true, true);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

/**************************************************************************//**
 * Wait for 100 msec using TIMER #0
 * !!! Doing nothing while waiting, but it's not necessary !!!
 *****************************************************************************/
void delay100ms() {
	TIMER_Init_TypeDef mytimerInit = TIMER_INIT_DEFAULT;
	mytimerInit.prescale = timerPrescale1024;
	TIMER_Init(TIMER0, &mytimerInit);
	TIMER_Enable(TIMER0, true);
	while (TIMER_COUNTER < WAITING100MS_TICKS) {
	}
	TIMER_Enable(TIMER0, false);
}

/**************************************************************************//**
 * Read temperature and humidity from HYT sensor, show result on the display
 *  - Send MR (Measurement Requests) command
 *  - Wait for 100 msec
 *  - Send DF (Data Fetch) command
 *  - Show result OR show error message
 *****************************************************************************/
void ReceiveDataAndShowIt(void) {
	performMRCommand();
	delay100ms();
	int8_t HYT_result = performDFCommand();
	if (HYT_result == HYT_IS_OK) {
		Display_ShowValues(temperature, humidity);
	} else if (HYT_result == HYT_COMMAND_MODE) {
		Display_ShowCMode();
	} else if (HYT_result == HYT_ERROR) {
		Display_ShowError();
	}
}

/**************************************************************************//**
 * GPIO Interrupt handler (pushbutton # 1)
 *  - Receive HYT data & show results on the display
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void) {
	uint32_t interruptMask = GPIO_IntGet();

	if (interruptMask & (1 << BUTTON_PIN)) {
		Display_WellcomeScreen();
		ReceiveDataAndShowIt();
	}

	GPIO_IntClear(interruptMask);
}

/**************************************************************************//**
 * I2C setup and enable
 *****************************************************************************/
void setupI2C(void) {
	/* MCU is master node with standard settings */
	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
	/* Module I2C0 is configured to location 6 */
	I2C0->ROUTE = (I2C0->ROUTE & ~_I2C_ROUTE_LOCATION_MASK) | I2C_ROUTE_LOCATION_LOC6;
	/* Enable signals SCL, SDA */
	I2C0->ROUTE |= I2C_ROUTE_SCLPEN | I2C_ROUTE_SDAPEN;
	I2C_Init(I2C0, &i2cInit);
}

/**************************************************************************//**
 * Main function
 *****************************************************************************/
int main(void) {
	CHIP_Init();
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);
	CMU_ClockEnable(cmuClock_I2C0, true);

	setupI2C();
	gpioSetup();
	Display_Init();
	Display_WellcomeScreen();

	while (1) {
	}
}
