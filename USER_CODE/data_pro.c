//**********************************************************************
//文件名:data_pro.c
//函数列表：
//1.计算潮气量:Cal_Vt
//2.计算管道压力:Cal_Paw
//3.计算氧传感器电压:Cal_O2_Sen_Vol
//4.计算氧气浓度:Cal_O2_Thickness
//5.计算蓄电池电压:Cal_Batt_V
//6.计算充电电流:Cal_Batt_I
//7.计算参考电压:Cal_Vref_Test
//8.由AD值计算电压值:Cal_Vol
//9.更新压力传感器曲线:Update_Paw_Curve
//10.检查压力标定点的AD值:Check_Pipeline_P_AD
//11.检查压力曲线:Check_EEP_Pipeline_P
//12.检查氧传感器标定点的AD值:Check_O2_AD
//13.检查氧气传感器的特征曲线:Check_O2_Curve
//14.检查氧气传感器的标定数据:Check_EEP_O2
//15.检查潮气量标定点的圈数:Check_Vt_Laps
//16.检查潮气量曲线:Check_EEP_Vte
//17.更新氧传感器曲线:Update_O2_Sen_Curve
//18.更新潮气量传感器曲线:Update_Vt_Cur
//19.从EEPROM中读取潮气量的标定数据并检查:Read_Vt_Dem_EEP
//20.从EEPROM中读取压力传感器的标定数据并检查:Read_Paw_Dem_EEP
//21.从EEPROM中读取氧气传感器的标定数据并检查:Read_O2_Dem_EEP
//**********************************************************************
#include "data_pro.h"
#include "global.h"
#include "eeprom.h"
#include "type_def.h"
#include "stdio.h"
//**********************************************************************
//函数  名:Cal_Vt
//函数功能:计算潮气量
//入口参数:lap--风轮圈数
//            *vt_units--返回的潮气量单位(1--1ml;10--0.1ml)
//出口参数:潮气量(单位:1ml)
//**********************************************************************
sint32 Cal_Vt(sint32 lap,uint8 *vt_units)
{
    sint32 vt,di;
    if (NULL == vt_units) return(VT_MEAS_NOT_VALID);
    if (machine_mode != MACHINE_DEMARC_MODE)
    {//没有处在标定模式下
        if(flag_eep_vte_error) return(VT_MEAS_NOT_VALID);    
    }
    *vt_units = 1;
    if (lap == 0) return(0);
    
    for (di=1; di<VT_DEMAR_POINTS; di++)
    {
        if (lap < vt_lap_tab[di])
        {
            break;
        }    
    }
    if (VT_DEMAR_POINTS == di) di--;//最后一条线段
    di--;
    vt = vt_demar_tab[di]*8192+vt_k_tab[di]*(lap-vt_lap_tab[di]);
    vt = (vt+4096)/8192;
    if(vt < 0) return(0);
    if(vt > 2000) return(2000);    
    return (vt);
}

//**********************************************************************
//函数  名:Cal_Paw
//函数功能:计算管道压力值
//入口参数:ad--管道压力AD值
//出口参数:管道压力(放大倍数:PAW_MUL)
//**********************************************************************
sint32 Cal_Paw(uint16 ad)
{
    uint8 di;
    sint32 paw;
    if (machine_mode != MACHINE_DEMARC_MODE)
    {//没有处在标定模式下    
        if (flag_eep_p_error) return (PAW_NOT_VALID);    //管压标定数据错误
    }
    for (di=1; di<PAW_DEMAR_POINTS; di++)
    {//寻找匹配线段(从第一条终点开始找)
        if (ad<paw_ad_tab[di])
        {
            break;
        }
    }
    if (PAW_DEMAR_POINTS == di) di--;//最后一条线段
    di--;

    paw = paw_demar_tab[di]*8192+paw_k_tab[di]*(ad+paw_zero_ad-paw_ad_tab[di]);
    paw = (paw*PAW_MUL+4096)/8192;    
    return (paw);
}

