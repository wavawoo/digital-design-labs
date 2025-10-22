const int buttonPin = 2;
const int ledPin = 3;
int brightnessLevel = 0;
int maxLevels = 5;
int currentLevel = 0;
boolean lastButtonState = HIGH;
boolean buttonPressed = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  boolean currentButtonState = digitalRead(buttonPin);
  
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50);
    currentButtonState = digitalRead(buttonPin);
    if (currentButtonState == LOW) {
      buttonPressed = true;
    }
  }
    if (buttonPressed) {
    buttonPressed = false;
    
    currentLevel++;
    if (currentLevel >= maxLevels) {
      currentLevel = 0;  
    }
    
    if (currentLevel == 0) {
      brightnessLevel = 0;
    } else {
      brightnessLevel = map(currentLevel, 1, maxLevels-1, 50, 255);
    }
    analogWrite(ledPin, brightnessLevel);
    delay(300);
  }
  lastButtonState = currentButtonState;
}
