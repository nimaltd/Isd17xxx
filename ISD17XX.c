
#include "ISD17XX.h"
#include "ISD17XXConfig.h"
#include "spi.h"
#include <string.h>


Isd17xx_t			isd17xx;
#if			(_ISD17XX_USE_FREERTOS==0)
bool	IsdLock=false;
#else
osSemaphoreId Isd17xxSemHandle;
#endif
//##########################################################################################################
void	Isd17xx_Transfer(uint8_t	*DataIn,uint8_t	*DataOut,uint8_t len)
{
	HAL_GPIO_WritePin(_ISD17XX_CS_PORT,_ISD17XX_CS_PIN,GPIO_PIN_RESET);
	_ISD17XX_DELAY(1);	
	HAL_SPI_TransmitReceive(&_ISD17XX_SPI,DataIn,DataOut,len,100);	
	HAL_GPIO_WritePin(_ISD17XX_CS_PORT,_ISD17XX_CS_PIN,GPIO_PIN_SET);
	_ISD17XX_DELAY(1);
}
//##########################################################################################################
//##########################################################################################################
//##########################################################################################################
void	Isd17xx_Init(void)
{
	HAL_GPIO_WritePin(_ISD17XX_CS_PORT,_ISD17XX_CS_PIN,GPIO_PIN_SET);
	#if	(_ISD17XX_USE_FREERTOS==1)
	osSemaphoreDef(Isd17xxSemHandle);
	Isd17xxSemHandle = osSemaphoreCreate(osSemaphore(Isd17xxSemHandle), 1);
	#endif
	_ISD17XX_DELAY(100);
	Isd17xx_Reset();
	_ISD17XX_DELAY(100);
	Isd17xx_PowerUp();
	_ISD17XX_DELAY(100);
	Isd17xx_ReadDeviceID();
	Isd17xx_ReadAnalogConfigurations();
	Isd17xx_SetEOM(true);	
}
//##########################################################################################################
void	Isd17xx_PowerUp(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x01;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_Stop(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x02;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_Reset(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x03;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_ClearInterrupt(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x04;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_ReadStatus(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x05;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[2]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	memcpy(&isd17xx.StatusRegister0,isd17xx.ReceiveData,2);	
	memcpy(&isd17xx.StatusRegister1,&isd17xx.ReceiveData[2],1);	
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_PowerDown(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x07;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_ReadDeviceID(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x09;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[2]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	memcpy(&isd17xx.DeviceID,&isd17xx.ReceiveData[2],1);
	isd17xx.DeviceID&=0xF8;
	switch(isd17xx.DeviceID)
	{
		case ISD1730:
			isd17xx.EndOfAddress = 0x00FF;
		break;
		case ISD1740:
			isd17xx.EndOfAddress = 0x014F;
		break;
		case ISD1750:
			isd17xx.EndOfAddress = 0x019F;
		break;
		case ISD1760:
			isd17xx.EndOfAddress = 0x01EF;
		break;
		case ISD1790:
			isd17xx.EndOfAddress = 0x02DF;
		break;
		case ISD17120:
			isd17xx.EndOfAddress = 0x03CF;
		break;
		case ISD17150:
			isd17xx.EndOfAddress = 0x04BF;
		break;
		case ISD17180:
			isd17xx.EndOfAddress = 0x05AF;
		break;
		case ISD17210:
			isd17xx.EndOfAddress = 0x069F;
		break;
		case ISD17240:
			isd17xx.EndOfAddress = 0x078F;
		break;		
	}
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_Play(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x40;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_Record(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x41;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_Erase(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x42;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_EraseAll(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x43;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_Forward(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x48;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_CheckMemory(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x49;
	isd17xx.SendData[1]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_ReadPlayPointer(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x06;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[2]=0x00;
	isd17xx.SendData[3]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,4);
	memcpy(&isd17xx.PlayPointer,&isd17xx.ReceiveData[2],2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_ReadRecordPointer(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x08;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[2]=0x00;
	isd17xx.SendData[3]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,4);
	memcpy(&isd17xx.RecordPointer,&isd17xx.ReceiveData[2],2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_ReadAnalogConfigurations(void)
{
	_ISD17XX_LOCK();
	isd17xx.SendData[0]=0x44;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[2]=0x00;
	isd17xx.SendData[3]=0x00;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,4);
	memcpy(&isd17xx.APC_Register,&isd17xx.ReceiveData[2],2);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_SetVolume(uint8_t	Vol_0_to_7)
{
	Isd17xx_ReadAnalogConfigurations();
	_ISD17XX_LOCK();
	memcpy(&isd17xx.SendData[1],&isd17xx.APC_Register,2);
	isd17xx.SendData[0]=0x65;
	if(Vol_0_to_7>7)
		Vol_0_to_7=7;
	Vol_0_to_7 = ~Vol_0_to_7;
	Vol_0_to_7 &= 0x07;
	isd17xx.SendData[1] &= 0xF8; 
	isd17xx.SendData[1] |= Vol_0_to_7;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	_ISD17XX_RELASE();
	Isd17xx_ReadAnalogConfigurations();	
}
//##########################################################################################################
void	Isd17xx_SetMonitorInput(bool	EnableDisable)
{
	Isd17xx_ReadAnalogConfigurations();
	_ISD17XX_LOCK();
	memcpy(&isd17xx.SendData[1],&isd17xx.APC_Register,2);
	isd17xx.SendData[0]=0x65;
	if(EnableDisable==false)
		isd17xx.SendData[1] &= 0xF7; 
	else
		isd17xx.SendData[1] |= 0x08; 
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	_ISD17XX_RELASE();
	Isd17xx_ReadAnalogConfigurations();
}
//##########################################################################################################
void	Isd17xx_SetAnalogInput(bool	MicEnable,bool AnalogInEnable)
{	
	Isd17xx_ReadAnalogConfigurations();
	_ISD17XX_LOCK();
	memcpy(&isd17xx.SendData[1],&isd17xx.APC_Register,2);
	isd17xx.SendData[0]=0x65;
	isd17xx.SendData[1] &= 0xAF;
	if((MicEnable==true) && (AnalogInEnable==false))
		isd17xx.SendData[1] |= 0x40; 
	if((MicEnable==false) && (AnalogInEnable==true))
		__NOP(); 
	if((MicEnable==true) && (AnalogInEnable==true))
		isd17xx.SendData[1] |= 0x10; 	
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	_ISD17XX_RELASE();
	Isd17xx_ReadAnalogConfigurations();
}
//##########################################################################################################
void	Isd17xx_SetAnalogOutputAUX(void)
{
	Isd17xx_ReadAnalogConfigurations();
	_ISD17XX_LOCK();
	memcpy(&isd17xx.SendData[1],&isd17xx.APC_Register,2);
	isd17xx.SendData[0]=0x65;
	isd17xx.SendData[1] |= 0x80;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	_ISD17XX_RELASE();
	Isd17xx_ReadAnalogConfigurations();
}
//##########################################################################################################
void	Isd17xx_SetAnalogOutputAUD(void)
{
	Isd17xx_ReadAnalogConfigurations();
	_ISD17XX_LOCK();
	memcpy(&isd17xx.SendData[1],&isd17xx.APC_Register,2);
	isd17xx.SendData[0]=0x65;
	isd17xx.SendData[1] &= (~0x80);
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	_ISD17XX_RELASE();
	Isd17xx_ReadAnalogConfigurations();
}
//##########################################################################################################
void	Isd17xx_SetPwmSpeaker(bool	EnableDisable)
{
	Isd17xx_ReadAnalogConfigurations();
	_ISD17XX_LOCK();
	memcpy(&isd17xx.SendData[1],&isd17xx.APC_Register,2);
	isd17xx.SendData[0]=0x65;
	if(EnableDisable==false)
		isd17xx.SendData[2] |= 0x01;
	else
		isd17xx.SendData[2] &= 0xFE;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	_ISD17XX_RELASE();
	Isd17xx_ReadAnalogConfigurations();
}
//##########################################################################################################
void	Isd17xx_SetEOM(bool	EnableDisable)
{
	Isd17xx_ReadAnalogConfigurations();
	_ISD17XX_LOCK();
	memcpy(&isd17xx.SendData[1],&isd17xx.APC_Register,2);
	isd17xx.SendData[0]=0x65;
	if(EnableDisable==false)
		isd17xx.SendData[2] &= 0xF7;
	else
		isd17xx.SendData[2] |= 0x08;
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,3);
	_ISD17XX_RELASE();
	Isd17xx_ReadAnalogConfigurations();
}
//##########################################################################################################
void	Isd17xx_PlayAddress(uint16_t	StartFrom,uint16_t	EndTo)
{
	_ISD17XX_LOCK();
	if(StartFrom < 0x10)
		StartFrom=0x10;
	if(EndTo > isd17xx.EndOfAddress)
		EndTo = isd17xx.EndOfAddress;
	isd17xx.SendData[0]=0x80;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[6]=0x00;
	memcpy(&isd17xx.SendData[2],&StartFrom,2);
	memcpy(&isd17xx.SendData[4],&EndTo,2);
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,7);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_RecordAddress(uint16_t	StartFrom,uint16_t	EndTo)
{
	_ISD17XX_LOCK();
	if(StartFrom < 0x10)
		StartFrom=0x10;
	if(EndTo > isd17xx.EndOfAddress)
		EndTo = isd17xx.EndOfAddress;
	isd17xx.SendData[0]=0x81;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[6]=0x00;
	memcpy(&isd17xx.SendData[2],&StartFrom,2);
	memcpy(&isd17xx.SendData[4],&EndTo,2);
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,7);
	_ISD17XX_RELASE();
}
//##########################################################################################################
void	Isd17xx_EraseAddress(uint16_t	StartFrom,uint16_t	EndTo)
{
	_ISD17XX_LOCK();
	if(StartFrom < 0x10)
		StartFrom=0x10;
	if(EndTo > isd17xx.EndOfAddress)
		EndTo = isd17xx.EndOfAddress;
	isd17xx.SendData[0]=0x82;
	isd17xx.SendData[1]=0x00;
	isd17xx.SendData[6]=0x00;
	memcpy(&isd17xx.SendData[2],&StartFrom,2);
	memcpy(&isd17xx.SendData[4],&EndTo,2);
	Isd17xx_Transfer(isd17xx.SendData,isd17xx.ReceiveData,7);
	_ISD17XX_RELASE();
}
//##########################################################################################################
//##########################################################################################################
//##########################################################################################################
void	Isd17xx_HighLevelRecord(uint16_t	StartFrom,uint16_t	EndTo)
{
	Isd17xx_Stop();
	_ISD17XX_DELAY(10);
	Isd17xx_RecordAddress(StartFrom,EndTo);
	Isd17xx_ReadStatus();
	if(isd17xx.StatusRegister0.CMD_Err==1)
	{
		Isd17xx_CheckMemory();
		do
		{
			_ISD17XX_DELAY(50);		
			Isd17xx_ReadStatus();						
		}
		while(isd17xx.StatusRegister0.CMD_Err==1);		
	}
	do
	{
		_ISD17XX_DELAY(50);		
		Isd17xx_ReadStatus();
		
	}
	while(isd17xx.StatusRegister1.Record==1);	
}
//##########################################################################################################
void	Isd17xx_HighLevelPlay(uint16_t	StartFrom,uint16_t	EndTo)
{
	Isd17xx_Stop();
	_ISD17XX_DELAY(10);
	Isd17xx_PlayAddress(StartFrom,EndTo);
	Isd17xx_ReadStatus();
	if(isd17xx.StatusRegister0.CMD_Err==1)
	{
		Isd17xx_CheckMemory();
		do
		{
			_ISD17XX_DELAY(50);		
			Isd17xx_ReadStatus();						
		}
		while(isd17xx.StatusRegister0.CMD_Err==1);		
	}
	do
	{
		_ISD17XX_DELAY(50);		
		Isd17xx_ReadStatus();
		
	}
	while(isd17xx.StatusRegister1.Play==1);	
}
//##########################################################################################################
void	Isd17xx_HighLevelErase(uint16_t	StartFrom,uint16_t	EndTo)
{
	Isd17xx_Stop();
	_ISD17XX_DELAY(10);
	Isd17xx_EraseAddress(StartFrom,EndTo);
	Isd17xx_ReadStatus();
	if(isd17xx.StatusRegister0.CMD_Err==1)
	{
		Isd17xx_CheckMemory();
		do
		{
			_ISD17XX_DELAY(50);		
			Isd17xx_ReadStatus();						
		}
		while(isd17xx.StatusRegister0.CMD_Err==1);		
	}
	do
	{
		_ISD17XX_DELAY(50);		
		Isd17xx_ReadStatus();
		
	}
	while(isd17xx.StatusRegister1.Erase==1);	
}
//##########################################################################################################
void	Isd17xx_HighLevelStop(void)
{
	Isd17xx_Stop();
	_ISD17XX_DELAY(10);
	Isd17xx_ReadStatus();
	do
	{
		_ISD17XX_DELAY(50);		
		Isd17xx_ReadStatus();		
	}
	while((isd17xx.StatusRegister1.Erase==1) || (isd17xx.StatusRegister1.Play==1) || (isd17xx.StatusRegister1.Record==1));	
}

