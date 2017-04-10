#ifndef	_ISD17XXCONFIG_H
#define	_ISD17XXCONFIG_H


#define		_ISD17XX_USE_FREERTOS			1
#define		_ISD17XX_SPI							hspi1
#define		_ISD17XX_CS_PORT					SPI1_CS_GPIO_Port
#define		_ISD17XX_CS_PIN						SPI1_CS_Pin



//#########################################################################################
#if			(_ISD17XX_USE_FREERTOS==0)
	#define	_ISD17XX_DELAY(x)							HAL_Delay(x)
	#define	_ISD17XX_LOCK()								{while(IsdLock==true);IsdLock=true;}
	#define	_ISD17XX_RELASE()							IsdLock=false;	
#else
	#include "cmsis_os.h"
	#define	_ISD17XX_DELAY(x)							osDelay(x)
	#define	_ISD17XX_LOCK()								osSemaphoreWait(Isd17xxSemHandle,osWaitForever)
	#define	_ISD17XX_RELASE()							osSemaphoreRelease(Isd17xxSemHandle)
#endif

#endif
