#define BLYNK_TEMPLATE_ID "TMPL38Z--_573"
#define BLYNK_TEMPLATE_NAME "Smart Earthing Monitoring System"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

// ================= WIFI & BLYNK =================
char auth[] = "2Ip70ugs9wabVCnyFGyY5bVyYJ0BQ4jO";
const char* ssid = "ss";
const char* password = "12345678";

// ================= PIN DEFINITIONS =================
#define VOLTAGE_PIN        34   // ZMPT101B
#define LOAD_CURRENT_PIN   35   // ACS712 - Load current
#define LEAK_CURRENT_PIN   33   // ACS712 - Leakage current
#define MOISTURE_PIN       32

#define LED_FAULT     5
#define LED_OK        18
#define LED_MOISTURE  17
#define BUZZER        23

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= CONSTANTS =================
#define ADC_MAX 4095.0
#define VREF 3.3
#define ACS_SENSITIVITY 0.100     // ACS712-20A (100 mV/A)
#define LOAD_CAL_FACTOR   0.111
#define LEAK_CAL_FACTOR   0.098

#define LEAKAGE_THRESHOLD 0.30     // 20 mA

#define MOISTURE_DRY 3300
#define MOISTURE_WET 1200

// ================= VARIABLES =================
float Vrms = 0;
float LoadIrms = 0;
float LeakIrms = 0;

float loadOffset = 0;
float leakOffset = 0;

int moistureValue = 0;
unsigned long lastRead = 0;
unsigned long leakStartTime = 0;
unsigned long lastLCD = 0;


// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  analogSetPinAttenuation(VOLTAGE_PIN, ADC_11db);
  analogSetPinAttenuation(LOAD_CURRENT_PIN, ADC_11db);
  analogSetPinAttenuation(LEAK_CURRENT_PIN, ADC_11db);
  analogSetPinAttenuation(MOISTURE_PIN, ADC_11db);

  pinMode(LED_FAULT, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_MOISTURE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Smart Earthing");
  lcd.setCursor(0,1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  calibrateCurrentSensors();

  WiFi.begin(ssid, password);
  Blynk.config(auth);
  Blynk.connect();
}

// ================= LOOP =================
void loop() {
  Blynk.run();


  if (millis() - lastRead >= 1000) {
    lastRead = millis();

    readVoltage();
    readLoadCurrent();
    readLeakageCurrent();
    readMoisture();

if (LoadIrms < 0.05) {LeakIrms = 0.0;}



    if (millis() - lastLCD >= 1000) {
    lastLCD = millis();
    updateLCD();
}

    sendToBlynk();
    handleAlerts();
  }
}

// ================= VOLTAGE RMS =================
void readVoltage() {
  const int samples = 1000;
  float sumSq = 0;

  for (int i = 0; i < samples; i++) {
    float adc = analogRead(VOLTAGE_PIN);
    float voltage = (adc * VREF) / ADC_MAX;
    float ac = voltage - (VREF / 2.0);
    sumSq += ac * ac;
    delayMicroseconds(200);
  }

  float sensorVrms = sqrt(sumSq / samples);
  float VOLT_CAL = 645.71;   // Adjust with multimeter
  Vrms = sensorVrms * VOLT_CAL;
}

// ================= CALIBRATION =================
void calibrateCurrentSensors() {
  const int samples = 1500;
  long sumLoad = 0;
  long sumLeak = 0;

  Serial.println("Calibrating ACS712...");
  Serial.println("REMOVE ALL LOADS!");
  delay(4000);

  for (int i = 0; i < samples; i++) {
    sumLoad += analogRead(LOAD_CURRENT_PIN);
    sumLeak += analogRead(LEAK_CURRENT_PIN);
    delayMicroseconds(200);
  }

  loadOffset = (sumLoad / (float)samples) * VREF / ADC_MAX;
  leakOffset = (sumLeak / (float)samples) * VREF / ADC_MAX;

  Serial.print("Load Offset: ");
  Serial.println(loadOffset, 4);
  Serial.print("Leak Offset: ");
  Serial.println(leakOffset, 4);
}

// ================= LOAD CURRENT =================
void readLoadCurrent() {
  const int samples = 1000;
  float sumSq = 0;

  for (int i = 0; i < samples; i++) {
    float adc = analogRead(LOAD_CURRENT_PIN);
    float voltage = (adc * VREF) / ADC_MAX;
    float ac = voltage - loadOffset;
    float current = (ac / ACS_SENSITIVITY) * LOAD_CAL_FACTOR;
    sumSq += current * current;
    delayMicroseconds(200);
  }

  LoadIrms = sqrt(sumSq / samples);
  if (LoadIrms < 0.01) LoadIrms = 0.0;
}


void readLeakageCurrent() {

  // If no load is flowing, leakage must be zero
  if (LoadIrms < 0.05) {
    LeakIrms = 0.0;
    return;
  }

  const int samples = 1000;
  float sumSq = 0;

  for (int i = 0; i < samples; i++) {
    float adc = analogRead(LEAK_CURRENT_PIN);
    float voltage = (adc * VREF) / ADC_MAX;
    float ac = voltage - leakOffset;
    float current = (ac / ACS_SENSITIVITY) * LEAK_CAL_FACTOR;
    sumSq += current * current;
    delayMicroseconds(200);
  }

  LeakIrms = sqrt(sumSq / samples);

  // HARD clamp
  if (LeakIrms < 0.1) LeakIrms = 0.0;
}



// ================= MOISTURE =================
void readMoisture() {
  int raw = analogRead(MOISTURE_PIN);

  moistureValue = map(raw, MOISTURE_DRY, MOISTURE_WET, 100, 0);
  moistureValue = constrain(moistureValue, 0, 100);
}

// ================= LCD =================
void updateLCD() {
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("L:");
  lcd.print(LoadIrms,2);
  lcd.print("A ");

  lcd.print("E:");
  lcd.print(LeakIrms,3);

  lcd.setCursor(0,1);
  lcd.print("Moist:");
  lcd.print(moistureValue);
  lcd.print("%");
}


// ================= BLYNK =================
void sendToBlynk() {
  Blynk.virtualWrite(V0, Vrms);
  Blynk.virtualWrite(V1, LoadIrms);
  Blynk.virtualWrite(V2, LeakIrms);
  Blynk.virtualWrite(V3, moistureValue);

  String alert = "OK";

  if (LeakIrms > LEAKAGE_THRESHOLD && LoadIrms > 0.1) {
    alert = "EARTH FAULT!";
  }else if (moistureValue < 30) {
    alert = "LOW MOISTURE";
  }

  Blynk.virtualWrite(V4, alert);
}

// ================= ALERTS =================
void handleAlerts() {
  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_FAULT, LOW);
  digitalWrite(LED_MOISTURE, LOW);
  digitalWrite(BUZZER, LOW);

  // ---------- EARTH FAULT CONFIRMATION ----------
  if (LeakIrms > LEAKAGE_THRESHOLD && LoadIrms > 0.1) {

    if (leakStartTime == 0) {
      leakStartTime = millis();   // start timing
    }

    if (millis() - leakStartTime >= 3000) {
      // EARTH FAULT CONFIRMED
      digitalWrite(LED_FAULT, HIGH);
      digitalWrite(BUZZER, HIGH);
    }

  } else {
    leakStartTime = 0;   // reset timer
    digitalWrite(LED_OK, HIGH);
  }

  // ---------- MOISTURE ALERT ----------
  if (moistureValue < 30) {
    digitalWrite(LED_MOISTURE, HIGH);
    digitalWrite(BUZZER, HIGH);
  }
}
