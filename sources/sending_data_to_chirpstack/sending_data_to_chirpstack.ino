#include <SoftwareSerial.h>

// Define RX and TX pins for Bluetooth communication (HC-05)
#define rxPin 2
#define txPin 3
SoftwareSerial mySerial(rxPin, txPin);

// Define gas sensor pin
const int gasSensorPin = A0;  // Analog input pin for gas sensor
int gasValue = 0;              // Variable to store the gas sensor reading

// Variables to track time (for sending data every 3 seconds)
unsigned long previousMillis = 0;
const long interval = 3000;    // Interval to read the sensor (3 seconds)

void setup() {
  // Initialize communication
  mySerial.begin(38400);  // HC-05 Bluetooth module baud rate
  Serial.begin(38400);    // Serial Monitor baud rate for debugging

  // Pin modes for gas sensor pin
  pinMode(gasSensorPin, INPUT);  // Set the gas sensor pin as input

  // Initialize Bluetooth (HC-05) communication
  Serial.println("Bluetooth communication started. Waiting for data...");
}

void loop() {
  unsigned long currentMillis = millis();  // Get current time in milliseconds

  // Check if it's time to read the gas sensor (every 3 seconds)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Save the last time a reading was taken

    // Read the gas sensor value (0-1023)
    gasValue = analogRead(gasSensorPin);

    // Send only the gas value to Bluetooth
    mySerial.println(gasValue);

    // Print the data to Serial Monitor for debugging
    Serial.println(gasValue);
  }

  // If there's data received from Bluetooth (incoming from the phone)
  while (mySerial.available()) {
    char incomingChar = mySerial.read();  // Read incoming byte
    Serial.print(incomingChar);           // Print incoming byte to Serial Monitor
  }
}
