
#include "../../include/ds1307/ds1307.hpp"


/*! 
	* @brief Constructor for the RTCDS1307 class
	* @param address I2C address of the DS1307 RTC
	* @param i2c_type Pointer to the I2C instance
	* @param data Data pin for software I2C
	* @param clock Clock pin for software I2C
	* @param CLKspeed I2C clock speed in kHz
*/
RTCDS1307::RTCDS1307(uint8_t address, i2c_inst_t* i2c_type, uint8_t data, uint8_t  clock, uint16_t CLKspeed) {
	_address = address;
	_i2c = i2c_type; 
	_DATA_IO = data;
	_CLOCK_IO = clock;
	_CLKSpeed = CLKspeed;
}

/*! 
	@brief Initializes the I2C communication 
	@return True for sucess 
*/
bool RTCDS1307::InitI2C(void) {
	//init I2C
	gpio_set_function(_DATA_IO, GPIO_FUNC_I2C);
	gpio_set_function(_CLOCK_IO, GPIO_FUNC_I2C);
	gpio_pull_up(_DATA_IO);
	gpio_pull_up(_CLOCK_IO);
	int returnBaudValue = i2c_init(_i2c, _CLKSpeed * 1000);
	if (returnBaudValue == 	(_CLKSpeed * 1000))
		return true;
	else
		return false;
}

/*! 
	@brief Deinitializes the I2C communication 
*/
void RTCDS1307::DeInitI2C()
{
	gpio_set_function(_DATA_IO, GPIO_FUNC_NULL);
	gpio_set_function(_CLOCK_IO, GPIO_FUNC_NULL);
	i2c_deinit(_i2c); 	
}

/*! 
	@brief Writes the default time to the DS1307 RTC 
	@details Hardcode set RTC time date to 20-07-09-Fri-18-56-00
		mostly used for testing and debugging  ... mostly
	@return Integer status of the write operation, less than 1 = error
*/
int RTCDS1307::writeClockDefault() {

	//20-07-09-Fri-18-56-00
	rtcData.secH = 0;
	rtcData.secH = 0;
	rtcData.minL = 6;
	rtcData.minH = 5;
	rtcData.hourL = 8;
	rtcData.hourH = 1;
	rtcData.day = 5;
	rtcData.dateL = 9;
	rtcData.dateH = 0;
	rtcData.monthL = 7;
	rtcData.monthH = 0;
	rtcData.yearL = 0;
	rtcData.yearH = 2;

	memcpy(rtcBuff, &rtcData, sizeof (rtcData));
	uint8_t data_I2C[8];
	data_I2C[0] = 0x00;
	data_I2C[1] = rtcBuff[0];
	data_I2C[2] = rtcBuff[1];
	data_I2C[3] = rtcBuff[2];
	data_I2C[4] = rtcBuff[3];
	data_I2C[5] = rtcBuff[4];
	data_I2C[6] = rtcBuff[5];
	data_I2C[7] = rtcBuff[6];

	_returnValue = i2c_write_timeout_us(_i2c, _address, data_I2C, sizeof(data_I2C) ,false, DS1307_I2C_Timeout);
	if (_returnValue < 1 )
	{
		printf("Error : Fail to write : I2Bus %i\n", _returnValue );
	}
	return _returnValue;
}

/*! 
	* @brief Writes a custom time to the DS1307 RTC
	* @param str Pointer to a string representing the time ,
				time date to write as char array YYMMDDWHHMM where W = day of the week
	* @return Integer status of the write operation, less than 1 = error
	*/
int RTCDS1307::writeClock(char *str) {
	// Check for null pointer
	if (str == nullptr) 
	{
		printf("Error: String is a null pointer.\n");
		return 0;
	}

	// Check for correct length (11 characters)
	if (strlen(str) != 11) 
	{
		printf("Error: Invalid time format. Expected 11 characters.\n");
		return 0;
	}

		// Check if all characters are digits (0-9)
	for (int i = 0; i < 11; i++) 
	{
		if (str[i] < '0' || str[i] > '9') 
		{
			printf("Error: Invalid character '%c' at position %d. Expected numeric digits only.\n", str[i], i);
			return 0;
		}
	}

 	rtcData.secH = 0;
	rtcData.secH = 0;
	rtcData.minL = str[10];
	rtcData.minH = str[9];
	rtcData.hourL = str[8];
	rtcData.hourH = str[7];
	rtcData.day = str[6];
	rtcData.dateL = str[5];
	rtcData.dateH = str[4];
	rtcData.monthL = str[3];
	rtcData.monthH = str[2];
	rtcData.yearL = str[1];
	rtcData.yearH = str[0];

	memcpy(rtcBuff, &rtcData, sizeof (rtcData));
	uint8_t data_I2C[8];
	data_I2C[0] = 0x00;
	data_I2C[1] = rtcBuff[0];
	data_I2C[2] = rtcBuff[1];
	data_I2C[3] = rtcBuff[2];
	data_I2C[4] = rtcBuff[3];
	data_I2C[5] = rtcBuff[4];
	data_I2C[6] = rtcBuff[5];
	data_I2C[7] = rtcBuff[6];
	_returnValue = i2c_write_timeout_us(_i2c, _address, data_I2C, sizeof(data_I2C) ,false, DS1307_I2C_Timeout);
	if (_returnValue < 1 )
	{
		printf("Error : Fail to write : I2Bus %i\n", _returnValue );
	}
	return _returnValue;
}


/*! 
	@brief Reads the current time from the DS1307 RTC
	@details updates rtcdata
	@returns _returnValue, less than 1 = error, of number of bytes  
*/
int RTCDS1307::readClock(void) {
	uint8_t data_I2C[1];
	data_I2C[0] = Ds1307SecondRegAddress;
	_returnValue = i2c_write_timeout_us(_i2c, _address, data_I2C, sizeof(data_I2C) ,false, DS1307_I2C_Timeout);
	if (_returnValue < 1 )
	{
		printf("Error : Fail to write : I2Bus %i\n", _returnValue );
		return _returnValue;
	}

	_returnValue = i2c_read_timeout_us(_i2c, _address, rtcBuff, sizeof(rtcBuff) ,false, DS1307_I2C_Timeout );
	if (_returnValue < 1) {
		printf("Error : Fail to Read : I2Bus %i\n", _returnValue );
		return _returnValue;
	}

	memcpy(&rtcData, rtcBuff, sizeof (rtcData));
	return _returnValue;
}

/*! 
	@brief Checks if the DS1307 clock is present on the I2C bus 
	@return true for present 
*/
bool RTCDS1307::IsPresentClock(void) {
	uint8_t data_TX_I2C[2];
	uint8_t data_RX_I2C[1];
	data_TX_I2C[0] = Ds1307ControlRegAddress;
	data_TX_I2C[1] = Ds1307SecondRegAddress;

	_returnValue = i2c_write_timeout_us(_i2c, _address, data_TX_I2C, sizeof(data_TX_I2C) ,false, DS1307_I2C_Timeout);
	if (_returnValue < 1 )
	{
		printf("Error : Fail to write : I2Bus %i\n", _returnValue );
		return false;
	}

	_returnValue = i2c_read_timeout_us(_i2c, _address, data_RX_I2C, sizeof(data_RX_I2C) ,false, DS1307_I2C_Timeout );
	if (_returnValue < 1) {
		printf("Error : Fail to read : I2Bus %i\n", _returnValue );
		return false;
	}
	return true;
}