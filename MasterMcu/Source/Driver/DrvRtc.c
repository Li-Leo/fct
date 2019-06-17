/*******************说明:**************************
将实时时钟数据通过LCD1602显示
--------------------------------------------------
基于战神单片机工作室51/AVR最小系统板及DS1302
实时时钟模块编写
淘宝店铺地址: http://zsmcu.taobao.com/
QQ:284083167
手机:15016732495
**************************************************/
/*******************************************
函数名称：delay
功    能：延时一段时间
参    数：time--延时长度
返回值  ：无
********************************************/
#include "DrvRtc.h"
#include "Common.h"

TS_TIME_INFO g_current_time;
T_U8 g_write_time_buf[16];
T_U8 g_read_time_buf[8]={0};

void delay(unsigned int time)
{
    unsigned int i;
    for(i=0;i<time;i++)
      asm("nop");
}

                       /*秒   分    时   日  月   星期    年*/

//功能:延时1毫秒
//入口参数:x
//出口参数:无
//说明:当晶振为12M时，j<112；当晶振为11.0592M时，j<122

//功能：12us延时
//STC89C52为1T单片机,即1个时钟/机器周期,速度为AT89C52的12倍

  
//DS1302初始化函数
void Ds1302Init(void) 
{
    DS_RST_OUT;  //RST对应的IO设置为输出状态
    DS_SCL_OUT;  //SCLK对应的IO设置为输出状态
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    delay(10);
    DS_SCL1;    //SCLK=1
}
//向DS1302写入一字节数据
void Write1Byte(T_U8 wdata) 
{
    T_U8 i;
    DS_SDA_OUT;     //SDA对应的IO设置为输出状态
    DS_RST1;        //REST=1;

    for(i=8; i>0; i--) 
    {
        if(wdata&0x01)  DS_SDA1;
        else            DS_SDA0;
        DS_SCL0;
        delay(100); 
        DS_SCL1;
        delay(100); 
        wdata >>=1; 
    } 
}

//从DS1302读出一字节数据
T_U8 Read1Byte() 
{

T_U8 i;
    T_U8 rdata=0X00;

    DS_SDA_IN;  //SDA对应的IO设置为输入状态
    DS_RST1;    //REST=1;

    for(i=8; i>0; i--)
    {
        DS_SCL1;
        delay(100);
        DS_SCL0;
        delay(100);
        rdata >>=1;
        if(DS_SDA_BIT)  rdata |= 0x80;
    } 
    
    return(rdata); 
}

/*******************************************
函数名称：W_Data
功    能：向某个寄存器写入一个字节数据
参    数：addr--寄存器地址
          wdata--写入的数据
返回值  ：无
********************************************/
void WData(T_U8 addr, T_U8 wdata)
{
    DS_RST0; 
    DS_SCL0;   
    _NOP();
    DS_RST1;
    Write1Byte(addr);   //写入地址
    Write1Byte(wdata);  //写入数据
    DS_SCL1;
    DS_RST0;
} 
/*******************************************
函数名称：R_Data
功    能：从某个寄存器读出一个字节数据
参    数：addr--寄存器地址
返回值  ：读出的数据
********************************************/
T_U8 RData(T_U8 addr)
{
    T_U8 rdata;
    
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(addr);    //写入地址
    rdata = Read1Byte();  //读出数据 
    DS_SCL1;
    DS_RST0;
    
    return(rdata);
}
/*******************************************
函数名称：BurstWrite1302
功    能：以burst方式向DS1302写入批量时间数据
参    数：ptr--指向时间数据存放地址的指针
返回值  ：读出的数据
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstWrite1302(T_U8 *ptr)
{
    T_U8 i;
    
    WData(0x8e,0x00);      //允许写入     
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xbe);       // 0xbe:时钟多字节写入命令 
    for (i=8; i>0; i--) 
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0;
    WData(0x8e,0x80);      // 禁止写入
} 
/*******************************************
函数名称：BurstRead1302
功    能：以burst方式从DS1302读出批量时间数据
参    数：ptr--指向存放时间数据地址的指针
返回值  ：无
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstRead1302(T_U8 *ptr)
{
    T_U8 i;
  
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xbf);             //0xbf:时钟多字节读命令
    for (i=8; i>0; i--) 
    {
       *ptr++ = Read1Byte(); 
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
函数名称：BurstWriteRAM
功    能：以burst方式向DS1302的RAM中写入批量数据
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：共写入31个字节的数据
********************************************/
void BurstWriteRAM(T_U8 *ptr)
{
    T_U8 i;
    
    WData(0x8e,0x00);         //允许写入
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xfe);          //0xfe:RAM多字节写命令 
    for (i = 31; i>0; i--)     //RAM共有31个字节
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0; 
    WData(0x8e,0x80);          //禁止写入
} 
/*******************************************
函数名称：BurstReadRAM
功    能：以burst方式从DS1302的RAM中读出批量数据
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：共读出31个字节的数据
********************************************/
void BurstReadRAM(T_U8 *ptr)
{
    T_U8 i;
  
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xff);             //0xff:RAM的多字节读命令 
    for (i=31; i>0; i--) 
    {
       *ptr++ = Read1Byte();   
    }
    DS_SCL1;
    DS_RST0;
}

