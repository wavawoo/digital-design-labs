const int ledPins[] = {5, 6, 9};
const int numLeds = 3;
const int buttonIncrease = 3;  
const int buttonDecrease = 2; 
const int buzzerPin = 12;

volatile int activeLeds = 0;
volatile bool updateNeeded = false;
volatile bool soundNeeded = false;
volatile byte soundType = 0;
volatile unsigned long lastInterruptTime = 0;

void setup() {
  
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  pinMode(buttonIncrease, INPUT_PULLUP);
  pinMode(buttonDecrease, INPUT_PULLUP);
  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  attachInterrupt(digitalPinToInterrupt(buttonIncrease), increaseLeds, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonDecrease), decreaseLeds, FALLING);
  
}

void loop() {
  if (updateNeeded) {
    updateLeds();
    updateNeeded = false;
  }
  
  if (soundNeeded) {
    playWarningSound(soundType);
    soundNeeded = false;
  }
  
}

void increaseLeds() {
  if (millis() - lastInterruptTime > 250) { 
    if (activeLeds < numLeds) {
      activeLeds++;
      updateNeeded = true;
    } else {
      soundType = 1;
      soundNeeded = true;
    }
    lastInterruptTime = millis();
  }
}

void decreaseLeds() {
  if (millis() - lastInterruptTime > 250) {
    if (activeLeds > 0) {
      activeLeds--;
      updateNeeded = true;
    } else {
      soundType = 2;
      soundNeeded = true;
    }
    lastInterruptTime = millis();
  }
}
void updateLeds() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], (i < activeLeds) ? HIGH : LOW);
  }
}

void playWarningSound() {
  tone(buzzerPin, HIGH);
  delay(1000);
  tone(buzzerPin, LOW);
  noTone(buzzerPin);
}
