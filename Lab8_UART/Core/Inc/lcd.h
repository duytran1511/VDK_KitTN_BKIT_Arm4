/*
 * lcd.h
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

/* Includes */
#include "gpio.h"

/* Constants */
#define DFT_SCAN_DIR  L2R_U2D

// Direction Adjustment
#define L2R_U2D  0x00
#define L2R_D2U  0x80
#define R2L_U2D  0x40
#define R2L_D2U  0xc0

#define U2D_L2R  0x20
#define U2D_R2L  0x60
#define D2U_L2R  0xa0
#define D2U_R2L  0xe0

// LCD
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x000ffffe))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
// Color
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40
#define BRRED 			 0XFC07
#define GRAY  			 0X8430
#define DARKBLUE      	 0X01CF
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458
#define LIGHTGREEN     	 0X841F
#define LIGHTGRAY        0XEF5B
#define LGRAY 			 0XC618
#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12

/* Struct */
typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t id;
} _lcd_dev;

typedef struct {
	__IO uint16_t LCD_REG;
	__IO uint16_t LCD_RAM;
} LCD_TypeDef;

/* Variables */
extern _lcd_dev lcddev;

/* Functions */
void lcd_set_cursor(uint16_t x, uint16_t y);
void lcd_set_address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_set_display_on(void);
void lcd_set_display_off(void);
uint16_t lcd_read_point(uint16_t x, uint16_t y);
void lcd_clear(uint16_t color);

void lcd_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,
		uint16_t color);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
		uint16_t color);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
		uint16_t color);

void lcd_show_char(uint16_t x, uint16_t y, uint8_t character, uint16_t fc,
		uint16_t bc, uint8_t sizey, uint8_t mode);
void lcd_show_int_num(uint16_t x, uint16_t y, uint16_t num, uint8_t len,
		uint16_t fc, uint16_t bc, uint8_t sizey);
void lcd_show_float_num(uint16_t x, uint16_t y, float num, uint8_t len,
		uint16_t fc, uint16_t bc, uint8_t sizey);

void lcd_show_picture(uint16_t x, uint16_t y, uint16_t length, uint16_t width,
		const uint8_t pic[]);

void lcd_set_direction(uint8_t dir);
void lcd_init(void);

void lcd_draw_circle(int xc, int yc, uint16_t c, int r, int fill);
void lcd_show_string(uint16_t x, uint16_t y, char *str, uint16_t fc, uint16_t bc,
		uint8_t sizey, uint8_t mode);
void lcd_show_string_center(uint16_t x, uint16_t y, char *str, uint16_t fc, uint16_t bc,
		uint8_t sizey, uint8_t mode);
#endif /* INC_LCD_H_ */
