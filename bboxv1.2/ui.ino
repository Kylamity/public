#define ui_sizeX      128  
#define ui_sizeY      128
#define ui_maxCell    64 // Higher values consume dynamic memory
#define ui_maxTasks   5 // Higher values consume dynamic memory, Actual max is -1 of this value
#define ui_frequency  15 // Time in ms between task updates, currently limits clock speed (delay based)

int ui_offsetX = 0; //Top margin for grid
int ui_offsetY = 0; //Left margin for grid
int ui_updateTask[ui_maxTasks] = {}; //Array to flag tasks for update
int ui_cellX[ui_maxCell] = {}; // Array of x coords for cells
int ui_cellY[ui_maxCell] = {}; // Array of y coords for cells
int ui_cellSize[2] = {}; //X,Y size of cells
int ui_curCell = 0; //Cell number of cursor
int ui_curPage = 0; //Current page
int ui_curGridPos[2] = {0,0}; //X,Y # of cells in grid
int ui_gridSize[2] = {}; //X,Y dimensions of grid measured by cells
unsigned int ui_gridColor;
unsigned int ui_cursorColor;

//_____Main Functions_____________________________________________________
//Function chains and logic to generate pages
void ui_page_splash() {
  for (int x = 0; x < 129; x++) {
    display.fillRect(x,64,25,25,white);
    delay(20);
    display.fillRect(x,64,25,25,black);
  }
  ui_page_splash();
}

void ui_page_home() {
  ui_pageSetup(1,0,20,white,red,black);
  ui_setText(28,0,2,white); display.print("(Home)");
  ui_drawGrid(1,3,26,ui_gridColor);
  ui_drawMode();
}

void ui_page_settings() {
  ui_pageSetup(2,0,9,white,blue,black);
  ui_setText(12,0,0,white); display.print("-(Settings)-");
  ui_updateTask[1] = 1;
  ui_drawGrid(2,8,2,ui_gridColor);
  ui_drawMode();
}

void ui_page_power() {
  ui_pageSetup(3,0,20,white,red,black);
  ui_setText(20,0,2,white); display.print("(Power)");
  ui_drawGrid(1,3,26,ui_gridColor);
  ui_drawMode();
}

void ui_page_grid64() {
  ui_pageSetup(100,0,0,blue,black,black);
  ui_drawGrid(8,8,-1,ui_gridColor);
  ui_drawMode();
  ui_setText(0,0,1,white);
  for (int i = 0; i < 64; i++) {
    display.setCursor(ui_cellX[i]+4,ui_cellY[i]+3); display.print(i + 1);
  }
}

void ui_page_power_wheelWarning() {
  ui_pageSetup(101,0,0,white,yellow,yellow);
  ui_setText(5,5,2,black); display.print("CAUTION!!!");
  ui_setText(20,25,1,black); display.print("CLEAR WHEEL\n");
  display.setCursor(10,35); display.print("FOR CALIBRATION!");
  for (int i = 5; i > 0; i--) {
    ui_setText(50,75,3,black);
    display.print(i);
    delay(1000);
    ui_setText(50,75,3,yellow);
    display.print(i);
  }
  ui_setText(15,75,4,black);
  display.print("NOW!");
  toggleRelay(1);
  delay(8500);
  ui_page_home();
}

void ui_page_screenOff() {
  ui_pageSetup(102,0,0,white,black,black);
  ui_drawGrid(1,1,0,ui_gridColor);
  modeTState = 0;
}

