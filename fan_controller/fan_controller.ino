#include <Arduino.h>

int analogPin = 5;

int main(void) {
  //Initialise arduino ( so millis, delay work ).
  init();

  // Set PWM timer registers
  TCCR2A = _BV()

  // Initialise constants
  pinMode(analogPin, OUTPUT);

  while ( true ) {
    // Serial.println("hello world");
    analogWrite(analogPin, 0);
    delay(1000); // Delay before next reading

    analogWrite(analogPin, 255/2);
    delay(1000); // Delay before next reading

    analogWrite(analogPin, 255);
    delay(1000); // Delay before next reading
    
  }

  // int green = 7;
  // pinMode(green, OUTPUT);

  // for (;;) {
  //   digitalWrite(green, HIGH);
  //   delay(1000);
  //   digitalWrite(green, LOW);
  //   delay(1000);
  // }


  return 0;
}

void set_registers() {
  // Timer counter 1 == 8-bit timer: OC0A and OC0B which is pins PD5 (D5) and PD6 (D6)
  pinMode(analogPin, OUTPUT);

  TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00)
  // Output frequency needs to be 25kHz, 16MHz / 
  TCCR0B = _BV()
  OCR0A = 180;
}