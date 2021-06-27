#include <SoftwareSerial.h>

#define RF_RX 8
#define RF_TX 9

SoftwareSerial apc220(RF_RX,RF_TX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  apc220.begin(9600); //for APC220
  Serial.println("Receiver started");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(apc220.available() > 0)
  {
   Serial.print(char(apc220.read()));
  }
}
