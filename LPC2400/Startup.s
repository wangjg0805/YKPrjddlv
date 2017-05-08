;/****************************************Copyright (c)***************************************************
;**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
;**                                     
;**                               http://www.embedtools.com
;**
;**--------------File Info--------------------------------------------------------------------------------
;** File name: 			Startup.s
;** Last modified Date:  
;** Last Version: 		
;** Descriptions: 		The start up codes for LPC2100, including the initializing codes for the entry 
;**                     point of exceptions and the stacks of user tasks. Every project should have a 
;**                     independent copy of this file for related modifications
;**-------------------------------------------------------------------------------------------------------
;** Created by: 		Chenmingji
;** Created date:   	2004-02-02
;** Version:			1.0
;** Descriptions: 		The original version
;**-------------------------------------------------------------------------------------------------------
;** Modified by: 		LinEnqiang
;** Modified date:		2008-05-05	
;** Version:			1.01
;** Descriptions: 		增加中断管理支持 for LPC2400
;**
;********************************************************************************************************/
                                INCLUDE     LPC2400.INC                 ;/*  引入头文件                 */
                                                                        ;/*  定义堆栈的大小             */
SVC_STACK_LEGTH     EQU         0
FIQ_STACK_LEGTH     EQU         10
IRQ_STACK_LEGTH     EQU         9 * 8                                   ;/*  每层嵌套需要8个字堆栈，允许*/
                                                                        ;/*  9嵌套层                    */
ABT_STACK_LEGTH     EQU         0
UND_STACK_LEGTH     EQU         0

NoInt               EQU         0x80                                    ;/*  禁止 IRQ 中断宏定义        */
NoFIQ		        EQU	        0x40                                    ;/*  禁止 FIQ 中断宏定义        */

USR32Mode           EQU         0x10                                    ;/*  用户模式宏定义             */
SVC32Mode           EQU         0x13                                    ;/*  管理模式宏定义             */
SYS32Mode           EQU         0x1f                                    ;/*  系统模式宏定义             */
IRQ32Mode           EQU         0x12                                    ;/*  IRQ模式宏定义              */
FIQ32Mode           EQU         0x11                                    ;/*  FIQ模式宏定义              */

WAITWEN0            EQU         0x0F                                    ;/* 配置EMCStaticWaitWen0       */
WAITOEN0            EQU         0x0F                                    ;/* 配置EMCStaticWaitOen0       */ 
WAITRD0             EQU         0x0F                                    ;/* 配置EMCStaticWaitRd0        */
WAITPAGE0           EQU         0x1F                                    ;/* 配置EMCStaticWaitPage0      */
WAITWR0             EQU         0x0F                                    ;/* 配置EMCStaticWaitWr0        */
WAITTURN0           EQU         0x0F                                    ;/* 配置EMCStaticWaitTurn0      */

WAITWEN1            EQU         0x00                                    ;/* 配置EMCStaticWaitWen1       */
WAITOEN1            EQU         0x01                                    ;/* 配置EMCStaticWaitOen1       */ 
WAITRD1             EQU         0x04                                    ;/* 配置EMCStaticWaitRd1        */
WAITPAGE1           EQU         0x1F                                    ;/* 配置EMCStaticWaitPage1      */
WAITWR1             EQU         0x02                                    ;/* 配置EMCStaticWaitWr1        */
WAITTURN1           EQU         0x0F                                    ;/* 配置EMCStaticWaitTurn1      */

WAITWEN2            EQU         0x0F                                    ;/* 配置EMCStaticWaitWen2       */
WAITOEN2            EQU         0x0F                                    ;/* 配置EMCStaticWaitOen2       */ 
WAITRD2             EQU         0x0F                                    ;/* 配置EMCStaticWaitRd2        */
WAITPAGE2           EQU         0x1F                                    ;/* 配置EMCStaticWaitPage2      */
WAITWR2             EQU         0x0F                                    ;/* 配置EMCStaticWaitWr2        */
WAITTURN2           EQU         0x0F                                    ;/* 配置EMCStaticWaitTurn2      */

