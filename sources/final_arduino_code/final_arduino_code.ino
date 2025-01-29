#include <SoftwareSerial.h>
#include <TheThingsNetwork.h>

#define rxPin 2  
#define txPin 3  

#define loraRxPin 10
#define loraTxPin 11

// Initialize SoftwareSerial for Bluetooth and LoRa communication
SoftwareSerial HC06(rxPin, txPin);                // Bluetooth serial
SoftwareSerial loraSerial(loraRxPin, loraTxPin);  // LoRa serial

TheThingsNetwork ttn(loraSerial, Serial, TTN_FP_EU868);

const int gasSensorPin = A1;                      // Analog input pin for gas sensor

// LoRaWAN credentials
const char *appEui = "0004A30B002159CA";
const char *appKey = "8d1630b9b6db6270fa1ae3268e610842";


void setup() {

  // Initialize Serial communication
  HC06.begin(9600);         // Bluetooth baud rate
  Serial.begin(9600);       // Serial Monitor baud rate
  loraSerial.begin(57600);  // LoRa serial baud rate

  // Join the LoRaWAN network
  ttn.join(appEui, appKey);
  
  // Set the gas sensor pin as input
  pinMode(gasSensorPin, INPUT);

  // Print debug info
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {

  // Read gas sensor value
  int gasValue = analogRead(gasSensorPin);

  // Send gas value via Bluetooth (HC-05)
  if (HC06.available() > 0) {
    HC06.print(gasValue);      // Send gas sensor voltage via Bluetooth
    Serial.print("Bluetooth Sending Data: ");
    Serial.println(gasValue);  // Print to Serial Monitor
  }
  
  // Send gas value via LoRaWAN
  byte payload[2];
  payload[0] = (gasValue >> 8) & 0xFF;      // High byte
  payload[1] = gasValue & 0xFF;             // Low byte
  ttn.sendBytes(payload, sizeof(payload));  // Send LoRaWAN payload

  Serial.print("LoRaWAN Sending Data: ");
  Serial.println(gasValue);  // Print to Serial Monitor

  delay(3000);  // Wait 3 seconds before sending the next reading
}
