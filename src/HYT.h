/* PB #1 */
#define BUTTON_PORT 			gpioPortC
#define BUTTON_PIN  			9

#define HYT_ADDR    0x50
/* HYT SENSOR PINS */
#define HYT_SDA_PORT			gpioPortE
#define HYT_SDA_PIN				12
#define HYT_SCL_PORT			gpioPortE
#define HYT_SCL_PIN				13
///* HYT SENSOR STATES */
#define HYT_IS_OK 				0
#define HYT_ERROR 				-1
#define HYT_COMMAND_MODE		-2

/* TIMING FOR HYT POLLING */
#define TIMER_COUNTER			(TIMER0 -> CNT)
/* PRESCALER = 1024 => 14 Mhz * 100 msec / 1024 = 1367 */
#define WAITING100MS_TICKS		1367
