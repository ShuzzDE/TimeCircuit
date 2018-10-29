

/**
 * Befehle: 
 * 
 * S1:YYYY-MM-DD-HH-MM-SS
 * S2:YYYY-MM-DD-HH-MM-SS
 * S3:YYYY-MM-DD-HH-MM-SS
 * Set the time for one of the three lines: 1,2,3
 * Note that 1,2,3 will directy correspond to the MAX6954 objects inside the array.
 * 
 * WARP : Do the timewarp... ;-)
 * 
 */

#define BUFFER_SIZE 25

char buffer[BUFFER_SIZE];
int bufferPointer=0;

// Call this regularly!
void doSerial(){
  while(Serial.available() > 0){
    int incoming = Serial.read();
    if(incoming == -1){
      return;
    }
    char c = incoming;
    if(c == '\n'){      
      checkLine(&dates[2]);
      memset(&buffer, 0, BUFFER_SIZE);
      bufferPointer = 0;
      return;
    }
    if(bufferPointer < BUFFER_SIZE){ 
      buffer[bufferPointer++] = c;
    }
  }
}

// 0         1         2
// 0123456789012345678901
// S1:YYYY-MM-DD-HH-MM-SS

int16_t getNumFromChar(char in){
  return (int16_t)(in-48);
}

boolean checkRange(char in, char lowest, char highest){
  return ((in >= lowest) && (in <= highest));
}

boolean checkLine(TimeStamp *tsp){
  if(buffer[0] == 'W' && buffer[1] == 'A' && buffer[2] == 'R' && buffer[3] == 'P'){
    doWarp();
    return false;
  }
  if(buffer[0] == 'S'){
    // Set command, now check the date.
    int offset = buffer[1] - 48;
    if(offset < 1 || offset > 3){
      return false;
    }
    
    if(!checkRange(buffer[1], '1', '3'))return false;
    if(!checkRange(buffer[2], ':', ':'))return false;
    if(!checkRange(buffer[3], '0', '9'))return false;
    if(!checkRange(buffer[4], '0', '9'))return false;
    if(!checkRange(buffer[5], '0', '9'))return false;
    if(!checkRange(buffer[6], '0', '9'))return false;
    if(!checkRange(buffer[7], '-', '-'))return false;
    if(!checkRange(buffer[8], '0', '1'))return false;
    if(!checkRange(buffer[9], '0', '9'))return false;
    if(!checkRange(buffer[10], '-', '-'))return false;
    if(!checkRange(buffer[11], '0', '3'))return false;
    if(!checkRange(buffer[12], '0', '9'))return false;
    if(!checkRange(buffer[13], '-', '-'))return false;
    if(!checkRange(buffer[14], '0', '2'))return false;
    if(!checkRange(buffer[15], '0', '9'))return false;
    if(!checkRange(buffer[16], '-', '-'))return false;
    if(!checkRange(buffer[17], '0', '5'))return false;
    if(!checkRange(buffer[18], '0', '9'))return false;
    if(!checkRange(buffer[19], '-', '-'))return false;
    if(!checkRange(buffer[20], '0', '5'))return false;
    if(!checkRange(buffer[21], '0', '9'))return false;

    Serial.print(buffer);

    Serial.println(F(" - looks sane, parsing..."));
    tsp->century = getNumFromChar(buffer[3]) * 10 + getNumFromChar(buffer[4]);
    tsp->year = getNumFromChar(buffer[5]) * 10 + getNumFromChar(buffer[6]);
    tsp->month = getNumFromChar(buffer[8]) * 10 + getNumFromChar(buffer[9]);
    tsp->day = getNumFromChar(buffer[11]) * 10 + getNumFromChar(buffer[12]);
    tsp->hour = getNumFromChar(buffer[14]) * 10 + getNumFromChar(buffer[15]);
    tsp->minute = getNumFromChar(buffer[17]) * 10 + getNumFromChar(buffer[18]);
    tsp->second = getNumFromChar(buffer[20]) * 10 + getNumFromChar(buffer[21]);
    setNewTarget();
    return true;
  }
}
