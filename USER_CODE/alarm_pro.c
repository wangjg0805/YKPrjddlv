//**********************************************************************
//文件名:alarm_pro.c
//函数列表:
//01.把指定的报警添加到报警链上:Log_Alm
//02.删除报警链上的某个报警:Cancel_Alm
//03.把当前报警移到报警链的最后:Move_Alm_Rec_To_End
//04.在报警区域显示报警信息:Put_Alm_Mess
//05.查询当前是否有报警: Check_Alm_Status
//06.把提示信息加入链表中:Log_Tips
//07.删除提示信息:Del_Tips
//08.显示提示信息:Put_Tips_Mess
//09.检查当前是否显示提示信息:Check_Tips_Disp
//10.查询当前是否显示指定的提示信息:Check_Special_Tips_Disp
//**********************************************************************
#include "config.h"
#include "alarm_pro.h"
#include "lcd2478.h"

#include "speaker.h"
#include "timer.h"
#include "Font_Lib.h"

uint8 Check_Tips_Disp(void);
//报警记录目录

Alm_Struct blank_alm     = {NONE_ALM,NULL,NULL,FALSE,NULL};    //空报警信息
//警示类报警 条件消失后删除
Alm_Struct warn_ph_out   = {ALARM_PH_OUT_WARN,       "PH 数据超出测量范围",       "PH DATA OVER",FALSE,NULL};
Alm_Struct warn_mv_out   = {ALARM_MV_OUT_WARN,       "毫伏数据超出测量范围",      "MV DATA OVER",FALSE,NULL};
Alm_Struct warn_temp_out = {ALARM_TEMP_OUT_WARN,     "温度参数偏差太大",      "TEMP OFFSET OVER",FALSE,NULL};
Alm_Struct warn_xx_out   = {ALARM_XX_OUT_WARN,       "上海佑科",                  "SHANGHAIYOKE",FALSE,NULL};


//提示类信息，显示一段时间后会自动删除
Alm_Struct tips_save_fault= {TIPS_NOT_STABLE_WARN,      "数据无效保存失败",                "SAVE FAILED",FALSE,NULL};
Alm_Struct tips_EE_bad    = {TIPS_EE_BAD_WARN,          "存储器故障保存失败",   "NOT SAVE OF BAD EEPROM",FALSE,NULL};
Alm_Struct tips_save_ok   = {TIPS_SAVE_OK_WARN,         "数据保存成功",                        "SAVE OK",FALSE,NULL};
Alm_Struct tips_clear_ok  = {TIPS_CLEAR_OK_WARN,        "数据已全部清除",                       "DEL OK",FALSE,NULL};
Alm_Struct tips_first_page= {TIPS_FIRST_PAGE_WARN,      "当前是第一页",                     "FIRST PAGE",FALSE,NULL};
Alm_Struct tips_last_page = {TIPS_LAST_PAGE_WARN,       "当前是最后一页",                    "LAST PAGE",FALSE,NULL};
Alm_Struct tips_invalid_data = {TIPS_NO_VALID_WARN,     "输入值超出范围",                "INVALID INPUT",FALSE,NULL};
Alm_Struct tips_bad_envir   =  {TIPS_BAD_ENV_WARN,      "数据不稳定或为负",         "WEIGHTMENT INVALID",FALSE,NULL};


Alm_Struct* warning_alm = &blank_alm;         //当前的警示类报警
Alm_Struct*    tips_alm = &blank_alm;         //当前的提示类信息
///////////////////机器提示信息
#define TIPS_DISP_TIME            150         //提示信息显示的时间
#define TIPS_DISP_SHORT_TIME      50
//工作模式下的提示信息
/*
Tips_Struct*        cur_tips_ptr;    //当前提示信息的指针
static sint16     tips_disp_time;    //提示信息显示计时器
static uint8          flag_saved;    //已经保存被覆盖区域的标志
static uint8   tips_disp_trigger;    //触发提示信息的显示
static uint32  save_buf[LINE_SIZE*SCR_YSIZE/4];//被提示信息覆盖的显示数据临时保存的缓冲区
*/                                                //最大尺寸为四分之一窗口
//**********************************************************************
//1函数  名:Log_Alm
//函数功能:添加报警记录节点(加到最前面)
//入口参数:alm--添加的报警记录
//            level--报警级别
//出口参数:无
//**********************************************************************
void Log_Alm(Alm_Struct* alm,uint8 level)
{
    if (NULL == alm) return;
    switch (level)
    {
        case ALM_LEV_WARN://警示类报警
            if (alm->alm_id < FIRST_WARN_ALARM)  return;    //无该记录
            if (alm->alm_id >  LAST_WARN_ALARM)  return;    //无该记录
            break;        
        case ALM_LEV_TIPS://提示类信息
            if (alm->alm_id < FIRST_TIPS_ALARM)  return;    //无该记录
            if (alm->alm_id >  LAST_TIPS_ALARM)  return;    //无该记录
            break;
       
       default:break;    
    }
    if (alm->flag_log == TRUE) return; //已经处在报警记录上则不再重新记录 
    alm->flag_log = TRUE;
   
    switch (level)
    {
       case ALM_LEV_WARN://警示类报警
             alm->next_record =   warning_alm;    //原先的记录推后
             warning_alm = alm;    
             warning_alm_timer = 0;    //重新计时 
             break;        
       case ALM_LEV_TIPS:
             alm->next_record =     tips_alm;    //原先的记录推后
             tips_alm = alm;
             tips_alm_timer = 0;    //重新计时
             break;
       default:break;
    }
}

