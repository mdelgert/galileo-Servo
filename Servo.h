#ifndef Servo_h
#define Servo_h

#include "Arduino.h"
#include "Wire.h"

#define MIN_ANGLE              0      // min angle
#define MAX_ANGLE            180      // max angle


#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached

#define INVALID_SERVO        255

/* How the pins are connected to cypress in FabD */

#define GPORT0_BIT4_PWM7   0x10   // pin 3
#define GPORT0_BIT1_PWM5   0x02   // pin 5
#define GPORT1_BIT0_PWM6   0xff   // pin 6   
#define GPORT0_BIT3_PWM1   0x08   // pin 9
#define GPORT0_BIT0_PWM7   0x01   // pin 10  
#define GPORT1_BIT1_PWM4   0xff   // pin 11 

#define CYPRESS_I2C_ADDRESS 0x20  // I2C address

#define MAX_NUMBER_OF_SERVOS  6   // same number of pins in PWM

#define FREQUENCY_DIVISOR_HI_RES 0x2 // 184Hz on cypress
#define FREQUENCY_DIVISOR_LO_RES 0x9 // 48Hz on cypress

#define MY_TRACE_PREFIX "ServoX86Lib"

typedef struct  {
  uint8_t pin;
  uint8_t reg28_data;
  bool isActive;             // true if this channel is enabled, pin not pulsed if false
} servoPinData_t;  

class Servo
{
public:
  Servo();
  uint8_t attach(int16_t pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes.
  void detach();
  void write(int val);               // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds
  void writeMicroseconds(int value); // Write pulse width in microseconds 
  int read();                        // returns current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
  bool attached();                   // return true if this servo is attached, otherwise false 
  void set48hz();                    // (deprecated) forces cypress to work in 47.8 hertz
  void set188hz();                   // (deprecated) forces cypress to work in 184 hertz (better angle resolution)
  void setResolution(int res);       // sets cypress angle resolution from 0 = low res/high compatibility to 7 = high res/low compatibility

private:
  int min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH
  int max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH

  uint8_t index;                 // servo index

  // this compiler does not accept [] should use [6]
  servoPinData_t pinData[6] = {{3,  GPORT0_BIT4_PWM7, false},
                               {5,  GPORT0_BIT1_PWM5, false},
                               {6,  GPORT1_BIT0_PWM6, false},
                               {9,  GPORT0_BIT3_PWM1, false},
                               {10, GPORT0_BIT0_PWM7, false},
                               {11, GPORT1_BIT1_PWM4, false}};


  static uint8_t counter;   // only for counting

  void prepare_pin(uint8_t pin);
  byte transform_cypress_duty_cycle_byte(int microsecs);
  int usecs;
  bool isAttached;
  byte pin;
  byte frequencyDivisor;
  int lastByteInDuty;          // to avoid jitter caused by analogWrite()
};

#endif
