#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <map>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define BUTTON_1 32  // Up
#define BUTTON_2 34  // Down
#define BUTTON_3 35  // Confirm
#define SS_PIN 4     // SDA (ESP32) for RFID 1
#define SS_PIN_2 5   // SDA (ESP32) for RFID 2 (Authorization)
#define RST_PIN -1
#define LED_PIN 15   // LED indicator
#define LED_PIN2 16  // LED indicator
#define BUZZER_PIN 17 // Buzzer pin

#define I2C_ADDR 0x27  // LCD I2C Address

// Solenoid lock pins (virtual simulation)
#define SOLENOID_PROJECTOR 33
#define SOLENOID_HDMI 25

// Ultrasonic sensor pins
#define TRIG_PIN_PROJECTOR 12
#define ECHO_PIN_PROJECTOR 13
#define TRIG_PIN_HDMI 14
#define ECHO_PIN_HDMI 27

LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);  // RFID 1
MFRC522 mfrc522_auth(SS_PIN_2, RST_PIN);  // RFID 2 (Authorization)

const char* ssid = "name";
const char* password = "password";
const char* scriptURL = "https://script.google.com/macros/s/AKfycbx4clkFSm7GklUDHpqFKG_FFgOm9VLl6Zh3txjHVhQvGGuT_cnPeD8wW7Y7_3iDlTU-sg/exec";

const char* equipment[] = {"Projector", "HDMI Cable"}; // Removed "Laptop"
int currentSelection = 0;
bool waitingForStudentID = true;
bool waitingForAuthID = false;
bool authorized = false;  // Track authorization status
String lastScannedStudentID = "";
String lastScannedStudentName = "";
std::map<String, bool> assetStatus;
std::map<String, bool> lockerStatus; // Track if equipment is in locker

// Variables for millis() implementation
unsigned long lockOpenStartTime = 0;  // Tracks when the lock was opened
bool isLockOpen = false;             // Tracks if the lock is currently open
String currentLockAsset = "";        // Tracks which asset's lock is open

void setup() {
    Serial.begin(115200); 
    SPI.begin();
   
    // Initialize RFID 1
    mfrc522.PCD_Init();  
    Serial.println("RFID 1 Initialized.");

    // Initialize RFID 2 (Authorization)
    mfrc522_auth.PCD_Init();  
    Serial.println("RFID 2 Initialized.");

    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_PIN2, LOW);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize solenoid locks (virtual simulation)
    pinMode(SOLENOID_PROJECTOR, OUTPUT);
    pinMode(SOLENOID_HDMI, OUTPUT);
    digitalWrite(SOLENOID_PROJECTOR, HIGH); // Ensure locks are closed initially
    digitalWrite(SOLENOID_HDMI, HIGH);

    // Initialize ultrasonic sensors
    pinMode(TRIG_PIN_PROJECTOR, OUTPUT);
    pinMode(ECHO_PIN_PROJECTOR, INPUT);
    pinMode(TRIG_PIN_HDMI, OUTPUT);
    pinMode(ECHO_PIN_HDMI, INPUT);

    lcd.init();
    lcd.backlight();

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    // Initialize buttons
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);

    // Initialize asset status
    for (int i = 0; i < 2; i++) { // Adjusted for 2 equipment
        assetStatus[equipment[i]] = false;
        lockerStatus[equipment[i]] = false; // Initialize locker status
    }

    showMenu();
}

void loop() {
    // Check locker status for each equipment
    checkLockerStatus();

    if (waitingForStudentID) {
        handleMenuSelection();
    } else if (waitingForAuthID) {
        handleAuthorization();
    } else {
        handleRFIDScan();
    }

    // Check if the lock should be closed
    checkLockStatus();
}

