#include <TimerOne.h>

const int LED_COUNT = 6;
int leds[LED_COUNT] = {3, 5, 6, 9, 10, 11};

volatile unsigned long timerCounter = 0;
volatile bool updateFlag = false;

int currentLed = 0;
float currentBrightness = 0;
bool isIncreasing = true;
bool forwardDirection = true;

int fadeOutLed1 = -1;
int fadeOutLed2 = -1;
int fadeOutBrightness1 = 0;
int fadeOutBrightness2 = 0;
bool fadingOut = false;

volatile unsigned long pauseStartCounter = 0;
volatile bool pauseActive = false;


const unsigned long FADE_INTERVAL = 1;
const unsigned long PAUSE_INTERVAL = 225;

const float BRIGHTNESS_STEP = 0.9;
const int FADE_DELAY = 50;

void setup() {
    for(int i = 0; i < LED_COUNT; i++) {
        pinMode(leds[i], OUTPUT);
        analogWrite(leds[i], 0);
    }
    
    Timer1.initialize(2000);
    Timer1.attachInterrupt(timerISR);
}

void timerISR() {
    timerCounter++;
    updateFlag = true;
    
    if(pauseActive) {
        if(timerCounter - pauseStartCounter >= PAUSE_INTERVAL) {
            pauseActive = false;
        }
    }
}

void loop() {
    if(updateFlag) {
        updateFlag = false;
        
        if(pauseActive) {
            // режим паузы
            return;
        }
        
        if(forwardDirection) {
            if(!fadingOut) {
                waveForward();
            } else {
                fadeOutLastTwoForward();
            }
        } else {
            waveBackward();
        }
    }
}

void waveForward() {
    if(isIncreasing) {
        currentBrightness += BRIGHTNESS_STEP;
        if(currentBrightness > 255) currentBrightness = 255;
        
        analogWrite(leds[currentLed], currentBrightness);
        
        if(currentLed >= 2) {
            int fadeLed = currentLed - 2;
            int fadeBrightness = 255 - currentBrightness;
            analogWrite(leds[fadeLed], fadeBrightness);
        }
        
        if(currentBrightness >= 255) {
            static unsigned long stepStartTime = 0;
            if(stepStartTime == 0) {
                stepStartTime = timerCounter;
            }
            
            if(timerCounter - stepStartTime >= 12) {
                stepStartTime = 0;
                currentLed++;
                currentBrightness = 0;
                
                if(currentLed >= LED_COUNT) {
                    fadeOutLed1 = LED_COUNT - 2;
                    fadeOutLed2 = LED_COUNT - 1;
                    fadeOutBrightness1 = 255;
                    fadeOutBrightness2 = 255;
                    fadingOut = true;
                }
            }
        }
    }
}


void fadeOutLastTwoForward() {
    static unsigned long fadeStartTime = 0;
    
    if(fadeOutBrightness1 > 0) {
        fadeOutBrightness1 -= BRIGHTNESS_STEP;
        if(fadeOutBrightness1 < 0) fadeOutBrightness1 = 0;
        analogWrite(leds[fadeOutLed1], fadeOutBrightness1);
    }
    
    if(fadeOutBrightness1 <= 200) { 
        if(fadeOutBrightness2 > 0) {
            fadeOutBrightness2 -= BRIGHTNESS_STEP;
            if(fadeOutBrightness2 < 0) fadeOutBrightness2 = 0;
            analogWrite(leds[fadeOutLed2], fadeOutBrightness2);
        }
    }
    
    if(fadeOutBrightness1 <= 0 && fadeOutBrightness2 <= 0) {
        forwardDirection = false;
        currentLed = LED_COUNT - 1;
        currentBrightness = 0;
        isIncreasing = true;
        fadingOut = false;
        fadeStartTime = 0;
    }
}

void waveBackward() {
    if(isIncreasing) {
        currentBrightness += BRIGHTNESS_STEP;
        if(currentBrightness > 255) currentBrightness = 255;
        
        analogWrite(leds[currentLed], currentBrightness);
        
        if(currentLed <= LED_COUNT - 3) {
            int fadeLed = currentLed + 2;
            int fadeBrightness = 255 - currentBrightness;
            analogWrite(leds[fadeLed], fadeBrightness);
        }
        
        if(currentBrightness >= 255) {
            static unsigned long stepStartTime = 0;
            if(stepStartTime == 0) {
                stepStartTime = timerCounter;
            }
            
            if(timerCounter - stepStartTime >= 12) {
                stepStartTime = 0;
                currentLed--;
                currentBrightness = 0;
                
                if(currentLed < 0) {
                    
                    fadeOutLed1 = 1; 
                    fadeOutLed2 = 0;
                    fadeOutBrightness1 = 255;
                    fadeOutBrightness2 = 255;
                    isIncreasing = false; 
                }
            }
        }
    } else {
        fadeOutLastTwoBackward();
    }
}

void fadeOutLastTwoBackward() {
    static unsigned long fadeStartTime = 0;
    
    if(fadeOutBrightness1 > 0) {
        fadeOutBrightness1 -= BRIGHTNESS_STEP;
        if(fadeOutBrightness1 < 0) fadeOutBrightness1 = 0;
        analogWrite(leds[fadeOutLed1], fadeOutBrightness1);
    }
    
    if(fadeOutBrightness1 <= 200) { 
        if(fadeOutBrightness2 > 0) {
            fadeOutBrightness2 -= BRIGHTNESS_STEP;
            if(fadeOutBrightness2 < 0) fadeOutBrightness2 = 0;
            analogWrite(leds[fadeOutLed2], fadeOutBrightness2);
        }
    }
    
    if(fadeOutBrightness1 <= 0 && fadeOutBrightness2 <= 0) {
        pauseActive = true;
        pauseStartCounter = timerCounter;
        
        forwardDirection = true;
        currentLed = 0;
        currentBrightness = 0;
        isIncreasing = true;
        fadingOut = false;
        fadeOutLed1 = -1;
        fadeOutLed2 = -1;
        fadeStartTime = 0;
    }
}
