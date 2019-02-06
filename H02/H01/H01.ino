#include <adc.h>

#include <i2c.h>
#include <serial.h>

// **********************************************************************************
// Example file reading temperature and humidity from TH02 sensor
// **********************************************************************************
// Creative Commons Attrib Share-Alike License
// You are free to use/extend this library but please abide with the CC-BY-SA license:
// http://creativecommons.org/licenses/by-sa/4.0/
//
// For any explanation see TH02 sensor information at
// http://www.hoperf.com/sensor/app/TH02.htm

// Code based on following datasheet
// http://www.hoperf.com/upload/sensor/TH02_V1.1.pdf
//
// Written by Charles-Henri Hallard (http://hallard.me)
//
// History : V1.00 2014-07-14 - First release
//           V1.10 2015-04-13 - changed to Wire library instead of I2C
//
// All text above must be included in any redistribution.
//
// **********************************************************************************


#include <Wire.h>
#include <TH02.h>

#define PIR_MOTION_SENSOR 2
#define LED 4

// Instantiate TH02 sensor
TH02 th02(TH02_I2C_ADDR);

boolean TH02_found = false;
int liiketta = 0; 



/* ======================================================================
  Function: printhex
  Purpose : print hex value in 2 digit format
  Input   : value
  Output  :
  Comments:
  ====================================================================== */
void printhex(uint8_t c)
{
  if (c < 16)
    Serial.print('0');
  Serial.print(c, HEX);
}

/* ======================================================================
  Function: i2cScan
  Purpose : scan I2C bus
  Input   : -
  Output  : number of I2C devices seens
  Comments: global var TH02_found set to true if found TH02
  ====================================================================== */
int i2cScan()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning I2C bus ...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print(F("I2C device found at address 0x"));
      printhex(address);

      if (address == 0x40)
      {

        Serial.println(F("-> TH02 !"));
        TH02_found = true;
      }
      else
        Serial.println(F("-> Unknown device !"));

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print(F("Unknow error at address 0x"));
      if (address < 16)
        Serial.print('0');
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found\n"));
  else
    Serial.println(F("Scan done"));

  return nDevices;
}

/* ======================================================================
  Function: setup
  Purpose :
  Input   :
  Output  :
  Comments:
  ====================================================================== */
void setup()
{
  uint8_t devID;
  uint8_t err;
  uint8_t status;
  uint8_t config;
  int liiketta = 0; 
  pinMode(PIR_MOTION_SENSOR, INPUT);

  Serial.begin(9600);
}

/* ======================================================================
  Function: loop
  Purpose :
  Input   :
  Output  :
  Comments:
  ====================================================================== */
void loop()
{
  uint8_t duration;
  //uint8_t status;




  Serial.println(F("Now waiting 10 sec before next conversion."));

   int value = analogRead(A0);
  value = map(value, 0, 800, 0, 10);
  
  Serial.print("Kirkkaus = ");
  Serial.println(value);
  float Resistense = (float)(1023-value)*10/value;
  Serial.print("Resistanssi = ");
  Serial.println(Resistense);

  if(digitalRead(PIR_MOTION_SENSOR))//if it detects the moving people?
  {
        liiketta = 1;
        Serial.print("Hi,people is coming \n");
        //digitalWrite(LED,HIGH);
  }
  else{
        liiketta = 0;
        Serial.print("Watching \n");
  }
        
  if(value < 7 and liiketta ==1)
  {
    digitalWrite(LED,HIGH);

  }
  if(value >= 7)
      digitalWrite(LED,LOW);
  
  
  delay(5000);
}