void checkLockerStatus() {
    // Check Projector locker
    float distanceProjector = getDistance(TRIG_PIN_PROJECTOR, ECHO_PIN_PROJECTOR);
    if (distanceProjector < 10) {
        lockerStatus["Projector"] = true; // Equipment is in locker
        digitalWrite(LED_PIN, HIGH);
    } else {
        lockerStatus["Projector"] = false; // Equipment is not in locker
        digitalWrite(LED_PIN, LOW);
    }

    // Check HDMI Cable locker
    float distanceHDMI = getDistance(TRIG_PIN_HDMI, ECHO_PIN_HDMI);
    if (distanceHDMI < 10) {
        lockerStatus["HDMI Cable"] = true; // Equipment is in locker
        digitalWrite(LED_PIN2, HIGH);
    } else {
        lockerStatus["HDMI Cable"] = false; // Equipment is not in locker
        digitalWrite(LED_PIN2, LOW);
    }

    // Print locker status to Serial Monitor
    Serial.print("Projector Locker: ");
    Serial.println(lockerStatus["Projector"] ? "Occupied" : "Empty");
    Serial.print("HDMI Cable Locker: ");
    Serial.println(lockerStatus["HDMI Cable"] ? "Occupied" : "Empty");
}

float getDistance(int trigPin, int echoPin) {
    // Send a pulse to the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the echo pulse duration
    long duration = pulseIn(echoPin, HIGH);

    // Calculate distance in centimeters
    float distance = duration * 0.034 / 2;
    return distance;
}

void handleMenuSelection() {
    if (digitalRead(BUTTON_1) == HIGH) {
        currentSelection = (currentSelection - 1 + 2) % 2; // Adjusted for 2 equipment
        showMenu();
        delay(300);
    }
    if (digitalRead(BUTTON_2) == HIGH) {
        currentSelection = (currentSelection + 1) % 2; // Adjusted for 2 equipment
        showMenu();
        delay(300);
    }
    if (digitalRead(BUTTON_3) == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Item: ");
        lcd.print(equipment[currentSelection]);
        lcd.setCursor(0, 1);
        lcd.print("Scan ID");
        waitingForStudentID = false;
        delay(500);
    }
}

void handleRFIDScan() {
    // Check for new student card
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String uidString = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            uidString += String(mfrc522.uid.uidByte[i], HEX);
        }

        if (uidString.length() < 8) {  // Ensure it's a valid UID length
            Serial.println("Invalid card detected.");
            return;
        }

        Serial.print("Student Card UID: ");
        Serial.println(uidString);
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);

        if (uidString.equalsIgnoreCase("c2cf4979")) {  // Replace with your student card UID
            lastScannedStudentName = "Mar Luar Igot";
            lastScannedStudentID = "23233299";
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ID Scanned");
            lcd.setCursor(0, 1);
            lcd.print("Scan Auth ID");
            waitingForAuthID = true;
            digitalWrite(BUZZER_PIN, HIGH);
        } else {
            Serial.println("Unknown card.");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unknown Card!");
            lcd.setCursor(0, 1);
            lcd.print("Try Again.");
            delay(2000);
            resetLCD();
        }

        mfrc522.PICC_HaltA();  // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    }
}

void handleAuthorization() {
    // Check for new authorization card
    if (mfrc522_auth.PICC_IsNewCardPresent() && mfrc522_auth.PICC_ReadCardSerial()) {
        String uidString = "";
        for (byte i = 0; i < mfrc522_auth.uid.size; i++) {
            uidString += String(mfrc522_auth.uid.uidByte[i], HEX);
        }

        if (uidString.length() < 8) {  // Ensure it's a valid UID length
            Serial.println("Invalid card detected.");
            return;
        }

        Serial.print("Authorization Card UID: ");
        Serial.println(uidString);

        if (uidString.equalsIgnoreCase("d31a9ca9")) {  // Replace with your authorization card UID
            authorized = true;
            digitalWrite(BUZZER_PIN, LOW);
            digitalWrite(LED_PIN, HIGH);
            delay(500);
            digitalWrite(LED_PIN, LOW);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Authorized!");
            lcd.setCursor(0, 1);
            lcd.print("Processing...");
            delay(2000);
            processBorrowing();
        } else {
            Serial.println("Unauthorized card.");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unauthorized!");
            lcd.setCursor(0, 1);
            lcd.print("Try Again.");
            delay(2000);
            resetLCD();
        }

        mfrc522_auth.PICC_HaltA();  // Halt PICC
        mfrc522_auth.PCD_StopCrypto1();  // Stop encryption on PCD
    }
}