//**********************************************************************
//2函数  名:Del_Alm
//函数功能:删除报警记录节点,如果该报警不在报警链上，则直接返回
//入口参数:alm--添加的报警记录
//            level--报警级别
//出口参数:无
//**********************************************************************
void Del_Alm(Alm_Struct* alm,uint8 level)
{
    Alm_Struct* pre;
  
    if (alm->flag_log == FALSE)          return; //该报警不在报警链上，直接返回    
    if (alm->alm_id < FIRST_WARN_ALARM)  return;
    if (alm->alm_id > LAST_TIPS_ALARM)   return; //无该记录
  
    switch (level)
    {
        case ALM_LEV_TIPS:
            if (alm == tips_alm)//头结点
            {
                tips_alm = alm->next_record;
            }
            else
            {
                pre = tips_alm;
                while (pre->next_record != NULL)
                {
                    if (pre->next_record == alm)    //找到前驱节点
                    {
                        break;
                    }
                    pre = pre->next_record;
                }
                pre->next_record = alm->next_record;
            }
            break;
        case ALM_LEV_WARN://高优先级报警
            if (alm == warning_alm)//头结点
            {
                warning_alm = alm->next_record;
            }
            else
            {
                pre = warning_alm;
                while (pre->next_record != NULL)
                {
                    if (pre->next_record == alm)    //找到前驱节点
                    {
                        break;
                    }
                    pre = pre->next_record;
                }
                pre->next_record = alm->next_record;
            }        
            break;        
       
        default:return;
    }
    alm->next_record = NULL;
    alm->flag_log = FALSE;
}

////////////////////////////////////////////////////////////
//删除当前提示信息
///////////////////////////////////////////////////////////
void Del_Tip_Alm(void)
{
    Alm_Struct* tips_tmp;
  
    if(tips_alm == &blank_alm)  return;
   
    tips_tmp = tips_alm;
    
    tips_alm = tips_alm->next_record; //指向下一条记录
    
    tips_tmp->next_record = NULL;     //把当前记录复位
    tips_tmp->flag_log    = FALSE;
    
}
//**********************************************************************
//3函数  名:Move_Alm_Rec_To_End
//函数功能: 把当前的报警记录移到最后
//入口参数:    level--报警级别
//出口参数:无
//**********************************************************************
static void Move_Alm_Rec_To_End(uint8 level)
{
    Alm_Struct* end,*head;
    switch (level)
    {
        case ALM_LEV_TIPS:
            if (tips_alm->alm_id == NONE_ALM)    //无报警信息
            {
                return;    
            }
            else if (tips_alm->next_record == &blank_alm)//无后续报警节点
            {
                return;
            }
            else
            {
                head = end = tips_alm;
                while (end->next_record != &blank_alm)//找到最后的报警节点
                {
                    end = end->next_record;
                }
                end->next_record = head;
                tips_alm = head->next_record;    //第二节点变为头结点
                head->next_record= &blank_alm;                
            }
            break;
        case ALM_LEV_WARN:
            if (warning_alm->alm_id == NONE_ALM)    //无报警信息
            {
                return;    
            }    
            else if (warning_alm->next_record == &blank_alm)//无后续报警节点
            {
                return;
            }
            else
            {
                head = end = warning_alm;
                while (end->next_record != &blank_alm)//找到最后的报警节点
                {
                    end = end->next_record;
                }
                end->next_record = head;
                warning_alm = head->next_record;    //第二节点变为头结点
                head->next_record= &blank_alm;                
            }            
            break;        
        default:break;
    }            
}

