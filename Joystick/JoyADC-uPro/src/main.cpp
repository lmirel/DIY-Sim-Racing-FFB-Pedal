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

#include "Joystick.h"
//#include <Wire.h>

//#define SERIAL_DEBUG
int RXLED = 17; // The RX LED has a defined Arduino pin
int TXLED = 30; // The TX LED has a defined Arduino pin

// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 12, JOYSTICK_DEFAULT_HATSWITCH_COUNT,
  true, true, true, false, false, false,
  true, true, true, true, true);
/*Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_GAMEPAD, 8, 8,
  false, false, false, false, false, false,
  false, true, false, true, false);*/

#define ADC_IN_3V3 //use for input from 0 to 3v3
#ifdef ADC_IN_3V3
int max_val = 675;
#else
int max_val = 1023;
#endif

//this uses prescaler to change the uPro clock from 16MHz to 1MHz more suitable for input devices
//!! to be called only in setup()
void setClockTo1MHz()
{
  cli(); //disable interrupts
  CLKPR = (1 << CLKPCE); //enable change of the clock prescaler
  CLKPR = (1 << CLKPS3); //set prescaler to 16 (16MHz / 16 = 1MHz)
  sei(); //re-enable interrupts
}

void setup() {
  //setClockTo1MHz(); //not really working
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RXLED, OUTPUT); // Set RX LED as an output
  pinMode(TXLED, OUTPUT); // Set TX LED as an output
#ifdef SERIAL_DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
#endif
  // Set Range Values
  //Joystick.setAcceleratorRange(0, max_val); // 1023 max 5v, 676 max 3.3v
  Joystick.setThrottleRange(0, max_val); // 1023 max 5v, 676 max 3.3v
  Joystick.setBrakeRange(0, max_val);       // 1023 max 5v, 676 max 3.3v
  //Joystick.setSteeringRange(0, 10000);
  Joystick.begin(false);//no autosend
  //Init Joy
  Joystick.setThrottle(0);
  Joystick.setBrake(0);
  //Joystick.setSteering(5000);
  Joystick.sendState();

#ifdef SERIAL_DEBUG
  Serial.println("Joy ready!");
#endif
}

void delayTo1MHz()
{
  delayMicroseconds(62);
}

void delayTo1MHzNop()
{
  for (int i = 0; i < 15; i++)
  {
    __asm("nop\n\t"); //each NOP = 62.5ns, 15 NOPs = 937.5ns
  }
}

void read_adc()
{
  int a0 = analogRead(A0); //ACC
  int a1 = analogRead(A1); //BRK
#ifdef SERIAL_DEBUG
  Serial.print("Acc A0:");
  Serial.println(a0);
  Serial.print("Brk A1:");
  Serial.println(a1);
#endif
  //Joystick.setAccelerator(a0);
  Joystick.setThrottle(max_val - a0); //reversed input: max_val to 0
  Joystick.setBrake(a1);              //normal input: 0 to max_val
  //Joystick.setSteering(5000);
  Joystick.sendState();
}

void loop() { // run over and over
  //digitalWrite(TXLED, LOW); // set the LED on
  read_adc();
  //short brake to reduce freq to 1MHz because uPro 5v runs at 16MHz
  //  running the joy interface at 16MHz is overkill for an input device
  //  TODO: test this approach at USB interface level
  //!! disable the delay ONLY if it causes issues or if prescaler is used
  delayTo1MHzNop();
  //digitalWrite(TXLED, HIGH); // set the LED off
}
