#include <Arduino.h>

int pwm_pin = 9; // Pin 9 has timer/counter 2
int rpm_pin = 2;
int baud_rate = 9600;
int icr = 639;
int duty_cycle = 0;
volatile unsigned int count = 0; // Variable used in interrupts to be volatile

int main(void) {
  int start_time;

  //Initialise arduino ( so millis, delay work ).
  init();
  Serial.begin(baud_rate);
  
  // Set pin 2 to be INPUT_PULLUP so internal 5V pullup resistor is used
  pinMode(rpm_pin, INPUT_PULLUP);
  // Set tacho pin to increment counter on interrupt
  attachInterrupt(digitalPinToInterrupt(rpm_pin), incremment_counter, RISING);

  // Setup registers for fast PWM
  setup_registers();

  while ( true ) {
    analogWrite(pwm_pin, 0);
    Serial.print("Duty cycle (%) = ");
    Serial.println(0);
    get_fan_rpm();
    delay(1000); // Delay before next reading

    analogWrite(pwm_pin, 255);
    Serial.print("Duty cycle (%) = ");
    Serial.println(100);
    get_fan_rpm();
    delay(1000); // Delay before next reading
  }
  return 0;
}

void setup_registers() {
  Serial.println("Setting up registers...");

  // Timer counter 1 == 8-bit timer: OC0A and OC0B which is pins PD5 (D5) and PD6 (D6)
  pinMode(pwm_pin, OUTPUT);

  // Clear timer/counter control registers
  TCCR1A = 0;
  TCCR1B = 0;

  // Set Fast PWM with TOP = ICR1 and no prescaler
  // Set Fast PWM mode clear OC1A on compare match
  TCCR1A |= (1 << COM1A1);

  // Set Fast PWM mode TOP = ICR1
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << WGM13);

  // Set no prescaling and start PWM
  TCCR1B |= (1 << CS10);

  // Set ICR1 = TOP = 639
  // Calculate from fpwm = fclk/N*(1+TOP)
  // Where:
  //  fpwm = 25kHz (fan frequency)
  //  fclk = 16MHz (Atmega328p speed)
  //  N = 1 (no prescaler applied)
  ICR1H = icr >> 8;
  ICR1L = icr & 0x00FF;

  OCR1A = icr * (duty_cycle / 100.0);

  Serial.println("Registers setup!");
}

void get_fan_rpm() {
  count = 0;
  delay(1000); // Count all interrupts in 1 second
  int rpm = count * 60 / 2;

  Serial.print("Rpm = ");
  Serial.println(rpm);
}

void incremment_counter() {
  count++;
}


/////////////////////////
// ONLINE WORKING EXAMPLE
/////////////////////////

// int icr, frecuency, duty_cycle;
// void setup() {
//   Serial.begin(9600);
//   pinMode(11, OUTPUT);
  
//   // Timer/Counter 1 initialization
//   TCCR1A = 0xA2;
//   TCCR1B = 0x19;
//   TCNT1H = 0x00;
//   TCNT1L = 0x00;

//   duty_cycle = 100; // Change this value to the desired duty cycle (0-100 %)
//   icr = 639; // icr and frecuency values are set to generate a 25 kHZ signal
//   frecuency = 16000000 / (1 + icr);

//   ICR1H = icr >> 8;
//   ICR1L = icr & 0x00ff;
//   // Sets the duty cycle with the value entered in line 12 
//   OCR1A = icr * (duty_cycle / 100.0);
// }

// void loop() {
//   // Prints the current duty cycle on the serial monitor every second
//   duty_cycle = 0;
//   OCR1A = icr * (duty_cycle / 100.0);
//   Serial.print(" Duty cycle (%) = "); Serial.println(duty_cycle);
//   delay(3000);

//   duty_cycle = 100;
//   OCR1A = icr * (duty_cycle / 100.0);
//   Serial.print(" Duty cycle (%) = "); Serial.println(duty_cycle);
//   delay(3000);
// }

////////////////////
// PULL_DOWN EXAMPLE
////////////////////
// https://docs.arduino.cc/tutorials/generic/digital-input-pullup/
/*

  Input Pull-up Serial

  This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital

  input on pin 2 and prints the results to the Serial Monitor.

  The circuit:

  - momentary switch attached from pin 2 to ground

  - built-in LED on pin 13

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal

  20K-ohm resistor is pulled to 5V. This configuration causes the input to read

  HIGH when the switch is open, and LOW when it is closed.

  created 14 Mar 2012

  by Scott Fitzgerald

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/InputPullupSerial

*/

// void setup() {

//   //start serial connection

//   Serial.begin(9600);

//   //configure pin 2 as an input and enable the internal pull-up resistor

//   pinMode(2, INPUT_PULLUP);
//   // pinMode(2, INPUT);

//   pinMode(13, OUTPUT);

// }

// void loop() {

//   //read the pushbutton value into a variable

//   int sensorVal = digitalRead(2);

//   //print out the value of the pushbutton

//   Serial.println(sensorVal);

//   // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes

//   // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the

//   // button's pressed, and off when it's not:

//   if (sensorVal == HIGH) {

//     digitalWrite(13, LOW);

//   } else {

//     digitalWrite(13, HIGH);

//   }
// }