//**********************************************************************
//函数  名:Cal_O2_Sen_Vol
//函数功能:计算氧传感器的电压
//入口参数:ad--氧传感器电压AD值
//出口参数:氧传感器电压(单位:0.1mV)
//**********************************************************************
sint32 Cal_O2_Sen_Vol(uint16 ad)
{
    sint32  volt;
    //volt = (ad * (AD_VREF_V * 100 / O2_SENSOR_V_AMP)) / 1023;
    volt = (ad * (AD_VREF_V * 100 / O2_SENSOR_V_AMP)) / 1024;
    //if(volt > 999) return(999);
    return(volt);
}

//**********************************************************************
//函数  名:Cal_O2_Thickness
//函数功能:计算氧气浓度
//入口参数:ad--氧气传感器电压AD值
//            *o2_units--返回的氧气浓度单位(1--1%;10--0.1%)
//出口参数:氧气浓度(单位:0.1%)
//**********************************************************************
sint32 Cal_O2_Thickness(uint16 ad,uint8* o2_units)
{
    sint32 o2_thick;
    if (machine_mode != MACHINE_DEMARC_MODE)
    {//没有处在标定模式下    
        if (flag_eep_o2_error) return (O2_MEAS_NOT_VALID);//标定数据错误
    }
    *o2_units = 10;
    o2_thick = o2_demar_tab[0]*8192+(ad-o2_ad_tab[0])*o2_k;
    if (o2_thick<0) return(0);
    o2_thick = (o2_thick*10+4096)/8192;
    return (o2_thick);
}

//**********************************************************************
//函数  名:Cal_Batt_V
//函数功能:计算蓄电池电压
//入口参数:ad--蓄电池电压AD值
//出口参数:蓄电池电压(单位:0.01V)
//**********************************************************************
sint32 Cal_Batt_V(uint16 ad)
{
    sint32 volt;
    
    volt = (ad * ((AD_VREF_V * BATT_V_AMP) / 10)) / 1024;
    //if(volt > 999) return(999);
    return(volt);
}


//**********************************************************************
//函数  名:Cal_Batt_I
//函数功能:计算蓄电池的充电电流
//入口参数:ad--蓄电池的电流AD值
//出口参数:蓄电池充电电流(单位:1mA)
//**********************************************************************
sint32 Cal_Batt_I(uint16 ad)
{    
    sint32 ci;
    ci = (ad * ((AD_VREF_V * 1000) / (BATT_I_AMP * BATT_I_RES))) / 1024
            - (1000 * BATT_I0_V) / (BATT_I_AMP * BATT_I_RES);
    //if(ci < 0) return(0);
   //if(ci > 999) return(999);
    return(ci);
}

//**********************************************************************
//函数  名:Cal_Vref_Test
//函数功能:计算参考电压值
//入口参数:ad--参考电压的AD值
//出口参数:参考电压的测试值(单位:0.01V)
//**********************************************************************
sint32 Cal_Vref_Test(uint16 ad)
{
    sint32 volt;
    volt = (ad * AD_VREF_V) / 1024;
    //if(volt > 999) return(999);
    return(volt);
}

//**********************************************************************
//函数  名:Cal_Vol
//函数功能:计算电压值
//入口参数:ad--电压AD值
//出口参数:电压的测试值(单位:0.01V)
//**********************************************************************
sint32 Cal_Vol(uint16 ad)
{
    sint32 volt;
    volt = (ad * AD_VREF_V) / 1024;
    return(volt);
}

