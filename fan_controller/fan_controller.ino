#include <Arduino.h>

int analogPin = 9; // Pin 9 has timer/counter 2
int baudRate = 9600;
int icr = 639;
int duty_cycle = 0;

int main(void) {
  //Initialise arduino ( so millis, delay work ).
  init();
  Serial.begin(baudRate);

  // Setup registers for fast PWM
  setup_registers();

  while ( true ) {
    analogWrite(analogPin, 0);
    delay(1000); // Delay before next reading
    Serial.print("Duty cycle (%) = ");
    Serial.println(0);

    analogWrite(analogPin, 255);
    delay(1000); // Delay before next reading
    Serial.print("Duty cycle (%) = ");
    Serial.println(100);
  }
  return 0;
}

void setup_registers() {
  Serial.println("Setting up registers...");

  // Timer counter 1 == 8-bit timer: OC0A and OC0B which is pins PD5 (D5) and PD6 (D6)
  pinMode(analogPin, OUTPUT);

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