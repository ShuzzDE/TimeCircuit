
#define VOL_MIN 0
#define VOL_MAX 10
#define INT_MIN 0
#define INT_MAX 15


void doConfig(){

  while(readKey() != -1);
  blankAll();

  disp[0].set_char(DECODE, 0b11001111);
  disp[2].set_char(0x20, 'B');
  disp[2].set_char(0x21, 'R');
  disp[2].set_char(0x22, 'I');
    
  disp[1].set_char(0x20, 'V');
  disp[1].set_char(0x21, 'O');
  disp[1].set_char(0x22, 'L');

  disp[0].set_char(0x20, 'T');
  disp[0].set_char(0x21, 'O');
  disp[0].set_char(0x22, 'N');
  disp[0].set_char(0x23, 0x0e);
  disp[0].set_char(0x2d, 0);



  boolean exitConfig = false;
  boolean saveValues = true;
  
  while(! exitConfig){
    disp[2].set_char(0x24, (byte)((byte)intensity/10));
    disp[2].set_char(0x2c, (byte)((byte)intensity%10));

    disp[1].set_char(0x24, (byte)((byte)volume/10));
    disp[1].set_char(0x2c, (byte)((byte)volume%10));
        
    disp[0].set_char(0x24, 0b01111110);
    disp[0].set_char(0x2c, blinkBeep?0b00010101:0b01000111);
    disp[0].set_char(0x25, blinkBeep?0x00:0b01000111);

    char in = readKey();
    if(in != -1){
      switch(in){
        case '1':
          if(intensity > INT_MIN)intensity--;
          setIntensity(intensity);
        break;
        case '3':
          if(intensity < INT_MAX)intensity++;
          setIntensity(intensity);
        break;
        case '4':
          if(volume > VOL_MIN)volume--;
          setVolume(volume);
        break;
        case '6':
          if(volume < VOL_MAX)volume++;
          setVolume(volume);
        break;
        case '7':
          blinkBeep = true;
        break;
        case '9':
          blinkBeep = false;
        break;
        case 'A':
          saveValues = false;
        case 'D':
          exitConfig = true;
        break;
      }
      while(readKey() != -1);
    }
  }
  disp[0].set_char(DECODE, 0b11111111);
  blankAll();
  if(saveValues){
    EEPROM[INTENSITY_STORAGE_LOCATION] = intensity;
    EEPROM[VOLUME_STORAGE_LOCATION] = volume;
    EEPROM[BLINKBEEP_STORAGE_LOCATION] = blinkBeep;
  } else {
    intensity = EEPROM[INTENSITY_STORAGE_LOCATION];
    volume = EEPROM[VOLUME_STORAGE_LOCATION];
    blinkBeep = EEPROM[BLINKBEEP_STORAGE_LOCATION];
  }
  setVolume(volume);
  setIntensity(intensity);
}
