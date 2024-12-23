/*
 Receives from software serial, sends to Joystick.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 ** Joystick - USB Joystick object on the Arduino Leonardo or Arduino Micro.
 Lib from Matthew Heironimus https://github.com/MHeironimus/ArduinoJoystickLibrary/archive/version-2.0.zip

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
#include "Joystick.h"

// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
  false, false, false, false, false, false,
  false, false, true, true, false);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}
  // Set Range Values
  Joystick.setAcceleratorRange(0, 10000);
  Joystick.setBrakeRange(0, 10000);
  //Joystick.setSteeringRange(0, 10000);
  //Init Joy
  Joystick.begin(false);//autosend
  delay(1000);
  //
  Joystick.setAccelerator(0);
  Joystick.setBrake(0);
  //Joystick.setSteering(5000);
  Joystick.sendState();

  Serial.println("Ready!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  mySerial.println("SS Ready!");
}

long int lval = 0;
void loop() { // run over and over
  long int val;
  byte mt;
  byte ml;
  if (mySerial.available() > 1)
  {
    mySerial.readBytes((char*)&mt, 1);
    mySerial.readBytes((char*)&ml, 1);
    if (mt == 0x0e && ml == 4)
    {
      if (mySerial.available()>= ml)
      {
        mySerial.readBytes((char*)&val, ml);
        Serial.print("mlen: ");
        Serial.print(ml);
        Serial.print(" value:");
        Serial.println(val);
        if (lval != val)
        {
          lval = val;
          //Joystick.setRudder(val);
          Joystick.setAccelerator(val);
          Joystick.setBrake(val);
          Joystick.sendState();
        }
      }
    }
    else
    {
      //flush serial data
      while(Serial.available()) Serial.read();
    }
  }
}
