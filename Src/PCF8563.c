/*
 * PCF8563.c
 *
 *  Created on: Apr 23, 2023
 *      Author: SpiritBoi
 *      Source: https://github.com/Bill2462/PCF8563-Arduino-Library/tree/master/src
 */

#include "PCF8563.h"
#ifdef CONFIG_USE_PCF8563

PCF8563_Handle *_pcf8563;
void PCF8563_Init(PCF8563_Handle *rtc,I2C_HandleTypeDef *hi2c){
	if(!rtc) return;
	_pcf8563 = rtc;
	if(hi2c) _pcf8563->hi2c = hi2c;
	else {if(!_pcf8563->hi2c) return;}
	_pcf8563->hi2c->Devaddress = PCF8563_address << 1;
	while(PCF8563_CHECKREADY!=HAL_OK);
}

void PCF8563_Write(uint8_t REG,uint8_t Value)
{
	uint8_t i2cTXData[2]={REG,Value};
	while(HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, i2cTXData, 2,HAL_MAX_DELAY)!=HAL_OK);
}

uint8_t PCF8563_Read(uint8_t REG)
{
	uint8_t i2cReceiveData[1];
	HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, &REG, 1,HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(PCF8563_I2C, PCF8563_I2C->Devaddress, i2cReceiveData, 1,HAL_MAX_DELAY);
	return i2cReceiveData[0];
}


void PCF8563_WriteTimeRegisters(PCF8563_Time *t)
{
	uint8_t i2cTXData[2]={REG,Value};
	while(HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, i2cTXData, 2,HAL_MAX_DELAY)!=HAL_OK);

}
uint8_t PCF8563_ReadTimeRegisters()
{
	uint8_t i2cReceiveData[8];
	uint8_t txData[1]={0x02};
	HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, txData, 1,HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(PCF8563_I2C, PCF8563_I2C->Devaddress, i2cReceiveData, 7,HAL_MAX_DELAY);
	pcfSecond = (BCD_Decode(i2cReceiveData[0]&0x7f));
	pcfMinute = (BCD_Decode(i2cReceiveData[1]&0x7f));
	pcfHour = (BCD_Decode(i2cReceiveData[2]&0x3f));
	pcfDay = (BCD_Decode(i2cReceiveData[3]&0x3f));
	pcfWeekday =(BCD_Decode(i2cReceiveData[4]&0x07));
	pcfMonth = (BCD_Decode(i2cReceiveData[5]&0x3f));
	pcfYear = (BCD_Decode(i2cReceiveData[6]&0x3f));
	__NOP();
	return 0;
}


uint8_t BCD_Decode(uint8_t BCD_value)
{
  uint8_t output;
  output = ((BCD_value&0xF0)>>4)*10;
  output += (BCD_value & 0x0F);
  return output;
}

uint8_t BCD_Encode(uint8_t Value)
{
  uint8_t output;
  output = ((Value/10) << 4);
  output |= (Value%10);
  return output;
}
#endif

