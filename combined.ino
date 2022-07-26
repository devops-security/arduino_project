
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// SCL A5
// SDA A4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
 
#define OLED_RESET 4
 
#define button1    9                       // Button B1 is connected to Arduino pin 9
#define button2    8                       // Button B2 is connected to Arduino pin 8

const unsigned char PROGMEM icon [] = {
  0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x1F, 0xF8, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xE0,
  0x00, 0x1F, 0xF8, 0xF0, 0x00, 0x7F, 0xFE, 0x70, 0x00, 0xFF, 0xFF, 0x20, 0x01, 0xF0, 0x0F, 0x80,
  0x03, 0xC1, 0x83, 0xC0, 0x03, 0x81, 0x81, 0xC0, 0x07, 0x01, 0x80, 0xE0, 0x07, 0x01, 0x80, 0xE0,
  0x0E, 0x01, 0x80, 0x70, 0x0E, 0x01, 0x80, 0x70, 0x0E, 0x03, 0xC0, 0x70, 0x0E, 0x03, 0xC0, 0x70,
  0x0E, 0x03, 0xC0, 0x70, 0x0E, 0x01, 0x80, 0x70, 0x0E, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0xE0,
  0x07, 0x00, 0x00, 0xE0, 0x07, 0x80, 0x01, 0xE0, 0x03, 0xC0, 0x03, 0xC0, 0x01, 0xE0, 0x07, 0x80,
  0x00, 0xF8, 0x1F, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x07, 0xE0, 0x00

};

const unsigned char PROGMEM swatch [] = {
  0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x00, 0x08, 0x07, 0xEC, 0x0E, 0x70, 0x18, 0x18, 0x30, 0x0C,
  0x30, 0x8C, 0x21, 0x84, 0x21, 0x84, 0x30, 0x0C, 0x10, 0x08, 0x18, 0x18, 0x0F, 0xF0, 0x03, 0xC0
};

void setupClock(void) {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  delay(1000);
 
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 
  // init done
 
  // Clear the display buffer.
  display.clearDisplay();
  display.display();
 
  display.setTextColor(WHITE, BLACK);
  display.drawRect(117, 25, 3, 3, WHITE);     // Put degree symbol ( ° )
  draw_text(0, 25, "TEMPERATURE =", 1);
  draw_text(122, 25, "C", 1);
}

void setupStopwatch()   {
  pinMode(6, INPUT_PULLUP); //Switch
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.display();


  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Viral Science");
  display.display();
  delay(800);
}

int poz = 1;
int broj = 1;
int kretanjeY = 30;

int sec1 = 0;
int min1 = 0;
unsigned long msec = 0;
unsigned long mili = 0;
int pres = 0;
int fase = 0;
int start = 0;
unsigned long tim = 0;

void loopStopWatch() {
  display.clearDisplay();
  if (digitalRead(6) == 0)
  {
    if (pres == 0)
    {
      fase = fase + 1;
      pres = 1;
      if (fase > 2)
        fase = 0;
    }

  } else {
    pres = 0;
  }

  if (fase == 0)
  {
    display.setFont(&FreeSans12pt7b);
    display.setFont();
    display.setCursor(50, 50);
    display.print("Start");
    display.drawBitmap(48, 8,  icon, 32, 32, WHITE);



    sec1 = 0;
    min1 = 0;
    tim = 0;
    mili = 0;
    msec = 0;
    start = 0;
  }

  if (fase == 1)
  {
    display.clearDisplay();
    display.setFont(&FreeSans12pt7b);
    display.setFont();
    display.setCursor(37, 0);
    display.print("Stopwatch");
    display.setFont(&FreeSans9pt7b);

    if (start == 0)
    {
      start = 1;
      tim = millis();
    }
    msec = (millis() - tim);



    min1 = msec / 60000;


    if ((msec / 1000) > 59)
    {
      sec1 = (msec / 1000) - (min1 * 60);
    } else {
      sec1 = msec / 1000;
    }

    mili = (msec % 1000) / 10;

    display.setCursor(42, 30);
    if (min1 <= 9)
    {
      display.print("0");
      display.print(min1);
    } else {
      display.print(min1);
    }

    display.print(":");

    if (sec1 <= 9)
    {
      display.print("0");
      display.print(sec1);
    } else {
      display.print(sec1);
    }
    display.setFont(&FreeSans12pt7b);
    display.setCursor(50, 57);

    if (mili <= 9)
    {
      display.print("0");
      display.print(mili);
    } else {
      display.print(mili);
    }
  }

  if (fase == 2)
  {
    display.clearDisplay();
    display.setFont(&FreeSans12pt7b);
    display.setFont();
    display.setCursor(52, 0);
    display.print("Time:");
    display.setFont(&FreeSans9pt7b);
    display.setCursor(42, 30);
    if (min1 <= 9)
    {
      display.print("0");
      display.print(min1);
    } else {
      display.print(min1);
    }

    display.print(":");

    if (sec1 <= 9)
    {
      display.print("0");
      display.print(sec1);
    } else {
      display.print(sec1);
    }
    display.setFont(&FreeSans12pt7b);
    display.setCursor(50, 57);

    if (mili <= 9)
    {
      display.print("0");
      display.print(mili);
    } else {
      display.print(mili);
    }
    display.drawBitmap(105, 20,  swatch, 16, 16, 1);
    display.drawBitmap(12, 20,  swatch, 16, 16, 1);


  }


  display.display();
}