//**********************************************************************
//函数  名:Update_Paw_Curve
//函数功能:更新压力传感器曲线,计算曲线的k,保存标定点AD值
//入口参数:point--更新点
//            ad--更新点的AD值
//出口参数:无
//**********************************************************************
void Update_Paw_Curve(uint16 ad,sint8 point)
{
    sint32 di;
    if (point < 0 ) return;    //无该点则返回
    if (point > PAW_DEMAR_POINTS-1) return;    //无该点则返回
    
    paw_ad_tab[point] = ad;    //更新标定点的AD值
    if (point != PAW_DEMAR_POINTS-1)    //不是最后一点
    {
        di = paw_ad_tab[point+1]-paw_ad_tab[point];
        if (di)
        {//计算斜率放大8192倍
            paw_k_tab[point] = (paw_demar_tab[point+1]-paw_demar_tab[point])*8192/di;
        }
        else        //无效
        {
            paw_k_tab[point] = 0;
        }                        
    }
    
    if (point)    //不是第一点
    {
        di = paw_ad_tab[point]-paw_ad_tab[point-1];
        if (di) //di!=0
        {//计算斜率放大8192倍        
            paw_k_tab[point-1] = (paw_demar_tab[point]-paw_demar_tab[point-1])*8192/di;    
        }
        else
        {
            paw_k_tab[point-1] = 0;
        }
    }
    paw_ad_filt_band = (paw_ad_tab[PAW_DEMAR_POINTS-1] - paw_ad_tab[0]) / 50 + 1;
}

//**********************************************************************
//函数名称：Check_Pipeline_P_AD
//函数功能：判断压力标定点AD值是否符合
//入口参数：point--压力点
//            ad--比较的AD值
//出口参数：最高字节:0 正确, 1 太小, 2太大,3无对应压力点;低2字节为正确AD值
//**********************************************************************
sint32 Check_Pipeline_P_AD(uint8 point,uint16 ad)
{
    /*switch (point)
    {
        case 0:
            if(ad < DEMA_MIN_0_P_AD) return(0x01000000|DEMA_MIN_0_P_AD);
            if(ad > DEMA_MAX_0_P_AD) return(0x02000000|DEMA_MAX_0_P_AD);
            return(ad);
        case 1:
            if (ad < DEMA_MIN_50_P_AD) return(0x01000000|DEMA_MIN_50_P_AD);
            if (ad > DEMA_MAX_50_P_AD) return(0x02000000|DEMA_MAX_50_P_AD);
            return(ad);    
        default:
        //return(0x03000000);
            return(ad);
    }*/
    if (ad < paw_ad_min_buf[point])    return(0x01000000|paw_ad_min_buf[point]);
    if (ad > paw_ad_max_buf[point]) return(0x02000000|paw_ad_min_buf[point]);
    return (ad);
}

/****************************************************************************
* 名称：Check_EEP_Pipeline_P()
* 功能：判断EEPROM中压力标定数据是否正确
* 入口参数：无
* 出口参数：0 正确, 其他为错误号
****************************************************************************/
 uint8 Check_EEP_Pipeline_P(void)
{
    /* uint8 res;
    res = Check_Pipeline_P_AD(0,paw_ad_tab[0])>>24;
    if(res) return(res);
    res = Check_Pipeline_P_AD(1,paw_ad_tab[1])>>24;
    if(res) return(res);
    if(paw_ad_tab[1] - paw_ad_tab[0] < DEMA_MIN_50_P_AD - DEMA_MIN_0_P_AD)
    {
        return(5);
    }
    if(paw_ad_tab[1] - paw_ad_tab[0] > 1023 - paw_ad_tab[1])
    {
        return(6);
    }*/
    if (Check_Sen_Curve_AD(paw_ad_tab,PAW_DEMAR_POINTS,paw_ad_min_buf,paw_ad_max_buf) == ERROR) return(1);
    return(0);
}

//**********************************************************************
//函数名称：Check_O2_AD
//函数功能：判断氧气AD值是否符合
//入口参数：point--氧气点
//            ad--比较的AD值
//出口参数：最高字节:0 正确, 1 太小, 2太大,3无对应点;低2字节为正确AD值
//**********************************************************************
sint32 Check_O2_AD(uint8 point,uint16 ad)
{
    switch (point)
    {
        case 0:
            if(ad < DEMA_MIN_21_O2_AD) return(0x01000000|DEMA_MIN_21_O2_AD);
            if(ad > DEMA_MAX_21_O2_AD) return(0x02000000|DEMA_MAX_21_O2_AD);
            return(ad);
        case 1:
            if (ad < DEMA_MIN_100_O2_AD) return(0x01000000|DEMA_MIN_100_O2_AD);
            if (ad > DEMA_MAX_100_O2_AD) return(0x02000000|DEMA_MAX_100_O2_AD);
            return(ad);    
        default:return(0x03000000);
    }
}

