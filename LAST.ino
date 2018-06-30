#include <Wire.h>

//#define DEV_ID 0x90 >> 1                        // shift required by wire.h
#define DEV_ID 000
//#define DEV_ID 1
//#define DEV_ID 2
//#define DEV_ID 3
//#define DEV_ID 4
//#define DEV_ID 5
//#define DEV_ID 6
//#define DEV_ID 7

// DS1621 Registers & Commands

#define RD_TEMP    0xAA                         // read temperature register
#define ACCESS_TH  0xA1                         // access high temperature register
#define ACCESS_TL  0xA2                         // access low temperature register
#define ACCESS_CFG 0xAC                         // access configuration register
#define RD_CNTR    0xA8                         // read counter register
#define RD_SLOPE   0xA9                         // read slope register
#define START_CNV  0xEE                         // start temperature conversion
#define STOP_CNV   0X22                         // stop temperature conversion

// DS1621 configuration bits

#define DONE       B10000000                    // conversion is done
#define THF        B01000000                    // high temp flag
#define TLF        B00100000                    // low temp flag
#define NVB        B00010000                    // non-volatile memory is busy
#define POL        0x02 //10                    // output polarity (1 = high, 0 = low)
#define ONE_SHOT   0x01 //01                    // 1 = one conversion; 0 = continuous conversion

void setup()
{
    Serial.begin(9600);
    Wire.setModule(0);
    
    Wire.begin();
    Wire.beginTransmission(DEV_ID);             // connect to DS1621 (#0)
    Wire.write(0xAC);                            // Access Config
    Wire.write(0x02);                            // set for continuous conversion
    Wire.beginTransmission(DEV_ID);             // restart
    Wire.write(0xEE);                            // start conversions
    Wire.endTransmission();
}

void loop()
{
    int8_t firstByte = 0;
    int8_t secondByte = 0;
    int8_t result = 0;

    delay(3000);                                // give time for measurement

    Wire.beginTransmission(DEV_ID);
    Wire.write(0xAA);                            // read temperature command
    Wire.endTransmission();
    Wire.requestFrom(DEV_ID, 3);    // request two bytes from DS1621 (0.5 deg. resolution)

    if(Wire.available())
    {
      firstByte = Wire.read();           // get first byte
      Serial.print("firstByte");
      Serial.println(firstByte);
    }
    else
    {
      Serial.println("NO sygnal for firstByte");
    }

    if(Wire.available())
    {
      secondByte = Wire.read();    // get second byte
      Serial.print("secondByte");
      Serial.println(secondByte);
    }
    else
    {
      Serial.println("NO sygnal for secondByte");
    }

    result = firstByte;

    if (secondByte)             // if there is a 0.5 deg difference
        result += 0.5;

Serial.print("RESULT: ");
Serial.println(result);
}
