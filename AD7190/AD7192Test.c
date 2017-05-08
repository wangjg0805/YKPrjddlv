/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   AD7192Test.c

 Description :	 Use the GPIO to simulate the SPI communication of AD7192

 Hardware plateform : 	ADuC7026 and AD7190/92EBZ
********************************************************************************/

#include "ADuC7026.h"
#include "ADuC7026Driver.h"
#include "AD7192.h"
#include "stdio.h"

int main(void)
{
	// Add the test functions here
	int i;
	float AD7192Temperature = 0.0;
	int NumberOfDataToContinuousRead = 10;
	int NumberOfDataToContinuousConvertion = 10;

    ADuC7026Initialization();   
	
	AD7192Initialization(); 
//	AD7192SoftwareReset();

	ReadFromAD7192ViaSPI(REG_COM_STA, 8, AD7192Registers, REG_COM_STA);
	for(i=0; i < 8; i++)
	printf("AD7192Register[%d] = 0x%06X \r\n", i+REG_COM_STA , AD7192Registers[i+REG_COM_STA]);

	AD7192InternalZeroScaleCalibration();
	AD7192InternalFullScaleCalibration();
//	AD7192ExternalZeroScaleCalibration();		//Connect manually
//	AD7192ExternalFullScaleCalibration();  		//Connect manually
	AD7192Registers[REG_MODE] = 0;
	AD7192Registers[REG_CONF] = 0;
	AD7192Registers[REG_MODE] = MODE_CONT|DAT_STA_DIS|INCLK_MCLK2EN|SINC_4|ENPAR_EN|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_DIS|0x080;		//Output Rate =	MCLK/1024/128 without chop
	AD7192Registers[REG_CONF] = CHOP_DIS|REF_IN2|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1;	// Gain = 1
	printf("\r\nWrite 0x0C2080  0x100100 to AD7192 Mode Register and Configuration Register and read back.\r\n");
//	AD7192Registers[REG_MODE] = MODE_CONT|DAT_STA_DIS|INCLK_MCLK2EN|SINC_4|ENPAR_EN|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_DIS|0x001;		//Output Rate =	MCLK/1024/1/4 with chop
//	AD7192Registers[REG_CONF] = CHOP_EN|REF_IN2|AIN1_COM|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1;	//Enable chop and Gain = 1
//	printf("\r\nWrite 0x0C2001  0x901000 to AD7192 Mode Register and Configuration Register and read back.\r\n");

	WriteToAD7192ViaSPI(REG_MODE, 2, AD7192Registers, REG_MODE);

	AD7192Registers[REG_MODE] = 0;
	AD7192Registers[REG_CONF] = 0;
	ReadFromAD7192ViaSPI(REG_MODE, 2, AD7192Registers, REG_MODE);
	printf("Mode Register=0x%06X \r\nConfiguration Register=0x%06X \r\n", AD7192Registers[REG_MODE], AD7192Registers[REG_CONF]);

	AD7192StartContinuousRead();
	for(i=0; i < NumberOfDataToContinuousRead; i++)
	{
		AD7192Data = AD7192ContinuousRead();
		printf("%d CReadData = 0x%06X \r\n", i+1 , AD7192Data);
	//	vin = AD7192Data*5/8388608.00000000-5.00000000;
	//	printf("%d input voltage = %2.8f V\r\n", i+1 , vin);
	}
	AD7192ExitContinuousRead();		

	AD7192StartSingleConvertion(AIN1_COM);

	AD7192Data = AD7192ReadConvertingData();
	printf("\r\nAIN1_COM = 0x%08X \r\n", AD7192Data);
	ReadFromAD7192ViaSPI(REG_DATA, 1, AD7192Registers, REG_DATA);
	printf("\r\nAIN1_COM = 0x%08X \r\n", AD7192Registers[REG_DATA]);
//	AD7192SoftwareReset();
	AD7192StartContinuousConvertion(AIN1_AIN2 | AIN2_COM);
	for(i=0; i < NumberOfDataToContinuousConvertion; i++)
	{
		AD7192Registers[REG_DATA] = 0;	
		AD7192Data = AD7192ReadConvertingData();
		printf("ReadData = 0x%08X \r\n", AD7192Data);
	}

	AD7192Data = AD7192ReadTemperature();
//	printf("\r\nTemperatureData = 0x%06X \r\n", AD7192Data);
	AD7192Temperature = RealTemperature(AD7192Data);
	printf("\r\nTemperature = %2.1f C\r\n", AD7192Temperature);

	while(1) {;}

}

