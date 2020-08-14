//_____Included libraries_____
#include <SPI.h>
#include <Adafruit_SSD1351.h>

//_____Display pin definitions_____
//#define scl     10
//#define sda     11
#define dc      12
#define rst     13  //LED_BUILTIN
#define cs      A0

//Adafruit_SSD1351 display = Adafruit_SSD1351(cs, dc, sda, scl, rst);
Adafruit_SSD1351 display = Adafruit_SSD1351(128, 128, &SPI, cs, dc, rst);

//_____Display color definitions_____
#define black   0x0000
#define brown   0xB340
#define brown1  0x7A40
#define brown2  0x5180 
#define red     0xF800
#define red1    0x7500
#define red2    0x2000
#define orange  0xFB40
#define orange1 0xCA80
#define orange2 0xAA20
#define yellow  0xFFE0
#define yellow1 0xAD80
#define yellow2 0x6340
#define green   0x07E0  
#define green1  0x0580  
#define green2  0x02A0
#define blue    0x001F
#define blue1   0x0016
#define blue2   0x000C
#define violet  0xF81F
#define violet1 0xB016
#define violet2 0x500B
#define grey    0xB596
#define grey1   0x73AE
#define grey2   0x4A69
#define white   0xFFFF

//_____Starts display, display library and UI_____
void display_init(){
  display.begin();
  delay(10);
//  ui_page_splash();
  ui_page_home();
}
