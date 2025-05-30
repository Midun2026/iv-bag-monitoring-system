#include <Wire.h>
#include "HX711.h"

// HX711 circuit wiring
#define DT D2   // HX711 DT pin connected to NodeMCU D2
#define SCK D1  // HX711 SCK pin connected to NodeMCU D1

// Buzzer pin
#define BUZZER_PIN D5

HX711 scale;

// Thresholds
const float NO_BAG_THRESHOLD = 0.0;     // Less than 10g = No bag
const float LOW_BAG_THRESHOLD = 25.0;   // Less than 50g = Refill needed

// Calibration factor
const float CALIBRATION_FACTOR = 225.34; // *** Your value ***

void setup() {
  Serial.begin(9600);
  delay(100);
  Wire.begin(D3, D4); // SDA = D3, SCL = D4

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Buzzer OFF initially

  Serial.println("System Initializing...");
  scale.begin(DT, SCK);
  delay(100);

  scale.set_scale(CALIBRATION_FACTOR); // Set your calibration
  scale.tare();                        // Reset scale to 0

  Serial.println("System Ready!");
}

void loop() {
  float weight = scale.get_units(5); // Average of 5 readings

  Serial.println("----------------------------");
  Serial.print("Measured Weight: ");
  Serial.print(weight, 1);
  Serial.println(" g");

  if (weight <= NO_BAG_THRESHOLD) {
    Serial.println("Status: No IV Bag Found!");
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer ON
  }
  else if (weight <= LOW_BAG_THRESHOLD) {
    Serial.println("Status: Refill IV Bag!");
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer ON
  }
  else {
    Serial.println("Status: IV Bag OK");
    digitalWrite(BUZZER_PIN, LOW); // Buzzer OFF
  }

  delay(1000); // 1-second interval
}
