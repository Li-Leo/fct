/*******************˵��:**************************
��ʵʱʱ������ͨ��LCD1602��ʾ
--------------------------------------------------
����ս��Ƭ��������51/AVR��Сϵͳ�弰DS1302
ʵʱʱ��ģ���д
�Ա����̵�ַ: http://zsmcu.taobao.com/
QQ:284083167
�ֻ�:15016732495
**************************************************/
/*******************************************
�������ƣ�delay
��    �ܣ���ʱһ��ʱ��
��    ����time--��ʱ����
����ֵ  ����
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

                       /*��   ��    ʱ   ��  ��   ����    ��*/

//����:��ʱ1����
//��ڲ���:x
//���ڲ���:��
//˵��:������Ϊ12Mʱ��j<112��������Ϊ11.0592Mʱ��j<122

//���ܣ�12us��ʱ
//STC89C52Ϊ1T��Ƭ��,��1��ʱ��/��������,�ٶ�ΪAT89C52��12��

  
//DS1302��ʼ������
void Ds1302Init(void) 
{
    DS_RST_OUT;  //RST��Ӧ��IO����Ϊ���״̬
    DS_SCL_OUT;  //SCLK��Ӧ��IO����Ϊ���״̬
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    delay(10);
    DS_SCL1;    //SCLK=1
}
//��DS1302д��һ�ֽ�����
void Write1Byte(T_U8 wdata) 
{
    T_U8 i;
    DS_SDA_OUT;     //SDA��Ӧ��IO����Ϊ���״̬
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

//��DS1302����һ�ֽ�����
T_U8 Read1Byte() 
{

T_U8 i;
    T_U8 rdata=0X00;

    DS_SDA_IN;  //SDA��Ӧ��IO����Ϊ����״̬
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
�������ƣ�W_Data
��    �ܣ���ĳ���Ĵ���д��һ���ֽ�����
��    ����addr--�Ĵ�����ַ
          wdata--д�������
����ֵ  ����
********************************************/
void WData(T_U8 addr, T_U8 wdata)
{
    DS_RST0; 
    DS_SCL0;   
    _NOP();
    DS_RST1;
    Write1Byte(addr);   //д���ַ
    Write1Byte(wdata);  //д������
    DS_SCL1;
    DS_RST0;
} 
/*******************************************
�������ƣ�R_Data
��    �ܣ���ĳ���Ĵ�������һ���ֽ�����
��    ����addr--�Ĵ�����ַ
����ֵ  ������������
********************************************/
T_U8 RData(T_U8 addr)
{
    T_U8 rdata;
    
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(addr);    //д���ַ
    rdata = Read1Byte();  //�������� 
    DS_SCL1;
    DS_RST0;
    
    return(rdata);
}
/*******************************************
�������ƣ�BurstWrite1302
��    �ܣ���burst��ʽ��DS1302д������ʱ������
��    ����ptr--ָ��ʱ�����ݴ�ŵ�ַ��ָ��
����ֵ  ������������
˵    ����ʱ�����ݵĴ�Ÿ�ʽ�ǣ�
          �룬�֣�ʱ���գ��£����ڣ��꣬����
            ��7�����ݣ�BCD��ʽ��+1�����ơ�
********************************************/
void BurstWrite1302(T_U8 *ptr)
{
    T_U8 i;
    
    WData(0x8e,0x00);      //����д��     
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xbe);       // 0xbe:ʱ�Ӷ��ֽ�д������ 
    for (i=8; i>0; i--) 
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0;
    WData(0x8e,0x80);      // ��ֹд��
} 
/*******************************************
�������ƣ�BurstRead1302
��    �ܣ���burst��ʽ��DS1302��������ʱ������
��    ����ptr--ָ����ʱ�����ݵ�ַ��ָ��
����ֵ  ����
˵    ����ʱ�����ݵĴ�Ÿ�ʽ�ǣ�
          �룬�֣�ʱ���գ��£����ڣ��꣬����
            ��7�����ݣ�BCD��ʽ��+1�����ơ�
********************************************/
void BurstRead1302(T_U8 *ptr)
{
    T_U8 i;
  
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xbf);             //0xbf:ʱ�Ӷ��ֽڶ�����
    for (i=8; i>0; i--) 
    {
       *ptr++ = Read1Byte(); 
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
�������ƣ�BurstWriteRAM
��    �ܣ���burst��ʽ��DS1302��RAM��д����������
��    ����ptr--ָ�������ݵ�ַ��ָ��
����ֵ  ����
˵��    ����д��31���ֽڵ�����
********************************************/
void BurstWriteRAM(T_U8 *ptr)
{
    T_U8 i;
    
    WData(0x8e,0x00);         //����д��
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xfe);          //0xfe:RAM���ֽ�д���� 
    for (i = 31; i>0; i--)     //RAM����31���ֽ�
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0; 
    WData(0x8e,0x80);          //��ֹд��
} 
/*******************************************
�������ƣ�BurstReadRAM
��    �ܣ���burst��ʽ��DS1302��RAM�ж�����������
��    ����ptr--ָ�����ݴ�ŵ�ַ��ָ��
����ֵ  ����
˵��    ��������31���ֽڵ�����
********************************************/
void BurstReadRAM(T_U8 *ptr)
{
    T_U8 i;
  
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(0xff);             //0xff:RAM�Ķ��ֽڶ����� 
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
�������ƣ�WriteDS1302
��    �ܣ�����DS1302�ڲ���ʱ��
��    ����ptr--ָ�������ݵ�ַ��ָ��
����ֵ  ����
˵��    ��д�����ݵĸ�ʽ��
            �� �� ʱ �� �� ���� ��  ����7���ֽڡ�
********************************************/
void IsSetDS1302(TE_TIME_STYLE type,T_U32 data) 
{
    WData(0x8e,0x00);    //����д��
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
   
    WData(0x8e,0x80);      //��ֹ
}
/*******************************************
�������ƣ�Set_DS1302
��    �ܣ�����DS1302�ڲ���ʱ��
��    ����ptr--ָ�������ݵ�ַ��ָ��
����ֵ  ����
˵��    ��д�����ݵĸ�ʽ��
            �� �� ʱ �� �� ���� ��  ����7���ֽڡ�
********************************************/
void SetDS1302(T_U8 *ptr) 
{
    T_U8 i;
    //ptr=&g_write_time_buf[0];
    T_U8 addr = 0x80; 
    
    WData(0x8e,0x00);    //����д��
    
    for(i =8;i>0;i--)
    { 
        WData(addr,*ptr++); 
        addr += 2;
    }
    WData(0x8e,0x80);      //��ֹ
}
/*******************************************
�������ƣ�Get_DS1302
��    �ܣ���ȡDS1302�ڲ���ʱ��
��    ����ptr--ָ�����ݴ�ŵ�ַ��ָ��
����ֵ  ����
˵��    ���������ݵĸ�ʽ��
            �� �� ʱ �� �� ���� ��  ����7���ֽڡ�
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
  g_current_time.year = 2000 + ConvertBcdToDec(read_time_buf[6]);//��ȡ�����ݣ�00-59
  g_current_time.month = ConvertBcdToDec(read_time_buf[4]);//��ȡ�·�����
  g_current_time.date = ConvertBcdToDec(read_time_buf[3]);//��ȡ�����ݣ�01-31��01-30��01-29��01-28
  g_current_time.hour = ConvertBcdToDec(read_time_buf[2]);//��ȡСʱ���ݣ�00-23
  g_current_time.minute = ConvertBcdToDec(read_time_buf[1]); //��ȡ�������ݣ�00-59
  g_current_time.second = ConvertBcdToDec(read_time_buf[0]);//��ȡ�����ݣ�00-59
  g_current_time.week = ConvertBcdToDec(read_time_buf[5]); //��ȡ�������ݣ�01-07
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

