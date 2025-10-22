const int LED_COUNT = 6;
int leds[LED_COUNT] = {3, 5, 6, 9, 10, 11};

unsigned long previousMillis = 0;
unsigned long pauseStartTime = 0;
const unsigned long FADE_DELAY = 5;
const unsigned long PAUSE_DELAY = 450;

int currentLed = 0;
int currentBrightness = 0;
bool isIncreasing = true;
bool forwardDirection = true;
bool inPause = false;

int fadeOutLed1 = -1;
int fadeOutLed2 = -1;
int fadeOutBrightness1 = 0;
int fadeOutBrightness2 = 0;
bool fadingOut = false;

void setup() {
    for(int i = 0; i < LED_COUNT; i++) {
        pinMode(leds[i], OUTPUT);
        analogWrite(leds[i], 0);
    }
}

void loop() {
    unsigned long currentMillis = millis();
    
    if(inPause) {
        if(currentMillis - pauseStartTime >= PAUSE_DELAY) {
            inPause = false;
            forwardDirection = true;
            currentLed = 0;
            currentBrightness = 0;
            isIncreasing = true;
            fadingOut = false;
            fadeOutLed1 = -1;
            fadeOutLed2 = -1;
        }
    } else if(currentMillis - previousMillis >= FADE_DELAY) {
        previousMillis = currentMillis;
        
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
        currentBrightness += 3;
        if(currentBrightness > 255) currentBrightness = 255;
        
        analogWrite(leds[currentLed], currentBrightness);
        
        if(currentLed >= 2) {
            int fadeLed = currentLed - 2;
            int fadeBrightness = 255 - currentBrightness;
            analogWrite(leds[fadeLed], fadeBrightness);
        }
        
        if(currentBrightness >= 255) {
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

void fadeOutLastTwoForward() {
    if(fadeOutBrightness1 > 0) {
        fadeOutBrightness1 -= 3;
        if(fadeOutBrightness1 < 0) fadeOutBrightness1 = 0;
        analogWrite(leds[fadeOutLed1], fadeOutBrightness1);
    }
    
    if(fadeOutBrightness1 <= 127 && fadeOutBrightness2 > 0) {
        fadeOutBrightness2 -= 3;
        if(fadeOutBrightness2 < 0) fadeOutBrightness2 = 0;
        analogWrite(leds[fadeOutLed2], fadeOutBrightness2);
    }
    
    if(fadeOutBrightness1 <= 0 && fadeOutBrightness2 <= 0) {
        forwardDirection = false;
        currentLed = LED_COUNT - 1;
        currentBrightness = 0;
        isIncreasing = true;
        fadingOut = false;
    }
}


void waveBackward() {
    if(isIncreasing) {
        currentBrightness += 3;
        if(currentBrightness > 255) currentBrightness = 255;
        
        analogWrite(leds[currentLed], currentBrightness);
        
        if(currentLed <= LED_COUNT - 3) {
            int fadeLed = currentLed + 2;
            int fadeBrightness = 255 - currentBrightness;
            analogWrite(leds[fadeLed], fadeBrightness);
        }
        
        if(currentBrightness >= 255) {
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
    } else {
        fadeOutLastTwoBackward();
    }
}

void fadeOutLastTwoBackward() {
    if(fadeOutBrightness1 > 0) {
        fadeOutBrightness1 -= 3;
        if(fadeOutBrightness1 < 0) fadeOutBrightness1 = 0;
        analogWrite(leds[fadeOutLed1], fadeOutBrightness1);
    }
    
    if(fadeOutBrightness1 <= 127 && fadeOutBrightness2 > 0) {
        fadeOutBrightness2 -= 3;
        if(fadeOutBrightness2 < 0) fadeOutBrightness2 = 0;
        analogWrite(leds[fadeOutLed2], fadeOutBrightness2);
    }
    
    if(fadeOutBrightness1 <= 0 && fadeOutBrightness2 <= 0) {
        inPause = true;
        pauseStartTime = millis();
    }
}
