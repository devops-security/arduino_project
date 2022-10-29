#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 

// Input pins to connect to the encoder
const uint8_t CLK = 2;
const uint8_t DT = 3;

int16_t inputDelta = 0;             // Counts up or down depending which way the encoder is turned
bool printFlag = false;             // Flag to indicate that the value of inputDelta should be printed

void setup() {
    Serial.begin(9600);
    pinMode(CLK, INPUT_PULLUP);
    pinMode(DT, INPUT_PULLUP);
    
    
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    
    lcd.init();
    lcd.backlight();
    lcd.setCursor(3,0);
        
    Serial.begin(9600);
    
}

void loop() {

    
    // every loop, read the encoder
    int encoderChangeValue = readEncoder();
    // If the encoder retunrs a zero nothing changed
    if ( encoderChangeValue != 0) {
      // modify your code here
      Serial.println("i have detected a change");
    }
    
    printDelta();
    
}

int readEncoder() {
    static uint8_t state = 0;
    bool CLKstate = digitalRead(CLK);
    bool DTstate = digitalRead(DT);
    switch (state) {
        case 0:                         // Idle state, encoder not turning
            if (!CLKstate){             // Turn clockwise and CLK goes low first
                state = 1;
            } else if (!DTstate) {      // Turn anticlockwise and DT goes low first
                state = 4;
            }
            break;
        // Clockwise rotation
        case 1:                     
            if (!DTstate) {             // Continue clockwise and DT will go low after CLK
                state = 2;
            } 
            break;
        case 2:
            if (CLKstate) {             // Turn further and CLK will go high first
                state = 3;
            }
            break;
        case 3:
            if (CLKstate && DTstate) {  // Both CLK and DT now high as the encoder completes one step clockwise
                state = 0;
                ++inputDelta;
                printFlag = true;
            }
            break;
        // Anticlockwise rotation
        case 4:                         // As for clockwise but with CLK and DT reversed
            if (!CLKstate) {
                state = 5;
            }
            break;
        case 5:
            if (DTstate) {
                state = 6;
            }
            break;
        case 6:
            if (CLKstate && DTstate) {
                state = 0;
                --inputDelta;
                printFlag = true;
            }
            break; 
    }

    if (printFlag) {
      return inputDelta;  
    } else {
      return 0;
    }
    
}

void printDelta() {
    if (printFlag) {
        printFlag = false;
        Serial.println(inputDelta);
            lcd.setCursor(0,0);
    lcd.print("Release 1: ");
    lcd.setCursor(11,0);
    //lcd.print(counter);
    lcd.setCursor(13,0);
    lcd.print(inputDelta);
    lcd.setCursor(0,1);
    lcd.print("Release 2: ");
    lcd.setCursor(11,1);
    //lcd.print(add12);
    lcd.setCursor(13,1);
    lcd.print(":00");
    }
}
