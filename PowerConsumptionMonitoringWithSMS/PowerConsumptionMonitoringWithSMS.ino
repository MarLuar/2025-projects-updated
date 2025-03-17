// Define Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL6IS9XnOZf"
#define BLYNK_TEMPLATE_NAME "Energy Monitoring System"
#define BLYNK_AUTH_TOKEN "S-1vdRwHuxtae2cMRKg2RqZDftI_I325"

// Include required libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <EmonLib.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>

// WiFi credentials
char ssid[] = "koogs";
char pass[] = "unsaypass";
int readingCount;

// LCD Display setup
#define LCD_I2C_ADDRESS 0x27
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

// Energy monitoring setup
EnergyMonitor emon;
const int sensorPin = 34;
#define BUZZER 27  // GPIO17
const double voltage = 220.0;
double energyWh = 0.0;
double ratePerKWh = 0.0;
unsigned long lastTime = 0;
bool smsSent = false;
unsigned long startupTime = 0;
unsigned long energyThresholdTime = 0;
bool timerStarted = false;
const unsigned long shutdownDelay = 15 * 60 * 1000; // 15 minutes in milliseconds

// SIM800L Serial Setup
HardwareSerial sim800l(1);
#define SIM800_TX 17
#define SIM800_RX 16

// Servo Motor Setup
Servo myServo;
const int servoPin = 33;

// Variables to store previous values for change detection
double prevIrms = 0.0;
double prevEnergyKWh = 0.0;
double prevEstimatedBill = 0.0;

// kWh limit variable
double kWhLimit = 1.0; // Default kWh limit

// Function to send SMS using SIM800L module
void sendSMS(String number, String message) {
  Serial.println("Sending SMS...");
  sim800l.println("AT+CMGF=1"); // Set to text mode
  delay(1000);
  sim800l.print("AT+CMGS=\"");
  sim800l.print(number);
  sim800l.println("\"");
  delay(1000);
  sim800l.print(message);
  delay(1000);
  sim800l.write(26); // End SMS
  Serial.println("SMS Sent!");
}

// Blynk virtual pin V4 handler to update rate per kWh
BLYNK_WRITE(V4) { 
  String rateString = param.asStr();
  ratePerKWh = rateString.toDouble();
}

// Blynk virtual pin V5 handler to update kWh limit
BLYNK_WRITE(V5) {
  String kwhString = param.asStr();
  kWhLimit = kwhString.toDouble(); // Update kWh limit from Blynk app
}

// Blynk virtual pin V6 handler to reset energy values
BLYNK_WRITE(V6) {
  int buttonState = param.asInt(); // Read the button state (0 or 1)
  if (buttonState == 1) { // If button is pressed (1)
    energyWh = 0.0; // Reset energy consumption
    energyThresholdTime = 0; // Reset timer
    timerStarted = false; // Stop the timer
    smsSent = false; // Reset SMS sent flag

    // Reset servo and buzzer
    myServo.write(0); // Turn servo back to default position
    digitalWrite(BUZZER, LOW); // Turn off buzzer

    // Reset Blynk displays
    Blynk.virtualWrite(V2, String(0.0, 4)); // Reset kWh display on Blynk
    Blynk.virtualWrite(V3, String(0.0, 2)); // Reset estimated bill on Blynk

    Serial.println("Energy values reset! System returned to normal state.");
    updateLCD(); // Update LCD display
  }
}

// Function to read energy data and send updates to Blynk
void sendDataToBlynk() {
  // Ignore initial unstable readings
  if (millis() - startupTime < 3000) {
    Serial.println("Ignoring startup readings...");
    return;
  }

  double Irms = emon.calcIrms(1100);
  
  if (Irms < 0.12) Irms = 0; // Ignore noise

  unsigned long currentTime = millis();
  unsigned long timeElapsed = currentTime - lastTime;
  lastTime = currentTime;

  double timeElapsedHours = timeElapsed / 3600000.0;
  energyWh += (voltage * Irms) * timeElapsedHours;
  double energyKWh = energyWh / 1000.0;
  double estimatedBill = energyKWh * ratePerKWh;

  // Update Blynk only if significant change is detected
  if ((abs(Irms - prevIrms) > 0.05) ||
      (abs(energyKWh - prevEnergyKWh) > 0.002) ||
      (abs(estimatedBill - prevEstimatedBill) > 0.02)) {

    Blynk.virtualWrite(V0, Irms);
    Blynk.virtualWrite(V2, String(energyKWh, 4));
    Blynk.virtualWrite(V3, String(estimatedBill, 2));

    prevIrms = Irms;
    prevEnergyKWh = energyKWh;
    prevEstimatedBill = estimatedBill;
  }

  // Check if energy consumption has reached the kWh limit
  if (energyKWh >= kWhLimit) {
    if (!smsSent) {
      sendSMS("+639923380007", "Alert: Energy consumption reached " + String(kWhLimit) + " kWh! Shutting down electricity in 15 mins.");
      delay(2000);
      digitalWrite(BUZZER, HIGH);
      sendSMS("+639321159210", "Alert: Energy consumption reached " + String(kWhLimit) + " kWh! Shutting down electricity in 15 mins.");
      delay(2000);
      sendSMS("+639082884705", "Alert: Energy consumption reached " + String(kWhLimit) + " kWh! Shutting down electricity in 15 mins.");
      delay(2000);
      sendSMS("+639942751941", "Alert: Energy consumption reached " + String(kWhLimit) + " kWh! Shutting down electricity in 15 mins.");
      smsSent = true;
    }
    if (!timerStarted) {
      energyThresholdTime = millis();
      timerStarted = true;
    }
  } else {
    timerStarted = false;
    smsSent = false;
  }

  // Check if shutdown delay has passed and activate servo
  if (timerStarted && (millis() - energyThresholdTime >= shutdownDelay)) {
    myServo.write(180); // Move servo to turn off power
    digitalWrite(BUZZER, LOW);
  } else {
    myServo.write(0); // Keep power on
  }
}

// Function to update LCD display
void updateLCD() {
  double Irms = emon.calcIrms(1100);
  if (Irms < 0.12) Irms = 0;

  double power = voltage * Irms;
  double energyKWh = energyWh / 1000.0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Current: ");
  lcd.print(Irms);
  lcd.print(" A");
  
  lcd.setCursor(0, 1);
  lcd.print("kWh: ");
  lcd.print(energyKWh, 4);
} 

// Setup function to initialize all components
void setup() {
  Serial.begin(115200);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Energy Monitor");
  delay(2000);
  pinMode(BUZZER, OUTPUT);
  emon.current(sensorPin, 1.2);
  
  sim800l.begin(9600, SERIAL_8N1, SIM800_RX, SIM800_TX);
  delay(1000);
  Serial.println("Initializing SIM800L...");
  
  myServo.attach(servoPin);
  myServo.write(0);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  startupTime = millis();
}

// Main loop function
void loop() {
  Blynk.run(); // Keep Blynk running
  sendDataToBlynk(); // Send sensor data
  updateLCD(); // Update display
  delay(500); // Small delay to avoid excessive processing

  static unsigned long lastLCDInit = 0;
  if (millis() - lastLCDInit > 10000) { // Reinitialize every 60 seconds
    lcd.init();
    lcd.backlight();
    lastLCDInit = millis();
  }
}