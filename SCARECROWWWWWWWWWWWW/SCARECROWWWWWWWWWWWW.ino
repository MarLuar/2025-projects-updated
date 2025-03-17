const int motorPin1 = 3;  // Motor driver input 1
const int motorPin2 = 5;  // Motor driver input 2
const int motorPin3 = 6;  // Motor driver input 3
const int motorPin4 = 9;  // Motor driver input 4
const int pirPin = 7;     // PIR sensor pin
const int buzzerPin = 2;  // Buzzer pin

bool motorRunning = false;
unsigned long motorStartTime = 0;
const unsigned long motorRunTime = 5000; // 5 seconds

int lastState = LOW;
unsigned long motionStartTime = 0;
const unsigned long debounceTime = 500; // 500ms debounce delay

unsigned long lastPirReadTime = 0;
const unsigned long pirReadDelay = 500; // 500ms PIR check delay

unsigned long setupDelayTime = 2000; // Initial startup delay
unsigned long setupStartTime;

bool setupComplete = false;
bool noiseDelayActive = false;
unsigned long noiseDelayStartTime = 0;
const unsigned long noiseDelayTime = 2000; // 1 sec noise delay

void setup() {
  setupStartTime = millis(); 
  Serial.begin(9600);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Handle initial setup delay using millis()
  if (!setupComplete) {
    if (millis() - setupStartTime >= setupDelayTime) {
      setupComplete = true;
      Serial.println("Setup complete. System ready.");
    }
    return;  // Stop loop execution until setup delay is over
  }

  // Check if noise delay is over
  if (noiseDelayActive && millis() - noiseDelayStartTime >= noiseDelayTime) {
    Serial.println("✅ Noise delay over. Ready for new motion detection.");
    noiseDelayActive = false; // Reset noise delay flag
    
    // Check if motion is still present after the noise delay
    int pirState = digitalRead(pirPin);
    if (pirState == HIGH) {
        Serial.println("🔄 Continuous motion detected! Restarting motor.");
        motorRunning = true;
        motorStartTime = millis();
        digitalWrite(buzzerPin, HIGH); // Turn on buzzer

        // Move motor forward
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
    }
}


  // Check PIR sensor every pirReadDelay
  if (millis() - lastPirReadTime >= pirReadDelay && !noiseDelayActive) {
    lastPirReadTime = millis();
    int pirState = digitalRead(pirPin);

    if (pirState == HIGH && lastState == LOW) {
      if (millis() - motionStartTime > debounceTime) {
        Serial.println("🔵 Motion detected! Activating motor and buzzer.");
        Serial.print("⏳ Motion detected at time (ms): ");
        Serial.println(millis());

        motorRunning = true;
        motorStartTime = millis();
        digitalWrite(buzzerPin, HIGH); // Turn on buzzer

        // Move motor forward
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
      }
      motionStartTime = millis();
    } else if (pirState == LOW && lastState == HIGH) {
      Serial.println("⚪ No motion detected.");
    }

    lastState = pirState;
  }

  if (motorRunning) {
    unsigned long elapsedTime = millis() - motorStartTime;
    
    Serial.print("⏳ Motor running for (ms): ");
    Serial.println(elapsedTime);

    if (elapsedTime >= motorRunTime && elapsedTime < 2 * motorRunTime) {
      Serial.println("🔄 Reversing motor direction.");
      // Move motor backward
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, HIGH);
    } else if (elapsedTime >= 2 * motorRunTime) {
      Serial.println("🛑 Stopping motor and buzzer.");
      Serial.print("⏳ Motor stopped at time (ms): ");
      Serial.println(millis());

      // Stop motor
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      digitalWrite(buzzerPin, LOW); // Turn off buzzer
      motorRunning = false;
      
      // Start noise delay
      noiseDelayActive = true;
      noiseDelayStartTime = millis();
      Serial.println("⏳ Noise delay started for 1 second...");
    }
  }
}
