#include "ESP8266WiFi.h"
#include "FirebaseArduino.h"

#define FIREBASE_HOST        "golfcart-cdf3a-default-rtdb.firebaseio.com"
//#define FIREBASE_HOST        "iotbuggy-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH        "Gl2sHNOsDiQr1hiPMgm11GeV7SjEdkWOKOiGcJFq"
//#define FIREBASE_AUTH        "hLKCnIMXYEkI8OWi0XDcBAXE9jSLmkx1GriZSaOF"

#define WIFI_SSID            "sh"  // Your WiFi SSID
#define WIFI_PASSWORD        "srihari2324"  // Your WiFi Password

// Pin Definitions
const int trigPin = D7;       // Ultrasonic trigger pin
//const int echoPin = D8;       // Ultrasonic echo pin
const int pwmPinSpeed = D8;   // PWM pin for motor speed control
const int Reverse = D2;       // Reverse control pin
const int LeftIN1 = D3;       // Left motor control pin
const int RightIN2 = D4;      // Right motor control pin
const int BrakeIn = D5;       // Brake engage pin
const int BrakeOut = D6;      // Brake release pin
const int WifiLed = D0;       // Wifi connection status LED pin (define the pin here)


// Firebase Control Variables
String speed, reverseEnable, brake, left, right;
int VALUE=0;

// Ultrasonic Sensor Variables
//long duration;
//int distance;
//const int thresholdDistance = 30;  // Obstacle detection threshold in cm

void setup() {
  // Initialize Serial Monitor and Pins
  Serial.begin(9600);
  //pinMode(trigPin, OUTPUT);
  //pinMode(echoPin, INPUT);
  pinMode(pwmPinSpeed, OUTPUT);
  pinMode(Reverse, OUTPUT);
  pinMode(LeftIN1, OUTPUT);
  pinMode(RightIN2, OUTPUT);
  pinMode(BrakeIn, OUTPUT);
  pinMode(BrakeOut, OUTPUT);
  pinMode(WifiLed, OUTPUT);  // Set the WifiLed pin as output
  
  // Initialize WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // Set default values for Firebase variables
  Firebase.setString("speed", "0");
  Firebase.setString("reverseEnable", "false");
  Firebase.setString("brake", "false");
  Firebase.setString("left", "false");
  Firebase.setString("right", "false");
}

void loop() {
  // Check WiFi Connection
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(WifiLed, HIGH);  // Turn on Wifi LED to indicate no connection
    delay(1000);
    digitalWrite(WifiLed, LOW);   // Turn off Wifi LED
    delay(1000);
  }

  // Get control values from Firebase
  speed = Firebase.getString("speed");
  reverseEnable = Firebase.getString("reverseEnable");
  brake = Firebase.getString("brake");
  left = Firebase.getString("left");
  right = Firebase.getString("right");

  // Control Speed
  controlSpeed();

  // Control Reverse
  controlReverse();

  // Control Brake
  controlBrake();

  // Control Left and Right
  controlLeft();
  controlRight();

  // Ultrasonic sensor logic for obstacle detection
 // ultrasonicSensor();
}

// void ultrasonicSensor() {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(20);
//   digitalWrite(trigPin, LOW);

//   duration = pulseIn(echoPin, HIGH);
//   distance = duration * 0.034 / 2;

//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.println(" cm");

//   // Emergency Brake if Obstacle Detected
//   if (distance < thresholdDistance) {
//     Serial.println("Obstacle Detected! Stopping...");
//     analogWrite(pwmPinSpeed, 0);
//     digitalWrite(BrakeIn, HIGH);
//     delay(1500);
//     digitalWrite(BrakeIn, LOW);
//   }
// }

// Function to control speed via Firebase
void controlSpeed() {
  int speedValue = speed.toInt();
  // switch (speedValue) {
  //   case 0: analogWrite(pwmPinSpeed, 0); break;
  //   case 1: analogWrite(pwmPinSpeed, 255); break;
  //   case 2: analogWrite(pwmPinSpeed, 512); break;
  //   case 3: analogWrite(pwmPinSpeed, 1023); break;
  // }
  if (speedValue==2) {
   VALUE = 512;
  }
  else if (speedValue == 1)
  {VALUE = 255;}
  else if(speedValue == 3){VALUE = 1024;}
  else{VALUE = 0;}
  Serial.print("Speed: ");
  analogWrite(pwmPinSpeed,VALUE);

  Serial.println(speedValue);
  Serial.print("VALUE");
  Serial.println(VALUE);
}

// Function to control reverse via Firebase
void controlReverse() {
  if (reverseEnable == "true") {
    digitalWrite(Reverse, HIGH);
    Serial.println("Reverse Enabled");
  } else {
    digitalWrite(Reverse, LOW);
    Serial.println("Reverse Disabled");
  }
}

// Function to apply brake via Firebase
void controlBrake() {
  if (brake == "true") {
    Serial.println("Brake Applied");
    analogWrite(pwmPinSpeed, 0);
    digitalWrite(BrakeIn, HIGH);
    digitalWrite(BrakeOut, LOW);
    delay(1500);
    digitalWrite(BrakeIn, LOW);
  } else {
    Serial.println("Brake Released");
    digitalWrite(BrakeOut, HIGH);
    delay(1500);
    digitalWrite(BrakeOut, LOW);
  }
}

// Function to control left turn via Firebase
void controlLeft() {
  if (left == "true") {
    digitalWrite(LeftIN1, HIGH);
    digitalWrite(RightIN2, LOW);
    Serial.println("Turning Left");
  } else {
    digitalWrite(LeftIN1, LOW);
  }
}

// Function to control right turn via Firebase
void controlRight() {
  if (right == "true") {
    digitalWrite(RightIN2, HIGH);
    digitalWrite(LeftIN1, LOW);
    Serial.println("Turning Right");
  } else {
    digitalWrite(RightIN2, LOW);
  }
}
