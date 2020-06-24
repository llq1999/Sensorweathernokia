#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define DISPLAY_WIDTH 84
#define DISPLAY_HEIGHT 45

#include "DHT.h"
#define DHTPIN 2    
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

#include <math.h>
double Thermister(int RawADC) {
double Temp;
Temp = log(((10240000/RawADC) - 10000));
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
Temp = Temp - 273.15; // Convert Kelvin to Celcius
return Temp;
}

//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);

boolean light = true;
volatile boolean buttonA = false;
volatile boolean buttonB = false;
volatile boolean buttonC = false;
int StateOfbuttonA = 0;
int StateOfbuttonB = 0;  
int StateOfbuttonC = 0;        
int NewStateOfbuttonA = 0;        
int NewStateOfbuttonB = 0;        
int NewStateOfbuttonC = 0; 
int x = 84;
int y = 803; //803
int t = 2000;
int yCounter = 0;
int yPosition = 0;
int readings[DISPLAY_WIDTH+1];
int menu=0;
int contrast=50;
void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW); //Turn Backlight ON
  display.begin();      
  display.setContrast(contrast); //Set contrast to 50
  display.clearDisplay(); 
}

void loop() 
{
 
  //  delay(2000);
  double tt;
  tt= analogRead(1);
  double reading = analogRead(0);
  float voltage = reading * 4.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100 ;
     
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  /*
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
    */
  Serial.print(h);
  Serial.print(" ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print(temperatureC);
  Serial.print(" ");
  Serial.print(Thermister(tt));
  Serial.print("\n");
 digitalWrite(LED_BUILTIN, LOW);
 delay(100);//delays 100ms
 digitalWrite(LED_BUILTIN, HIGH);
 delay(100);//delays 100ms
  
  NewStateOfbuttonA = digitalRead(5);
  NewStateOfbuttonB = digitalRead(6);
  NewStateOfbuttonC = digitalRead(7);
  buttonAisPressed();
  buttonBisPressed();
  buttonCisPressed();
         
   if (buttonA) {
   if (menu == 1 ) {
    buttonA = false;
    contrast++;
    setContrast();
    }  
  else if (menu == 2 ) {
    buttonA = false;
  if (light) 
      {
        light = false;
        lightOff();
        }
      else 
      {
        light = true; 
        lightOn();
       }
}
  else if (menu == 3 ) {
    buttonA = false;
     y = y+20;
  }  
  else if (menu == 4 ) {
    buttonA = false;
    x = x + 1;
    if (x > 84 ) {
    x = 84;
  } 
  } 
  else if (menu == 5 ) {
    buttonA = false;
    t = t + 50;
    if (t > 5000 ) {
    t = 5000;
  } 
    }  
  else if (menu == 6 ) {
    buttonA = false;
      menu = 0;
  } 
    }
   if (buttonB) {
    buttonB = false;
      menu++;     
  if (menu == 7 ) {
      menu = 3;
  }
    }
   if (buttonC) {
   if (menu == 1 ) {
    buttonC = false;
    contrast--;
    setContrast();
    } 
  else if (menu == 2 ) {
    buttonC = false;
    if (light) 
      {
        light = false;
        lightOff();
        }
      else 
      {
        light = true; 
        lightOn();
       }
       }     
  else if (menu == 3 ) {
    buttonC = false;
    y = y-20;
    if (y < 10 ) {
    y = 10;
  }    
  }    
  else if (menu == 4 ) {
    buttonC = false;
    x = x - 1;
    if (x < 1 ) {
    x = 1;
  } 
  } 
  else if (menu == 5 ) {
    buttonC = false;
    t = t - 50;
    if (t < 0 ) {
    t = 0;
  } 
  } 
  else if (menu == 6 ) {
    buttonC = false;
      menu = 0;
  } 
  
    }
  display.clearDisplay();
  for(yCounter = 0; yCounter < x; yCounter++)
  {                             
    display.setCursor(0,5);
    display.print(t);
    display.print(" C ");
    display.print(h);
    display.print("%");
    display.setCursor(0,15);
    display.print(temperatureC);
    display.print(" TMP36");
    display.setCursor(0,25);
    display.print(Thermister(tt));
    display.print(" DS18b20");

   //yPosition = (analogRead(A0))*30/y+1;
   //yPosition = (analogRead(A0)*44)/y;
    readings[yCounter] = yPosition;
    delayMicroseconds(t);
  }
  /* 
  for(yCounter = 84; yCounter >= x; yCounter--)
  {              
     readings[yCounter] = readings[yCounter-x];
  }
  for(yCounter = 1; yCounter <= 84; yCounter++)
  {
    display.drawLine(yCounter-1, (DISPLAY_HEIGHT-1)-readings[yCounter-1], yCounter, (DISPLAY_HEIGHT-1)-readings[yCounter], BLACK);
  }
  display.drawLine( 10, 0, 10, DISPLAY_HEIGHT-1, BLACK);
  display.drawLine( 7, DISPLAY_HEIGHT-1, 10, DISPLAY_HEIGHT-1, BLACK);
  display.drawLine( 7, (DISPLAY_HEIGHT-1)-(6.2 *1023/y), 10, (DISPLAY_HEIGHT-1)-(6.2 *1023/y), BLACK);
  display.drawLine( 7, (DISPLAY_HEIGHT-1)-(12.4 *1023/y), 10, (DISPLAY_HEIGHT-1)-(12.4 *1023/y), BLACK);
  display.drawLine( 7, (DISPLAY_HEIGHT-1)-(18.6 *1023/y), 10, (DISPLAY_HEIGHT-1)-(18.6 *1023/y), BLACK);
  display.drawLine( 7, (DISPLAY_HEIGHT-1)-(24.8 *1023/y), 10, (DISPLAY_HEIGHT-1)-(24.8 *1023/y), BLACK);
  display.drawLine( 7, (DISPLAY_HEIGHT-1)-(31 *1023/y), 10, (DISPLAY_HEIGHT-1)-(31 *1023/y), BLACK);
  display.setCursor(0,DISPLAY_HEIGHT-4);
  display.print(0);
  display.setCursor(0,((DISPLAY_HEIGHT-1)-(6.2 *1023/y))-3);
  display.print(1);
  display.setCursor(0,((DISPLAY_HEIGHT-1)-(12.4 *1023/y))-3);
  display.print(2);
  display.setCursor(0,((DISPLAY_HEIGHT-1)-(18.6 *1023/y))-3);
  display.print(3);
  display.setCursor(0,((DISPLAY_HEIGHT-1)-(24.8 *1023/y))-3);
  display.print(4);
  display.setCursor(0,((DISPLAY_HEIGHT-1)-(31 *1023/y))-3);
  display.print(5);
  */
    if (menu == 1) 
      {
    display.setCursor(20, 0);
    display.print("con:");
    display.print(contrast);
    }
    else if (menu == 2) 
      {
    display.setCursor(75, 0);
    display.print("*");
    }
    else if (menu == 3) 
      {
    display.setCursor(35, 0);
    display.setTextColor(WHITE, BLACK);
    display.print("y: ");
    display.setTextColor(BLACK, WHITE);
    display.println(y);
    display.setCursor(35, 8);
    display.print("x:");
    display.println(x);
    display.setCursor(35, 16);
    display.print("t:");
    display.println(t);
    display.setCursor(35, 24);
    display.print("Exit");
    }
    else if (menu == 4) 
      {
    display.setCursor(35, 0);
    display.print("y: ");
    display.println(y);
    display.setCursor(35, 8);
    display.setTextColor(WHITE, BLACK);
    display.print("x:");
    display.setTextColor(BLACK, WHITE);
    display.println(x);
    display.setCursor(35, 16);
    display.print("t:");
    display.println(t);
    display.setCursor(35, 24);
    display.print("Exit");
    }
    else if (menu == 5) 
      {
    display.setCursor(35, 0);
    display.print("y: ");
    display.println(y);
    display.setCursor(35, 8);
    display.print("x:");
    display.println(x);
    display.setCursor(35, 16);
    display.setTextColor(WHITE, BLACK);
    display.print("t:");
    display.setTextColor(BLACK, WHITE);
    display.println(t);
    display.setCursor(35, 24);
    display.print("Exit");
    }
    else if (menu == 6) 
      {
    display.setCursor(35, 0);
    display.print("y: ");
    display.println(y);
    display.setCursor(35, 8);
    display.print("x:");
    display.println(x);
    display.setCursor(35, 16);
    display.print("t:");
    display.println(t);
    display.setCursor(35, 24);
    display.setTextColor(WHITE, BLACK);
    display.print("Exit");
    display.setTextColor(BLACK, WHITE);
    }
    

  display.display();
}
void buttonAisPressed()
  {
    if (NewStateOfbuttonA != StateOfbuttonA) 
  {
    if (NewStateOfbuttonA == 0) 
    {
      buttonA=true;
    }
    delay(50);
  }
   StateOfbuttonA = NewStateOfbuttonA;
  }

void buttonBisPressed()
{
  if (NewStateOfbuttonB != StateOfbuttonB) 
  {
    if (NewStateOfbuttonB == 0) {
      buttonB=true;
    }
    delay(50);
  }
   StateOfbuttonB = NewStateOfbuttonB;
}

void buttonCisPressed()
{
   if (NewStateOfbuttonC != StateOfbuttonC) 
  {
    if (NewStateOfbuttonC == 0) {
      buttonC=true;
    }
    delay(50);
  }
   StateOfbuttonC = NewStateOfbuttonC;
}

void lightOn()
  {
    digitalWrite(4,LOW);
  }

    void lightOff()
  {
    digitalWrite(4,HIGH);
  }
  void setContrast()
  {
    display.setContrast(contrast);
    display.display();
  }
