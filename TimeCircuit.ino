#include <DS3231_Simple.h>
#include "MAX6954.h"
#include "datatypes.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <PCF8574.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


#define CS1        10  // MAX6954
#define CS2        9  // MAX6954
#define CS3        8  // MAX6954
#define DATAOUT    12 // MOSI
#define CLK        11 // sck


#define DATAIN     7 // MISO


PCF8574 pcf;
SoftwareSerial mySoftwareSerial(A0, A1); // RX, TX
DFRobotDFPlayerMini myDFPlayer;



const byte DECODE             = 0x01;
const byte INTENSITY         = 0x02;
const byte ScanLIMIT          = 0x03;
const byte CONFIGURATION = 0x04;
const byte TEST                  = 0x07;
const byte DigitTYPE            = 0x0C;

MAX6954 disp[3] = {MAX6954(DATAOUT, CLK, CS1), MAX6954(DATAOUT, CLK, CS2), MAX6954(DATAOUT, CLK, CS3)};
DS3231_Simple Clock;


TimeStamp dates[3] = {TimeStamp(), TimeStamp(), TimeStamp()};


#define INTENSITY_STORAGE_LOCATION 5
#define VOLUME_STORAGE_LOCATION 6
#define BLINKBEEP_STORAGE_LOCATION 7

byte intensity = EEPROM[INTENSITY_STORAGE_LOCATION];
byte volume = EEPROM[VOLUME_STORAGE_LOCATION];
boolean blinkBeep = EEPROM[BLINKBEEP_STORAGE_LOCATION];

byte oldSecond = 0;

uint32_t blinkTS = 0;


void setup() {

  delay(40);
  Serial.begin(115200);
  Serial.println(F("Setup Keypad..."));
  pcf.begin(0x21);
  Serial.println(F("begin() done"));
  pcf.pinMode(0, INPUT_PULLUP);
  pcf.pinMode(1, INPUT_PULLUP);
  pcf.pinMode(2, INPUT_PULLUP);
  pcf.pinMode(3, INPUT_PULLUP);
  pcf.pinMode(4, OUTPUT);
  pcf.pinMode(5, OUTPUT);
  pcf.pinMode(6, OUTPUT);
  pcf.pinMode(7, OUTPUT);
  Serial.println(F("DONE!"));


  Clock.begin();

// Clock.promptForTimeAndDate(Serial);
  Serial.println();
  Clock.printDateTo_YMD(Serial);
  Serial.print(' ');
  Clock.printTimeTo_HMS(Serial);
  Serial.println();



  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  mySoftwareSerial.begin(9600);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  setVolume(volume);
  
  setupExternalTrigger();



  Serial.println(F("Init Displays..."));
  for(int j = 0; j < 3; j++){

    disp[j].set_char(DECODE, 0xff);
    disp[j].set_char(INTENSITY, intensity);
    disp[j].set_char(ScanLIMIT, 0b00000111);
    disp[j].set_char(CONFIGURATION, 0x01);
    disp[j].set_char(TEST, 0x00);
    disp[j].set_char(DigitTYPE, 0x00);
  }
  Serial.println(F("DONE!"));

//  disp[0].set_char(TEST, 0x01);
//  disp[1].set_char(TEST, 0x01);
//  disp[2].set_char(TEST, 0x01);
//  delay(5000);
//  disp[0].set_char(TEST, 0x00);
//  disp[1].set_char(TEST, 0x00);
//  disp[2].set_char(TEST, 0x00);



  blankAll();


  readAllTimestamps();
  readTimestampFromRTC(&dates[1]);
  
  Serial.println(dates[1].year);
  Serial.println(dates[1].month);
  Serial.println(dates[1].day);
  Serial.println(dates[1].hour);
  Serial.println(dates[1].minute);
  Serial.println(dates[1].second);

  playSystemStartupSound();
  turnOnSequence();
  delay(750);

  blinkTS = millis();
  readTimestampFromRTC(&dates[1]);
  oldSecond = dates[1].second;
}




const char months_en[12][4] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
const char months_de[12][4] = {"JAN","FEB","MAE","APR","MAI","JUN","JUL","AUG","SEP","OKT","NOV","DEC"};
#define MONTHS months_en


void loop() {


  if(isExtIntTriggered()){
    doWarp();
  }

  doKeypad();
  doSerial();
  readTimestampFromRTC(&dates[1]);
//  Serial.println(dates[1].second);
  if(dates[1].second != oldSecond){
//    Serial.println(F("Blink ON"));
    oldSecond = dates[1].second;
    blinkTS = millis();
    if(blinkBeep){
      doBlinkBeep();
    }
    updateAll();
  } else if((millis() - blinkTS) > 500){
//    Serial.println(F("Blink OFF"));
    updateAll();
  }
}
