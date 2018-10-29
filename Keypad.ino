

#define INPUT_DATE dates[2]

char kpbuff[13];
uint8_t kpbuffp = 0;



void setRowActive(uint8_t rowNum){
  pcf.digitalWrite(4, (rowNum == 0)?LOW:HIGH);
  pcf.digitalWrite(5, (rowNum == 1)?LOW:HIGH);
  pcf.digitalWrite(6, (rowNum == 2)?LOW:HIGH);
  pcf.digitalWrite(7, (rowNum == 3)?LOW:HIGH);
}


char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'F','0','E','D'}
};

char readKey(){

  uint8_t result = 0;

  for(int row = 0; row < 4; row++){
    setRowActive(row);
    result = pcf.read();
//    Serial.print(row);
//    Serial.print(F("   "));
//    Serial.println(result, BIN);
    for(int col = 0; col < 4; col++){
      if(! (result & (1 << col))){
        return keys[row][col];
      }
    }
  }
  return -1;
}


boolean checkBuffer(){
  Serial.println(F("checkBuffer() called..."));
  if(kpbuff[kpbuffp] == 'A'){
    if(kpbuffp == 0){
      Serial.println(F("warping."));
      doWarp();
    }
    return true;
  }
  if(kpbuff[kpbuffp] == 'D'){

    if(kpbuffp == 10){
      if( kpbuff[0] == '1' &&
          kpbuff[1] == '2' &&
          kpbuff[2] == '3' &&
          kpbuff[3] == '4' &&
          kpbuff[4] == '5' &&
          kpbuff[5] == '6' &&
          kpbuff[6] == '7' &&
          kpbuff[7] == '8' &&
          kpbuff[8] == '9' &&
          kpbuff[9] == '0'){
          doConfig();
          return true;
      }
    }
    Serial.println(F("D received, checking input."));
    TimeStamp ts;
    copyTimestamp(&INPUT_DATE, &ts);
    
    if(kpbuffp < 4)return true;
    if(!checkRange(kpbuff[0], '0', '9'))return errorTone();
    if(!checkRange(kpbuff[1], '0', '9'))return errorTone();
    if(!checkRange(kpbuff[2], '0', '9'))return errorTone();
    if(!checkRange(kpbuff[3], '0', '9'))return errorTone();
    ts.century = getNumFromChar(kpbuff[0]) * 10 + getNumFromChar(kpbuff[1]);
    Serial.print(ts.century);
    ts.year = getNumFromChar(kpbuff[2]) * 10 + getNumFromChar(kpbuff[3]);
    Serial.println(ts.year);
    if(kpbuffp < 6){
      copyTimestamp(&ts, &INPUT_DATE);
      setNewTarget();
      return true;
    }

    if(!checkRange(kpbuff[4], '0', '1'))return errorTone();
    if(!checkRange(kpbuff[5], '0', '9'))return errorTone();
    ts.month = getNumFromChar(kpbuff[4]) * 10 + getNumFromChar(kpbuff[5]);
    if(ts.month < 1 || ts.month > 12)return errorTone();
    if(kpbuffp < 8){
      copyTimestamp(&ts, &INPUT_DATE);
      setNewTarget();
      return true;
    }
    
    if(!checkRange(kpbuff[6], '0', '3'))return errorTone();
    if(!checkRange(kpbuff[7], '0', '9'))return errorTone();
    ts.day = getNumFromChar(kpbuff[6]) * 10 + getNumFromChar(kpbuff[7]);
    // TODO: Proper checking for Leap days needed here!!! ;-)
    if(ts.day < 1 || ts.day > 31)return errorTone();
    if(kpbuffp < 10){
      copyTimestamp(&ts, &INPUT_DATE);
      setNewTarget();
      return true;
    }
    
    if(!checkRange(kpbuff[8], '0', '2'))return errorTone();
    if(!checkRange(kpbuff[9], '0', '9'))return errorTone();
    ts.hour = getNumFromChar(kpbuff[8]) * 10 + getNumFromChar(kpbuff[9]);
    if(ts.hour > 23)return errorTone();
    if(kpbuffp < 12){
      copyTimestamp(&ts, &INPUT_DATE);
      setNewTarget();
      return true;
    }

    if(!checkRange(kpbuff[10], '0', '5'))return errorTone();
    if(!checkRange(kpbuff[11], '0', '9'))return errorTone();
    ts.minute = getNumFromChar(kpbuff[10]) * 10 + getNumFromChar(kpbuff[11]);
    if(ts.minute > 59)return errorTone();
    copyTimestamp(&ts, &INPUT_DATE);
    setNewTarget();
    return true;
  }
  return false;
}




void doKeypad(){
  char res = readKey();
  if(res >= 0){
    kpbuff[kpbuffp] = res;
    if(res >= '0' && res <= '9'){
      myDFPlayer.play(res-47);
    }

    if(checkBuffer()){
      Serial.println(F("checkBuffer() returned true, clearing buffer..."));
      memset(kpbuff, 0, sizeof(kpbuff));
      kpbuffp = 0;
    } else if(kpbuffp < sizeof(kpbuff)-1){
      kpbuffp++;
    }
    while(readKey() != -1);
    Serial.println(res);
  }
}
