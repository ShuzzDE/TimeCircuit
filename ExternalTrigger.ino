#define MIN_JUMP_DELAY 5000
#define TRIGGER_PIN 4


void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

volatile uint32_t lastJump = 0;

volatile boolean extIntTriggered = false;

boolean isExtIntTriggered(){
  if(extIntTriggered){
    extIntTriggered = false;
    return true;
  }
  return false;
}

ISR(PCINT2_vect){
  digitalWrite(13, HIGH);
  if(!digitalRead(TRIGGER_PIN) && (millis() - lastJump) > MIN_JUMP_DELAY){
    lastJump = millis();
    extIntTriggered = true;
  }
  digitalWrite(13, LOW);
}

void setupExternalTrigger(){
  pinMode(TRIGGER_PIN, INPUT);
  digitalWrite(TRIGGER_PIN, HIGH);
  pciSetup(TRIGGER_PIN);

  pinMode(13, OUTPUT);
}
