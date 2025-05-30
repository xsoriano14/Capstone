
// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO 16

void setup() {
  // set the LED as an output
  pinMode(ledPin, OUTPUT);
}

void loop(){
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    digitalWrite(LED_BUILTIN, HIGH);    // Send the signal to the ESC
    delay(15);                 // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(50); 
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
     digitalWrite(LED_BUILTIN, HIGH);    // Send the signal to the ESC
    delay(15);                 // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(50);
  }
}