String getLastActionFromSheets(String assetName, String studentID) {
    HTTPClient http;
    String url = String(scriptURL) + "?action=getLastAction&asset=" + urlencode(assetName) + "&student=" + urlencode(studentID);

    Serial.println("Requesting last action from: " + url);
    http.begin(url);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);  // ✅ Follow redirects

    int httpResponseCode = http.GET();
    String response = "None";  // Default if no response

    if (httpResponseCode == 200) {
        response = http.getString();
        response.trim();
        Serial.println("Last action from Sheets: " + response);
    } else {
        Serial.print("Error getting last action: ");
        Serial.println(httpResponseCode);
        response = "Error";  
    }

    http.end();
    return (response.isEmpty() || response == "Error") ? "None" : response;
}

void processBorrowing() {
    String assetName = equipment[currentSelection];

    if (lastScannedStudentID == "" || lastScannedStudentName == "") {
        Serial.println("Error: No valid student data. Skipping update.");
        return;
    }

    Serial.println("Checking current status...");
    String lastAction = getLastActionFromSheets(assetName, lastScannedStudentID);

    if (lastAction == "Borrowed") {  
        assetStatus[assetName] = false;
        sendToGoogleSheets(assetName, lastScannedStudentName, lastScannedStudentID, "Returned");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(assetName);
        lcd.setCursor(0, 1);
        lcd.print("Returned");
    } 
    else if (lastAction == "Returned" || lastAction == "None") {  
        assetStatus[assetName] = true;
        sendToGoogleSheets(assetName, lastScannedStudentName, lastScannedStudentID, "Borrowed");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(assetName);
        lcd.setCursor(0, 1);
        lcd.print("Borrowed");
    } 
    else {
        Serial.println("Error: Invalid last action received!");
    }

    // Always open the lock for 1 minute after authorization
    openSolenoidLock(assetName);

    delay(2000);
    resetLCD();
}

// Solenoid lock functions (virtual simulation)
void openSolenoidLock(String assetName) {
    if (assetName == "Projector") {
        Serial.println("Projector locker opened.");
        digitalWrite(SOLENOID_PROJECTOR, LOW); // Simulate opening lock
    } else if (assetName == "HDMI Cable") {
        Serial.println("HDMI Cable locker opened.");
        digitalWrite(SOLENOID_HDMI, LOW); // Simulate opening lock
    }

    // Start the timer and set the state
    lockOpenStartTime = millis();
    isLockOpen = true;
    currentLockAsset = assetName;
}
void checkLockStatus() {
    if (isLockOpen && (millis() - lockOpenStartTime >= 30000)) { // 30 seconds elapsed
        closeSolenoidLock(currentLockAsset);
        isLockOpen = false; // Reset the state
    }
}

void closeSolenoidLock(String assetName) {
    if (assetName == "Projector") {
        Serial.println("Projector locker closed.");
        digitalWrite(SOLENOID_PROJECTOR, HIGH); // Simulate closing lock
    } else if (assetName == "HDMI Cable") {
        Serial.println("HDMI Cable locker closed.");
        digitalWrite(SOLENOID_HDMI, HIGH); // Simulate closing lock
    }
}

String urlencode(String str) {
    String encoded = "";
    for (int i = 0; i < str.length(); i++) {
        char c = str.charAt(i);
        if (c == ' ') encoded += "%20";
        else if (c == '&') encoded += "%26";
        else if (c == '=') encoded += "%3D";
        else if (c == '?') encoded += "%3F";
        else encoded += c;
    }
    return encoded;
}

void sendToGoogleSheets(String assetName, String studentName, String studentID, String action) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(scriptURL) + "?asset_name=" + urlencode(assetName) 
            + "&student_name=" + urlencode(studentName) 
            + "&student_id=" + urlencode(studentID)
            + "&action=" + urlencode(action);

        Serial.println("Sending data to Google Sheets...");
        http.begin(url);
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); // ✅ FOLLOW REDIRECTS

        int httpCode = http.GET();

        if (httpCode > 0) {  
            String response = http.getString();
            Serial.println("Server Response: " + response);
        } else {
            Serial.print("Error sending data. HTTP Code: ");
            Serial.println(httpCode);
        }

        http.end();
    } else {
        Serial.println("WiFi not connected!");
    }
}

void showMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select Equipment:");
    lcd.setCursor(0, 1);
    lcd.print("> ");
    lcd.print(equipment[currentSelection]);
}

void resetLCD() {
    waitingForStudentID = true;
    waitingForAuthID = false;
    authorized = false;  // Reset authorization after each transaction
    showMenu();
}