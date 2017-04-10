#ifndef	_ISD17XX_H
#define	_ISD17XX_H

#include <stdint.h>
#include <stdbool.h>
//########################################################################################
typedef	struct
{
	uint8_t		CMD_Err					:		1;
	uint8_t		Memory_Full			:		1;
	uint8_t		Power_Up				:		1;
	uint8_t		EOM							:		1;
	uint8_t		Interrupt				:		1;
	uint16_t	Address					:		11;	
}Isd17xx_StatusRegister0_t;
//////////////////
typedef	struct
{
	uint8_t		Ready						:		1;
	uint8_t		Erase						:		1;
	uint8_t		Play						:		1;
	uint8_t		Record					:		1;
	uint8_t		SE1							:		1;
	uint8_t		SE2							:		1;
	uint8_t		SE3							:		1;
	uint8_t		SE4							:		1;
}Isd17xx_StatusRegister1_t;
//////////////////
typedef	enum
{
	ISD1730				=			0x80,
	ISD1740				=			0xB0,
	ISD1750				=			0xA1,
	ISD1760				=			0xA0,
	ISD1790				=			0xD0,
	ISD17120			=			0xC1,
	ISD17150			=			0xC0,
	ISD17180			=			0xF0,
	ISD17210			=			0xE1,
	ISD17240			=			0xE0,
}Isd17xx_DeviceID_t;
//////////////////
typedef	struct
{
	uint8_t		VOL0						:		1;			
	uint8_t		VOL1						:		1;
	uint8_t		VOL2						:		1;
	uint8_t		MonitorInput		:		1;
	uint8_t		MixInput				:		1;
	uint8_t		SE_Editing			:		1;
	uint8_t		SPI_Ft					:		1;
	uint8_t		AnalogOutput		:		1;
	uint8_t		PWM_SP					:		1;
	uint8_t		PowerUpAnalog		:		1;
	uint8_t		vAlert					:		1;
	uint8_t		EOM_Enable			:		1;	
}Isd17xx_APC_Register_t;
//////////////////
typedef	struct
{
	uint8_t	SendData[8];
	uint8_t	ReceiveData[8];	
	Isd17xx_StatusRegister0_t	StatusRegister0;	
	Isd17xx_StatusRegister1_t	StatusRegister1;
	Isd17xx_DeviceID_t	DeviceID;
	uint16_t	EndOfAddress;
	uint16_t	PlayPointer;
	uint16_t	RecordPointer;
	Isd17xx_APC_Register_t	APC_Register;
}Isd17xx_t;
extern Isd17xx_t	isd17xx;
//########################################################################################
void		Isd17xx_Init(void);
void		Isd17xx_PowerUp(void);
void		Isd17xx_Stop(void);
void		Isd17xx_Reset(void);
void		Isd17xx_ClearInterrupt(void);
void		Isd17xx_ReadStatus(void);
void		Isd17xx_ReadDeviceID(void);
void		Isd17xx_Play(void);
void		Isd17xx_Record(void);
void		Isd17xx_Erase(void);
void		Isd17xx_EraseAll(void);
void		Isd17xx_Forward(void);
void		Isd17xx_CheckMemory(void);
void		Isd17xx_ReadPlayPointer(void);
void		Isd17xx_ReadRecordPointer(void);
void		Isd17xx_ReadAnalogConfigurations(void);
void		Isd17xx_SetVolume(uint8_t	Vol_0_to_7);
void		Isd17xx_SetMonitorInput(bool	EnableDisable);
void		Isd17xx_SetAnalogInput(bool	MicEnable,bool AnalogInEnable);
void		Isd17xx_SetAnalogOutputAUX(void);
void		Isd17xx_SetAnalogOutputAUD(void);
void		Isd17xx_SetPwmSpeaker(bool	EnableDisable);
void		Isd17xx_SetEOM(bool	EnableDisable);
void		Isd17xx_PlayAddress(uint16_t	StartFrom,uint16_t	EndTo);
void		Isd17xx_RecordAddress(uint16_t	StartFrom,uint16_t	EndTo);
void		Isd17xx_EraseAddress(uint16_t	StartFrom,uint16_t	EndTo);
//########################################################################################
void		Isd17xx_HighLevelRecord(uint16_t	StartFrom,uint16_t	EndTo);
void		Isd17xx_HighLevelPlay(uint16_t	StartFrom,uint16_t	EndTo);
void		Isd17xx_HighLevelErase(uint16_t	StartFrom,uint16_t	EndTo);
//########################################################################################
#endif
