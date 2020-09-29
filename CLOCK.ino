//Created by Darshana Ariyarathna.
#include <Wire.h>

int sec, mins, hrs, temp1, temp2,temp3; 

// Pin assignment
int alarm = 0;

int btn1 = 2;
int btn2 = 3;

int adr2 = 1;
int adr1 = 4;
int adr0 = 5;

int A = 6;
int B = 7;
int C = 8;
int D = 9;
int E = 10;
int F = 11;
int G = 12;
int DP = 13;

int delayDisp = 2; //digit hold time
int segVals[]  =  {0, 1, 2, 3, 4, 5};
int modeNum = 0;

void saveTime();

void setup() {
  Wire.begin();
  pinMode(alarm, OUTPUT);
  //
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  //    pinMode(btn1, INPUT);
  //    pinMode(btn2, INPUT);

  pinMode(adr0, OUTPUT);
  pinMode(adr1, OUTPUT);
  pinMode(adr2, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);

  pinMode(DP, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(2), setMode, RISING);
  attachInterrupt(digitalPinToInterrupt(3), valueUp, RISING);
  
//  setupRTC();
  delay(1000);
}
void setupRTC() {
  Wire.beginTransmission(0b1101000);
  Wire.write(0x00);
  Wire.write(0b00000000);
  Wire.endTransmission();  

//  min
  Wire.beginTransmission(0b1101000);
  Wire.write(0x01);
  Wire.write(0x35);
  Wire.endTransmission();
// hrs
  Wire.beginTransmission(0b1101000);
  Wire.write(0x02);
  Wire.write(0x09);
  Wire.endTransmission();  
}
void readTime() {
  Wire.beginTransmission(0b1101000);        //7bit address of sleve
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 3);           //7bit address of sleve and quantity of data bytes to requiest.
  while (Wire.available() < 3);
  sec = Wire.read();
  mins = Wire.read();
  hrs = Wire.read();

  segVals[0] = 7;
  procTime();
}
void procTime() {
  //seconds process
  temp1 = sec & 0b00001111;
  temp2 = (sec & 0b11110000) >> 4;
  segVals[5] = temp1;
  segVals[4] = temp2;
  
  //mins process
  temp1 = mins & 0b00001111;
  temp2 = (mins & 0b11110000) >> 4;
  segVals[3] = temp1;
  segVals[2] = temp2;
  
  //seconds process
  temp1 = hrs & 0b00001111;
  temp2 = (hrs & 0b11110000) >> 4;
  segVals[1] = temp1;
  segVals[0] = temp2; 
  segVals[1] = temp1;
  segVals[0] = temp2;
}

