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

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Setup LCD

// Define button pins
const int buttonHourPin = 10;    // Increment Hours (Button 1)
const int buttonMinutePin = 9;   // Increment Minutes (Button 2)
const int buttonSetAlarmPin = 8; // Set/Reset Alarm (Button 3)
const int buttonSetTimePin = 13; // Set/Reset Time (Button 5)
const int buttonToggleAlarmPin = 6; // Toggle Alarm On/Off (Button 4)
const int buzzerPin = 7; // Buzzer pin

// Time tracking variables
unsigned long previousMillis = 0;
const long interval = 60000; // Update interval (1 minute)
unsigned int minutes = 0, hours = 0;
unsigned int alarmHours = 0, alarmMinutes = 0;
bool isAlarmSet = false, isAlarmRinging = false, isSettingAlarm = false, isSettingTime = false;

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonHourPin, INPUT_PULLUP);
  pinMode(buttonMinutePin, INPUT_PULLUP);
  pinMode(buttonSetAlarmPin, INPUT_PULLUP);
  pinMode(buttonToggleAlarmPin, INPUT_PULLUP);
  pinMode(buttonSetTimePin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    incrementTime();
    checkAlarm();
  }

  if (isAlarmRinging && digitalRead(buttonToggleAlarmPin) == LOW) {
    silenceAndResetAlarm();
  } else {
    handleButtonInputs();
  }

  updateDisplay();
}

void handleButtonInputs() {
    // Toggle alarm on and off
    if (digitalRead(buttonToggleAlarmPin) == LOW) {
        isAlarmSet = !isAlarmSet;
        delay(200);
    }

    // Enter or exit alarm setting mode
    if (digitalRead(buttonSetAlarmPin) == LOW) {
        if (!isSettingAlarm && !isSettingTime) {
            isSettingAlarm = true;
        } else if (isSettingAlarm) {
            isSettingAlarm = false;
        }
        delay(200);
    }

    // Enter or exit time setting mode
    if (digitalRead(buttonSetTimePin) == LOW) {
        if (!isSettingTime && !isSettingAlarm) {
            isSettingTime = true;
        } else if (isSettingTime) {
            isSettingTime = false;
        }
        delay(200);
    }

    // Adjust alarm hours and minutes
    if (isSettingAlarm) {
        if (digitalRead(buttonHourPin) == LOW) {
            alarmHours = (alarmHours + 1) % 24;
            delay(200);
        }
        if (digitalRead(buttonMinutePin) == LOW) {
            alarmMinutes = (alarmMinutes + 1) % 60;
            delay(200);
        }
    }

    // Adjust current time hours and minutes
    if (isSettingTime) {
        if (digitalRead(buttonHourPin) == LOW) {
            hours = (hours + 1) % 24;
            delay(200);
        }
        if (digitalRead(buttonMinutePin) == LOW) {
            minutes = (minutes + 1) % 60;
            delay(200);
        }
    }
}

void updateDisplay() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(isSettingTime ? "Set Time: " : "Time: ");
    printTime(hours, minutes);

    lcd.setCursor(0, 1);
    if (isSettingAlarm) {
        lcd.print("Set Alarm: ");
    } else if (isAlarmSet) {
        lcd.print("Alarm: ");
    } else {
        lcd.print("Alarm: OFF        ");
    }
    printTime(alarmHours, alarmMinutes);
}

void printTime(unsigned int h, unsigned int m) {
  char timeBuffer[11];
  sprintf(timeBuffer, "%02d:%02d", h, m);
  lcd.print(timeBuffer);
}

void silenceAndResetAlarm() {
  isAlarmRinging = false;
  analogWrite(buzzerPin, 0); // Turn off the buzzer
  delay(200);
}

void incrementTime() {
  minutes++;
  if (minutes >= 60) {
    minutes = 0;
    hours = (hours + 1) % 24;
  }
}

void checkAlarm() {
  if (isAlarmSet && hours == alarmHours && minutes == alarmMinutes && !isAlarmRinging) {
    isAlarmRinging = true;
    analogWrite(buzzerPin, 128);
  }
}
