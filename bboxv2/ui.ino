bool ui_mode;

bool ui_control(bool uiToggleBtn1, bool uiToggleBtn2, bool up, bool down, bool left, bool right){
  if (uiToggleBtn1 and uiToggleBtn2){
    if (ui_mode){
      ui_mode = 0;
      //add re-draw what would be under the cursor/control indicators on screen here
    }
    else {
      ui_mode = 1;
      //add draw cursor/control indicators on screen here
    }
  }
  if (ui_mode){
    if (up)     {display.fillScreen(black);}
    if (down)   {display.fillScreen(brown);}
    if (left)   {display.fillScreen(red);}
    if (right)  {display.fillScreen(orange);}
    return 0;
  }
    return 1;
}

int ui_gridHandler(int resX, int resY, int clmns, int rows, int cellNum, gridHandlerOutput mode){
  int spaceX = resX / clmns;
  int spaceY = resY / rows;
  int multX = 0;
  int multY = 0;
  if (mode == x || mode == y){
    for (int i = 0; i <= cellNum; i++){
      if ((multX * spaceX) + spaceX > (clmns * spaceX)){
        multX = 0;
        multY++;
      }
      multX++;
    }
  }
  switch(mode){
    case x:     return (spaceX * multX) - spaceX;
    case y:     return spaceY * multY;
    case spcX:  return spaceX;
    case spcY:  return spaceY;
  }
}

void ui_gridHandler_debug(){
  #define gridLimit 8
  #define numOffsetX 2
  #define numOffsetY 1
  for (int xy = 1; xy <= gridLimit; xy++){
    display.fillScreen(black);
    for (int num = 0; num < xy * xy; num++){
      display.drawRect(ui_gridHandler(screenResX,screenResY,xy,xy,num,x), ui_gridHandler(screenResX,screenResY,xy,xy,num,y),
                      ui_gridHandler(screenResX,screenResY,xy,xy,num,spcX), ui_gridHandler(screenResX,screenResY,xy,xy,num,spcY), white);
      display.setCursor(ui_gridHandler(screenResX,screenResY,xy,xy,num,x)+numOffsetX, ui_gridHandler(screenResX,screenResY,xy,xy,num,y)+numOffsetY);
      display.print(num+1);
    }
    delay(500);
  }
  ui_gridHandler_debug();
}
