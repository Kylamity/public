//_____Included libraries_____
//#include <Adafruit_SSD1331.h>
#include <Adafruit_SSD1351.h>

//_____Display pin definitions_____
#define scl     10
#define sda     11
#define dc      12
#define rst     13  //LED_BUILTIN
#define cs      A0

//Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, sda, scl, rst);
Adafruit_SSD1351 display = Adafruit_SSD1351(cs, dc, sda, scl, rst);


//_____Display color definitions_____
#define black   0x0000
//#define brown
#define red     0xF800
//#define orange
#define yellow  0xFFE0
#define green   0x07E0
#define blue    0x001F
#define violet  0xF81F
//#define grey
#define white   0xFFFF

//_____Starts display, display library and UI_____
void display_init(){
  display.begin();
  delay(10);
  ui_page_home();
}
