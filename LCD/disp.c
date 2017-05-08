//**********************************************************************
//文件名:disp.c
//**********************************************************************
#include "global.h"
#include <stdlib.h>
#include <math.h>
#include "Font_Lib.h"
#include "lcd2478.h"

#include "LCD2478_graph.h"
#include "disp.h"

#include "eeprom.h"

#include "locale.h"

#if COLOR_NUMBER == 16
//16-color mode
//定义字模的掩膜数据(1字节横排字模8点-->1字8图形点)
const uint32 ZIMO_MASK[256]=
{
	0x00000000, 0x0000000F, 0x000000F0, 0x000000FF, 0x00000F00, 0x00000F0F, 0x00000FF0, 0x00000FFF,
	0x0000F000, 0x0000F00F, 0x0000F0F0, 0x0000F0FF, 0x0000FF00, 0x0000FF0F, 0x0000FFF0, 0x0000FFFF,
	0x000F0000, 0x000F000F, 0x000F00F0, 0x000F00FF, 0x000F0F00, 0x000F0F0F, 0x000F0FF0, 0x000F0FFF,
	0x000FF000, 0x000FF00F, 0x000FF0F0, 0x000FF0FF, 0x000FFF00, 0x000FFF0F, 0x000FFFF0, 0x000FFFFF,
	0x00F00000, 0x00F0000F, 0x00F000F0, 0x00F000FF, 0x00F00F00, 0x00F00F0F, 0x00F00FF0, 0x00F00FFF,
	0x00F0F000, 0x00F0F00F, 0x00F0F0F0, 0x00F0F0FF, 0x00F0FF00, 0x00F0FF0F, 0x00F0FFF0, 0x00F0FFFF,
	0x00FF0000, 0x00FF000F, 0x00FF00F0, 0x00FF00FF, 0x00FF0F00, 0x00FF0F0F, 0x00FF0FF0, 0x00FF0FFF,
	0x00FFF000, 0x00FFF00F, 0x00FFF0F0, 0x00FFF0FF, 0x00FFFF00, 0x00FFFF0F, 0x00FFFFF0, 0x00FFFFFF,
	0x0F000000, 0x0F00000F, 0x0F0000F0, 0x0F0000FF, 0x0F000F00, 0x0F000F0F, 0x0F000FF0, 0x0F000FFF,
	0x0F00F000, 0x0F00F00F, 0x0F00F0F0, 0x0F00F0FF, 0x0F00FF00, 0x0F00FF0F, 0x0F00FFF0, 0x0F00FFFF,
	0x0F0F0000, 0x0F0F000F, 0x0F0F00F0, 0x0F0F00FF, 0x0F0F0F00, 0x0F0F0F0F, 0x0F0F0FF0, 0x0F0F0FFF,
	0x0F0FF000, 0x0F0FF00F, 0x0F0FF0F0, 0x0F0FF0FF, 0x0F0FFF00, 0x0F0FFF0F, 0x0F0FFFF0, 0x0F0FFFFF,
	0x0FF00000, 0x0FF0000F, 0x0FF000F0, 0x0FF000FF, 0x0FF00F00, 0x0FF00F0F, 0x0FF00FF0, 0x0FF00FFF,
	0x0FF0F000, 0x0FF0F00F, 0x0FF0F0F0, 0x0FF0F0FF, 0x0FF0FF00, 0x0FF0FF0F, 0x0FF0FFF0, 0x0FF0FFFF,
	0x0FFF0000, 0x0FFF000F, 0x0FFF00F0, 0x0FFF00FF, 0x0FFF0F00, 0x0FFF0F0F, 0x0FFF0FF0, 0x0FFF0FFF,
	0x0FFFF000, 0x0FFFF00F, 0x0FFFF0F0, 0x0FFFF0FF, 0x0FFFFF00, 0x0FFFFF0F, 0x0FFFFFF0, 0x0FFFFFFF,
	0xF0000000, 0xF000000F, 0xF00000F0, 0xF00000FF, 0xF0000F00, 0xF0000F0F, 0xF0000FF0, 0xF0000FFF,
	0xF000F000, 0xF000F00F, 0xF000F0F0, 0xF000F0FF, 0xF000FF00, 0xF000FF0F, 0xF000FFF0, 0xF000FFFF,
	0xF00F0000, 0xF00F000F, 0xF00F00F0, 0xF00F00FF, 0xF00F0F00, 0xF00F0F0F, 0xF00F0FF0, 0xF00F0FFF,
	0xF00FF000, 0xF00FF00F, 0xF00FF0F0, 0xF00FF0FF, 0xF00FFF00, 0xF00FFF0F, 0xF00FFFF0, 0xF00FFFFF,
	0xF0F00000, 0xF0F0000F, 0xF0F000F0, 0xF0F000FF, 0xF0F00F00, 0xF0F00F0F, 0xF0F00FF0, 0xF0F00FFF,
	0xF0F0F000, 0xF0F0F00F, 0xF0F0F0F0, 0xF0F0F0FF, 0xF0F0FF00, 0xF0F0FF0F, 0xF0F0FFF0, 0xF0F0FFFF,
	0xF0FF0000, 0xF0FF000F, 0xF0FF00F0, 0xF0FF00FF, 0xF0FF0F00, 0xF0FF0F0F, 0xF0FF0FF0, 0xF0FF0FFF,
	0xF0FFF000, 0xF0FFF00F, 0xF0FFF0F0, 0xF0FFF0FF, 0xF0FFFF00, 0xF0FFFF0F, 0xF0FFFFF0, 0xF0FFFFFF,
	0xFF000000, 0xFF00000F, 0xFF0000F0, 0xFF0000FF, 0xFF000F00, 0xFF000F0F, 0xFF000FF0, 0xFF000FFF,
	0xFF00F000, 0xFF00F00F, 0xFF00F0F0, 0xFF00F0FF, 0xFF00FF00, 0xFF00FF0F, 0xFF00FFF0, 0xFF00FFFF,
	0xFF0F0000, 0xFF0F000F, 0xFF0F00F0, 0xFF0F00FF, 0xFF0F0F00, 0xFF0F0F0F, 0xFF0F0FF0, 0xFF0F0FFF,
	0xFF0FF000, 0xFF0FF00F, 0xFF0FF0F0, 0xFF0FF0FF, 0xFF0FFF00, 0xFF0FFF0F, 0xFF0FFFF0, 0xFF0FFFFF,
	0xFFF00000, 0xFFF0000F, 0xFFF000F0, 0xFFF000FF, 0xFFF00F00, 0xFFF00F0F, 0xFFF00FF0, 0xFFF00FFF,
	0xFFF0F000, 0xFFF0F00F, 0xFFF0F0F0, 0xFFF0F0FF, 0xFFF0FF00, 0xFFF0FF0F, 0xFFF0FFF0, 0xFFF0FFFF,
	0xFFFF0000, 0xFFFF000F, 0xFFFF00F0, 0xFFFF00FF, 0xFFFF0F00, 0xFFFF0F0F, 0xFFFF0FF0, 0xFFFF0FFF,
	0xFFFFF000, 0xFFFFF00F, 0xFFFFF0F0, 0xFFFFF0FF, 0xFFFFFF00, 0xFFFFFF0F, 0xFFFFFFF0, 0xFFFFFFFF,
};