/****************************************************************************
* 名称：Check_O2_Curve()
* 功能：判断氧传感器曲线是否符合要求(100%与21%为5倍关系)
* 入口参数：无
* 出口参数：0 符合, 1 斜率太小, 2 斜率太大
****************************************************************************/
 uint8 Check_O2_Curve(void)
{
    if(o2_ad_tab[1] < o2_ad_tab[0] * 4) return(1);
    if(o2_ad_tab[1] > o2_ad_tab[0] * 6) return(2);
    return(0);
}

/****************************************************************************
* 名称：Check_EEP_O2()
* 功能：判断EEPROM中氧浓度标定数据是否正确
* 入口参数：无
* 出口参数：0 正确, 其他为错误号
****************************************************************************/
 uint8 Check_EEP_O2(void)
{
     uint8 res;
    res = Check_O2_AD(0,o2_ad_tab[0])>>24;
    if(res) return(res);
    res = Check_O2_AD(1,o2_ad_tab[1])>>24;
    if(res) return(res + 2);
    res = Check_O2_Curve();
    if(res) return(res + 4);
    return(0);
}

//**********************************************************************
//函数名称：Check_Vt_Laps
//函数功能：判断潮气量标定点的圈数是否符合预设范围
//入口参数：point--标定点
//           lps--圈数
//出口参数：最高字节:0 正确, 1 太小, 2太大,3无对应点;低2字节为正确lps
//**********************************************************************
sint32 Check_Vt_Laps(uint8 point,sint32 lps)
{
    switch (point)
    {
        case 0://50mL点
            if(lps < DEMA_MIN_50_LAP) return(0x01000000|DEMA_MIN_50_LAP);
            if(lps > DEMA_MAX_50_LAP) return(0x02000000|DEMA_MAX_50_LAP);
            return(lps);
        case 1://100mL点
            if (lps < DEMA_MIN_100_LAP) return(0x01000000|DEMA_MIN_100_LAP);
            if (lps > DEMA_MAX_100_LAP) return(0x02000000|DEMA_MAX_100_LAP);
            return(lps);
        case 2://200mL点
            if (lps < DEMA_MIN_200_LAP) return(0x01000000|DEMA_MIN_200_LAP);
            if (lps > DEMA_MAX_200_LAP) return(0x02000000|DEMA_MAX_200_LAP);
            return(lps);
        case 3://400mL点
            if (lps < DEMA_MIN_400_LAP) return(0x01000000|DEMA_MIN_400_LAP);
            if (lps > DEMA_MAX_400_LAP) return(0x02000000|DEMA_MAX_400_LAP);
            return(lps);
        case 4://800mL点
            if (lps < DEMA_MIN_800_LAP) return(0x01000000|DEMA_MIN_800_LAP);
            if (lps > DEMA_MAX_800_LAP) return(0x02000000|DEMA_MAX_800_LAP);
            return(lps);                                                    
        default:return(0x03000000);
    }
}

/****************************************************************************
* 名称：Check_EEP_Vte()
* 功能：判断EEPROM中潮气量标定数据是否正确
* 入口参数：无
* 出口参数：0 正确, 其他为错误号
****************************************************************************/
 uint8 Check_EEP_Vte(void)
{
     uint8 res;
    sint32 di;
    res = Check_Vt_Laps(0,vt_lap_tab[0])>>24;
    if(res) return(res);
    res = Check_Vt_Laps(1,vt_lap_tab[1])>>24;
    if(res) return(res + 2);
    res = Check_Vt_Laps(2,vt_lap_tab[2])>>24;
    if(res) return(res + 4);
    res = Check_Vt_Laps(3,vt_lap_tab[3])>>24;
    if(res) return(res + 6);
    res = Check_Vt_Laps(4,vt_lap_tab[4])>>24;
    if(res) return(res + 8);
    //100ml与50ml圈数差值在(15,60)
    di = vt_lap_tab[1] - vt_lap_tab[0];
    if(di < 15) return(11);
    if(di > 60) return(12);
    //200ml与100ml圈数差值在(30,100)
    di = vt_lap_tab[2] - vt_lap_tab[1];
    if(di < 30) return(13);
    if(di > 100) return(14);
    //400ml与200ml圈数差值在(50,150)
    di = vt_lap_tab[3] - vt_lap_tab[2];
    if(di < 50) return(15);
    if(di > 150) return(16);
    //800ml与400ml圈数差值在(100,300)
    di = vt_lap_tab[4] - vt_lap_tab[3];
    if(di < 100) return(17);
    if(di > 300) return(18);
    return(0);
}

