#include <M5StickC.h>

const int analogInPin = 26; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

void setup() {
//  Serial.begin(9600);
  // Initialize the M5StickC object
  M5.begin();

  // Lcd display
  M5.Lcd.begin();
  M5.Lcd.print("Starting");
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.printf("pH sensor");
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(0,10);
  double vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  M5.Lcd.printf("Battery:%.3fV\r\n",vbat);  //battery voltage
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(30,30);
  M5.Lcd.print("Ph = ");
  M5.Lcd.setTextSize(3);
}

void loop() {
  for(int i=0;i<10;i++) 
  { 
    buf[i]=analogRead(analogInPin);
    Serial.print("AD = ");
    Serial.println(buf[i]);
    delay(10);
  }
  for(int i=0;i<9;i++)
    {
    for(int j=i+1;j<10;j++)
      {
      if(buf[i]>buf[j])
        {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
    avgValue+=buf[i];
  //float pHVol=(float)avgValue*5.0/1024/6;
  float pHVol=(float)avgValue/6*3.4469/4095;
  Serial.print("v = ");
  Serial.println(pHVol);

  float phValue = -5.70 * pHVol + 21.34;    
  //float phValue = 7 + ((2.5 - pHVol) / 0.18);
  Serial.print("Ph=");
  Serial.println(phValue);

  // LCD display
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(30,50);
  M5.Lcd.print(phValue);
  delay(5000);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(30,50);
  M5.Lcd.print(phValue);
}
