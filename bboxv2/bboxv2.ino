#include <HID-Project.h>
typedef enum {x, y, spcX, spcY} gridHandlerOutput;

#define sda_piso 4 //PISO Shift Register cascade pins
#define sck_piso 5
#define ld_piso  6
#define totalInputBits 16 //Total inputs in cascade

bool inputBits[totalInputBits];
int checkSum;

void setup(){
  pinMode(sda_piso,INPUT);
  pinMode(sck_piso,OUTPUT);
  pinMode(ld_piso,OUTPUT);
  
  Serial.begin(9600);;
  digitalWrite(ld_piso,HIGH);
  digitalWrite(sck_piso,LOW);
  display_init();
}

void loop(){
  //ui_control(toggle1,toggle2,up,down,left,right)
  if (checkInputs() and ui_control(inputBits[0], inputBits[1], inputBits[2], inputBits[0], inputBits[1], inputBits[3])){
    updateGamepad();
  }
}

void updateGamepad(){
  for (int i = 0; i < totalInputBits; i++){
    if (inputBits[i]){
      Gamepad.press(i+1);
    }
    else{
      Gamepad.release(i+1);
    }
    Gamepad.write();
  }
}

bool checkInputs(){
  int bitSum = 0;
  digitalWrite(ld_piso,LOW);
  digitalWrite(ld_piso,HIGH);
  for (int i = 0; i < totalInputBits; i++){
    inputBits[i] = digitalRead(sda_piso);
    bitSum = bitSum + inputBits[i];
    digitalWrite(sck_piso,HIGH);
    digitalWrite(sck_piso,LOW);
  }
  if (bitSum == checkSum){
    return 0;
  }
  checkSum = bitSum;
  return 1;
}
