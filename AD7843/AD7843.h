/*
**********************************************************************************************************
* File           : ad7843.h
* Programmer(s)  : Wangjg	
***********************************************************************************************************
*/
#ifndef  _AD7843_H
#define  _AD7843_H


/*
**************************************************************************************************************
*                                           TYPE DEFINITIONS OF DATA TYPES
**************************************************************************************************************
*/

typedef  unsigned int    INT32U;
typedef  signed   int    INT32S;
typedef  unsigned short  INT16U;
typedef  signed   short  INT16S;
typedef  uint8   INT08U;
typedef  signed   char   INT08S;
typedef  unsigned  char  BOOLEAN; 
#endif

/*
**************************************************************************************************************
*                                           PIN ASSIGNMENT
**************************************************************************************************************
*/
#define IO_AD7843_SET      FIO2SET
#define IO_AD7843_CLR      FIO2CLR
#define IO_AD7843_PIN      FIO2PIN
#define IO_AD7843_DIR      FIO2DIR
#define IO_AD7843_IRQPIN   FIO0PIN
#define IO_AD7843_IRQDIR   FIO0DIR

#define AD7843_CLK      (1<<21)            /* P2.21    */
#define AD7843_CS       (1<<19)            /* P2.19    */
#define AD7843_DIN      (1<<23)            /* P2.23    */
#define AD7843_BUSY     (1<<26)            /* P2.26    */
#define AD7843_DOUT     (1<<25)            /* P2.25    */
#define AD7843_IRQ      (1<<14)            /* P0.14     */

#define BIT0            (1)
#define BIT1			(1<<1)
#define BIT2            (1<<2)
#define BIT3			(1<<3)
#define BIT4            (1<<4)
#define BIT5			(1<<5)
#define BIT6            (1<<6)
#define BIT7			(1<<7)

/*
**************************************************************************************************************
*                                         COMMAND WORD
**************************************************************************************************************
*/
//           BIT7     BIT6 BIT5 BIT4    BIT3     BIT2      BIT1  BIT0 
//         STARTBIT    A2   A1   A0     MODE    SER/DFR     PD1   PD0
#define    STARTBIT         (1<<7)
#define    MX               (5<<4)
#define    MY               (1<<4)
#define    CONVER12BIT      (0<<3)
#define    CONVER08BIT      (1<<3)
#define    SINGL_END        (1<<2)
#define    DIFF_REFE        (0<<2)
#define    PD_ENABLE         0
#define    PD_DISABLE        3

/*
**************************************************************************************************************
*                           触摸屏相关参数设置
*                           需根据不同屏设置
**************************************************************************************************************
*/

#define ACCP_VALUE      30
#define SAMPLE_TIMES    1
#define DLYCONST        50

#define    X0_DEFAULT        100
#define    Y0_DEFAULT        210
#define    XMAX_DEFAULT      3950
#define    YMAX_DEFAULT      4000

#define    XMID_DEFAULT      ((XMAX_DEFAULT-X0_DEFAULT)/2)
#define    YMID_DEFAULT      ((YMAX_DEFAULT-Y0_DEFAULT)/2)

#define    X0_MIN    10
#define    Y0_MIN    10
#define    X0_MAX    500
#define    Y0_MAX    500

#define    XMAX_MAX  4095   //USE 12BIT AD
#define    YMAX_MAX  4095
#define    XMAX_MIN  (XMAX_MAX-1000)
#define    YMAX_MIN  (YMAX_MAX-1000)

/*
**************************************************************************************************************
*                                           FUNCTION DECLARATION
**************************************************************************************************************
*/
														  //屏幕校准修改此参数
extern  INT32U      x0,y0,xmid,ymid,xmax,ymax;                      //用户原点和最大点设置
/*
**************************************************************************************************************
*                                           FUNCTION DECLARATION
**************************************************************************************************************
*/

void        Start_SPI (void); 
void        DlyMs(INT32U m); 
void        WriteCharTo7843(INT08U command); 
INT16U      ReadFromCharFrom7843(void);

BOOLEAN     Set_Originalx0y0(INT32U a,INT32U b);
BOOLEAN     Set_XmidYmid(INT32U a,INT32U b);
BOOLEAN     Set_XmaxYmax(INT32U c,INT32U d);
BOOLEAN     Get_PositionAD(INT32U *PositionAD);
INT32U      Get_PositionXY(void);

//////////////////////////////END OF THE FILE

