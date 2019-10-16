//Part Sensor
#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004T.h>

PZEM004T pzem(6,5);  // (RX,TX) connect to TX,RX of PZEM
IPAddress ip(192,168,1,1);

//Display
#include <LedControlMS.h>
int DINa = 12;
int CLKa =  11;
int CSa = 10;

int DINb = 9;
int CLKb =  8;
int CSb = 7;

LedControl lcA=LedControl(DINa,CLKa,CSa,0);
LedControl lcB=LedControl(DINb,CLKb,CSb,0);
char chr;

//character
byte a_1[8]=    {0x7e, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};//
byte a_2[8]=    {0x7f, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x7f};//
byte a_3[8]=    {0x7f, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7f};
byte a_4[8]=    {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7f};
byte a_5[8]=    {0x7f, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7f};//
byte a_6[8]=    {0x7f, 0x7f, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7f};
byte a_7[8]=    {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60};
byte a_8[8]=    {0x7f, 0x7f, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7f};
byte a_9[8]=    {0x7f, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7f};
byte a_0[8]=    {0x7f, 0x7f, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63};

byte b_1[8]=    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1e, 0x1e};//
byte b_2[8]=    {0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7f, 0x7f};//
byte b_3[8]=    {0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7f, 0x7f};
byte b_4[8]=    {0x7f, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63};
byte b_5[8]=    {0x7f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x7f, 0x7f};//
byte b_6[8]=    {0x7f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x7f, 0x7f};
byte b_7[8]=    {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7f, 0x7f};
byte b_8[8]=    {0x7f, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7f, 0x7f};
byte b_9[8]=    {0x7f, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7f, 0x7f};
byte b_0[8]=    {0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7f, 0x7f};
byte emp[8]=    {0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x00};
byte dot[8]=    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

byte V[8]=    {0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42};
byte A[8]=    {0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x24, 0x18};
byte W[8]=    {0x24, 0x5a, 0x5a, 0x42, 0x42, 0x42, 0x00, 0x00};
byte h[8]=	  {0x22, 0x22, 0x22, 0x1e, 0x02, 0x02, 0x02, 0x00};


double Watt = 0;
const int button = 3;
int cnt = 0;

unsigned long time1;
unsigned long time2;

float v = 0;
float a = 0;
float p = 0;
float e = 0;

void setup() {
  Serial.begin(9600);
  pzem.setAddress(ip);
  pinMode(button,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), button_push, RISING);
  for(int i=0 ; i<=4 ; i++){
   lcA.shutdown(i,false);       //The MAX72XX is in power-saving mode on startup
   lcA.setIntensity(i,0);        // Set the brightness to maximum value
   lcA.clearDisplay(i);         // and clear the display
   lcB.shutdown(i,false);       //The MAX72XX is in power-saving mode on startup
   lcB.setIntensity(i,0);        // Set the brightness to maximum value
   lcB.clearDisplay(i);         // and clear the display
  }
}

void loop() {
  //part sensor
  time1 = millis();
  int flag = 0;
  while(flag == 0){
    time2 = millis();
    if(time2-time1>1000){
      flag = 1;
      Serial.print("update value\t");
      v = pzem.voltage(ip);
      if (v < 0.0){ v = 0.0;
      Serial.print(v);Serial.print("V; ");}
    
      a = pzem.current(ip)-0.02;
      if(a >= 0.0){ Serial.print(a);Serial.print("A; "); }
      
      p = pzem.power(ip);
      if(p >= 0.0){ Serial.print(p);Serial.print("W; "); }
      
      e = e+(p/3600);
      if(e >= 0.0){ Serial.print(e);Serial.print("Wh; "); }
      
      Serial.println();
    }
    //part display
    
      if (v<0){
        a=0;
        p=0;
      }
    switch (cnt) {
      
      case 0: led_print2(v);
              printByte1(V,dot,4);
              break;
      case 1: 
              if (v == -3){
                a=0;
                p=0;
                e=e;
              }
              led_print2(a); 
              printByte1(A,dot,4);
              break;
      case 2: led_print2(p);
              printByte1(W,dot,4);
              break;
      case 3: led_print2(e);
              printByte1(h,W,4);
              break;
    }
    
  }//  delay(1000);
}


void led_print2(float v){
    //Serial.print("led = ");
    //Serial.print(v);
    int num;
    String str = String(v);
    for(int i=0 ; i<4 ; i++){
      
      chr = str[i];
      if (chr=="."){
        if(i < 3){
          num == 0;
          }
        else{num = 0;}
        
      }
      else {
        num = chr-'0';
      }
      Serial.print("\t");
      Serial.print(num);
      if(i == 3 && num == -2){
        print_ch(i,99);
      }
      else{
    
        
          print_ch(i,num);
        
      }
      
    }
    Serial.println("");
}

void print_ch(int index,int num){
  switch (num){
    case 0 : printByte1(a_0,b_0,index);break;
    case 1 : printByte1(a_1,b_1,index);break;
    case 2 : printByte1(a_2,b_2,index);break;
    case 3 : printByte1(a_3,b_3,index);break;
    case 4 : printByte1(a_4,b_4,index);break;
    case 5 : printByte1(a_5,b_5,index);break;
    case 6 : printByte1(a_6,b_6,index);break;
    case 7 : printByte1(a_7,b_7,index);break;
    case 8 : printByte1(a_8,b_8,index);break;
    case 9 : printByte1(a_9,b_9,index);break;
    case -2: printByte1(emp,dot,index);break;
    default: printByte1(dot,dot,index);break;
  }
}

void printByte1(byte character_a [],byte character_b [] ,int index)
{
  for(int i=0;i<8;i++)
  {
    lcB.setRow(index,i,character_a[i]);
  }

  for(int i=0;i<8;i++)
  {
    lcA.setRow(index,i,character_b[i]);
  }
}

void button_push() {
  if (!digitalRead(button)){
      while (!digitalRead(button)){Serial.println("button is push");}
      cnt++;
      if (cnt==4){
        cnt = 0;  
      }
    }
}

