//WIN USB HID Device (Race sim button box)
//Kylamity
//Arduino Micro, Arduino IDE, Win10
//v0.1

//_____Included libraries_____
#include <HID-Project.h>

//_____Pin definitions_____
const int row[5]  = {0,1,2,3,4};
const int clmn[5] = {5,6,7,8,9};
#define mode_in A1
#define pot1_in A2 
#define pot2_in A3 
#define r1_out  A4
#define r2_out  A5

//_____Global Variables_____
int btnState[25];
int btnMode[25];
int potState[2];
int relayState[2];
int modeState = 0;
int modeTState = 0;
int checksum = 0;
int checksum2 = 0;

void setup() {
  //_____Pin modes_____
  for (int i = 0; i <= 4; i++) {pinMode(row[i],OUTPUT); pinMode(clmn[i],INPUT);}
  pinMode(r1_out,OUTPUT);
  pinMode(r2_out,OUTPUT);
  pinMode(pot1_in,INPUT);
  pinMode(pot2_in,INPUT);
  pinMode(mode_in,INPUT_PULLUP);
  Serial.begin(300);
  Gamepad.begin();
  display_init();
}

void loop() {
  //_____Main input pin read__________________________________________
  int btn[25];
  int btnID = 0;
  
  for (int rowID = 0; rowID <= 4; rowID++) {
    digitalWrite(row[rowID],HIGH);
    for (int clmnID = 0; clmnID <= 4; clmnID++) {
      btn[btnID] = digitalRead(clmn[clmnID]);
      btnID++;
    }
    digitalWrite(row[rowID],LOW);
  }
  int pot[2] = {analogRead(pot1_in)/10,analogRead(pot2_in)/10};
  int mode = 1 - digitalRead(mode_in);

  //_____Sum_____________________
  int sum = 0;
  int potSum = 0;
  for (int i = 0; i <= 24; i++) {
    sum = sum + btn[i];
    if (i <= 1) {
      potSum = potSum + pot[i];
    }
  }
  //_____Mode button______________________
  if (mode != modeState){
    if (mode == 1) {
      modeTState = toggler(modeTState);
    }
    modeState  = mode;
    ui_drawMode();
  }

  //_____Base user input logic______________________________________________
  if (sum != checksum) {
    switch(modeTState) {
        //_____User input to HID_____________________
        case 0: for (int i = 0; i <= 24; i++) {
                  updateButton(btn[i],btnState[i],i);
                }
                Gamepad.write();
                break;
        //_____User input to UI_______________________________________
        case 1: ui_control(btn[2],btn[9],btn[8],btn[10],btn[3]); break;
                break;
    }
    //_____State vars & checksum______________
    checksum = 0;
    for (int i = 0; i <= 24; i++) {
      btnState[i] = btn[i];
      checksum = checksum + btnState[i];
    }
  }
  //seperate pots from button update loops to keep analog fluctuation from bouncing ui inputs
  if (potSum != checksum2) {
    checksum2 = 0;
    for (int i = 0; i <= 1; i++) {
      updatePot(pot[i],potState[i],i);
      potState[i] = pot[i];
      checksum2 = checksum2 + pot[i];
    }
    Gamepad.write();
  }
  ui_tasks();
}

//_____Called Functions___________________________________

int updateButton(int btn,int btnState,int btnID) {
  if (btn != btnState) {
    switch(btn) {
      case 0: Gamepad.release(btnID+1); break;
      case 1: Gamepad.press(btnID+1); break;
    }
  }
}

void updatePot(int pot, int potState,int potID) {
  if (pot != potState) {
    switch(potID+1) {
      case 1: Gamepad.xAxis(pot); break;
      case 2: Gamepad.yAxis(pot); break;
    }
  }
}

int toggler(int tstate) {
    tstate++;
    switch(tstate) {
      case 1: return 1;
      case 2: return 0;
  }
}

void toggleRelay(int relayID){
  switch (relayID) {
    case 1: relayState[0] = toggler(relayState[0]);
            switch (relayState[0]) {
              case 1: digitalWrite(r1_out,HIGH); break; 
              case 0: digitalWrite(r1_out,LOW); break;
            } 
            delay(250);
            break;
    case 2: relayState[1] = toggler(relayState[1]);
            switch (relayState[1]) {
              case 1: digitalWrite(r2_out,HIGH); break; 
              case 0: digitalWrite(r2_out,LOW); break;
            }
            delay(250); 
            break;
  }
}
