#define ui_sizeX   128  
#define ui_sizeY   128
#define ui_maxCell 64 // Higher values consume dynamic memory

int ui_offsetX = 0;
int ui_offsetY = 0;
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
//Function calls to initialize and draw pages
void ui_page_home() {
  ui_pageSetup(1,0,20,white,red);
  ui_setText(28,0,2,white); display.print("(Home)");
  ui_drawGrid(1,3,30,ui_gridColor);
  ui_drawMode();
}

void ui_page_settings() {
  ui_pageSetup(2,0,9,white,blue);
  ui_setText(12,0,0,white); display.print("-(Settings)-");
  ui_drawGrid(2,4,2,ui_gridColor);
  ui_drawMode();
}

void ui_page_buttons() {
  ui_pageSetup(10,0,0,blue,black);
  ui_drawGrid(5,5,-1,ui_gridColor);
  ui_drawMode();
  ui_setText(0,0,1,white);
  for (int i = 0; i < 25; i++) {
    display.setCursor(ui_cellX[i]+4,ui_cellY[i]+3); display.print(i + 1);
  }
}

void ui_page_power() {
  ui_pageSetup(3,0,9,white,red);
  ui_setText(22,0,0,white); display.print("-(Power)-");
  ui_drawGrid(1,3,30,ui_gridColor);
  ui_drawMode();
}

//Logic to display cell contents. Based on specified cell and current page
int ui_drawCell(int cellID) {
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

                      ui_setText(ui_cellX[0]+45, ui_cellY[0]+2, 2, white); display.print("Pwr"); break;
              case 1: ui_setText(ui_cellX[1]+30, ui_cellY[1]+2, 2, white); display.print("Gauge>"); break;
              case 2: ui_setText(ui_cellX[2]+27, ui_cellY[2]+2,2,white); display.print("Option>"); break;
    }
    break;
    //  SETTINGS__________________________________________________________________________________________
    case 2: switch (cellID) {
              case 0: ui_setText(ui_cellX[0]+2,ui_cellY[0]+2,1,white); display.print("< Back"); break;
              case 1: ui_setText(ui_cellX[1]+2,ui_cellY[1]+2,1,white); display.print("Btn Mod"); break;
              case 2: break;
              case 7: break;
    }
    break;
    // POWER_____________________________________________________________________________________________
    case 3: switch (cellID) {
              case 0: switch (relayState[0]) {case 0: color = red; break; case 1: color = green; break;}
              ui_setText(ui_cellX[0]+2,ui_cellY[0]+2,2,color); display.print("Relay 1"); break;   
              case 1: switch (relayState[1]) {case 0: color = red; break; case 1: color = green; break;}
              ui_setText(ui_cellX[1]+2,ui_cellY[1]+2,2,color); display.print("Relay 2"); break;     
              case 2: ui_setText(ui_cellX[2]+2,ui_cellY[2]+2,2,white); display.print("Back"); break;
    }
    break;
    //  BUTTON SELECT____________________________________________________________________________________
    case 10: break;
  }
}

//Use button triggers. Based on current cell and page
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
              case 1: ui_page_buttons(); break;
              case 2: break;
              case 7: break;
    }
    break;
    //  POWER________________________________________________________
    case 3: switch (ui_curCell) {
              case 0: toggleRelay(1,btnState[4],1); break;
              case 1: toggleRelay(1,btnState[4],2); break;
              case 2: ui_page_home(); break;
    }
    break;
    //  BUTTON SELECT________________________________________________
    case 10: if (ui_curCell > -1) {ui_page_settings();} break; 
  }
  ui_drawCell(ui_curCell);
}

//_____Other Functions____________________________________________________________________
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
//      ui_setText(ui_cellX[cellCt]+1,ui_cellY[cellCt]+1,1,white);
//      display.print(ui_cellX[cellCt]);
      cellCt++;
    }
  }
  ui_gridSize[0] = clmns - 1;
  ui_gridSize[1] = rows - 1;
  ui_cellSize[0] = space[0] - padding;
  ui_cellSize[1] = space[1] - padding;

  for (int i = 0; i <= clmns * rows; i++) {
    ui_drawCell(i);
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

void ui_pageSetup(int curPage, int offsetX, int offsetY, unsigned int cursorColor, unsigned int gridColor ) {
  display.fillScreen(black);
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