#endif

const         uint8     *asc_lib_ptr;
const  struct font_strc *hz_lib_ptr;    //中英文字库指针

static sint16 chr_space,line_space;     //字符间距和行间距
static uint8  cur_language;             //语言类型

static uint16 char_col,char_back_col;    //文字区背景色,字符色

/*********************************************************************************************************
*1 名称: Init_Cilib1
*1 功能: 初试化文字输出系统
*1 入口参数: 无
*1 出口参数: 无
*********************************************************************************************************/
void Init_Cilib(void)
{
    Init_LCD2478(); 
    
	asc_lib_ptr   = NULL;
	hz_lib_ptr    = &ST_11;            //只有一种汉字字体
	char_col      = COL_WHITE;   
	char_back_col = COL_BLACK;
	line_space    = 0;	 
    chr_space     = 0;	
	cur_language  = CHINESE;
}
//**********************************************************************
//函数  名:Set_ASC_Lib2
//函数功能:设置显示ASCII字符时所用的字库
//入口参数:asc_font_ptr--ASCII字库指针
//出口参数:无
//**********************************************************************
void Set_ASC_Lib(const  uint8 *asc_font_ptr )
{
    asc_lib_ptr = asc_font_ptr;
}

//**********************************************************************
//函数  名:Set_HZ_Lib3
//函数功能:设置显示中文时所用的字体字库
//入口参数:中文字体的字库指针
//出口参数:无
//**********************************************************************
void Set_HZ_Lib(const struct font_strc *hz_font_ptr)
{
    hz_lib_ptr = hz_font_ptr;
}

