#include "DHT.h"
#include <Wire.h>
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0);


#define LEDPIN 5
#define BUZZERPIN 17

#define LIGHTPIN 34
#define TEMPPIN 4

#define SDA_PIN 21
#define SCK_PIN 22

#define DHTTYPE DHT11
DHT dht(TEMPPIN, DHTTYPE);

struct SystemData {
  float temperature;
  float humidity;
  int lightLevel;

  bool ledState = false;

  bool alarmEnabled = true;
  bool alarmActive = false;

  bool tempAlarm = false;
  bool lightAlarm = false;

   bool dhtValid = true;
};
SystemData sys;

const int LIGHT_THRESH = 2000;
const int TEMP_THRESH = 23;

void setup() {

  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(TEMPPIN, INPUT);
  pinMode(LIGHTPIN, INPUT);

  Wire.begin(SDA_PIN, SCK_PIN);

  dht.begin();
  display.begin();
}

void loop() {
  unsigned long now = millis();

  taskStatusLED(now);

  taskReadLight(now);

  taskReadTempHum(now);

  taskHandleAlarm();

  taskHandleOLED(now);

  taskProcessSerial();

}

void taskStatusLED(unsigned long now) {
  static unsigned long lastBlink = 0;
  unsigned long blinkPeriod;

  if (sys.alarmActive) {
    blinkPeriod = 200;
  }
  else {
    blinkPeriod = 1000;
  }

  if (now - lastBlink >= blinkPeriod) {
    sys.ledState = !sys.ledState;

    digitalWrite(LEDPIN, sys.ledState);

    lastBlink = now;

  }
}

void taskReadLight(unsigned long now) {
  static unsigned long lastRead = 0;

  if (now - lastRead >= 200) {
    long total = 0;

    for (int i = 0; i < 10; i++) {
      total += analogRead(LIGHTPIN);
    }
    sys.lightLevel = total / 10;

    lastRead = now;
  }
}

void taskReadTempHum(unsigned long now) {
  static unsigned long lastRead = 0;

  if (now - lastRead >= 1500) {

    sys.humidity = dht.readHumidity();
    sys.temperature = dht.readTemperature();

    if (isnan(sys.humidity) || isnan(sys.temperature)) {
      sys.dhtValid = false;
    }
    else {
      sys.dhtValid = true;
    }

    lastRead = now;
  }
}

void taskHandleAlarm() {

  sys.tempAlarm = false;
  sys.lightAlarm = false;
  sys.alarmActive = false;

  if (!sys.alarmEnabled) {
    noTone(BUZZERPIN);
    return;
  }

  if (sys.dhtValid && sys.temperature > TEMP_THRESH ) {
    sys.tempAlarm = true;
  }

  if (sys.lightLevel > LIGHT_THRESH) {
    sys.lightAlarm = true;
  }

  if (sys.lightAlarm || sys.tempAlarm) {
    sys.alarmActive = true;
    tone(BUZZERPIN, 800);
  }

  else {
    noTone(BUZZERPIN);
  }
}

void taskHandleOLED(unsigned long now) {
  static unsigned long lastUpdate = 0;

  if (now - lastUpdate < 500) {
    return;
  }

  lastUpdate = now;

  display.clearBuffer();

  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(0, 9, "ENVIREONMENT MONITOR");

  display.drawHLine(0, 11, 128);
  display.setCursor(0, 22);

  if (sys.dhtValid) {
    display.print("Temp: ");
    display.print(sys.temperature, 1);
    display.print(" C");

  }
  else {
    display.print("Temp: ERROR");
  }

  display.setCursor(0, 33);
  if (sys.dhtValid) {
    display.print("Hum: ");
    display.print(sys.humidity, 1);
    display.print(" %");
  }
  else {
    display.print("Hum: ERROR");
  }

  display.setCursor(0, 44);
  display.print("Light: ");
  display.print(sys.lightLevel);

  display.setCursor(0, 55);
  if (sys.alarmActive) {
    display.print("ALARM: ACTIVE");
  }
  else if (!sys.alarmEnabled) {
    display.print("ALARM: DISABLED");
  }
  else {
    display.print("ALARM: NORMAL");
  }

  display.setCursor(0, 64);

  if (sys.tempAlarm && sys.lightAlarm) {
    display.print("TEMP + LIGHT HIGH");
  }
  else if (sys.tempAlarm) {
    display.print("TEMP HIGH");
  }
  else if (sys.lightAlarm) {
    display.print("LIGHT HIGH");
  }
  else {
    display.print("SYSTEM OK");
  }

  display.sendBuffer();
}

void taskProcessSerial() {

  if (Serial.available() > 0) {

    String cmd = Serial.readStringUntil('\n');

    cmd.trim();

    if (cmd == "ALARM ON") {

      sys.alarmEnabled = true;

      Serial.println("Alarm enabled");
    }

    else if (cmd == "ALARM OFF") {

      sys.alarmEnabled = false;
      sys.lightAlarm = false;
      sys.tempAlarm = false;
      sys.alarmActive = false;

      noTone(BUZZERPIN);

      Serial.println("Alarm disabled");
    }

    else if (cmd == "STATUS") {

  Serial.println();
  Serial.println("----- SYSTEM STATUS -----");

  Serial.print("Temperature: ");
  if (sys.dhtValid) {
    Serial.print(sys.temperature, 1);
    Serial.println(" C");
  }
  else {
    Serial.println("ERROR");
  }

  Serial.print("Humidity:    ");
  if (sys.dhtValid) {
    Serial.print(sys.humidity, 1);
    Serial.println(" %");
  }
  else {
    Serial.println("ERROR");
  }

  Serial.print("Light:       ");
  Serial.println(sys.lightLevel);

  Serial.print("Alarm:       ");
  if (!sys.alarmEnabled) {
    Serial.println("DISABLED");
  }
  else if (sys.alarmActive) {
    Serial.println("ACTIVE");
  }
  else {
    Serial.println("NORMAL");
  }

  Serial.print("Temp Alarm:  ");
  Serial.println(sys.tempAlarm ? "YES" : "NO");

  Serial.print("Light Alarm: ");
  Serial.println(sys.lightAlarm ? "YES" : "NO");

  Serial.println("-------------------------");
  Serial.println();
}

    else if (cmd == "LED ON") {

      digitalWrite(LEDPIN, HIGH);

      Serial.println("LED ON");
    }

    else if (cmd == "LED OFF") {

      digitalWrite(LEDPIN, LOW);

      Serial.println("LED OFF");
    }

    else {

      Serial.println("Unknown command");
    }
  }
}