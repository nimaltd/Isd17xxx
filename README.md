# ISD17xxx series library
<br />
I hope use it and enjoy.
<br />
I use Stm32f103C8 and Keil Compiler and Stm32CubeMX wizard.
 <br />
Please Do This ...
<br />
<br />
1) you can Enable FreeRTOS or not.
<br />
2) Config your spi and CS pin on CubeMX.
<br />
3) SPI must be LSB first.
<br />
4) Select "General peripheral Initalizion as a pair of '.c/.h' file per peripheral" on project settings.
<br />
5) Config your ISD17XX.h file.
<br />
6) call  Isd17xx_Init(void) on your app.
<br />
8) use Isd17xx_HighLevelRecord() , Isd17xx_HighLevelPlay() , Isd17xx_HighLevelErase() on your app.



