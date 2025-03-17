#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 5  // Buzzer connected to GPIO 5

// Initialize the LCD (Address 0x27, 20 columns x 4 rows)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Struct for receiving data
typedef struct {
    int irValue;
    int beatAvg;
    int fakeSpO2;
    float objectTemp;
    int ecgValue;
} SensorData;

// Function to check if values are abnormal
bool isAbnormal(SensorData data) {
    if (data.beatAvg == 0 && data.fakeSpO2 == 0) {
        Serial.println("No finger detected - Buzzer OFF");
        return false;  // No finger detected, do not trigger buzzer
    }

    bool bpmAbnormal = (data.beatAvg < 60 || data.beatAvg > 100);
    bool spo2Abnormal = (data.fakeSpO2 < 95);
    bool tempAbnormal = (data.objectTemp < 34.5 || data.objectTemp > 38.5);

    Serial.print("BPM Abnormal: "); Serial.println(bpmAbnormal);
    Serial.print("SpO2 Abnormal: "); Serial.println(spo2Abnormal);
    Serial.print("Temp Abnormal: "); Serial.print(tempAbnormal);
    Serial.print(" (Temp: "); Serial.print(data.objectTemp); Serial.println(" °C)");

    return bpmAbnormal || spo2Abnormal || tempAbnormal;
}


// Callback function to handle received data
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    SensorData receivedData;
    memcpy(&receivedData, data, sizeof(receivedData));

    // Print data to Serial Monitor
    Serial.println("Received Data:");
    Serial.print("IR Value: "); Serial.println(receivedData.irValue);
    Serial.print("BPM: "); Serial.println(receivedData.beatAvg);
    Serial.print("SpO2: "); Serial.print(receivedData.fakeSpO2); Serial.println("%");
    Serial.print("Temperature: "); Serial.print(receivedData.objectTemp); Serial.println(" °C");
    Serial.print("ECG Value: "); Serial.println(receivedData.ecgValue);
    Serial.println("-----------------------------");

    // Display data on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    if (receivedData.beatAvg > 0) {
        lcd.print("BPM: "); lcd.print(receivedData.beatAvg);
    } else {
        lcd.print("No Finger Detected");
    }

    lcd.setCursor(0, 1);
    lcd.print("SpO2: "); lcd.print(receivedData.fakeSpO2); lcd.print("%");

    lcd.setCursor(0, 2);
    lcd.print("Temp: "); lcd.print(receivedData.objectTemp); lcd.print(" C");

    lcd.setCursor(0, 3);
    lcd.print("ECG: "); lcd.print(receivedData.ecgValue);

    // Check for abnormal values and activate buzzer if needed
    if (isAbnormal(receivedData)) {
        digitalWrite(BUZZER_PIN, HIGH);
    } else {
        digitalWrite(BUZZER_PIN, LOW);
    }
}

void setup() {
    Serial.begin(115200);
    
    // Initialize LCD
    lcd.init();
    lcd.setBacklight(255);

    // Initialize Buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);  // Ensure buzzer is off initially

    WiFi.mode(WIFI_STA); // ESP-NOW requires WiFi in Station mode

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed!");
        lcd.setCursor(0, 1);
        lcd.print("ESP-NOW Failed!");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // Nothing needed in loop; data is received asynchronously
}
