const int LED_COUNT = 6;
int leds[LED_COUNT] = {3, 5, 6, 9, 10, 11};

void setup() {
    for(int i = 0; i < LED_COUNT; i++) {
        pinMode(leds[i], OUTPUT);
    }
}

void loop() {
    for(int i = 0; i < LED_COUNT; i++) {
        for(int brightness = 0; brightness <= 255; brightness += 3) {
            analogWrite(leds[i], brightness);
            if(i >= 2) {
                analogWrite(leds[i-2], 255 - brightness);
            }
            delay(5);
        }
        delay(25);
    }
    
    for(int brightness = 255; brightness >= 0; brightness -= 3) {
        analogWrite(leds[LED_COUNT-2], brightness); 
        delay(5);
    }
    for(int brightness = 255; brightness >= 0; brightness -= 3) {
        analogWrite(leds[LED_COUNT-1], brightness); 
        delay(5);
    }
    
    for(int i = LED_COUNT - 1; i >= 0; i--) {
        for(int brightness = 0; brightness <= 255; brightness += 3) {
            analogWrite(leds[i], brightness);
            if(i <= LED_COUNT - 3) {
                analogWrite(leds[i+2], 255 - brightness);
            }
            delay(5);
        }
        delay(25);
    }
    for(int brightness = 255; brightness >= 0; brightness -= 3) {
        analogWrite(leds[1], brightness);
        delay(5);
    }
    
    for(int brightness = 255; brightness >= 0; brightness -= 3) {
        analogWrite(leds[0], brightness);
        delay(5);
    }
    
    delay(450);
}