//**********************************************************************
//函数  名:Set_Chr_Color4
//函数功能:设置文字显示区域的背景色和文字颜色
//入口参数:chr_color--文字颜色
//           back_color--背景色
//出口参数:无
//**********************************************************************
void Set_Chr_Color(uint16 chr_color,uint16 back_color)
{
    #if COLOR_NUMBER == 16
//16-color mode
	char_col = chr_color & 0x0F;
	char_back_col = back_color & 0x0F;
#else
//65536-color mode
	char_col = chr_color;
	char_back_col = back_color;
#endif
  //  char_col = chr_color;
  //  char_back_col = back_color;
}

//**********************************************************************
//函数  名:Set_Line_Space5
//函数功能:设置显示的行间距
//入口参数:space--行间距
//出口参数:无
//**********************************************************************
void Set_Line_Space(sint16 space)
{
    line_space =  space;
}

//**********************************************************************
//函数  名:Set_Char_Space6
//函数功能:设置显示的字符间距
//入口参数:space--字符间距
//出口参数:无
//**********************************************************************
void Set_Char_Space(sint16 space)
{
    chr_space = space;
}

//**********************************************************************
//函数  名:Set_Language_Type7
//函数功能:设置显示语言
//入口参数:language--语言类型
//出口参数:无
//**********************************************************************
void Set_Language_Type(uint16 language)
{
    cur_language = language;
}

//**********************************************************************
//函数  名:Get_Language_Type8
//函数功能:获取当前的显示语言
//入口参数:无
//出口参数:CHINESE--中文
//            ENGLISH--英文
//**********************************************************************
 uint8 Get_Language_Type(void)
{
    return (cur_language);
}

/*********************************************************************************************************
* 名称: Is_HZstr9
* 功能: 判断是否是汉字字符串
* 入口参数: *str字符串
* 出口参数: 1是, 0不是
*********************************************************************************************************/
uint8 Is_HZstr(char *str)
{
	while(*str!='\x0')
	{//未结束
		if((*str<0xA0) && (*str!='\n')) return(0);
		str++;
	}
	return(1);
}

/*********************************************************************************************************
* 名称: Get_Char_Height
* 功能: 返回输入字符的高度
* 入口参数: ch为字符值,对于ASC字符则只用低字节(高字节为0),对于汉字则占2个字节
* 出口参数: 字符高度
*********************************************************************************************************/
uint16 Get_Char_Height(uint16 ch)
{
	if(ch<0xA0)
	{//ASC字符
		if(asc_lib_ptr==NULL) return(0);
		return(asc_lib_ptr[1]);
	}
	//汉字符
	if(hz_lib_ptr==NULL) return(0);
	return(hz_lib_ptr->height);
}

/*********************************************************************************************************
* 名称: Get_Char_Width
* 功能: 返回输入字符的宽度
* 入口参数: ch为字符值,对于ASC字符则只用低字节(高字节为0),对于汉字则占2个字节
* 出口参数: 字符宽度
*********************************************************************************************************/
uint16 Get_Char_Width(uint16 ch)
{
	if(ch<0xA0)
	{//ASC字符
		if(asc_lib_ptr==NULL) return(0);
		if(ch<0x20) return(asc_lib_ptr[0]);
		return(asc_lib_ptr[ch-0x20+2]);
	}
	//汉字符
	if(hz_lib_ptr==NULL) return(0);
	return(hz_lib_ptr->width);
}


