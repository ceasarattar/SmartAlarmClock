// Group 63 - Smart Alarm Clock

// Enkh-Amgalan Altanshagai - ealtan3@uic.edu
// Ceasar Attar - catta2@uic.edu
// Isabella Linarez - ilina2@uic.edu

// Our project develops a smart alarm clock using multiple Arduinos to enhance the traditional wake-up experience. 
// It features real-time indoor temperature display, humidity reading, and a light sensor to mimic natural sunrise, 
// all aimed at improving user convenience. The Arduinos will communicate via serial connection for synchronized functionality,
// while using buttons to set the time and alarm. It will also make use of a buzzer to wake the user up to 2 LCD 
// displays that have all the information they need to start their day! This clock is designed for individuals with 
// rigorous schedules who value added information to start their day.

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long lastSwitchTime = 0;
bool displayTempHumidity = true; // Start with displaying temperature and humidity

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n'); // Read the incoming data
    float temperature = getValue(data, ',', 0).toFloat();
    float humidity = getValue(data, ',', 1).toFloat();
    int lightValue = getValue(data, ',', 2).toInt();
    String lightStatus = getLightStatus(lightValue);

    // Check if it's time to switch display
    if (millis() - lastSwitchTime >= 5000) { // 5 seconds have passed
      displayTempHumidity = !displayTempHumidity; // Toggle the display state
      lastSwitchTime = millis(); // Reset the timer
      lcd.clear(); // Clear the LCD screen
    }

    if (displayTempHumidity) {
      // Display temperature and humidity
      lcd.setCursor(0, 0);
      lcd.print("Temperature: ");
      int tempFahrenheit = static_cast<int>((temperature * 1.8) + 32); // Convert to Fahrenheit and cast to int
      lcd.print(tempFahrenheit);
      lcd.print((char)223); // Degree symbol
      lcd.print("F"); // Add Fahrenheit symbol

      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(static_cast<int>(humidity)); // Cast to int to remove decimal places
      lcd.print("%");
    } else {
      // Display light status
      lcd.setCursor(0, 0);
      lcd.print("Room Light:");
      lcd.setCursor(0, 1);
      lcd.print(lightStatus);
    }
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String getLightStatus(int lightValue) {
  if (lightValue < 200) return "Night";
  else if (lightValue < 400) return "Cloudy";
  else if (lightValue < 600) return "Sunrise";
  else if (lightValue < 800) return "Sunny";
  else if (lightValue > 800) return "Very Sunny";
}

