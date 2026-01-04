#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ledPin = 6;

const int encoderCLK = 2;
const int encoderDT = 3;
const int encoderSW = 4;

volatile int encoderPos = 0;
volatile int lastEncoded = 0;
int lastEncoderPos = 0;

int brightness = 0;
const int maxBrightness = 255;
const int minBrightness = 0;

const int stepsPerRevolution = 24;
const int targetRevolutions = 2;
const int totalSteps = stepsPerRevolution * targetRevolutions;

unsigned long lastDisplayUpdate = 0;
const long displayUpdateInterval = 100;

void setup() {
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization error"));
    for(;;);
  }
  
  pinMode(ledPin, OUTPUT);
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(encoderCLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderDT), updateEncoder, CHANGE);
  
  analogWrite(ledPin, brightness);
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  
  display.setCursor(0, 0);
  display.println(F("LED Encoder"));
  display.println(F("Rotate to"));
  display.println(F("adjust brightness"));
  display.display();
  
  delay(2000);
  
  Serial.println("System ready");
}

void loop() {
  if (encoderPos != lastEncoderPos) {
    brightness = map(encoderPos, 0, totalSteps, minBrightness, maxBrightness);
    brightness = constrain(brightness, minBrightness, maxBrightness);
    
    analogWrite(ledPin, brightness);
    
    Serial.print("Position: ");
    Serial.print(encoderPos);
    Serial.print(" / ");
    Serial.print(totalSteps);
    Serial.print(" | Brightness: ");
    Serial.print(brightness);
    Serial.print(" (");
    Serial.print((brightness * 100) / 255);
    Serial.println("%)");
    
    lastEncoderPos = encoderPos;
    
    updateDisplay();
  }
  
  if (millis() - lastDisplayUpdate >= displayUpdateInterval) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
  if (digitalRead(encoderSW) == LOW) {
    delay(50);
    if (digitalRead(encoderSW) == LOW) {
      resetBrightness();
      while (digitalRead(encoderSW) == LOW);
    }
  }
  
  delay(10);
}

void updateEncoder() {
  int MSB = digitalRead(encoderCLK);
  int LSB = digitalRead(encoderDT);
  
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;
  
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderPos++;
  }
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderPos--;
  }
  
  encoderPos = constrain(encoderPos, 0, totalSteps);
  lastEncoded = encoded;
}

void resetBrightness() {
  encoderPos = 0;
  brightness = 0;
  analogWrite(ledPin, brightness);
  
  Serial.println("Brightness reset to 0");
  updateDisplay();
}

void updateDisplay() {
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("BRIGHTNESS"));
  
  display.setTextSize(3);
  display.setCursor(0, 20);
  int percentage = (brightness * 100) / 255;
  if (percentage < 10) {
    display.print(" ");
  }
  display.print(percentage);
  display.println("%");
  
  drawProgressBar(0, 50, 120, 8, percentage);
  
  display.setTextSize(1);
  display.setCursor(0, 60);
  display.print(F("Step: "));
  display.print(encoderPos);
  display.print(F("/"));
  display.print(totalSteps);
  
  display.display();
}

void drawProgressBar(int x, int y, int width, int height, int progress) {
  display.drawRect(x, y, width, height, SSD1306_WHITE);
  
  int fillWidth = (progress * (width - 2)) / 100;
  display.fillRect(x + 1, y + 1, fillWidth, height - 2, SSD1306_WHITE);
}
