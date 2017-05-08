;/****************************************Copyright (c)***************************************************
;**                               Guangzou ZHIYUAN electronics Co.,LTD.
;**                                      
;**                                 http://www.embedtools.com
;**
;**--------------File Info--------------------------------------------------------------------------------
;** File Name: IRQ.s
;** Last modified Date:  2004-06-14
;** Last Version: 1.1
;** Descriptions: The irq handle that what allow the interrupt nesting. 
;**
;**-------------------------------------------------------------------------------------------------------
;** Created By: 	Chenmingji
;** Created date:   2004-09-17
;** Version: 1.0
;** Descriptions: 	First version
;**
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;** Version:
;** Descriptions:
;**
;********************************************************************************************************/

NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11

    CODE32

    AREA    IRQ,CODE,READONLY

    MACRO
$IRQ_Label HANDLER $IRQ_Exception_Function

        EXPORT  $IRQ_Label                      						;  输出的标号       
        IMPORT  $IRQ_Exception_Function         						;  引用的外部标号

$IRQ_Label
        SUB     LR, LR, #4                      						;  计算返回地址
        STMFD   SP!, {R0-R3, R12, LR}           						;  保存任务环境
        MRS     R3, SPSR                        						;  保存状态
        STMFD   SP, {R3,LR}^                    						;  保存SPSR和用户状态的SP,                                               							
        NOP
        SUB     SP, SP, #4*2

        MSR     CPSR_c, #(NoInt | SYS32Mode)    						;  切换到系统模式 
       
        BL      $IRQ_Exception_Function         						;  调用c语言的中断处理程序

        MSR     CPSR_c, #(NoInt | IRQ32Mode)    						;  切换回irq模式
        LDMFD   SP, {R3,LR}^                    						;  恢复SPSR和用户状态的SP                                                				
        MSR     SPSR_cxsf, R3
        ADD     SP, SP, #4*2                    

        LDMFD   SP!, {R0-R3, R12, PC}^          
    MEND

;/******************************************************************************************************** 
;	以下添加中断句柄，用户根据实际情况改变 
;********************************************************************************************************/

;Timer0_Handler  HANDLER Timer0

    END
;/********************************************************************************************************
; 	END FILE
;********************************************************************************************************/
