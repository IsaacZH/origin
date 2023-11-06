//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖÐ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼þ Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : Evk123
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üÐÞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : 0.69´çOLED ½Ó¿ÚÑÝÊ¾Àý³Ì(STM32F103ZEÏµÁÐIIC)
//              ËµÃ÷: 
//              ----------------------------------------------------------------
//              GND   µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              SCL   ½ÓPB8£¨SCL£©
//              SDA   ½ÓPB7£¨SDA£©            
//              ----------------------------------------------------------------
//Copyright(C) ÖÐ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////£

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "drv_haltick.h"
//OLEDµÄÏÔ´æ
//´æ·Å¸ñÊ½ÈçÏÂ.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
#if 1
//IO¿Ú·½ÏòÉèÖÃ
#define SDA_IN() 	{GPIOB->MODER&=~((uint32_t)3<<(7*2));GPIOB->MODER|=(uint32_t)0<<(7*2);}	//PB15ÊäÈëÄ£Ê½
#define SDA_OUT() 	{GPIOB->MODER&=~((uint32_t)3<<(7*2));GPIOB->MODER|=(uint32_t)1<<(7*2);}	//PB15ÊäÈëÄ£Ê½
//IO¿Ú²Ù×÷º¯Êý
#define IIC_SCL 	PBout(8)
#define IIC_SDA 	PBout(7)
#define READ_SDA 	PBin(7)	

uint8_t OLED_GRAM[144][4];

//·´ÏÔº¯Êý
void OLED_ColorTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//Õý³£ÏÔÊ¾
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//·´É«ÏÔÊ¾
		}
}

//ÆÁÄ»Ðý×ª180¶È
void OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//Õý³£ÏÔÊ¾
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//·´×ªÏÔÊ¾
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

//Ð´ÈëÒ»¸ö×Ö½Ú
void Send_Byte(uint8_t dat)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//½«datµÄ8Î»´Ó×î¸ßÎ»ÒÀ´ÎÐ´Èë
		{
			OLED_SDIN_Set();
    }
		else
		{
			OLED_SDIN_Clr();
    }
		delay_us(4);	
		OLED_SCLK_Set();
		delay_us(4);	
		OLED_SCLK_Clr();//½«Ê±ÖÓÐÅºÅÉèÖÃÎªµÍµçÆ½
		dat<<=1;
  }
}

//¸üÐÂÏÔ´æµ½OLED	
void OLED_Refresh(void)
{
	uint8_t i,n;
	for(i=0;i<4;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD); //ÉèÖÃÐÐÆðÊ¼µØÖ·
		OLED_WR_Byte(0x00,OLED_CMD);   //ÉèÖÃµÍÁÐÆðÊ¼µØÖ·
		OLED_WR_Byte(0x10,OLED_CMD);   //ÉèÖÃ¸ßÁÐÆðÊ¼µØÖ·
		OLED_IIC_Start();
		Send_Byte(0x78);
		OLED_IIC_Wait_Ack();
		Send_Byte(0x40);
		OLED_IIC_Wait_Ack();
		for(n=0;n<128;n++)
		{
			Send_Byte(OLED_GRAM[n][i]);
			OLED_IIC_Wait_Ack();
		}
		OLED_IIC_Stop();
  }
}



//num ÏÔÊ¾ºº×ÖµÄ¸öÊý
//space Ã¿Ò»±éÏÔÊ¾µÄ¼ä¸ô
//mode:0,·´É«ÏÔÊ¾;1,Õý³£ÏÔÊ¾
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode)
{
	uint8_t i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
//	    OLED_ShowChinese(128,8,t,16,mode); //Ð´ÈëÒ»¸öºº×Ö±£´æÔÚOLED_GRAM[][]Êý×éÖÐ
			t++;
		}
		if(t==num)
		{
			for(r=0;r<16*space;r++)      //ÏÔÊ¾¼ä¸ô
			 {
				for(i=1;i<144;i++)
					{
						for(n=0;n<4;n++)
						{
							OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
						}
					}
				 OLED_Refresh();
			 }
			t=0;
		}
		m++;
		if(m==16)
			m=0;
		for(i=1;i<144;i++)   //ÊµÏÖ×óÒÆ
		{
			for(n=0;n<4;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}





static void OLED_IIC_Start()
{

	SDA_OUT();     //sdaÏßÊä³ö
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//Ç¯×¡I2C×ÜÏß£¬×¼±¸·¢ËÍ»ò½ÓÊÕÊý¾Ý 
}

/**********************************************
//IIC Stop
**********************************************/
static void OLED_IIC_Stop()
{
	SDA_OUT();//sdaÏßÊä³ö
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//·¢ËÍI2C×ÜÏß½áÊøÐÅºÅ
	delay_us(4);	
	
}

static void OLED_IIC_Wait_Ack()
{

	uint8_t ucErrTime=0;
	SDA_IN();      //SDAÉèÖÃÎªÊäÈë  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			OLED_IIC_Stop();
			return ;
		}
	}
	IIC_SCL=0;//Ê±ÖÓÊä³ö0 	   
	return ;  
}
/**********************************************
// IIC Write byte
**********************************************/