//**********************************************************************
//4函数 名:Put_Alm_Mess
//函数功能:显示报警信息
//入口参数:无
//出口参数:无 
//**********************************************************************
void Put_Alm_Mess(void) //100ms 
{
   
    static uint32 tips_alm_cnt = 0 ;
    static uint32 manu_alm_display_cnt = 0 ;
    
    //Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
    //Bar(100,0,480,69);  //擦除报警信息
    
    //Set_HZ_Lib(&ST_11);    //设置字库
    //Set_ASC_Lib(Arial10B_Font);
    //////////////////////////////////////////////////////警示类
    if((warning_alm_timer<4)&&(warning_alm->alm_id != NONE_ALM))//有报警信息
    {
         Set_Graphic_Color(COL_THINRED,COL_THINRED);
         Bar(250,5,250+300,35);
         Set_Graphic_Color(COL_WHITE,COL_WHITE);
         Rectangle(250,5,250+300,35);
       
         Set_Chr_Color(COL_WHITE,COL_THINRED);  //红底白字
         Set_ASC_Lib(Arial16B_Font);
         Put_Line_Str(270, 10,Language_Str((uint8*)warning_alm->ch_mess,(uint8*)warning_alm->en_mess));
    }
    else if(warning_alm->alm_id == NONE_ALM)
     {
      Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
      Bar(250,5,250+300,35);  //擦除报警信息
     }
     
    if (warning_alm_timer >= 20)    //2s高优先级报警
    {    
        warning_alm_timer = 0;
        Move_Alm_Rec_To_End(ALM_LEV_WARN);    //显示时间到,切换报警信息
    }        
    
    //////////////////////提示类信息显示在下方，会自动消失
    if(tips_alm->alm_id != NONE_ALM)//有提示信息
    {
        tips_alm_cnt++;
        
        Set_Graphic_Color(COL_THINYELLOW,COL_THINYELLOW);
        Bar(250,40,250+300,70);
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
        Rectangle(250,40,250+300,70);
     
        Set_Chr_Color(COL_BLACK,COL_THINYELLOW); //黄底黑字
        Set_ASC_Lib(Arial16B_Font);
        Put_Line_Str(270,45,Language_Str((uint8*)tips_alm->ch_mess,(uint8*)tips_alm->en_mess));
    }
    else if(tips_alm->alm_id == NONE_ALM)
      {
       Set_Graphic_Color(COL_THINBLUE,COL_THINBLUE);
       Bar(250,40,250+300,70);  //擦除报警信息
      }
      
    if (tips_alm_cnt >= 30)    //提示信息显示时间
    {
        tips_alm_cnt = 0;
        Del_Tip_Alm();   //把当前提示信息删除掉
        //Move_Alm_Rec_To_End(ALM_LEV_TIPS);//切换报警信息
    }    
}

//**********************************************************************
//5函数  名:Check_Alm_Status
//函数功能:查询当前是否有报警
//入口参数:无
//出口参数:TRUE--有报警
//            FALSE--无报警
//**********************************************************************
uint8 Check_Alm_Status(void)
{
    return ((tips_alm->alm_id!=NONE_ALM)||
            (warning_alm->alm_id!=NONE_ALM));
}

/****************************************************************************
* 名称：Set_Clr_Alarm_Sound()
* 功能：设置或消除报警声音的处理(100ms一次)
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Set_Clr_Alarm_Sound(void)
{
    //优先级别：1按键 2：报警  3：提示
    
    if(Get_Key_Sound_Type() != SOUND_TYPE_NONE) //按键声音优先
       {
        Set_Alm_Sound_Type(SOUND_TYPE_NONE);
        return;
       }
       
    ////当提示类与报警类声音都有时候，报警类声音优先
    ////所以先检查提示类信息
    
    if(tips_alm->alm_id !=NONE_ALM)
    {  //有报警
       if(Get_Alm_Sound_Type() != SOUND_TYPE_TIPS)
         { //当前不是此类报警声
            Set_Alm_Sound_Type(SOUND_TYPE_TIPS);           //设置报警声音类型
         }
    }
    else
     Set_Alm_Sound_Type(SOUND_TYPE_NONE);
    //////////////////////////////////////////////// 
    ////////////检查报警类信息 
    if(warning_alm->alm_id !=NONE_ALM)
    {  //有报警
       if(Get_Alm_Sound_Type() != SOUND_TYPE_WARNING)
         { //当前不是此类报警声
            Set_Alm_Sound_Type(SOUND_TYPE_WARNING);        //设置报警声音类型
         }
    }
    else
     Set_Alm_Sound_Type(SOUND_TYPE_NONE);
    
}


