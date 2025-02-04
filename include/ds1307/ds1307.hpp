/*
	@file ds1307.hpp
	@brief header file for ds1307 RTC library for raspberry pi Pico
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#pragma once

/*! 
 * @brief Class for DS1307 real-time clock library
 */
class RTCDS1307 {

public:

	RTCDS1307(uint8_t address, i2c_inst_t* i2c_type, uint8_t sdata, uint8_t sclk, uint16_t clockspeed);

	bool InitI2C(void);
	void DeInitI2C(void);
	bool IsPresentClock(void);
	int readClock(void);
	int writeClockDefault(void);
	int writeClock(char *str);



	/*! @brief Enumeration representing days of the week */
	enum DayOfWeek_e : uint8_t {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};

	/*! @brief Structure representing the DS1307 time format */
	struct rtc_time_read_t {
		uint8_t secL:4;   /*!< Low nibble of seconds */
		uint8_t secH:3;   /*!< High nibble of seconds */
		uint8_t hold_clock:1; /*!< Clock halt bit */

		uint8_t minL:4;   /*!< Low nibble of minutes */
		uint8_t minH:3;   /*!< High nibble of minutes */
		uint8_t rfu1:1;   /*!< Reserved for future use */

		uint8_t hourL:4;  /*!< Low nibble of hours */
		uint8_t hourH:2;  /*!< High nibble of hours */
		uint8_t hour_mode:1; /*!< 12/24-hour mode */
		uint8_t rfu2:1;   /*!< Reserved for future use */

		uint8_t day:3;    /*!< Day of the week */
		uint8_t rfu3:5;   /*!< Reserved for future use */

		uint8_t dateL:4;  /*!< Low nibble of date */
		uint8_t dateH:2;  /*!< High nibble of date */
		uint8_t rfu4:2;   /*!< Reserved for future use */

		uint8_t monthL:4; /*!< Low nibble of month */
		uint8_t monthH:1; /*!< High nibble of month */
		uint8_t rfu5:3;   /*!< Reserved for future use */

		uint8_t yearL:4;  /*!< Low nibble of year */
		uint8_t yearH:4;  /*!< High nibble of year */
	};
	DayOfWeek_e DayofWeek;
	rtc_time_read_t rtcData; /*!< Structure holding RTC data */
	uint8_t RTC_ADDR = 0x68; /*!< I2C address of the DS1307 */
protected:

private:
	uint8_t rtcBuff[7] = {0}; /*!< Buffer for reading/writing RTC data */
	
	int _returnValue = 0; /*! Holds I2C status, number of bytes returned, negative number indicates error */
	const uint8_t Ds1307SecondRegAddress  = 0x00; /*!< Address of DS1307 seconds register */
	const uint8_t Ds1307DateRegAddress    = 0x04; /*!< Address of DS1307 date register */
	const uint8_t Ds1307ControlRegAddress = 0x07; /*!< Address of DS1307 control register */
	const uint8_t DS1307_ERROR            = 0xFF; /*!< Error code for I2C communication failure */

	i2c_inst_t * _i2c; /*!< Pointer to I2C instance */
	uint8_t _address; /*!< I2C address of the device */
	uint8_t _DATA_IO; /*!< Data IO pin */
	uint8_t _CLOCK_IO; /*!< Clock IO pin */
	uint16_t _CLKSpeed = 100; /*!< I2C clock speed in kHz (typically 100-400) */
	bool _isConnected = false; /*!< Connection status flag */
	uint DS1307_I2C_Timeout  = 50000; /*!< Timeout for I2C communication in microseconds */
};