char Time[]     = "  :  :  ";
char Calendar[] = "  /  /20  ";
char temperature[] = " 00.00";
char temperature_msb;
byte i, second, minute, hour, day, date, month, year, temperature_lsb;
 
void display_day(){
  switch(day){
    case 1:  draw_text(0, 0, " SUNDAY  ", 1); break;
    case 2:  draw_text(0, 0, " MONDAY  ", 1); break;
    case 3:  draw_text(0, 0, " TUESDAY ", 1); break;
    case 4:  draw_text(0, 0, "WEDNESDAY", 1); break;
    case 5:  draw_text(0, 0, "THURSDAY ", 1); break;
    case 6:  draw_text(0, 0, " FRIDAY  ", 1); break;
    default: draw_text(0, 0, "SATURDAY ", 1);
  }
}
 
void DS3231_display(){
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour >> 4)   * 10 + (hour & 0x0F);
  date   = (date >> 4)   * 10 + (date & 0x0F);
  month  = (month >> 4)  * 10 + (month & 0x0F);
  year   = (year >> 4)   * 10 + (year & 0x0F);
  // End conversion
 
  Time[7]     = second % 10 + 48;
  Time[6]     = second / 10 + 48;
  Time[4]     = minute % 10 + 48;
  Time[3]     = minute / 10 + 48;
  Time[1]     = hour   % 10 + 48;
  Time[0]     = hour   / 10 + 48;
  Calendar[9] = year   % 10 + 48;
  Calendar[8] = year   / 10 + 48;
  Calendar[4] = month  % 10 + 48;
  Calendar[3] = month  / 10 + 48;
  Calendar[1] = date   % 10 + 48;
  Calendar[0] = date   / 10 + 48;
  if(temperature_msb < 0){
    temperature_msb = abs(temperature_msb);
    temperature[0] = '-';
  }
  else
    temperature[0] = ' ';
  temperature_lsb >>= 6;
  temperature[2] = temperature_msb % 10  + 48;
  temperature[1] = temperature_msb / 10  + 48;
  if(temperature_lsb == 0 || temperature_lsb == 2){
    temperature[5] = '0';
    if(temperature_lsb == 0) temperature[4] = '0';
    else                     temperature[4] = '5';
  }
  if(temperature_lsb == 1 || temperature_lsb == 3){
    temperature[5] = '5';
    if(temperature_lsb == 1) temperature[4] = '2';
    else                     temperature[4] = '7';
  }
 
  draw_text(60,  0, Calendar, 1);                     // Display the date (format: dd/mm/yyyy)
  draw_text(10, 9, Time, 2);                         // Display the time
  draw_text(75, 25, temperature, 1);                  // Display the temperature
}
 
void blink_parameter(){
  byte j = 0;
  while(j < 10 && digitalRead(button1) && digitalRead(button2)){
    j++;
    delay(25);
  }
}
 
byte edit(byte x_pos, byte y_pos, byte parameter){
  char text[3];
  sprintf(text,"%02u", parameter);
  while(!digitalRead(button1));                      // Wait until button B1 released
  while(true){
    while(!digitalRead(button2)){                    // If button B2 is pressed
      parameter++;
      if(i == 0 && parameter > 31)                   // If date > 31 ==> date = 1
        parameter = 1;
      if(i == 1 && parameter > 12)                   // If month > 12 ==> month = 1
        parameter = 1;
      if(i == 2 && parameter > 99)                   // If year > 99 ==> year = 0
        parameter = 0;
      if(i == 3 && parameter > 23)                   // If hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 4 && parameter > 59)                   // If minutes > 59 ==> minutes = 0
        parameter = 0;
      sprintf(text,"%02u", parameter);
      draw_text(x_pos, y_pos, text, 1);
      delay(200);                                    // Wait 200ms
    }
    draw_text(x_pos, y_pos, "  ", 1);
    blink_parameter();
    draw_text(x_pos, y_pos, text, 1);
    blink_parameter();
    if(!digitalRead(button1)){                       // If button B1 is pressed
      i++;                                           // Increament 'i' for the next parameter
      return parameter;                              // Return parameter value and exit
    }
  }
}
 
