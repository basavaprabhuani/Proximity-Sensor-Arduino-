#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int TRIG = 8;
int ECHO = 9;
int LED = 7; // LED and Buzzer are in the same circuit, meaning, both work simulataneously

int ledState = LOW;
bool show_text = true;

LiquidCrystal_I2C display(0x27, 16, 2);

// Separate timers
unsigned long previousMillisLED = 0;
unsigned long previousMillisLCD = 0;

void setup() {
  display.init();
  display.backlight();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Ultrasonic trigger
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  float time_twice = pulseIn(ECHO, HIGH, 100000);
  float distance = time_twice * 0.0343 / 2;

  // LED BLINKING
  long intervalLED = 50 + (distance * 10);

  if (distance <= 30 && distance > 5) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillisLED >= intervalLED) {
      previousMillisLED = currentMillis;

      ledState = !ledState;
      digitalWrite(LED, ledState);
    }
  } 
  else if (distance <= 5) {
    digitalWrite(LED, HIGH);
  } 
  else {
    digitalWrite(LED, LOW);
  }


  // LCD DISPLAY

  // Row 0 → Distance
  display.setCursor(0, 0);
  display.print("Dist.: ");
  display.print(distance);
  display.print("   "); // clear leftover digits

  // Row 1 → Status
  if (distance < 30 && distance > 5) {
    unsigned long currentMillis = millis();
    display.setCursor(0, 1);

    if (currentMillis - previousMillisLCD >= 500) {
      previousMillisLCD = currentMillis;
      show_text = !show_text;

      if (show_text) {
        display.print("     CLOSE      ");   // text
      } else {
        display.print("           "); // blank
      }
    }
  }
  else if (distance <= 5) {
    display.setCursor(2, 1);
    display.print("TOO CLOSE!!   ");
  }
  else {
    display.setCursor(0, 1);
    display.print("                "); // clear row
  }

  // LCD DISPLAY - END
  delay(50); // small delay (safe)
}