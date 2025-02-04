/*!
	@file examples/ds1307/main.cpp
	@brief RPI PICO RP2040  SDK C++ ds1307 library test file
*/

// === Libraries ===

// Standard
#include "pico/stdlib.h"
// Custom libraries
#include "ds1307/ds1307.hpp"


// Misc

const uint StatusLEDPin = 25; // PICO on_board Status LED

//ds1307
uint8_t I2C_CONNECTION_ATTEMPTS = 3; // No of attempts to establish I2C connect at start
#define SDATA_DS1307 16
#define SCLK_DS1307  17
RTCDS1307  myRTC(myRTC.RTC_ADDR, i2c0, SDATA_DS1307 , SCLK_DS1307 , 100);


// === Function Prototypes ===
bool Setup(void);
bool IsConnected(void);
void printClockData(void);

// === Main ====
int main()
{
	// "YYMMDDWHHMM" , where W is day of week (sunday =0)
	char setTimeDate[12] = "24020441230"; // Example: 2024-Feb-04,Thursday, 12:30:00
	if(!Setup())
		return -1;
	if(!IsConnected())
		return -1;
	busy_wait_ms(1000);
	if (myRTC.writeClockDefault() < 0) //Write the default test data
		return -1;
	busy_wait_ms(1000);
	myRTC.readClock(); // read back the default data to rtcData
	printClockData();  // print it out to console (rtcData)
	busy_wait_ms(10000);

	myRTC.writeClock(setTimeDate); //write the time from setTimeDate
	busy_wait_ms(1000);
	myRTC.readClock(); // read back the default data to rtcData
	printClockData();  // print it out to console (rtcData)
	busy_wait_ms(10000);

	return 0;
}

// === End  of main ===


// === Function Space ===
bool Setup(void)
{
	busy_wait_ms(100);

	// Initialize Status LED pin
	gpio_init(StatusLEDPin);
	gpio_set_dir(StatusLEDPin, GPIO_OUT);
	gpio_put(StatusLEDPin, true);

	// Init default  serial port, baud 38400
	stdio_init_all();
	busy_wait_ms(1000);
	printf("Test Start\n");
	if (myRTC.InitI2C())
	{
		printf("RTC INIT Success\n");
		return true;
	}
	else
	{
		printf("RTC INIT Failure\n");
		return false;
	}
}

bool IsConnected(void)
{
	uint8_t connectionAttempts = 0;
	// Start the sensor comms
	while (myRTC.IsPresentClock() != true) 
	{
		busy_wait_ms(2000);
		connectionAttempts++;
		if (connectionAttempts == I2C_CONNECTION_ATTEMPTS){return false;} 
	}
	return true;
}


void printClockData(void)
{
	char nameDay[4];
	switch (myRTC.rtcData.day) {
		case RTCDS1307::Sunday: strcpy(nameDay, "Sun");
			break;
		case RTCDS1307::Monday: strcpy(nameDay, "Mon");
			break;
		case RTCDS1307::Tuesday: strcpy(nameDay, "Tue");
			break;
		case RTCDS1307::Wednesday: strcpy(nameDay, "Wed");
			break;
		case RTCDS1307::Thursday: strcpy(nameDay, "Thu");
			break;
		case RTCDS1307::Friday: strcpy(nameDay, "Fri");
			break;
		case RTCDS1307::Saturday: strcpy(nameDay, "Sat");
			break;
	}

	char printBuffer[25] = {0};

	sprintf(printBuffer, "20%d%d/%d%d/%d%d %s",
			myRTC.rtcData.yearH,
			myRTC.rtcData.yearL,
			myRTC.rtcData.monthH,
			myRTC.rtcData.monthL,
			myRTC.rtcData.dateH,
			myRTC.rtcData.dateL,
			nameDay
			);
	printf(" Date :: %s\n", printBuffer);
	
	sprintf(printBuffer, "%d%d:%d%d:%d%d",
			myRTC.rtcData.hourH,
			myRTC.rtcData.hourL,
			myRTC.rtcData.minH,
			myRTC.rtcData.minL,
			myRTC.rtcData.secH,
			myRTC.rtcData.secL
			);
	printf(" Time :: %s\n", printBuffer);
}
// === EOF ===