static T_U8 ConvertDecToBcd(T_U32 dec)
{
	T_U8 bcd;
	bcd = (dec / 10) * 16 + dec % 10;
	return bcd;
}

/*******************************************
函数名称：WriteDS1302
功    能：设置DS1302内部的时间
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：写入数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void IsSetDS1302(TE_TIME_STYLE type,T_U32 data) 
{
    WData(0x8e,0x00);    //允许写入
    switch(type)
    {
      case E_TIME_YEAR:
	  	WData(0x8C,ConvertDecToBcd(data));
		break;
      case E_TIME_MONTH:
	  	WData(0x88,ConvertDecToBcd(data));
		break;
      case E_TIME_DATE:
	  	WData(0x86,ConvertDecToBcd(data));
		break;
      case E_TIME_DAY:
	  	WData(0x8A,ConvertDecToBcd(data));
		break;
      case E_TIME_HOUR:
	  	WData(0x84,ConvertDecToBcd(data));
		break;
      case E_TIME_MINUTE:
	  	WData(0x82,ConvertDecToBcd(data));
		break;
      case E_TIME_SECOND:
	  	WData(0x80,ConvertDecToBcd(data));
		break;
      default:
	  	break;
    }
   
    WData(0x8e,0x80);      //禁止
}
/*******************************************
函数名称：Set_DS1302
功    能：设置DS1302内部的时间
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：写入数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void SetDS1302(T_U8 *ptr) 
{
    T_U8 i;
    //ptr=&g_write_time_buf[0];
    T_U8 addr = 0x80; 
    
    WData(0x8e,0x00);    //允许写入
    
    for(i =8;i>0;i--)
    { 
        WData(addr,*ptr++); 
        addr += 2;
    }
    WData(0x8e,0x80);      //禁止
}
/*******************************************
函数名称：Get_DS1302
功    能：读取DS1302内部的时间
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：读出数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void GetDS1302(void) 
{
    T_U8 i;
    T_U8 addr=0x81;
    
    for(i=0;i<7;i++)
    {
        g_read_time_buf[i]=RData(addr);
        addr+=2;
    }
}

static T_U32 ConvertBcdToDec(T_U8 bcd)
{
	T_U32 dec;
	dec = (bcd / 16) * 10 + bcd % 16;
	return dec;
}

void Ds1302TimeRead(void)
{
  T_U8 read_time_buf[8];

  BurstRead1302(read_time_buf);
  g_current_time.year = 2000 + ConvertBcdToDec(read_time_buf[6]);//读取秒数据，00-59
  g_current_time.month = ConvertBcdToDec(read_time_buf[4]);//获取月份数据
  g_current_time.date = ConvertBcdToDec(read_time_buf[3]);//读取日数据，01-31，01-30，01-29，01-28
  g_current_time.hour = ConvertBcdToDec(read_time_buf[2]);//读取小时数据，00-23
  g_current_time.minute = ConvertBcdToDec(read_time_buf[1]); //读取分钟数据，00-59
  g_current_time.second = ConvertBcdToDec(read_time_buf[0]);//读取秒数据，00-59
  g_current_time.week = ConvertBcdToDec(read_time_buf[5]); //读取星期数据，01-07
  if(g_current_time.week<1 || g_current_time.week>7)
  {
    g_current_time.week = 1;
    M_Assert(0);
  }

  //reset pin to reduce power consume
    DS_SDA_OUT;  //SDA
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    DS_SDA0;
}