void draw_text(byte x_pos, byte y_pos, char *text, byte text_size) {
  display.setCursor(x_pos, y_pos);
  display.setTextSize(text_size);
  display.print(text);
  display.display();
}

void setup() {
  pinMode(10, INPUT);
  Serial.begin(9600);
  
}

boolean clockSetupRun = false;
boolean stopWatchSetupRun = false;

// if the state is zero show the clock
// if the state is one show the stopwatch
int currentState = 0;

//int buttonState = 0;   

boolean buttonState = LOW; 
 
void loop() {

  //buttonState = digitalRead(10);

  /*
   * CHeck for button press using debounce routine. If pressed flip flags to display 
   * CLock or stopwatch. Reset setup flags so setups dont run over and over
   */
  Serial.println("===== START =========="); 
  Serial.println("CurrentState:" + String(currentState));
  Serial.println("clockSetupRun:" + String(clockSetupRun));
  Serial.println("stopWatchSetupRun:" + String(stopWatchSetupRun));
  
 
   

  if(debounceButton(buttonState,10) == HIGH && buttonState == LOW)
  {
    buttonState = HIGH;
    // button was pressed
    if (currentState == 0) {
      currentState = 1;   
    } else {
      currentState = 0;
    }

    clockSetupRun = false;
    stopWatchSetupRun = false;
  } else if(debounceButton(buttonState,10) == LOW && buttonState == HIGH) {
     buttonState = LOW;
     currentState = 0;
  }

  if (currentState == 0) {
    if (clockSetupRun == false) {
      setupClock();
      clockSetupRun = true;  
    }
    loopClock();
  }
  
  if (currentState == 1) {  
    if (stopWatchSetupRun == false) {
      setupStopwatch();
      stopWatchSetupRun = true;
    }
    loopStopWatch();
  }

  
}



void loopClock() {

 if(!digitalRead(button1)){                         // If button B1 is pressed
    i = 0;
    while(!digitalRead(button1));                    // Wait for button B1 release
    while(true){
      while(!digitalRead(button2)){                  // While button B2 pressed
        day++;                                       // Increment day
        if(day > 7) day = 1;
        display_day();                               // Call display_day function
        delay(200);                                  // Wait 200 ms
      }
      draw_text(0, 0, "         ", 1);
      blink_parameter();                             // Call blink_parameter function
      display_day();                                 // Call display_day function
      blink_parameter();                             // Call blink_parameter function
      if(!digitalRead(button1))                      // If button B1 is pressed
        break;
    }
    //set position of text when editing on button press
    date   = edit(60, 0, date);                      // Edit date
    month  = edit(80, 0, month);                    // Edit month
    year   = edit(110,0, year);                    // Edit year
    hour   = edit(14, 9, hour);                     // Edit hours
    minute = edit(50, 9, minute);                   // Edit minutes
 
    // Convert decimal to BCD
    minute = ((minute / 10) << 4) + (minute % 10);
    hour = ((hour / 10) << 4) + (hour % 10);
    date = ((date / 10) << 4) + (date % 10);
    month = ((month / 10) << 4) + (month % 10);
    year = ((year / 10) << 4) + (year % 10);
    // End conversion
 
    // Write data to DS3231 RTC
    Wire.beginTransmission(0x68);               // Start I2C protocol with DS3231 address
    Wire.write(0);                              // Send register address
    Wire.write(0);                              // Reset sesonds and start oscillator
    Wire.write(minute);                         // Write minute
    Wire.write(hour);                           // Write hour
    Wire.write(day);                            // Write day
    Wire.write(date);                           // Write date
    Wire.write(month);                          // Write month
    Wire.write(year);                           // Write year
    Wire.endTransmission();                     // Stop transmission and release the I2C bus
    delay(200);                                 // Wait 200ms
  }
 
  Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0);                                // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS3231 and release I2C bus at end of reading
  second = Wire.read();                         // Read seconds from register 0
  minute = Wire.read();                         // Read minuts from register 1
  hour   = Wire.read();                         // Read hour from register 2
  day    = Wire.read();                         // Read day from register 3
  date   = Wire.read();                         // Read date from register 4
  month  = Wire.read();                         // Read month from register 5
  year   = Wire.read();                         // Read year from register 6
  Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0x11);                             // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 2);                    // Request 2 bytes from DS3231 and release I2C bus at end of reading
  temperature_msb = Wire.read();                // Read temperature MSB
  temperature_lsb = Wire.read();                // Read temperature LSB
 
  display_day();
  DS3231_display();                             // Diaplay time & calendar
 
  delay(50);                                    // Wait 50ms 
  
  
}




boolean debounceButton(boolean state,int buttonPin)
{
  boolean stateNow = digitalRead(buttonPin);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(buttonPin);
  }
  Serial.println("Determined button state is:" + String(stateNow));
  return stateNow;
  
}