//What is drawn inside each grid cell. Based on specified cell and current page
int ui_drawCell(int cellID, int cellClear) {
  switch (ui_curPage) {
    unsigned int color;
    //  HOME_____________________________________________________________________________________________
    case 1: switch (cellID) {
              case 0: unsigned int rColor1; unsigned int rColor2;
                      switch (relayState[0]) {case 0: rColor1 = red; break; case 1: rColor1 = green; break;}
                      display.fillRect(ui_cellX[0]+2, ui_cellY[0]+2, (ui_cellSize[0]/4)-1, ui_cellSize[1]-4, rColor1);
                      ui_setText(ui_cellX[0]+4, ui_cellY[0]+3,1,black); display.print("R1");
                      ui_setText(ui_cellX[0]+3, ui_cellY[0]+4,1,white); display.print("R1");
                      
                      switch (relayState[1]) {case 0: rColor2 = red; break; case 1: rColor2 = green; break;}
                      display.fillRect(ui_cellX[0]-1+((ui_cellSize[0]/4)*3), ui_cellY[0]+2, (ui_cellSize[0]/4)-1, ui_cellSize[1]-4, rColor2);  
                      ui_setText(ui_cellX[0]+1+((ui_cellSize[0]/4)*3), ui_cellY[0]+3,1,black); display.print("R2");
                      ui_setText(ui_cellX[0]+((ui_cellSize[0]/4)*3), ui_cellY[0]+4,1,white); display.print("R2");

                      ui_setText(ui_cellX[0]+45, ui_cellY[0]+2,2, white); display.print("Pwr"); break;
              case 1: ui_setText(ui_cellX[1]+30, ui_cellY[1]+2,2,white); display.print("empty"); break;
              case 2: ui_setText(ui_cellX[2]+27, ui_cellY[2]+2,2,white); display.print("Option>"); break;
    }
    break;
    //  SETTINGS__________________________________________________________________________________________
    case 2: switch (cellID) {
              case 0: ui_setText(ui_cellX[0]+2,ui_cellY[0]+2,1,white); display.print("< Back"); break;
              case 1: ui_setText(ui_cellX[1]+2,ui_cellY[1]+2,1,white); display.print("Grid"); break;
              case 2: ui_setText(ui_cellX[2]+2,ui_cellY[2]+2,1,white); display.print("Scrn Off"); break;
              case 3: switch (cellClear) {
                        case 0: ui_setText(ui_cellX[3]+2, ui_cellY[3]+2,1,white); display.print(potState[0]); break;
                        case 1: ui_setText(ui_cellX[3]+2, ui_cellY[3]+2,1,black); display.print(potState[0]); break;
              }
              break;
              case 4: switch (cellClear) {
                        case 0: ui_setText(ui_cellX[4]+2, ui_cellY[4]+2,1,white); display.print(potState[1]); break;
                        case 1: ui_setText(ui_cellX[4]+2, ui_cellY[4]+2,1,black); display.print(potState[1]); break;
              }     
    }
    break;
    // POWER_____________________________________________________________________________________________
    case 3: switch (cellID) {
              case 0: switch (relayState[0]) {case 0: color = red; break; case 1: color = green; break;}
              ui_setText(ui_cellX[0]+2,ui_cellY[0]+2,2,color); display.print("(1)Wheel"); break;   
              case 1: switch (relayState[1]) {case 0: color = red; break; case 1: color = green; break;}
              ui_setText(ui_cellX[1]+2,ui_cellY[1]+2,2,color); display.print("(2)Other"); break;     
              case 2: ui_setText(ui_cellX[2]+2,ui_cellY[2]+2,2,white); display.print("Back"); break;
    }
    break;
    //  BUTTON SELECT____________________________________________________________________________________
    case 100: break;
  }
}

//Use button input triggers. Based on current cell and current page
void ui_useCursor() {
  switch (ui_curPage) {
    //  HOME________________________________________________________
    case 1: switch (ui_curCell) {
              case 0: ui_page_power(); break;
              case 2: ui_page_settings(); break;
    }
    break;
    //  SETTINGS____________________________________________________
    case 2: switch (ui_curCell) {
              case 0: ui_page_home(); break;
              case 1: ui_page_grid64(); break;
              case 2: ui_page_screenOff(); break;
              case 3: break;
              case 4: break;
    }
    break;
    //  POWER________________________________________________________
    case 3: switch (ui_curCell) {
              case 0: switch (relayState[0]) {
                        case 0: ui_page_power_wheelWarning(); break;
                        case 1: toggleRelay(1); 
                                ui_drawCell(ui_curCell,0); break;
              }
              break;
              case 1: toggleRelay(2);
                      ui_drawCell(ui_curCell,0); break;
              case 2: ui_page_home(); break;
    }
    break;
    //  BUTTON SELECT________________________________________________
    case 100: if (ui_curCell > -1) {ui_page_settings();} break;
    case 102: ui_page_home(); break;
  }
}