//**********************************************************************
//函数  名:Save_Lg_Type12
//函数功能:保存语言类型设置
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Lg_Type(void)
{
    if (Write_EEPROM(EEP_LANGUAGE_ADDR,( uint8*)(&cur_language),sizeof(cur_language)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Lg_Type_EEP13
//函数功能:从EEPROM中读出语言类型,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Lg_Type_EEP(void)
{
    if (Read_EEPROM(EEP_LANGUAGE_ADDR,( uint8*)(&cur_language),sizeof(cur_language)) )
    {
        cur_language = LG_TYPE_DEF;
        return(ERROR);
    }
    else
    {
        if ((cur_language != CHINESE)&&(cur_language != ENGLISH))
        {
            cur_language = LG_TYPE_DEF;
            Save_Lg_Type();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Language_Str14
//函数功能:根据字体设置选择正确的字符串
//入口参数:ch_str--中文字符串
//           en_str--英文字符串
//出口参数:与字体设置相对应的字符串
//**********************************************************************
uint8 * Language_Str(uint8 *ch_ptr,uint8 *en_ptr)
{
    if (cur_language == CHINESE)
    {
        return (ch_ptr);
    }
    else
    {
        return (en_ptr);
    }
}

//**********************************************************************
//函数  名:Language_Pos15
//函数功能:根据字体设置选择正确的显示位置
//入口参数:ch_pos--中文的显示位置
//           en_pos--英文的显示位置
//出口参数:与字体设置相对应的显示位置
//**********************************************************************
uint16 Language_Pos(uint16 ch_pos,uint16 en_pos)
{
    if (cur_language == CHINESE)
    {
        return (ch_pos);
    }
    else
    {
        return (en_pos);
    }
}

/*********************************************************************************************************
* 名称: Put_Char
* 功能: 在指定位置显示一个字符(点阵不能超过64*64)
* 入口参数: (cx,cy)是输出字符的坐标,ch为字符值,对于ASC字符则只用低字节(高字节为0),对于汉字则占2个字节
* 出口参数: 无
* 字库格式: 以字节为单位每位对应1点,b7~b0依此对应从左到右8个点,横排方式.
*********************************************************************************************************/
uint16  Put_Char(uint16 cx, uint16 cy, uint16 ch)
{
const uint8 *libptr;
uint16 wid,hig,di;
uint32 chsize,ascw,hzn;
#if COLOR_NUMBER == 16
//16-color mode
uint32 *chptr,fc,gc,mask;
uint32 charmap[1600];	//字模位图区,最大支持4096点(64x64)字符
#else                   //字模位图区,最大支持12800点(80x160)字符
//65536-color mode
uint16 *dispbuf,j,fc,gc,wm;
uint32 zimo;
#endif
//获取字符的高度和宽度
	wid = Get_Char_Width(ch);
	if(wid==0) return;	//无宽度则退出
	hig = Get_Char_Height(ch);
	if(hig==0) return;	//无高度则退出
	chsize = ((wid + 7) >> 3) * hig;	//字模大小
#if COLOR_NUMBER == 16
//16-color mode
	/*if(chsize>512)
	{//字模太大则显示一字符大小的空格并返回
		Clr_Bar(cx, cy, wid, hig, char_back_col);
		return;
	}*/
#else
//65536-color mode
	if(cx+wid>SCR_XSIZE) return;	//字宽超出显示范围
	if(cy+hig>SCR_YSIZE) return;	//字高超出显示范围
#endif
//获取字符字模在字库中的位置
	if(ch<0xA0)
	{//是ASC字符
		if(ch<0x20)
		{//ASC字符不在字库范围内,则显示一空格并返回
			Clr_Bar(cx, cy, wid, hig, char_back_col);
			return;
		}
		libptr = &asc_lib_ptr[2];
		for(ascw=0; ch>0x20; ch--)
		{//前面字符宽度累加(以字节为单位)
			ascw += (*libptr + 7) >> 3;
			libptr++;
		}
		libptr = &asc_lib_ptr[130+ascw*hig];
	}
	else{//是汉字字符
	  for (di=0; di<hz_lib_ptr->chr_sum; di++)    //搜索匹配汉字
        {
            if ( (hz_lib_ptr->chr_code)[di] == ch)//找到汉字
            {
                break;
            }
        }
        if (di >=hz_lib_ptr->chr_sum )    //字库中无该汉字
        {
            Clr_Bar(cx, cy, wid, hig, char_back_col);
			return(cx+wid);
        }
        libptr = &(hz_lib_ptr->chr_lattice_ptr)[di*((hz_lib_ptr->width+7)>>3)*hz_lib_ptr->height];
     	}
#if COLOR_NUMBER == 16
//16-color mode
//将显示的字符位图转换为put_image使用的位图格式
	chptr = charmap;
	*chptr = (hig << 16) + wid;
	fc = char_col * 0x11111111;
	gc = char_back_col * 0x11111111;
	//开始将字模转换为put_image使用的位图格式
	for(; chsize>0; chsize--)
	{//字符顺序取模
		mask = ZIMO_MASK[*libptr++];
		*++chptr = (fc & mask) | (gc & ~mask);
	}
//显示字符
	Put_Image(cx, cy, charmap);
#else
//65536-color mode
	fc = char_col;
	gc = char_back_col;
	wm = Get_Write_Mode();
	dispbuf = &disp_buffer[LINE_SIZE*cy+cx];		//calculate pixel address of dispbuf
	if(wm==0) for(; hig>0; hig--)
	{//cover write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] = fc; else dispbuf[j] = gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
	else if(wm==1) for(; hig>0; hig--)
	{//xor write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] ^= fc; else dispbuf[j] ^= gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
	else if(wm==2) for(; hig>0; hig--)
	{//or write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] |= fc; else dispbuf[j] |= gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
	else if(wm==3) for(; hig>0; hig--)
	{//and write
		zimo = 0x10000;
		for(j=0; j<wid; j++)
		{
			if(zimo&0xFFFF0000) zimo = 0x100 | *libptr++;
			if(zimo&0x80) dispbuf[j] &= fc; else dispbuf[j] &= gc;
			zimo <<= 1;
		}
		dispbuf += LINE_SIZE;			//calculate next line start address
	}
#endif
}

//**********************************************************************
//函数  名:Get_Line_Height17
//函数功能:获取一行字符串的高度(选择字符串中最高的字符高度)
//入口参数: *str--输入的字符串(以换行或字串结尾符作为结束符)
//出口参数:字符串的显示高度
//**********************************************************************
uint16 Get_Line_Height(uint8 *str)
{
    uint16 s=0,h1,h2;

    while((*str!='\x0') && (*str!='\n'))
    {//不是字符串结尾或行尾则继续
        if(*str++ < 0xA0)
        {//是ASC字符
            s |= 1;
            continue;
        }
        //是中文字符
        if((*str=='\x0') || (*str=='\n')) break;    //行尾或结束
        s |= 2;
        str++;
    }
    h1 = Get_Char_Height('0');
    h2 = Get_Char_Height(0x100);
    if(s == 1) return(h1);                            //仅有ASC字符
    if(s == 2) return(h2);                            //仅有中文字符
    //既有ASC又有中文或空串,则取高的作为总的高度
    return((h1>h2) ? h1:h2);
}

//**********************************************************************
//函数  名:Get_Line_Width18
//函数功能:获取输入的当前行的字符串的显示宽度(包括字符间隔)
//入口参数:*str--为输入的字符串(以换行或字串结尾符作为结束符)
//出口参数:字符串的显示宽度
//**********************************************************************
uint16 Get_Line_Width(uint8 *str)
{
    uint32 w=0;

    while((*str!='\x0') && (*str!='\n'))
    {//不是字符串结尾或行尾则继续
        if(*str<0xA0)
        {//是ASC字符
            w += Get_Char_Width(*str) + chr_space;
        }
        else
        {//是中文字符
            str++;
            if((*str=='\x0') || (*str=='\n')) break;    //行尾或结束
            w += Get_Char_Width(0x100) + chr_space;
        }
        str++;
    }
    if(w==0) return(0);
    return(w - chr_space);
}

//**********************************************************************
//函数  名:Put_Line_Str19
//函数功能:在指定位置显示当前行字串(可中英文混合)
//入口参数:(x,y)--输出字符串的坐标
//            *str--出的字符串(以换行或字串结尾符作为结束符)
//出口参数:光标位置
//**********************************************************************
uint16 Put_Line_Str(uint16 cx, uint16 cy, uint8 *str)
{
uint16 ch,hig,wid,lh;
uint32 ysp1,ysp2;
//begin
	if((*str=='\x0') || (*str=='\n')) return;	//首字符为行尾或结束则立即返回
	lh = Get_Line_Height(str);
	while(1)
	{
		ch = (uint8)(*str);
		if(ch>=0xA0)
		{//字符值大于等于0xA0则表示是汉字,否则就是ASC字符
			str++;
			if((*str=='\x0') || (*str=='\n')) break;	//行尾或结束
			ch = ch * 256 + (uint8)(*str);				//取出汉字内码(2字节)
		}
		wid = Get_Char_Width(ch);		//读字符宽
		hig = Get_Char_Height(ch);		//读字符高
		ysp1 = (lh - hig) / 2;			//计算上面间隙的高度
		ysp2 = lh - hig - ysp1;			//计算下面间隙的高度
		if(ysp1>0)
		{//如果上面间隙>0,则用底色清除字符上方
			Clr_Bar(cx, cy, wid, ysp1, char_back_col);
		}
		if(ysp2>0)
		{//如果下面间隙>0,则用底色清除字符下方
			Clr_Bar(cx, cy+lh-ysp2, wid, ysp2, char_back_col);
		}
		Put_Char(cx, cy+ysp1, ch);		//输出一字符(上下居中对齐)
		str++;
		if((*str=='\x0') || (*str=='\n')) break;			//行尾或结束
		if(chr_space>0) Clr_Bar(cx+wid, cy, chr_space, lh, char_back_col);	//用底色清除字符间隔
		cx += wid + chr_space;			//下一字符的横坐标
	}   
}

//**********************************************************************
//函数  名:Get_Str_Height20
//函数功能:获取输入的段落的显示高度(包括行间隔)
//入口参数:*str--输入的字符串(支持多行段落)
//出口参数:段落的显示高度
//**********************************************************************
uint16 Get_Str_Height(uint8 *str)
{
    uint16 lh,height=0,flg=1;

    lh = Get_Line_Height("\n");
    while(*str != '\x0')
    {//不是字符串结尾则继续
        if(*str == '\n')
        {//碰到换行符则累加行高+行间隔
            height += lh + line_space;
            flg = 1;
            str++;
            continue;
        }
        if(flg)
        {
            lh = Get_Line_Height(str);        //获取新行的行高
            flg = 0;
        }
        str++;
    }
    if(flg) return(height);
    return(height + lh);        //最后一行高度未加,所以要加上
}

//**********************************************************************
//函数  名:Get_Str_Width21
//函数功能:获取输入的段落的最宽行的显示宽度(包括字符间隔)
//入口参数:    *str--输入的字符串(支持多行段落)
//出口参数:段落的最宽行的显示宽度
//**********************************************************************
uint16 Get_Str_Width(uint8 *str)
{
    uint16 max_wid=0,width;

    while(*str != '\x0')
    {//不是字符串结尾则继续
        width = Get_Line_Width(str);
        if(max_wid < width)
        {//取最宽的行宽
            max_wid = width;
        }
        while(*str++ != '\n')
        {//向后搜索行尾
            if(*str == '\x0') break;
        }
    }
    return(max_wid);
}

//**********************************************************************
//函数  名:Put_Str22
//函数功能:在指定位置显示字串段落(可中英文混合)
//入口参数:(cx,cy)--输出字符串的坐标
//            *str--输出的字符串(支持段落)
//出口参数:无
//**********************************************************************
void Put_Str(uint16 cx, uint16 cy, uint8 *str)
{
uint16 lineh,linew=0,flag=1;
//begin
	lineh = Get_Line_Height("\n");
	while(*str!='\x0')
	{//不是字符串结尾则继续
		if(*str=='\n')
		{//碰到行尾
			if(!flag)
			{//若上一行有内容则需要清除行间隔
				if(line_space>0) Clr_Bar(cx, cy+lineh, linew, line_space, char_back_col);
				flag = 1;
			}
			cy += lineh + line_space;		//换行
			str++;
			continue;
		}
		if(flag)
		{//输出一行字串并获取行宽和行高
			lineh = Get_Line_Height(str);
			linew = Get_Line_Width(str);
			Put_Line_Str(cx, cy, str);
			flag = 0;
		}
		str++;
	}
}	

/*********************************************************************************************************
* 名称: Put_Data23
* 功能: 在指定位置按一定格式显示数据
* 入口参数: (cx,cy)是输出字符串的坐标,*msg为转换格式字串,val为显示的数据
* 出口参数: 无
;modify by wangjg :del *msg
*********************************************************************************************************/
void Put_Data(uint16 cx, uint16 cy,char *msg,uint32 val)
{
    char dstr[20];
    //setlocale(LC_ALL,"C");
	sprintf(dstr,msg, val);
	Put_Str(cx, cy, dstr);
}
/*********************************************************************************************************
* 名称: Put_Data24
* 功能: 在指定位置按一定格式显示数据
* 入口参数: (cx,cy)是输出字符串的坐标,*msg为转换格式字串,val为显示的数据
* 出口参数: 无
;modify by wangjg :del *msg
*********************************************************************************************************/
void Put_Data_Float(uint16 cx, uint16 cy,char *msg,float val)
{
    char dstr[20];
    //setlocale(LC_ALL,"C");
	sprintf(dstr,msg, val);
	Put_Str(cx, cy, dstr);
}

//**********************************************************************
//函数  名:Save_Area
//函数功能:保存指定区域的显示数据
//入口参数:(x1,y1)--区域左上角坐标
//            (x2,y2)--区域右下角坐标
//            buf--显示数据保存的指针
//出口参数:无
//**********************************************************************
void Save_Area(uint16 x1,uint16 y1,
               uint16 x2,uint16 y2,uint32 * buf)
{
    uint32 *disp_buf;
    uint16 byte_width,start_byte,end_byte;
    uint32 di;
    
    if (y1 > y2) return;
    if (x1 > x2) return;
    if (NULL == buf) return;
    start_byte = x1>>3;    //每行的起始字节
    end_byte = x2>>3;    //每行的末字节

    byte_width = end_byte-start_byte+1;//每行所占的字节数
    //buf_temp = buf;     //目的缓存
    for(;y1<=y2;y1++)
    {
        disp_buf = &disp_buffer[y1*LINE_SIZE+start_byte]; //源缓存
        for (di=0; di<byte_width; di++)
        {
            *buf++ = *disp_buf++ ;
        }
    }        
}

//**********************************************************************
//函数  名:Recover_Area
//函数功能:恢复指定区域的显示数据
//入口参数:(x1,y1)--区域左上角坐标
//            (x2,y2)--区域右下角坐标
//            buf--保存恢复数据的指针
//出口参数:无
//**********************************************************************
void Recover_Area(uint16 x1,uint16 y1,
               uint16 x2,uint16 y2,uint32 * buf)
{
    uint32 *disp_buf;
    uint16 byte_width,start_byte,end_byte;
    uint32 di;
    
    if (y1 > y2) return;
    if (x1 > x2) return;
    if (buf == NULL) return;
    
    start_byte = x1>>3;//行的首字节
    end_byte = x2>>3;//行的末字节

    byte_width = end_byte-start_byte+1;//每行所占的字节数
    for(;y1<=y2;y1++)
    {
        disp_buf = &disp_buffer[y1*LINE_SIZE+start_byte];
        for (di=0; di<byte_width; di++)
        {
            *disp_buf++ = *buf++;
        }
    }    
}

                                     