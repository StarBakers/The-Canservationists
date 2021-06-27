#include <NeoICSerial.h>
#include <TinyGPS++.h>
#include <Adafruit_BMP280.h>
#include <SparkFunCCS811.h>
#include <SPI.h>
#include <SD.h>

// NEoICSerial GNSS Receive 8 
// NeoICSerial GNSS Transmit 9
#define buzzer 3 //buzzer to arduino pin 3
#define GPSBaud 9600 //Default baud of NEO-6M is 9600
#define CCS811_ADDR 0x5B //I2C Address
#define BM280_ADDR 0x76
#define chipSelect 10 //for SD card
/*
  SD card circuit:
    SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module. use chipSelect var
*/

NeoICSerial gnss_connection;
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the work with the NMEA data
Adafruit_BMP280 bmp;// use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
CCS811 ccs811Sensor(CCS811_ADDR);

File dataFile;
float latitude = 0.0;
float longitude = 0.0;
float gnss_altitude = 0.0;
float gnss_speed = 0.0;
unsigned long int runtime = 0; //in milliseconds
float temperature = 0.0;
float pressure = 0.0;
float alt = 0.0;
int co2 = 0;
int tvoc = 0;
boolean beep = false;

void gnss()
{
   while(gnss_connection.available())//While there are characters to come from the GPS
   {
    byte gpsData = gnss_connection.read();
    //Serial.write(gpsData);//useful for troubleshooting, uncomment to show communication with the sensor.
    gps.encode(gpsData);
    //Serial.println(F("Satellite Count:"));
    //Serial.println(gps.satellites.value());
    if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
      {
        //Get the latest info from the gps object which it derived from the data sent by the GPS unit
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        gnss_speed = gps.speed.kmph();
        gnss_altitude = gps.altitude.meters();
       }
    }  
}

void radio()
{
  //Serial.pc220.print(gpsData);
  Serial.print((runtime));
  Serial.print(F(","));
  Serial.print(latitude,6);
  Serial.print(F(","));
  Serial.print(longitude,6);
  Serial.print(F(","));
  Serial.print(gnss_altitude);
  Serial.print(F(","));
  Serial.print(alt);
  Serial.print(F(","));
  Serial.print(temperature);
  Serial.print(F(","));
  Serial.print(pressure);
  Serial.print(F(","));
  Serial.print(co2);
  Serial.print(F(","));
  Serial.print(tvoc);
  Serial.print(F(","));
  Serial.println(gnss_speed);
}

void bmp_measurements()
{
  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);
  temperature = temp_event.temperature;
  pressure = pressure_event.pressure;
 //temperature = bmp.readTemperature();
 //pressure = bmp.readPressure();
 alt = bmp.readAltitude(1002.75);
  }

void cs811_measurements()
{
  if (ccs811Sensor.dataAvailable())
  {
    ccs811Sensor.readAlgorithmResults();
    co2 = ccs811Sensor.getCO2();
    tvoc = ccs811Sensor.getTVOC();
  }
}

void write_to_sd_card(){
  dataFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      //Serial.println("data file opened");
        dataFile.print(runtime);
        dataFile.print(F(","));
        dataFile.print(latitude,6);
        dataFile.print(F(","));
        dataFile.print(longitude,6);
        dataFile.print(F(","));
        dataFile.print(gnss_altitude);
        dataFile.print(F(","));
        dataFile.print(alt);
        dataFile.print(F(","));
        dataFile.print(temperature);
        dataFile.print(F(","));
        dataFile.print(pressure);
        dataFile.print(F(","));
        dataFile.print(co2);
        dataFile.print(F(","));
        dataFile.print(tvoc);
        dataFile.print(F(","));
        dataFile.println(gnss_speed);
        dataFile.close();}
}

void setup()
{
  Serial.begin(9600);//Opens up communications to Hardware Serial - used for the radio
  delay(10);
  gnss_connection.begin(GPSBaud);
    
  if (!SD.begin(chipSelect)) {
  Serial.println(F("Card failed, or not present"));
  }
  else {
    Serial.println(F("card initialized."));  
  }
    dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
        Serial.println(F("data file opened"));
        dataFile.println(F("Runtime,Latitude,Longitude,altitude_from_GNSS,altitude_per_pressure,temperature,pressure,co2,tvoc,speed"));
        dataFile.close();
        }
  
    Wire.begin(); //Inialize I2C Hardware
  if (!bmp.begin(BM280_ADDR)) {
   Serial.println(F("Could not connect to BMP280 sensor!"));
  }
  else{
      // Default settings from datasheet. 
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode. 
                  Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling 
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling 
                  Adafruit_BMP280::FILTER_X16,      // Filtering. 
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time. 
                  //bmp_temp->printSensorDetails();
    }
  if (!ccs811Sensor.begin())
  {
    Serial.println(F("Could not connect to CCS811 sensor!"));
  }
   pinMode(buzzer, OUTPUT);
}

void loop() {
  runtime = millis();
  gnss();
  bmp_measurements();
  cs811_measurements();
  write_to_sd_card();
  radio();
  //if ((gnss_altitude > 500.0)||(alt > 500.0)){ //set the buzzer flag above 500)
  if (gnss_altitude > 500.0){ //set the buzzer flag above 500)
    beep = true;
  }
  //if ((beep == true) && ((gnss_altitude < 10.0)||(alt < 10.0))){ 
  if ((beep == true) && (gnss_altitude < 10.0)){ //start the buzzer if buzzer flag is set ans alt < 10m
    tone(buzzer, 1000);
  }
  delay(100);
}