WAITWEN3            EQU         0x0F                                    ;/* 配置EMCStaticWaitWen3       */
WAITOEN3            EQU         0x0F                                    ;/* 配置EMCStaticWaitOen3       */ 
WAITRD3             EQU         0x0F                                    ;/* 配置EMCStaticWaitRd3        */
WAITPAGE3           EQU         0x1F                                    ;/* 配置EMCStaticWaitPage3      */
WAITWR3             EQU         0x0F                                    ;/* 配置EMCStaticWaitWr3        */
WAITTURN3           EQU         0x0F                                    ;/* 配置EMCStaticWaitTurn3      */


BCFG_08DEF	        EQU	        0x00000000                              ;/*   8Bit Bus                  */
BCFG_16DEF	        EQU	        0x00000001                              ;/*  16Bit Bus                  */
BCFG_32DEF	        EQU	        0x00000002                              ;/*  32Bit Bus                  */

;//                       |  页模式    |  片选极性  |字节定位状态|  延长等待  |  写缓冲区  |   写保护   |
;//                       |    PM      |    PC      |    PB      |     EW     |     B      |     P      |
;//                       |0:禁能1:使能| 0:低 1:高  |            |0:禁能1:使能|0:禁能1:使能|0:禁能1:使能|
BCFG0               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )
BCFG1               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )
BCFG2               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )
BCFG3               EQU   ( (0x00<<03) | (0x00<<06) | (0x01<<07) | (0x00<<8)  | (0x00<<19) | (0x00<<20) )

STATICCFG0	        EQU   ( BCFG_16DEF | BCFG0 ) 
STATICCFG1          EQU	  ( BCFG_16DEF | BCFG1 )
STATICCFG2	        EQU	  ( BCFG_16DEF | BCFG2 )
STATICCFG3          EQU	  ( BCFG_16DEF | BCFG3 )

;/********************************************************************************************************
; The imported labels    
; 引入的外部标号在这声明
;********************************************************************************************************/

    IMPORT  __main                                                      ;/*  C语言主程序入口            */        
    IMPORT  RunFirst
    IMPORT  SDRAMInit
    IMPORT  vicControl
    IMPORT  FIQ_Exception                                               ;/*  快速中断异常处理程序       */
    IMPORT  TargetResetInit                                             ;/*  目标板基本初始化           */    
    IMPORT	SoftwareInterrupt    
    IMPORT __use_two_region_memory
    IMPORT __use_no_semihosting_swi
;/********************************************************************************************************
; The emported labels    
; 给外部使用的标号在这声明
;********************************************************************************************************/

    EXPORT  RESET  
	EXPORT  ResetInit  
	EXPORT  StackUsr   
	EXPORT  InitStack
	EXPORT  bottom_of_heap  
	EXPORT  SoftwareInterruptAdd
	EXPORT  __user_initial_stackheap
	
    PRESERVE8

    AREA    RESET,CODE,READONLY
	;ARM
        ENTRY
