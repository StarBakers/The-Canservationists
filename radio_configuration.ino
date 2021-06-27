#include <SoftwareSerial.h>

const int pinRX = 2;
const int pinTX = 3;
const int pinSET= 13;

SoftwareSerial apc220(pinRX, pinTX);

void setupSoftAPC(void){
pinMode(pinSET, HIGH);
apc220.begin(9600);
}

void getSettings(void) {
digitalWrite(pinSET, LOW); // pulling SET to low will put apc220 in config mode
delay(10); // stabilize please
apc220.println("RD"); // ask for data
delay(10);
while (apc220.available()) {
Serial.write(apc220.read());
}
digitalWrite(pinSET, HIGH); // put apc220 back in operation
delay(200);
}

void setSettings(void){
digitalWrite(pinSET, LOW); // pulling SET to low will put apc220 in config mode
delay(10); // stabilize please
apc220.println("WR 433900 2 9 3 0"); // ask for data
delay(10);
while (apc220.available()) {
Serial.write(apc220.read());
}
digitalWrite(pinSET, HIGH); // put apc220 back in operation
delay(200);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupSoftAPC();
  setSettings();
  getSettings();
}

void loop() {
  // put your main code here, to run repeatedly:
  apc220.println("Hello World!");
  Serial.println("Hello World!");
  delay(1000);
}

  
  
  