void ui_tasks() {
  for(int a = 0; a <= 1; a++) {
    for(int i = 1; i <= ui_maxTasks; i++) {
      switch (ui_updateTask[i]*i) {
        case 1: ui_drawCell(3,a);
                ui_drawCell(4,a);
      }
    }
    if(a == 0) {delay(ui_frequency);}
  }
}

//_____Called Functions____________________________________________________________________
void ui_control(int up, int down, int left, int right, int use) {
  if (up)    {if (ui_curGridPos[1] > 0) {ui_drawCursor(ui_curCell - ui_gridSize[0] - 1, ui_cursorColor); ui_curGridPos[1]--;}}
  if (down)  {if (ui_curGridPos[1] < ui_gridSize[1]) {ui_drawCursor(ui_curCell + ui_gridSize[0] + 1, ui_cursorColor); ui_curGridPos[1]++;}}
  if (left)  {if (ui_curGridPos[0] > 0) {ui_drawCursor(ui_curCell - 1, ui_cursorColor); ui_curGridPos[0]--;}}
  if (right) {if (ui_curGridPos[0] < ui_gridSize[0]) {ui_drawCursor(ui_curCell + 1, ui_cursorColor); ui_curGridPos[0]++;}}
  if (use)   {ui_useCursor();}
}

void ui_drawGrid(int clmns, int rows, int padding, unsigned int color) {
  int space[2] = {
    ((ui_sizeX + padding) - ui_offsetX) / clmns, //x
    ((ui_sizeY + padding) - ui_offsetY) / rows   //y
  };
  int cellCt = 0;
  for (int y = ui_offsetY; y < (space[1] * rows); y = y + space[1]) {
    for (int x = ui_offsetX; x < (space[0] * clmns); x = x + space[0]) {
      display.drawRect(x, y, space[0] - padding, space[1] - padding, color);
      ui_cellX[cellCt] = x;
      ui_cellY[cellCt] = y;
      cellCt++;
    }
  }
  ui_gridSize[0] = clmns - 1;
  ui_gridSize[1] = rows - 1;
  ui_cellSize[0] = space[0] - padding;
  ui_cellSize[1] = space[1] - padding;

  for (int i = 0; i <= clmns * rows; i++) {
    ui_drawCell(i,0);
  }
}

void ui_drawCursor(int cellID, unsigned int color) {
  switch (cellID == ui_curCell) {
    case 0: display.drawRect(ui_cellX[ui_curCell], ui_cellY[ui_curCell], ui_cellSize[0], ui_cellSize[1], ui_gridColor);
            display.drawRect(ui_cellX[cellID], ui_cellY[cellID], ui_cellSize[0], ui_cellSize[1], color);
            break;
    case 1: display.drawRect(ui_cellX[ui_curCell], ui_cellY[ui_curCell], ui_cellSize[0], ui_cellSize[1], color);
            break;
  }
  ui_curCell = cellID;
}

void ui_drawMode(){
  switch (modeTState) {
    case 0: ui_drawCursor(ui_curCell,ui_gridColor);
            if (ui_curPage >= 10) {break;}
            ui_setText (1,0,1,black); display.print("[                   ]");
            break;
    case 1: ui_drawCursor(ui_curCell,ui_cursorColor);
            if (ui_curPage >= 10) {break;}
            ui_setText (1,0,1,white); display.print("[                   ]");
            break;
  }
}

void ui_pageSetup(int curPage, int offsetX, int offsetY, unsigned int cursorColor, unsigned int gridColor, unsigned int bgColor ) {
  for(int i = 1; i <= ui_maxTasks; i++) {
  ui_updateTask[i] = 0;
  }
  display.fillScreen(bgColor);
  delay(10);
  ui_curCell = 0;
  ui_curGridPos[0] = 0;
  ui_curGridPos[1] = 0;
  ui_curPage = curPage;
  ui_offsetY = offsetY;
  ui_offsetX = offsetX;
  ui_cursorColor = cursorColor;
  ui_gridColor = gridColor;
}

void ui_setText(int x, int y, int size_,unsigned int color) {
  display.setCursor(x,y);
  display.setTextSize(size_);
  display.setTextColor(color);
}
