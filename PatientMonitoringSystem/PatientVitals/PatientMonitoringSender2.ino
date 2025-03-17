#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <esp_now.h>
#include <WiFi.h>

#define I2C_ADDR 0x27
#define RATE_SIZE 4
#define SPO2_UPDATE_INTERVAL 5000
#define FINGER_THRESHOLD 50000
#define DISPLAY_UPDATE_INTERVAL 500
#define TEMP_UPDATE_INTERVAL 2000

#define MLX_SDA 18
#define MLX_SCL 19
#define AD8232_OUTPUT 34  // ECG analog signal

MAX30105 particleSensor;
LiquidCrystal_I2C lcd(I2C_ADDR, 20, 4);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute = 0;
int beatAvg = 0;
int fakeSpO2 = 98;
int displayedSpO2 = 0;

unsigned long lastSpO2Update = 0;
unsigned long lastUpdateTime = 0;
unsigned long lastTempUpdateTime = 0;
unsigned long fingerStartTime = 0;
bool fingerDetected = false;
bool fingerPreviouslyDetected = false;

TwoWire MLXWire = TwoWire(1);

typedef struct {
    int irValue;
    int beatAvg;
    int fakeSpO2;
    float objectTemp;
    int ecgValue;
    bool fingerDetected;
} SensorData;

SensorData dataToSend;

uint8_t receiverMAC[] = {0x3C, 0x8A, 0x1F, 0xA8, 0x2F, 0x94};  // Replace with receiver MAC

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(100000);
    MLXWire.begin(MLX_SDA, MLX_SCL);
    MLXWire.setClock(100000);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("MAX30105 not detected!");
        while (1);
    }

    if (!mlx.begin(MLX90614_I2CADDR, &MLXWire)) {
        Serial.println("MLX90614 not detected!");
        while (1);
    }

    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed");
        return;
    }

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, receiverMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void sendData() {
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&dataToSend, sizeof(dataToSend));
    if (result == ESP_OK) {
        Serial.println("Data sent successfully!");
    } else {
        Serial.println("Error sending data.");
    }
}

void loop() {
    long irValue = particleSensor.getIR();
    fingerDetected = (irValue > FINGER_THRESHOLD);

    if (fingerDetected) {
        if (!fingerPreviouslyDetected) {
            fingerStartTime = millis();
        }

        if (checkForBeat(irValue)) {
            long delta = millis() - lastBeat;
            lastBeat = millis();
            beatsPerMinute = 60.0 / (delta / 1000.0);
            if (beatsPerMinute > 20 && beatsPerMinute < 255) {
                rates[rateSpot++] = (byte)beatsPerMinute;
                rateSpot %= RATE_SIZE;

                beatAvg = 0;
                for (byte x = 0; x < RATE_SIZE; x++) {
                    beatAvg += rates[x];
                }
                beatAvg /= RATE_SIZE;
                beatAvg = beatAvg + 15;
            }
        }

        if (millis() - lastSpO2Update >= SPO2_UPDATE_INTERVAL && beatAvg > 0) {
            fakeSpO2 = getFakeSpO2();
            lastSpO2Update = millis();
        }

        if (millis() - fingerStartTime >= 3000) {
            if (displayedSpO2 < fakeSpO2 && beatAvg > 0) {
                displayedSpO2++;
            }
        }
    } else {
        beatAvg = 0;
        displayedSpO2 = 0;
    }

    fingerPreviouslyDetected = fingerDetected;

    if (millis() - lastUpdateTime >= DISPLAY_UPDATE_INTERVAL) {
        lastUpdateTime = millis();
        lcd.clear();
        lcd.setCursor(0, 0);

        if (fingerDetected) {
            lcd.print("BPM: "); lcd.print(beatAvg);
        } else {
            lcd.print("No Finger Detected");  
        }

        lcd.setCursor(0, 1);
        lcd.print("SpO2: ");
        lcd.print(displayedSpO2);
        lcd.print("%");

        float objectTemp = mlx.readObjectTempC() + 2;
        lcd.setCursor(0, 2);
        lcd.print("Obj Temp: ");
        lcd.print(isnan(objectTemp) ? "Error" : String(objectTemp) + " C");

        int ecgValue = analogRead(AD8232_OUTPUT);
        lcd.setCursor(0, 3);
        lcd.print("ECG: ");
        lcd.print(ecgValue);

        Serial.print("ECG Value: ");
        Serial.println(ecgValue);

        dataToSend.irValue = irValue;
        dataToSend.beatAvg = fingerDetected ? beatAvg : 0;
        dataToSend.fakeSpO2 = displayedSpO2;
        dataToSend.objectTemp = objectTemp;
        dataToSend.ecgValue = ecgValue;
        dataToSend.fingerDetected = fingerDetected;

        sendData();
    }
}

int getFakeSpO2() {
    return random(95, 101);
}
