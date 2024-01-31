/*
 * lcd.c
 *
 *  Created on: Sep 25, 2023
 *      Author: HaHuyen
 */

#include "lcd.h"
#include "lcdfont.h"

unsigned char s[50];

_lcd_dev lcddev;

void LCD_WR_REG(uint16_t reg)
{
	LCD->LCD_REG=reg;
}

void LCD_WR_DATA(uint16_t data)
{
	LCD->LCD_RAM=data;
}

uint16_t LCD_RD_DATA(void)
{
	__IO uint16_t ram;
	ram=LCD->LCD_RAM;
	return ram;
}


void lcd_AddressSet(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
		LCD_WR_REG(0x2a);
		LCD_WR_DATA(x1>>8);
		LCD_WR_DATA(x1&0xff);
		LCD_WR_DATA(x2>>8);
		LCD_WR_DATA(x2&0xff);
		LCD_WR_REG(0x2b);
		LCD_WR_DATA(y1>>8);
		LCD_WR_DATA(y1&0xff);
		LCD_WR_DATA(y2>>8);
		LCD_WR_DATA(y2&0xff);
		LCD_WR_REG(0x2c);
}


void lcd_SetCursor(uint16_t x,uint16_t y)
{
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(x>>8);
	LCD_WR_DATA(x&0xff);
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(y>>8);
	LCD_WR_DATA(y&0xff);
}

void lcd_DisplayOn(void)
{
	LCD_WR_REG(0X29);
}

void lcd_DisplayOff(void)
{
	LCD_WR_REG(0X28);
}

uint16_t lcd_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;
	lcd_SetCursor(x,y);
	LCD_WR_REG(0X2E);
	r=LCD_RD_DATA();
	r=LCD_RD_DATA();
	b=LCD_RD_DATA();
	g=r&0XFF;
	g<<=8;
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
}

/**
  * @brief  Fill all pixels with a color
  * @param  color Color to fill the screen
  * @retval None
  */
void lcd_Clear(uint16_t color) //
{
	uint16_t i,j;
	lcd_AddressSet(0,0,lcddev.width-1,lcddev.height-1);
	for(i=0;i<lcddev.width;i++)
	{
		for(j=0;j<lcddev.height;j++)
		{
			LCD_WR_DATA(color);
		}
	}
}

/**
  * @brief  Fill a group of pixels with a color
  * @param  xsta	Start column
  * @param  ysta	Start row
  * @param  xend	End column
  * @param  yend	End row
  * @param  color Color to fill
  * @retval None
  */
void lcd_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color) //add a hcn = 1 mau car been trogn
{
	uint16_t i,j;
	lcd_AddressSet(xsta,ysta,xend-1,yend-1);
	for(i=ysta;i<yend;i++)
	{
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	}
}

/**
  * @brief  Fill a pixel with a color
  * @param  x X coordinate
  * @param  y Y coordinate
  * @param  color Color to fill
  * @retval None
  */
void lcd_DrawPoint(uint16_t x,uint16_t y,uint16_t color) // 1 ddieemr anhr
{
	lcd_AddressSet(x,y,x,y);//ÉèÖÃ¹â±êÎ»ÖÃ
	LCD_WR_DATA(color);
}

/**
  * @brief  Draw a line with a color
  * @param  x1 X coordinate of start point
  * @param  y1 Y coordinate of start point
  * @param  x2 X coordinate of end point
  * @param  y2 Y coordinate of end point
  * @param  color Color to fill
  * @retval None
  */
void lcd_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color) // ve duong
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1;
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1;
	else if (delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x;
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		lcd_DrawPoint(uRow,uCol,color);
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


void lcd_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color) // ve hcn(vien
{
	lcd_DrawLine(x1,y1,x2,y1,color);
	lcd_DrawLine(x1,y1,x1,y2,color);
	lcd_DrawLine(x1,y2,x2,y2,color);
	lcd_DrawLine(x2,y1,x2,y2,color);
}

void lcd_ShowChar(uint16_t x,uint16_t y,uint8_t character,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode) // 1ky tu size = 12 16 24 32, fc: mau chuw, bc, mauf neefn, mode: hien neen
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	character=character-' ';
	lcd_AddressSet(x,y,x+sizex-1,y+sizey-1);
	for(i=0;i<TypefaceNum;i++)
	{
		if(sizey==12);
		else if(sizey==16)temp=ascii_1608[character][i];
		else if(sizey==24)temp=ascii_2412[character][i];
		else if(sizey==32)temp=ascii_3216[character][i];
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else
			{
				if(temp&(0x01<<t))lcd_DrawPoint(x,y,fc);
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}
}

uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)result*=m;
	return result;
}

void lcd_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey) //len: ddooj daif cuar so
{
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				lcd_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1;

		}
	 	lcd_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


void lcd_ShowFloatNum(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			lcd_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	lcd_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}

void lcd_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]) //code of picture
{
	uint8_t picH,picL;
	uint16_t i,j;
	uint32_t k=0;
	lcd_AddressSet(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			picH=pic[k*2];
			picL=pic[k*2+1];
			LCD_WR_DATA(picH<<8|picL);
			k++;
		}
	}
}


void lcd_SetDir(uint8_t dir) //chinh huong man hinh
{
	if((dir>>4)%4)
	{
		lcddev.width=320;
		lcddev.height=240;
	}else
	{
		lcddev.width=240;
		lcddev.height=320;
	}
}


void lcd_init(void)
{
	HAL_GPIO_WritePin(FSMC_RES_GPIO_Port, FSMC_RES_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(FSMC_RES_GPIO_Port, FSMC_RES_Pin, GPIO_PIN_SET);
	HAL_Delay(500);
	lcd_SetDir(DFT_SCAN_DIR);
	LCD_WR_REG(0XD3);
	lcddev.id=LCD_RD_DATA();	//dummy read
	lcddev.id=LCD_RD_DATA();
	lcddev.id=LCD_RD_DATA();
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();

	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC1);
	LCD_WR_DATA(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x7A);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xC0);    //Power control
	LCD_WR_DATA(0x1B);   //VRH[5:0]
	LCD_WR_REG(0xC1);    //Power control
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0]
	LCD_WR_REG(0xC5);    //VCM control
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2
	LCD_WR_DATA(0XB7);
	LCD_WR_REG(0x36);    // Memory Access Control

	LCD_WR_DATA(0x08|DFT_SCAN_DIR);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1A);
	LCD_WR_REG(0xB6);    // Display Function Control
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xA2);
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x26);    //Gamma curve selected
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xE0);    //Set Gamma
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0XA9);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0XE1);    //Set Gamma
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x0F);
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);
	LCD_WR_REG(0x11); //Exit Sleep
	HAL_Delay(120);
	LCD_WR_REG(0x29); //display on
	HAL_GPIO_WritePin(FSMC_BLK_GPIO_Port, FSMC_BLK_Pin, 1);
}

static void _draw_circle_8(int xc, int yc, int x, int y, uint16_t c)
{
	lcd_DrawPoint(xc + x, yc + y, c);

	lcd_DrawPoint(xc - x, yc + y, c);

	lcd_DrawPoint(xc + x, yc - y, c);

	lcd_DrawPoint(xc - x, yc - y, c);

	lcd_DrawPoint(xc + y, yc + x, c);

	lcd_DrawPoint(xc - y, yc + x, c);

	lcd_DrawPoint(xc + y, yc - x, c);

	lcd_DrawPoint(xc - y, yc - x, c);
}

void lcd_DrawCircle(int xc, int yc,uint16_t c,int r, int fill) //ve duong or hinh, c: color
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;


	if (fill)
	{
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else
	{
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

void lcd_ShowStr(uint16_t x, uint16_t y,char *str,uint16_t fc, uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint16_t x0=x;
  uint8_t bHz=0;
	while(*str!=0)
	{
		if(!bHz)
		{
			if(x>(lcddev.width-sizey/2)||y>(lcddev.height-sizey)) return;
			if(*str>0x80)bHz=1;
			else
			{
				if(*str==0x0D)
				{
					y+=sizey;
					x=x0;
					str++;
				}else
				{
					lcd_ShowChar(x,y,*str,fc,bc,sizey,mode);
					x+=sizey/2;
				}
			  str++;
			}
		}
	}
}

void lcd_StrCenter(uint16_t x, uint16_t y,char *str,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint16_t len=strlen((const char *)str);
	uint16_t x1=(lcddev.width-len*8)/2;
	lcd_ShowStr(x+x1,y,str,fc,bc,sizey,mode);
}


