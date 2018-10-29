


void setVolume(byte vol){
  Serial.print(F("Setting Volume to "));
  Serial.println(vol);
  myDFPlayer.volume(vol * 2);
  playSelectSound();
}

void playSelectSound(){
  myDFPlayer.play(11);
}

void playSystemStartupSound(){
  myDFPlayer.play(13);
  while(! myDFPlayer.available());
  myDFPlayer.play(11);
}

void doBlinkBeep(){
  myDFPlayer.play(12);
  Serial.println(F("BlinkBeep!"));
}

boolean errorTone(){
  myDFPlayer.play(14);
  return true;
}
