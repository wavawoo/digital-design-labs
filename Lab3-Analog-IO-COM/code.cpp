const int potPin = A7;
const int ledPin = 8;

int potValue = 0;
int lastMode = -1;
bool alertMode = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);
  int mode = getMode(potValue);

  if (mode != lastMode) {
    lastMode = mode;
    switch (mode) {
      case 0:
        Serial.println("Непрерывное горение");
        break;
      case 1:
        Serial.println("Редкое мигание");
        break;
      case 2:
        Serial.println("Частое мигание");
        break;
    }
  }

  switch (mode) {
    case 0:
      digitalWrite(ledPin, HIGH);
      break;
    case 1:
      blinkLed(1000);
      break;
    case 2:
      blinkLed(50);
      break;
  }

  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'T') {
      alertMode = true;
      Serial.println("Режим оповещения включен");
    } else if (command == 'F') {
      alertMode = false;
      Serial.println("Режим оповещения отключен");
    }
  }
}

int getMode(int value) {
  if (value < 50) return 0;
  else if (value > 200) return 2;
  else return 1;
}

void blinkLed(int interval) {
  digitalWrite(ledPin, HIGH);
  delay(interval);
  digitalWrite(ledPin, LOW);
  delay(interval);
}
