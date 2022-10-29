#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define CLK 2
#define DT 3
#define SW 4
#define DGFD1 5
#define DGFD2 6

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int16_t inputDelta = 0;             // Counts up or down depending which way the encoder is turned
bool printFlag = false;             // Flag to indicate that the value of inputDelta should be printed


int counter = 0;
int currentStateCLK;
int lastStateCLK;
int maxmode;
String currentDir ="";
unsigned long lastButtonPress = 0;
int add12 = 0;

void setup() {
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(SW, INPUT_PULLUP);
  Serial.begin(9600);
  lastStateCLK = digitalRead(CLK);

}

void loop() {
lcd.setCursor(0,0);
lcd.print("Release 1: ");
lcd.setCursor(11,0);
lcd.print(counter);
lcd.setCursor(13,0);
lcd.print(":00");
lcd.setCursor(0,1);
lcd.print("Release 2: ");
lcd.setCursor(11,1);
lcd.print(add12);
lcd.setCursor(13,1);
lcd.print(":00");
tmElements_t tm;

if (tm.Hour==counter){
      if(tm.Minute==0){
        if(tm.Second==0){
          digitalWrite (6,HIGH);
          delay (5000);
          digitalWrite (6,LOW);
        }
      }
   }      
(RTC.read(tm));
if (tm.Hour==add12){
    if(tm.Minute==0){
      if(tm.Second==0){
        digitalWrite (6,HIGH);
        delay (5000);
        digitalWrite (6,LOW);
      }
   }      
}
if (maxmode==true){
    if (tm.Hour==counter){
      if(tm.Minute==0){
        if(tm.Second==0){
          digitalWrite (6,HIGH);
          delay (5000);
          digitalWrite (6,LOW);
        }
      }
   }      
}
if (maxmode==true){
    if (tm.Hour==add12){
      if(tm.Minute==0){
        if(tm.Second==0){
          digitalWrite (6,HIGH);
          delay (5000);
          digitalWrite (6,LOW);
        }
      }
   }      
}
add12=(counter+8);

/*
currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir ="CCW";
    } else {
      counter ++;
      currentDir ="CW";
    }

  //  Serial.print("Direction: ");
 //   Serial.print(currentDir);
  //  Serial.print(" | Counter: ");
    Serial.println(counter);
  //  Serial.print(" ");
   // Serial.print(add12);
   // Serial.println();
  }


  
  maxmode = digitalRead (6);
  lastStateCLK = currentStateCLK;
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }
    lastButtonPress = millis();
  }
  delay(1);
   // when characters arrive over the serial port...
*/

  // Call the function to get state of encoder
  int stateOfEncoder = readEncoder();
  
  // the printstate will be set IF there is a change
  if (printFlag == true) {
    Serial.println(stateOfEncoder);
  }
  

   
}   // END OF LOOP




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

    return inputDelta;
}

