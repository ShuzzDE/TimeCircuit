
#define TIMESTAMP_FOR_BLINKING dates[1]


void blankLine(MAX6954 line){
  for(int i = 0x20; i < 0x30; i++){
    line.set_char(i, 0x10);
    line.set_char(i+0x20, 0x10);
    line.set_char(i+0x40, 0x10);
  }
}

void blankMonth(MAX6954 line){
  for(int i = 0x20; i < 0x23; i++){
    line.set_char(i, 0x10);
    line.set_char(i+0x20, 0x10);
    line.set_char(i+0x40, 0x10);
  }
}

void blankAllMonths(){
  blankMonth(disp[0]);
  blankMonth(disp[1]);
  blankMonth(disp[2]);
}

void updateMonth(MAX6954 line, TimeStamp ts){
  
  line.set_char(0x20, MONTHS[ts.month-1][0]);
  line.set_char(0x21, MONTHS[ts.month-1][1]);
  line.set_char(0x22, MONTHS[ts.month-1][2]);


  
}


boolean isSecondLightOn(){
  return (millis() - blinkTS)<500;
//  return (TIMESTAMP_FOR_BLINKING.second % 2) == 1;
}

void updateTime(MAX6954 line, TimeStamp ts){

      uint16_t year = (ts.century * 100) + ts.year;

      boolean isPm = ts.hour >= 12;

      byte displayHour = (ts.hour % 12) == 0 ? 12 : ts.hour%12;

  
      line.set_char(0x23, (ts.day/10));
      line.set_char(0x2b, (ts.day%10));
      line.set_char(0x24, (year/1000)%10);
      line.set_char(0x2c, (year%1000)/100);
      line.set_char(0x25, (year%100)/10);
      line.set_char(0x2d, (year)%10);
      line.set_char(0x26, (byte)(displayHour/10) + (byte)(isPm?0:0x80));
      line.set_char(0x2e, (byte)(displayHour%10) + (byte)(isPm?0x80:0));
      line.set_char(0x27, (byte)(ts.minute/10) + (byte)(isSecondLightOn()?0x80:0x00));
      line.set_char(0x2f, (byte)(ts.minute%10) + (byte)(isSecondLightOn()?0x80:0x00));
  
}


void updateAll(){
  for(int i = 0; i < 3; i++){
    updateTime(disp[i], dates[i]);
    updateMonth(disp[i], dates[i]);
  }
}

void setIntensity(char intensity){
  disp[0].set_char(INTENSITY, intensity);
  disp[1].set_char(INTENSITY, intensity);
  disp[2].set_char(INTENSITY, intensity);
}

void turnOnSequence(){
  for(int i = 0; i < 3; i++){
    updateTime(disp[i], dates[i]);
  }
  delay(100);
  for(int i = 0; i < 3; i++){
//    delay(150);
    updateMonth(disp[i], dates[i]);
  }
  
}

void blankAll(){
  blankLine(disp[0]);
  blankLine(disp[1]);
  blankLine(disp[2]);
}
