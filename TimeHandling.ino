
// Timestamps are stored 48 bytes deep in the EEPROM.
#define TIMESTAMP_STORAGE_EEPROM_OFFSET 48

#define RED dates[2]
#define GREEN dates[1]
#define YELLOW dates[0]

/**
 * Store the time and date from the Timestamp within the RTC.
 * This will preserve the second counter from the RTC.
 */
void updateRTCFromTimestamp(TimeStamp *tsp){
  DateTime dt = Clock.read();
  dt.Minute = tsp->minute;
  dt.Hour = tsp->hour;
  dt.Day = tsp->day;
  dt.Month = tsp->month;
  dt.Year = tsp->year;
  dt.Second = 0;
  Clock.write(dt);
}

void readTimestampFromRTC(TimeStamp *tsp){
  DateTime dt = Clock.read();
  tsp->second = dt.Second;
  tsp->minute = dt.Minute;
  tsp->hour   = dt.Hour;
  tsp->day    = dt.Day;
  tsp->month  = dt.Month;
  tsp->year   = dt.Year;  
}

void copyTimestamp(TimeStamp *from, TimeStamp *to){
  to->second  = from->second;
  to->minute  = from->minute;
  to->hour    = from->hour;
  to->day     = from->day;
  to->month   = from->month;
  to->year    = from->year;
  to->century = from->century;
}

boolean isTimeStampSane(TimeStamp *tsp){

  if(tsp->second > 59)return false;                     // 00-59 seconds per minute
  if(tsp->minute > 59)return false;                     // 00-59 minutes per hour
  if(tsp->hour > 23)return false;                       // 00-23 hours per day
  if(tsp->month > 12 || tsp->month < 1)return false;    // 01-12 months per year
  if(tsp->day < 1)return false;                         // 01-?? days in a month, day 0 does not compute...
  
  uint16_t year = ((uint16_t)tsp->century)*(uint16_t)100 + ((uint16_t)tsp->year);
  boolean isLeapYear = ((year%4) == 0 ) && (((year % 100) != 0) || ((year % 1000) == 0));

  // February has 28 days (29 in a leap year)
  // TODO: Check leap year calculation, this is pretty shitty... ;-)
  if(tsp->month == 2){
    if(tsp->day > (isLeapYear?29:28))return false;
  }
  // January, March, May, July, August, October and December have 31 days-
  if(tsp->month == 1 || tsp->month == 3 || tsp->month == 5 || tsp->month == 7 || tsp->month == 8 || tsp->month == 10 || tsp->month == 12 ){
    if(tsp->day > 31)return false;
  }




  return true;
}

void storeTimestamp(int tsIndex, TimeStamp *tsp){

  int offset = TIMESTAMP_STORAGE_EEPROM_OFFSET + (tsIndex * sizeof(TimeStamp));
  EEPROM[offset++] = tsp->second;
  EEPROM[offset++] = tsp->minute;
  EEPROM[offset++] = tsp->hour;
  EEPROM[offset++] = tsp->day;
  EEPROM[offset++] = tsp->month;
  EEPROM[offset++] = tsp->year;
  EEPROM[offset++] = tsp->century;
}

void readTimestamp(int tsIndex, TimeStamp *tsp){
  int offset = TIMESTAMP_STORAGE_EEPROM_OFFSET + (tsIndex * sizeof(TimeStamp));
  tsp->second  = EEPROM[offset++];  
  tsp->minute  = EEPROM[offset++];  
  tsp->hour    = EEPROM[offset++];  
  tsp->day     = EEPROM[offset++];  
  tsp->month   = EEPROM[offset++];  
  tsp->year    = EEPROM[offset++];  
  tsp->century = EEPROM[offset++];  
}

void storeAllTimestamps(){
  for(int i = 0; i < 3; i++){
    storeTimestamp(i, &dates[i]);
  }
}

void readAllTimestamps(){
  for(int i = 0; i < 3; i++){
    readTimestamp(i, &dates[i]);
  }
}



void setNewTarget(){
  storeTimestamp(2, &dates[2]);
  blankAll();

  delay(200);
  myDFPlayer.play(11);
  turnOnSequence();
  delay(750);
}

void doWarp(){
  
  Serial.println(F("88mph! 1.21GW!!! WOOOoooooo....."));

  copyTimestamp(&GREEN, &YELLOW);
  copyTimestamp(&RED, &GREEN);
  
  updateRTCFromTimestamp(&GREEN);
  storeAllTimestamps();
  blankAllMonths();
  playSelectSound();
  turnOnSequence();
}
