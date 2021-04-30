bool ui_mode;

bool ui_control(bool uiToggleBtn1, bool uiToggleBtn2, bool up, bool down, bool left, bool right) {
  if (uiToggleBtn1 and uiToggleBtn2) {
    if (ui_mode) {
      ui_mode = 0;
      //add re-draw what would be under the cursor/control indicators on screen here
    }
    else {
      ui_mode = 1;
      //add draw cursor/control indicators on screen here
    }
  }
  delay(10);
  
  if (ui_mode) {
    if (up)     {display.fillScreen(black);}
    if (down)   {display.fillScreen(brown);}
    if (left)   {display.fillScreen(red);}
    if (right)  {display.fillScreen(orange);}
    return 0;
  }
    else {
      return 1;
    }
}
