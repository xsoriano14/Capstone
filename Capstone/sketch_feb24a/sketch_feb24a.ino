#include <ESP32Servo.h>
 
Servo myservo, myservo1;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

 
void setup() {
	// Allow allocation of all timers
	Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo1.setPeriodHertz(50);
	myservo.attach(16, 1000, 2500); // attaches the servo on pin 18 to the servo object
	myservo1.attach(17, 1000, 2500); // attaches the servo on pin 18 to the servo object

  // using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
  pinMode(2, OUTPUT);

  pinMode(4, OUTPUT);
  digitalWrite(2, LOW); 
   
  delay(3000); // delay to plug power in 
  myservo.write(0);
  myservo1.write(0);
  
  delay(2000); // delay to wait for confirmation beeps


}

void loop() {
   digitalWrite(2, LOW);  
    delay(500);

   //int potValue = analogRead(4);
  // Serial.println(potValue);
   //map(potValue, 0, 4095, 0, 180);
    //myservo.write(potValue);

   //delay(500); 

   Serial.println("Enter Speed");

   while(Serial.available() == 0) {
    digitalWrite(2,HIGH);

   }
   digitalWrite(2,LOW);
   int speed = Serial.parseInt();
    //myservo.write(speed);
    //myservo1.write(speed);
   switch (speed){

    case 1:
      Serial.println("Speed is 0");
      myservo.write(0);
      myservo1.write(0);
      break;

    case 2:
      Serial.println("Speed is 20");
      //myservo.write(20);
      //myservo1.write(20);
      myservo.write(5);
      myservo1.write(5);
      break;

    case 3:
      Serial.println("Speed is 30");
      //myservo.write(30);
      //myservo1.write(30);
      myservo.write(10);
      myservo1.write(10);
      break;
    
    case 4:
     Serial.println("Speed is 45");
     //myservo.write(45);
     //myservo1.write(45);
       myservo.write(15);
      myservo1.write(15);
    
     break;

    case 5:
      Serial.println("Speed is 90");
      //myservo.write(90);
      //myservo1.write(90);
         myservo.write(20);
      myservo1.write(20);
      break;

    case 6:
      Serial.println("Speed is 90");
      //myservo.write(90);
      //myservo1.write(90);
         myservo.write(25);
      myservo1.write(25);
      break;

      case 7:
      Serial.println("Speed is 90");
      //myservo.write(90);
      //myservo1.write(90);
         myservo.write(35);
      myservo1.write(35);
      break;

      case 8:
      Serial.println("Speed is 90");
      //myservo.write(90);
      //myservo1.write(90);
         myservo.write(50);
      myservo1.write(50);
      break;
    case 9:
        Serial.println("Speed is 180");
        myservo1.write(60);
        myservo.write(60);
     break;

    default:
      Serial.println("choose selection to change");
      //myservo.write(0);
      

   }

/*
 myservo.write(180);
 myservo1.write(180);
  digitalWrite(2, HIGH);    
  delay(15);                  // wait for a second
  //digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(100); 
  myservo1.write(0);

  myservo.write(0);
  digitalWrite(2, LOW); 
  delay(4000);  */
 
 
 
 /*
 
  //potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  //.potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
 myservo.write(180);
 myservo1.write(180);
  digitalWrite(2, HIGH);    
  delay(4000);                  // wait for a second
  //digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(100); 
  myservo1.write(0);

  myservo.write(0);
  digitalWrite(2, LOW); 
  delay(4000);  

 myservo.write(10);
  myservo1.write(10);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000); 


 myservo.write(20);
  myservo1.write(20);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000); 

   myservo.write(25);
  myservo1.write(25);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000); 
 
 
  myservo.write(30);
  myservo1.write(30);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000); 

    myservo.write(45);
  myservo1.write(45);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000);  */

  ///////////////////////////////////////////////////////////////////////



/*
    myservo.write(60);
  myservo1.write(60);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000);   

    myservo.write(75);
  myservo1.write(75);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000);  

    myservo.write(90);
  myservo1.write(90);
  digitalWrite(2, HIGH);    // Send the signal to the ESC
  delay(100);
  digitalWrite(2, LOW); 
  delay(10000);   

*/
}