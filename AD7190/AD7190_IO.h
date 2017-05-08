#ifndef  AD7190_IO_H
#define  AD7190_IO_H
#endif
///////////////////////////////////////ADDED
/*
**************************************************************************************************************
*                                           PIN ASSIGNMENT
**************************************************************************************************************
*/

#define AD7190_SYNC         (1<<0)            /* P1.0    */

#define AD7190_CS           (1<<11)           /* P0.11    */

#define AD7190_DIN          (1<<10)           /* P0.10    */

#define AD7190_CLK          (1<<15)           /* P2.15    */
#define AD7190_DOUT         (1<<12)           /* P2.12 use int2 */  


#define AD7190_CS_L                FIO0CLR=AD7190_CS
#define AD7190_CS_H                FIO0SET=AD7190_CS

#define AD7190_DIN_L               FIO0CLR=AD7190_DIN
#define AD7190_DIN_H               FIO0SET=AD7190_DIN

#define AD7190_CLK_L               FIO2CLR=AD7190_CLK
#define AD7190_CLK_H               FIO2SET=AD7190_CLK

///////////////////DOUT AS INT INPUT
#define AD7190_DOUT_INT_MODE       PINSEL4&=0xfcffffff;PINSEL4|=0x01000000
#define AD7190_DOUT_DAT_MODE       PINSEL4&=0xfcffffff;FIO2DIR&=0xffffefff

//////////////////////////////////////////////ADD OVER 

//Register Map and bit definition

//Communications Register
#define	WEN			 0x00
#define WEN_DIS			 0x80
#define RW_W		 	 0x00
#define	RW_R			 0x40
	#define REG_COM_STA	 0x00
	#define	REG_MODE	 0x01
	#define	REG_CONF	 0x02
	#define	REG_DATA	 0x03
	#define	REG_ID		 0x04
	#define	REG_GPOCON	 0x05
	#define	REG_OFFSET	 0x06
	#define	REG_FS		 0x07
#define	CREAD_EN		 0x04
#define	CREAD_DIS		 0x00

//Status Register
#define	RDY_H			0x80
#define	RDY_L			0x00
#define	ERR_H			0x40
#define	ERR_L			0x00
#define	NOREF_H			0x20
#define	NOREF_L			0x00
#define	PARITY_H		0x10
#define	PARITY_L		0x00
#define	CHDST_AIN1_AIN2	        0x00
#define	CHDST_AIN3_AIN4		0x01
#define	CHDST_TEMP		0x02
#define	CHDST_AIN2_AIN2		0x03
#define	CHDST_AIN1_COM		0x04
#define	CHDST_AIN2_COM		0x05
#define	CHDST_AIN3_COM		0x06
#define	CHDST_AIN4_COM		0x07

//Mode Register
#define	MODE_CONT				0x00
#define	MODE_SING				0x20
#define	MODE_IDLE				0x40
#define	MODE_PD					0x60
#define	MODE_INZCL				0x80
#define	MODE_INFCL				0xA0
#define	MODE_SYSZCL				0xC0
#define	MODE_SYSFCL				0xE0
#define	DAT_STA_EN				0x10
#define	DAT_STA_DIS				0x00	  
#define	EXT_XTAL				0x00
#define	EXT_CLK					0x04
#define	INCLK_MCLK2TRI		    0x08
#define	INCLK_MCLK2EN		    0x0C
#define	SINC_4					0x00
#define	SINC_3					0x80
#define	ENPAR_EN				0x20
#define	ENPAR_DIS				0x00
#define	CLK_DIV_2				0x10 //USE ONLY IN AD7192
#define	CLK_DIV_DIS				0x00
#define	SINGLECYCLE_EN			0x08
#define	SINGLECYCLE_DIS			0x00
#define	REJ60_EN				0x04
#define	REJ60_DIS				0x00
										
//Configuration Register			
#define CHOP_EN					0x80
#define	CHOP_DIS				0x00
#define	REF_IN1					0x00
#define	REF_IN2					0x10
#define	AIN1_AIN2				0x01
#define	AIN3_AIN4				0x02
#define	TEMP					0x04
#define	AIN2_AIN2				0x08
#define	AIN1_COM				0x10
#define	AIN2_COM				0x20
#define	AIN3_COM				0x40
#define	AIN4_COM				0x80
#define	BURN_EN					0x80
#define	BURN_DIS				0x00
#define	REFDET_EN				0x40
#define	REFDET_DIS				0x00
#define	BUF_EN					0x10
#define	BUF_DIS					0x00
#define	UB_UNI					0x08
#define UB_BI					0x00
#define	GAIN_1					0x00
#define	GAIN_8					0x03
#define	GAIN_16					0x04
#define	GAIN_32					0x05
#define	GAIN_64					0x06
#define	GAIN_128				0x07

//GPOCON Register
#define BPDSW_CLOSE				0x40
#define	BPDSW_OPEN				0x00
#define	GP32EN					0x20
#define	GP32DIS					0x00
#define	GP10EN					0x10
#define	GP10DIS					0x00
#define	P3DAT_H					0x08
#define	P3DAT_L					0x00
#define	P2DAT_H					0x04
#define	P2DAT_L					0x00
#define	P1DAT_H					0x02
#define	P1DAT_L					0x00
#define	P0DAT_H					0x01
#define	P0DAT_L					0x00

void WriteToAD7190(uint8 count,  uint8 *buf);
void ReadFromAD7190(uint8 count, uint8 *buf);
void AD7190Configuration(uint32 ch);