//**********************************************************************
//函数  名:Update_O2_Sen_Curve
//函数功能:更新氧气传感器曲线,计算曲线的k,保存标定点AD值
//入口参数:point--更新点
//            ad--更新点的AD值
//出口参数:无
//**********************************************************************
void Update_O2_Sen_Curve(uint16 ad,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > 1) return;
    
    o2_ad_tab[point] = ad;//更新标定点的AD值
    di = o2_ad_tab[1]-o2_ad_tab[0];
    if (di)
    {
        o2_k = (o2_demar_tab[1]-o2_demar_tab[0])*8192/di;
    }
    else
    {
        o2_k = 0;
    }
}

//**********************************************************************
//函数  名:Update_Vt_Cur
//函数功能:更新潮气量曲线,保存标定点的圈数,计算曲线斜率
//入口参数:laps--圈数;point--标定点
//出口参数:无 
//**********************************************************************
void Update_Vt_Cur(sint32 laps,sint8 point)
{
    sint32 di;
    
    if (point<0) return;
    if (point> VT_DEMAR_POINTS-1) return;
    
    vt_lap_tab[point] = laps;    //更新标定点的圈数
    
    if (point != VT_DEMAR_POINTS-1)    //不是最后一点,更新后一段曲线
    {
        di = vt_lap_tab[point+1]-vt_lap_tab[point];
        if (di)
        {
            vt_k_tab[point] = (vt_demar_tab[point+1]-vt_demar_tab[point])*8192/di;
        }
        else
        {
            vt_k_tab[point] = 0;
        }    
    }
    
    if (point)    //不是第一点,更新前一段的曲线
    {
        di = vt_lap_tab[point]-vt_lap_tab[point-1];
        if (di)
        {
            vt_k_tab[point-1] = (vt_demar_tab[point]-vt_demar_tab[point-1])*8192/di;
        }
        else
        {
            vt_k_tab[point-1] = 0;
        }            
    }
}

//**********************************************************************
//函数  名:Read_Vt_Dem_EEP
//函数功能:从EEPROM中取出VT的标定数据,并计算VT的特征曲线
//入口参数:无
//出口参数:无
//**********************************************************************
void Read_Vt_Dem_EEP(void)
{
    sint32 di;
    
    flag_eep_vte_error = TRUE;
    Read_EEPROM(EEP_VTE,( uint8 *)vt_lap_tab,sizeof(vt_lap_tab));//读取标定数据
    if ((Check_EEP_Vte()==0)&&(!Check_EEP_Wr_ERR()))
    {
        for (di=1; di<VT_DEMAR_POINTS;di+=2)
        {//更新特征曲线
            Update_Vt_Cur(vt_lap_tab[di],di);
        }
        flag_eep_vte_error = FALSE;
    }    
}

