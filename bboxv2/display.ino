//_____Included libraries_____
#include <SPI.h>
#include <Adafruit_SSD1351.h>

//_____Display definitions_____
#define dc  3
#define cs  2
#define screenResX  128
#define screenResY  128

//_____Display color definitions_____
#define black   0x0000
#define brown   0xB340
#define red     0xF800
#define orange  0xFB40
#define yellow  0xFFE0
#define green   0x07E0  
#define blue    0x001F
#define violet  0xF81F
#define grey    0xB596
#define white   0xFFFF

Adafruit_SSD1351 display = Adafruit_SSD1351(screenResX, screenResY, &SPI, cs, dc);

//_____Starts display, display library and UI_____
void display_init(){
  display.begin();
  delay(10);
  display.fillScreen(black);
  display.setTextColor(blue);
  ui_gridHandler_debug();
}