static void Write_IIC_Byte(unsigned char IIC_Byte)
{
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(IIC_Byte&0x80)>>7;
        IIC_Byte<<=1; 	  
		delay_us(2);   //¶ÔTEA5767ÕâÈý¸öÑÓÊ±¶¼ÊÇ±ØÐëµÄ
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	
}
/**********************************************
// IIC Write Command
**********************************************/
static void Write_IIC_Command(unsigned char IIC_Command)
{
   OLED_IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	OLED_IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	OLED_IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	OLED_IIC_Wait_Ack();	
   OLED_IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
static void Write_IIC_Data(unsigned char IIC_Data)
{
   OLED_IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	OLED_IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	OLED_IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	OLED_IIC_Wait_Ack();	
   OLED_IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
//static void Delay_50ms(unsigned int Del_50ms)
//{
//	HAL_Delay(50*Del_50ms);
//}

//static void Delay_1ms(unsigned int Del_1ms)
//{
//	HAL_Delay(Del_1ms);
//}

//×ø±êÉèÖÃ

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//¿ªÆôOLEDÏÔÊ¾    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//¹Ø±ÕOLEDÏÔÊ¾     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//ÇåÆÁº¯Êý,ÇåÍêÆÁ,Õû¸öÆÁÄ»ÊÇºÚÉ«µÄ!ºÍÃ»µãÁÁÒ»Ñù!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //¸üÐÂÏÔÊ¾
}
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //¸üÐÂÏÔÊ¾
}
//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//mode:0,·´°×ÏÔÊ¾;1,Õý³£ÏÔÊ¾				 
//size:Ñ¡Ôñ×ÖÌå 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^nº¯Êý
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//ÏÔÊ¾2¸öÊý×Ö
//x,y :Æðµã×ø±ê	 
//len :Êý×ÖµÄÎ»Êý
//size:×ÖÌå´óÐ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µþ¼ÓÄ£Ê½
//num:ÊýÖµ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//ÏÔÊ¾Ò»¸ö×Ö·ûºÅ´®
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=6;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//»­µã 
//x:0~127
//y:0~63
//t:1 Ìî³ä 0,Çå¿Õ	
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

//»­Ïß
//x1,y1:Æðµã×ø±ê
//x2,y2:½áÊø×ø±ê
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //¼ÆËã×ø±êÔöÁ¿ 
	delta_y=y2-y1;
	uRow=x1;//»­ÏßÆðµã×ø±ê
	uCol=y1;
	if(delta_x>0)incx=1; //ÉèÖÃµ¥²½·½Ïò 
	else if (delta_x==0)incx=0;//´¹Ö±Ïß 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//Ë®Æ½Ïß 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //Ñ¡È¡»ù±¾ÔöÁ¿×ø±êÖá 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//»­µã
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

//x,y:Ô²ÐÄ×ø±ê
//r:Ô²µÄ°ë¾¶
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//¼ÆËã»­µÄµãÀëÔ²ÐÄµÄ¾àÀë
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


//ÏÔÊ¾ºº×Ö
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}


////ÏÔÊ¾ºº×Ö
////x,y:Æðµã×ø±ê
////num:ºº×Ö¶ÔÓ¦µÄÐòºÅ
////mode:0,·´É«ÏÔÊ¾;1,Õý³£ÏÔÊ¾
//void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t num,uint8_t mode)
//{
//	uint8_t m,temp;
//	uint8_t x0=x,y0=y;
//	uint16_t i,size3=(16/8+((16%8)?1:0))*16;  //µÃµ½×ÖÌåÒ»¸ö×Ö·û¶ÔÓ¦µãÕó¼¯ËùÕ¼µÄ×Ö½ÚÊý
//	for(i=0;i<size3;i++)
//	{
//		temp=Hzk[num][i];//µ÷ÓÃ16*16×ÖÌå
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp>>=1;
//			y++;
//		}
//		x++;
//		if((x-x0)==16)
//		{x=x0;y0=y0+8;}
//		y=y0;
//	}
//}



/***********¹¦ÄÜÃèÊö£ºÏÔÊ¾ÏÔÊ¾BMPÍ¼Æ¬128¡Á64ÆðÊ¼µã×ø±ê(x,y),xµÄ·¶Î§0¡«127£¬yÎªÒ³µÄ·¶Î§0¡«7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//³õÊ¼»¯ 					    
void OLED_Init(void)
{ 	

    HAL_Delay(200);
    OLED_WR_Byte(0xAE,OLED_CMD);//¹Ø±ÕÏÔÊ¾
	
	OLED_WR_Byte(0x40,OLED_CMD);//---set low column address
	OLED_WR_Byte(0xB0,OLED_CMD);//---set high column address

	OLED_WR_Byte(0xC8,OLED_CMD);//-not offset

	OLED_WR_Byte(0x81,OLED_CMD);//ÉèÖÃ¶Ô±È¶È
	OLED_WR_Byte(0xff,OLED_CMD);

	OLED_WR_Byte(0xa1,OLED_CMD);//¶ÎÖØ¶¨ÏòÉèÖÃ

	OLED_WR_Byte(0xa6,OLED_CMD);//
	
	OLED_WR_Byte(0xa8,OLED_CMD);//ÉèÖÃÇý¶¯Â·Êý
	OLED_WR_Byte(0x1f,OLED_CMD);
	
	OLED_WR_Byte(0xd3,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	
	OLED_WR_Byte(0xd5,OLED_CMD);
	OLED_WR_Byte(0xf0,OLED_CMD);
	
	OLED_WR_Byte(0xd9,OLED_CMD);
	OLED_WR_Byte(0x22,OLED_CMD);
	
	OLED_WR_Byte(0xda,OLED_CMD);
	OLED_WR_Byte(0x02,OLED_CMD);
	
	OLED_WR_Byte(0xdb,OLED_CMD);
	OLED_WR_Byte(0x49,OLED_CMD);
	
	OLED_WR_Byte(0x8d,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	
	OLED_WR_Byte(0xaf,OLED_CMD);
	OLED_Clear();
}


#endif