//**********************************************************************
//函数  名:Read_Paw_Dem_EEP
//函数功能:从EEPROM中取出Paw的标定数据,并计算Paw的特征曲线
//入口参数:无
//出口参数:无
//**********************************************************************
void Read_Paw_Dem_EEP(void)
{
    sint32 di;
    flag_eep_p_error = TRUE;
    Read_EEPROM(EEP_PIPELINE_P,( uint8 *)paw_ad_tab,sizeof(paw_ad_tab));//读取标定数据
    if ((!Check_EEP_Wr_ERR())&&(Check_EEP_Pipeline_P()==0))    //数据无误
    {
        for (di=1; di<PAW_DEMAR_POINTS;di+=2)
        {//更新特征曲线
            Update_Paw_Curve(paw_ad_tab[di],di);
        }
        flag_eep_p_error = FALSE;
    }
    Read_EEPROM(EEP_ZERO_P,( uint8 *)(&paw_zero_ad),sizeof(paw_zero_ad));//读取校零数据
    if((paw_zero_ad > MAX_P_CAL0_P) || (paw_zero_ad < MIN_P_CAL0_P))
    {//零位压力超范围
        paw_zero_ad = 0;
        Write_EEPROM_2B(EEP_ZERO_P, paw_zero_ad);
    }        
}

//**********************************************************************
//函数  名:Read_O2_Dem_EEP
//函数功能:从EEPROM中取出O2的标定数据,并计算O2的特征曲线
//入口参数:无
//出口参数:无
//**********************************************************************
void Read_O2_Dem_EEP(void)
{
    sint32 di;
    flag_eep_o2_error = TRUE;
    Read_EEPROM(EEP_O2,( uint8 *)o2_ad_tab,sizeof(o2_ad_tab));//读取标定数据
    if ((!Check_EEP_Wr_ERR())&&(Check_EEP_O2()==0))
    {
        for (di=1; di<2;di+=2)
        {//更新特征曲线
            Update_O2_Sen_Curve(o2_ad_tab[di],di);
        }
        flag_eep_o2_error = FALSE;
    }
}

/****************************************************************************
* 函数名: Check_Curve_Data_Valid
* 功能: 检查标定曲线的数据是否有效
* 入口参数: *cdp数据指针,*ydpY轴数据,len长度,min最小值,max最大值,
*             reldt相邻标定点的最小斜率(扩大100倍)
* 出口参数: 1有效,0无效
****************************************************************************/
uint8 Check_Curve_Data_Valid(sint16 *cdp, sint16 *ydp, sint16 len, sint16 min, sint16 max, sint16 reldt)
{
    if(cdp[0]<min) return(FALSE);
    if(cdp[len-1]>max) return(FALSE);
    for(;len>1;len--)
    {
        if((cdp[1]-cdp[0])*100<(ydp[1]-ydp[0])*reldt) return(FALSE);
        cdp++;
        ydp++;
    }
    return(TRUE);
}

//**********************************************************************
//函数  名:Check_Sen_Curve_AD
//函数功能:检查传感器曲线
//入口参数:len--标定点个数
//            ad_buf--标定点的AD缓冲区
//            ad_min_buf--标定点AD最小值
//            ad_max_buf--标定点AD最大值
//出口参数:OK--曲线数据正常
//            ERROR--曲线数据错误
//**********************************************************************
uint8 Check_Sen_Curve_AD(uint16 *ad_buf,uint16 len,const uint16 *ad_min_buf,const uint16 *ad_max_buf)
{
    uint16 i;
    for (i=0; i<len; i++)
    {
        //检查AD值
        if (ad_buf[i] < ad_min_buf[i])//错误
        {
            return (ERROR);
        }
        if (ad_buf[i] > ad_max_buf[i])//错误
        {
            return (ERROR);
        }
    }
    return(OK);
}

//**********************************************************************
//函数  名:Check_Sen_Curve_K
//函数功能:检查传感器曲线
//入口参数:len--标定点个数
//            k_buf--曲线的斜率缓冲区
//            k_min_buf--斜率最小值
//            k_max_buf--斜率最大值
//出口参数:OK--曲线数据正常
//            ERROR--曲线数据错误
//**********************************************************************
uint8 Check_Sen_Curve_K(uint16 len,sint32 *k_buf,sint32* k_min_buf,sint32 *k_max_buf)
{
    uint16 i;
    for (i=0; i<len; i++)
    {
        if (k_buf[i] < k_min_buf[i])//错误
        {
            return (ERROR);
        }
        if (k_buf[i] > k_max_buf[i])//错误
        {
            return (ERROR);
        }            
    }
    return(OK);
}

