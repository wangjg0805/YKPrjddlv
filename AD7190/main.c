/*-----------------------ADI AD7190 Driver Test Code-----------------------------------

Author: 		ADI CAST (China Application Support Team)
Date:			2009-02-20
Rev:			V1.0
Description:	Realize AD7190 Driver£¬Use ADuC7026 as MCU£¬Development Tool: KEIL C
				Test program by reading and writing register of AD7190

---------------------------------------------------------------------------------------------------------*/
#include <ADuC7026.h>
#include "AD7190_IO.h"

unsigned	char	buf[4] = {0,0,0,0};

void putchar(unsigned char ch)/* Write character to Serial Port  */  
{          
	COMTX = ch;				 //COMTX is an 8-bit transmit register.
    while(!(0x020==(COMSTA0 & 0x020)))
    {;}
}

void ADuC7026_Initiate(void)
{
    //Clock Initial
    POWKEY1 = 0x01;				//Start PLL Setting
    POWCON = 0x00;				//Set PLL Active Mode With CD = 0  CPU CLOCK DIVIDER = 41.78MHz
    POWKEY2 = 0xF4;				//Finish PLL Setting

	GP1CON = 0x011;				  //PIN set up for UART
     
	GP0DAT = GP0DAT & 0xDFFFFFFF; //Configure the P0.5 pin as input for DOUT of AD7190

	GP4DAT = GP4DAT | 0x38380000; //Configure the P4.3 pin as output for CS of AD7190, CS Stall High
								  //Configure the P4.4 pin as output for SCLK of AD7190, SCLK Stall High
								  //Configure the P4.5 pin as output for DIN of AD7190
   	//UART Initial£¬Baud Rate = 9600								  	
	COMCON0 = 0x080;  
	COMDIV0 = 0x088;    		
	COMDIV1 = 0x000;
	COMCON0 = 0x007; 

}  
void main (void)
{ 
    ADuC7026_Initiate();

	buf[0] = 0x10;
	WriteToAD7190(1,buf);	  	//write communication register 0x10 to control the progress to write configuration register 

	buf[2] = 0x00;
	buf[1] = 0x01;
	buf[0] = 0x08;
	WriteToAD7190(3,buf);		//write configuration register,unipolar operation,gain=1,channel:AIN1 to AIN2

	buf[0] = 0x08;
	WriteToAD7190(1,buf);		//write communication register 0x08 to control the progress to write mode register 

	buf[2] = 0x08;
	buf[1] = 0x03;
	buf[0] = 0xFF;
	WriteToAD7190(3,buf);		//write mode register,internal 4.92MHz clock,output data rate=4.7Hz

 	buf[0] = 0x50;
	WriteToAD7190(1,buf);		//write communication register 0x50 to control the progress to read configuration register
	ReadFromAD7190(3,buf);		//read configuration register
	putchar(buf[2]);
	putchar(buf[1]);
	putchar(buf[0]);

 	buf[0] = 0x58;
	WriteToAD7190(1,buf);		//write communication register 0x58 to control the progress to read data register
	ReadFromAD7190(3,buf);		//read data register
	putchar(buf[2]);
	putchar(buf[1]);
	putchar(buf[0]);

	while(1)
    {;}

}


