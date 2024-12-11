
// This project develops a smart alarm clock using multiple Arduinos to enhance the traditional wake-up experience. 
// It features real-time indoor temperature display, humidity reading, and a light sensor to mimic natural sunrise, 
// all aimed at improving user convenience. The Arduinos will communicate via serial connection for synchronized functionality,
// while using buttons to set the time and alarm. It will also make use of a buzzer to wake the user up to 2 LCD 
// displays that have all the information they need to start their day! This clock is designed for individuals with 
// rigorous schedules who value added information to start their day.

#include <DHT.h>

#define DHTPIN 10             // DHT11 sensor pin
#define DHTTYPE DHT11         // DHT11 type
#define LIGHT_SENSOR_PIN A0   // Photoresistor pin

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LIGHT_SENSOR_PIN, INPUT);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightValue = analogRead(LIGHT_SENSOR_PIN);

  // Send the data over serial in a CSV format
  Serial.print(temperature, 1);
  Serial.print(",");
  Serial.print(humidity, 1);
  Serial.print(",");
  Serial.println(lightValue);

  delay(5000); // Wait 5 seconds before sending new data
}