void valueUp() {
  digitalWrite(alarm, 1);
  delay(25);
  digitalWrite(alarm, 0);

  if(modeNum==1){
    //hrs selected
    temp3 = hrs;
    temp3 = (temp3 >> 4) * 10 + (temp3 & 0b00001111);
    temp3++;
    if(temp3==24){
      temp3 = 0;
    }
    temp1 = temp3%10;
    temp2 = (temp3-temp1)/10;
    hrs = temp2<<4 | temp1;
    
  }
  if(modeNum==2){
    //mins selected
    temp3 = mins;
    temp3 = (temp3 >> 4) * 10 + (temp3 & 0b00001111);
    temp3++;
    if(temp3==60){
      temp3 = 0;
    }
    temp1 = temp3%10;
    temp2 = (temp3-temp1)/10;
    mins = temp2<<4 | temp1;
        
  }
  if(modeNum==3){
    //secs selected
    temp3 = sec;
    temp3 = (temp3 >> 4) * 10 + (temp3 & 0b00001111);
    temp3++;
    if(temp3==60){
      temp3 = 0;
    }
    temp1 = temp3%10;
    temp2 = (temp3-temp1)/10;
    sec = temp2<<4 | temp1;        
  }  
  procTime();
}
void saveTime(){
  //save hrs:
  //temp3 = 
//  hrs = 0;
  Wire.beginTransmission(0b1101000);
  Wire.write(0x02);
  Wire.write(0b0000000);
  Wire.endTransmission();

  
}
void setMode() {
  digitalWrite(alarm, 1);
  delay(10);
  digitalWrite(alarm, 0);

  modeNum++;
  if(modeNum == 4){ 
    modeNum=0;    
  }
}
void segmentOn(int segmentNo) {
  switch (segmentNo) {
    case 0:         //all seg off
      digitalWrite(adr0, 1);
      digitalWrite(adr1, 1);
      digitalWrite(adr2, 1);
      break;
    case 1:
      digitalWrite(adr0, 0);
      digitalWrite(adr1, 0);
      digitalWrite(adr2, 0);
      break;
    case 2:
      digitalWrite(adr0, 1);
      digitalWrite(adr1, 0);
      digitalWrite(adr2, 0);
      
//      digitalWrite(DP, 1);
      break;
    case 3:
      digitalWrite(adr0, 0);
      digitalWrite(adr1, 1);
      digitalWrite(adr2, 0);
      break;
    case 4:
      digitalWrite(adr0, 1);
      digitalWrite(adr1, 1);
      digitalWrite(adr2, 0);
      
//      digitalWrite(DP, 1);
      break;
    case 5:
      digitalWrite(adr0, 0);
      digitalWrite(adr1, 0);
      digitalWrite(adr2, 1);
      break;
    case 6:
      digitalWrite(adr0, 1);
      digitalWrite(adr1, 0);
      digitalWrite(adr2, 1);
      break;
    case 7:
      digitalWrite(adr0, 0);
      digitalWrite(adr1, 1);
      digitalWrite(adr2, 1);
      break;
    default:
      break;
  }
}
void showNumber(int character) {
  switch (character) {
    case 0:
      digitalWrite(A, 1);
      digitalWrite(B, 1);
      digitalWrite(C, 1);
      digitalWrite(D, 1);
      digitalWrite(E, 1);
      digitalWrite(F, 1);
      digitalWrite(G, 0);
      break;
    case 1:
      digitalWrite(A, 0);
      digitalWrite(B, 1);
      digitalWrite(C, 1);
      digitalWrite(D, 0);
      digitalWrite(E, 0);
      digitalWrite(F, 0);
      digitalWrite(G, 0);
      break;
    case 2:
      digitalWrite(A, 1);
      digitalWrite(B, 1);
      digitalWrite(C, 0);
      digitalWrite(D, 1);
      digitalWrite(E, 1);
      digitalWrite(F, 0);
      digitalWrite(G, 1);
      break;
    case 3:
      digitalWrite(A, 1);
      digitalWrite(B, 1);
      digitalWrite(C, 1);
      digitalWrite(D, 1);
      digitalWrite(E, 0);
      digitalWrite(F, 0);
      digitalWrite(G, 1);
      break;
    case 4:
      digitalWrite(A, 0);
      digitalWrite(B, 1);
      digitalWrite(C, 1);
      digitalWrite(D, 0);
      digitalWrite(E, 0);
      digitalWrite(F, 1);
      digitalWrite(G, 1);
      break;
    case 5:
      digitalWrite(A, 1);
      digitalWrite(B, 0);
      digitalWrite(C, 1);
      digitalWrite(D, 1);
      digitalWrite(E, 0);
      digitalWrite(F, 1);
      digitalWrite(G, 1);
      break;
    case 6:
      digitalWrite(A, 1);
      digitalWrite(B, 0);
      digitalWrite(C, 1);
      digitalWrite(D, 1);
      digitalWrite(E, 1);
      digitalWrite(F, 1);
      digitalWrite(G, 1);
      break;
    case 7:
      digitalWrite(A, 1);
      digitalWrite(B, 1);
      digitalWrite(C, 1);
      digitalWrite(D, 0);
      digitalWrite(E, 0);
      digitalWrite(F, 0);
      digitalWrite(G, 0);
      break;
    case 8:
      digitalWrite(A, 1);
      digitalWrite(B, 1);
      digitalWrite(C, 1);
      digitalWrite(D, 1);
      digitalWrite(E, 1);
      digitalWrite(F, 1);
      digitalWrite(G, 1);
      break;
    case 9:
      digitalWrite(A, 1);
      digitalWrite(B, 1);
      digitalWrite(C, 1);
      digitalWrite(D, 1);
      digitalWrite(E, 0);
      digitalWrite(F, 1);
      digitalWrite(G, 1);
      break;
    default:
      break;
  }
}
void dispNum(int segVal[]) {
  for (int i = 0; i <= 5; i++) {
    segmentOn(0);//turn off all segments
    showNumber(segVal[i]);
    if(i==1 || i==3){
      digitalWrite(DP, 1);
    }
    segmentOn(i + 1);
    delay(delayDisp);
    digitalWrite(DP, 0);
  }
}
void dispHrOnly(){
  for (int i = 0; i <= 1; i++) {
    segmentOn(0);//turn off all segments
    showNumber(segVals[i]);
    if(i==1 || i==3){
      digitalWrite(DP, 1);
    }
    segmentOn(i + 1);
    delay(delayDisp);
    digitalWrite(DP, 0);
  }
}
void dispMinOnly(){
  for (int i = 2; i <= 3; i++) {
    segmentOn(0);//turn off all segments
    showNumber(segVals[i]);
    if(i==1 || i==3){
      digitalWrite(DP, 1);
    }
    segmentOn(i + 1);
    delay(delayDisp);
    digitalWrite(DP, 0);
  }
}
void dispSecOnly(){
for (int i = 4; i <= 5; i++) {
    segmentOn(0);//turn off all segments
    showNumber(segVals[i]);
    if(i==1 || i==3){
      digitalWrite(DP, 1);
    }
    segmentOn(i + 1);
    delay(delayDisp);
    digitalWrite(DP, 0);
  }
}
void loop() {
  if(modeNum == 0 ){
    for (int i = 0; i < 10; i++) {
      dispNum(segVals);
    }
    readTime();  
  }
  if(modeNum == 1){
    dispHrOnly();
  }
  if(modeNum == 2){
    dispMinOnly();
  }
  if(modeNum == 3){
    dispSecOnly();
  }
  
}
