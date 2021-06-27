#include <U8g2lib.h>
#include <U8x8lib.h>
#include <U8g2Graphing.h>
#include <Arduino.h>
//Gravity Acceleration
#include "LIS3DHTR.h"
#ifdef SOFTWAREWIRE
    #include <SoftwareWire.h>
    SoftwareWire myWire(3, 2);
    LIS3DHTR<SoftwareWire> LIS;       //Software I2C
    #define WIRE myWire
#else
    #include <Wire.h>
    LIS3DHTR<TwoWire> LIS;           //Hardware I2C
    #define WIRE Wire
#endif

#define NTD3 193
#define NTD5 256


U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); //page buffer

//Constructor, takes a reference to the current instance of U8g2 display.
U8g2Graphing graph(&u8g2);
int mag = 0;
int tune[]=
{
NTD3,NTD5,NTD3,NTD5
};

float durt[]=
{
0.625,1,0.625,1
};
float value1 = 0;
bool on = false;
int buttonState = 0;
int length;
int tonepin=5;
int ledp=4;
float x,y,z;
float _ndx = 0;
int state = 0;

void setup(void) {
      Serial.begin(9600);
       pinMode(A0, INPUT);

      pinMode(tonepin,OUTPUT);
      pinMode(ledp,OUTPUT);
      length=sizeof(tune)/sizeof(tune[0]);
      u8g2.begin();
      u8g2.firstPage();
      do {
        u8g2.setCursor(1, 17);
        u8g2.setFont(u8g2_font_ncenB12_tr);
        u8g2.print("Earthquake");
        u8g2.setCursor(1, 35);
        u8g2.print("Seismometer");
      } 
      while ( u8g2.nextPage() );
      delay(2500);
      u8g2.clearDisplay();
      u8g2.setFontMode(1);
      u8g2.setDrawColor(2);
      while (!Serial) {};
      LIS.begin(WIRE, 0x19); //IIC init
      delay(100);
      LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  graph.beginInt(0, 15, 127, 63);

  graph.displaySet(true, false);
Serial.println("hifd");
      
}

void loop(void) {
  switching();
  pausableGraph();
    x = LIS.getAccelerationX();
    y = LIS.getAccelerationY();
    z = LIS.getAccelerationZ();
    value1 = (sqrt(sq(x)+sq(y)+sq(z)));
    Serial.println(value1);
    Serial.print(" ");


  /*  u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(2, 0);
    u8x8.print(x);
    u8x8.setCursor(2, 1);
    u8x8.print(y);
    u8x8.setCursor(2, 2);
    u8x8.print(z);
    if (z > 1.1 &&  x >0.1 && y>0.1) {
        while (z > 1.1, x >.1, y>.1) {
          alert();
        }
          /*u8x8.setCursor(0, 0);
          u8x8.print("EARTHQUAKE");
          u8x8.setCursor(0, 1);
          u8x8.print("WARNING!");
          u8x8.setCursor(0, 2);
          u8x8.print("TAKE SHELTER");
          delay(1000);
          u8x8.clearDisplay();
    }*/
  // put your main code here, to run repeatedly:
void();
}
void alert(){

    u8g2.firstPage();
      do {
        u8g2.setCursor(4, 27);
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.print("EARTHQUAKE");
        u8g2.setCursor(4, 45);
        u8g2.print("WARNING!");
            } 
      while ( u8g2.nextPage() );
      for(int t = 0; t<50; t++) {
        digitalWrite(ledp, HIGH);
        delay(100);
        digitalWrite(ledp, LOW);
     
    
        delay(100);
        
      }
      for(int u=0;u<length;u++)
  {
    

  }
      

      /*for(int g=0;g<length;g++)
  {


    tone(tonepin,tune[g]);
    delay(400*durt[g]);
    delay(100*durt[g]);
    noTone(tonepin);
Serial.print("test");
  }
        delay(5000);
}*/}

void magt() {
  mag = log(value1/1000*100)/1000;
  while (on == false) {}
}
void pausableGraph() {
  static bool setActive = true;
  //Initialize setting
  graph.displaySet(true, false);
  graph.pointerSet(false);
  graph.rangeSet(false);


  //10 second loop
  while (on == true) {
    switching();
    x = LIS.getAccelerationX();
    y = LIS.getAccelerationY();
    z = LIS.getAccelerationZ();
    value1 = (sqrt(sq(x)+sq(y)+sq(z)));
    Serial.println(value1);
    Serial.print(" ");
 
    
    if (value1 > 2) {
      Serial.print("bbb");
      delay(1000); 
      alert();
    }
    //Set the graph to run or pause.
    //Take note that this will mess with X axis label(time) accuracy.


    graph.inputValue(value1*1000);
    graph.pointerSet(true, (int)_ndx);

    u8g2.firstPage();
    do {
      u8g2.setCursor(1, 7);
      u8g2.setFont(u8g2_font_6x10_tr);
      if (setActive){
        u8g2.print("Y = Velocity(um)");
        u8g2.setCursor(1, 15);
        u8g2.print("X = Time(ms) ");
      } else {
        u8g2.print("Paused ");
      }
      

      graph.displayGraph();
    } while (u8g2.nextPage());

  }
  graph.intervalSet(0);



  u8g2.firstPage();
  do {
  } while (u8g2.nextPage());
  

}

void switching() {
  buttonState = analogRead(A0);
  if (buttonState < 500) {
    on = true;
  }
  else if (buttonState > 500) {
    on = false;
  }
  else{
    on = false;
  }
}