Resetpoint                                                                   ;/*  中断向量表                 */
        LDR         PC, ResetAddr
        LDR     	PC, UndefinedAddr
        LDR    	 	PC, SWI_Addr
        LDR     	PC, PrefetchAddr
        LDR     	PC, DataAbortAddr
        DCD     	0xb9206e50
        LDR     	PC, [PC, #-0x120]
        LDR     	PC, FIQ_Addr

ResetAddr           DCD     ResetInit
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterruptAdd
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
Nouse               DCD     0
IRQ_Addr            DCD     0
FIQ_Addr            DCD     FIQ_Handler

Undefined
        B       .
PrefetchAbort
        B       .
DataAbort
        B       .               
SoftwareInterruptAdd 
        CMP     R0, #0x100
        BLO     SoftwareInterrupt
        B       vicControl    

;/********************************************************************************************************
; 快速中断
;********************************************************************************************************/
FIQ_Handler
        STMFD   SP!, {R0-R3, LR}
        BL		FIQ_Exception      
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4
;/********************************************************************************************************
;** Function name:          ResetInit
;**
;** Descriptions:           复位处理
;**
;** input parameters:       None
;** Returned value:         None
;**
;** Created by:             Chenmingji
;** Created Date:           2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            LinEnqiang
;** Modified date:          2008-01-15
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
ResetInit 
        
        BL      RunFirst
              
        LDR     R1, =STATICCFG0		                                    ;/* 配置EMCStaticConfig0        */
        LDR     R2, =WAITWEN0			                                ;/* 配置EMCStaticWaitWen0       */
        LDR     R2, =WAITWEN0			                                ;/* 配置EMCStaticWaitWen0       */
        LDR     R3, =WAITOEN0		                                    ;/* 配置EMCStaticWaitOen0       */
        LDR     R4, =WAITRD0		    	                            ;/* 配置EMCStaticWaitRd0        */
        LDR     R5, =WAITPAGE0			                                ;/* 配置EMCStaticWaitPage0      */
        LDR     R6, =WAITWR0	    		                            ;/* 配置EMCStaticWaitWr0        */			   
        LDR     R7, =WAITTURN0			                                ;/* 配置EMCStaticWaitTurn0      */
        LDR		R0, =EMCStaticConfig0 
        STMIA   R0!, {R1-R7}  
 
        LDR     R1, =STATICCFG1		                                    ;/* 配置EMCStaticConfig1        */
        LDR     R2, =WAITWEN1			                                ;/* 配置EMCStaticWaitWen1       */
        LDR     R3, =WAITOEN1		                                    ;/* 配置EMCStaticWaitOen1       */
        LDR     R4, =WAITRD1		    	                            ;/* 配置EMCStaticWaitRd1        */
        LDR     R5, =WAITPAGE1			                                ;/* 配置EMCStaticWaitPage1      */
        LDR     R6, =WAITWR1	    		                            ;/* 配置EMCStaticWaitWr1        */			   
        LDR     R7, =WAITTURN1			                                ;/* 配置EMCStaticWaitTurn1      */
        LDR		R0, =EMCStaticConfig1
        STMIA   R0!, {R1-R7}
    
        LDR     R1, =STATICCFG2		                                    ;/* 配置EMCStaticConfig2        */
        LDR     R2, =WAITWEN2			                                ;/* 配置EMCStaticWaitWen2       */
        LDR     R3, =WAITOEN2		                                    ;/* 配置EMCStaticWaitOen2       */
        LDR     R4, =WAITRD2		    	                            ;/* 配置EMCStaticWaitRd2        */
        LDR     R5, =WAITPAGE2			                                ;/* 配置EMCStaticWaitPage2      */
        LDR     R6, =WAITWR2	    		                            ;/* 配置EMCStaticWaitWr2        */			   
        LDR     R7, =WAITTURN2			                                ;/* 配置EMCStaticWaitTurn2      */
        LDR		R0, =EMCStaticConfig2       
        STMIA   R0!, {R1-R7}  
        
        LDR     R1, =STATICCFG3		                                    ;/* 配置EMCStaticConfig3        */
        LDR     R2, =WAITWEN3			                                ;/* 配置EMCStaticWaitWen3       */
        LDR     R3, =WAITOEN3		                                    ;/* 配置EMCStaticWaitOen3       */
        LDR     R4, =WAITRD3		    	                            ;/* 配置EMCStaticWaitRd3        */
        LDR     R5, =WAITPAGE3			                                ;/* 配置EMCStaticWaitPage3      */
        LDR     R6, =WAITWR3	    		                            ;/* 配置EMCStaticWaitWr3        */			   
        LDR     R7, =WAITTURN3			                                ;/* 配置EMCStaticWaitTurn3      */
        LDR		R0, =EMCStaticConfig3
        STMIA   R0!, {R1-R7}
        BL      InitStack               		                        ;/*  初始化堆栈                 */
        BL      TargetResetInit         		                        ;/*  目标板基本初始化           */       

        B       __main                                                  ;/*  跳转到c语言入口            */
                    		                            
;/********************************************************************************************************
;** Function name:          __user_initial_stackheap
;**
;** Descriptions:           库函数初始化堆和栈，不能删除
;**
;** input parameters:       参考库函数手册
;** Returned value:         参考库函数手册
;**
;** Created by:             Chenmingji
;** Created Date:           2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            LinEnqiang
;** Modified date:          2008-01-15
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
__user_initial_stackheap    
    LDR   R0, =bottom_of_heap		
    ;LDR   R1, =StackUsr			
    LDR   R2, =top_of_heap		
    LDR   R3, =bottom_of_Stacks	      
    MOV   PC, LR

    EXPORT _sys_exit
_sys_exit
    B       .

    EXPORT __rt_div0
    EXPORT fputc
    EXPORT fgetc
    EXPORT _sys_close
    EXPORT _sys_write
    EXPORT _sys_read
    EXPORT _sys_istty
    EXPORT _sys_seek
    EXPORT _sys_ensure
    EXPORT _sys_flen
    EXPORT _sys_tmpnam
    EXPORT _sys_command_string

__rt_div0
fputc
fgetc
_sys_close
_sys_write
_sys_read
_sys_istty
_sys_seek
_sys_ensure
_sys_flen
_sys_tmpnam
_sys_command_string
    NOP	
    MOV     R0, #0
    MOV     PC, LR		
    
;/********************************************************************************************************
;** Function name:          CrpData
;**
;** Descriptions:           加密芯片
;**
;** input parameters:       NONE
;** Returned value:         NONE
;**
;** Created by:             Chenmingji
;** Created Date:           2004/03/27 
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            
;** Modified date:          
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc
    NOP
    WEND
CrpData1
    DCD     0x87654321                                                  ;/*  当此数为0x87654321时，用户 */
                                                                        ;/*  程序被保护                 */
    ENDIF
;/********************************************************************************************************
;** Function name:          InitStack
;**
;** Descriptions:           初始化堆栈
;**
;** input parameters:       None
;** Returned value:         None
;**
;** Created by:             Chenmingji
;** Created Date:           2004/02/02
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            LinEnqiang
;** Modified date:          2008-01-15
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
InitStack    
        MOV     R0, LR                
;Build the SVC stack
;设置管理模式堆栈
        MSR     CPSR_c, #0xd3		
        LDR     SP, StackSvc
;Build the IRQ stack
;设置中断模式堆栈
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;Build the FIQ stack	
;设置快速中断模式堆栈
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;Build the DATAABORT stack
;设置中止模式堆栈
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;Build the UDF stack
;设置未定义模式堆栈
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
;Build the SYS stack
;设置系统模式堆栈
        MSR     CPSR_c, #0xdf
        LDR     SP, =StackUsr              
        MOV     PC, R0
               
    LTORG
StackSvc           DCD     SvcStackSpace  + (SVC_STACK_LEGTH - 1) * 4
StackIrq           DCD     IrqStackSpace  + (IRQ_STACK_LEGTH - 1) * 4
StackFiq           DCD     FiqStackSpace  + (FIQ_STACK_LEGTH - 1) * 4
StackAbt           DCD     AbtStackSpace  + (ABT_STACK_LEGTH - 1) * 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1) * 4    
    
        AREA    MyStacks, DATA, NOINIT, ALIGN = 2;                      ;/*  分配堆栈空间               */    
        
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4                          ;/*  管理模式堆栈空间           */
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4                          ;/*  中断模式堆栈空间           */
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4                          ;/*  快速中断模式堆栈空间       */
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4                          ;/*  中止义模式堆栈空间         */
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4                          ;/*  未定义模式堆栈             */   

        AREA    HeapBottom, DATA, NOINIT
bottom_of_heap    	SPACE   1

        AREA    StackBottom, DATA, NOINIT
bottom_of_Stacks    SPACE   1

        AREA    HeapTop, DATA, NOINIT
top_of_heap

        AREA    StacksTop, DATA, NOINIT
StackUsr  
    END
    
;/********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
