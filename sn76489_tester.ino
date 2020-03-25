// PIN_ is the 76489AN
#define PIN_D0 2
#define PIN_D1 3
#define PIN_D2 4
#define PIN_NotWE 5
#define PIN_D3 6
#define PIN_D4 7
#define PIN_D5 12
#define PIN_D6 11
#define PIN_D7 10

#define LATCH_MODE 0x80 // set bit 7 to 1
#define LATCH_CH0 0x00 // set bit 6, 5 to 0
#define LATCH_CH1 0x40 // set bit 6 to 1, 5 to 0
#define LATCH_CH2 0x20 // set bit 6 to 0, 5 to 1
#define LATCH_CH3 0x60 // set bit 6 to 1, 5 to 1
#define LATCH_VOLUME 0x10 // set bit 4 to 1
#define LATCH_TONE 0x0 // set bit 4 to 0
#define DATA_MODE 0x0 // set bit 7 to 0

// based on http://danceswithferrets.org/geekblog/?p=93

void sendByte(byte b) {
  digitalWrite(PIN_D0, (b & 1) ? HIGH : LOW);
  digitalWrite(PIN_D1, (b & 2) ? HIGH : LOW);
  digitalWrite(PIN_D2, (b & 4) ? HIGH : LOW);
  digitalWrite(PIN_D3, (b & 8) ? HIGH : LOW);
  digitalWrite(PIN_D4, (b & 16) ? HIGH : LOW);
  digitalWrite(PIN_D5, (b & 32) ? HIGH : LOW);
  digitalWrite(PIN_D6, (b & 64) ? HIGH : LOW);
  digitalWrite(PIN_D7, (b & 128) ? HIGH : LOW);
  digitalWrite(PIN_NotWE, HIGH);
  digitalWrite(PIN_NotWE, LOW);
  digitalWrite(PIN_NotWE, HIGH);
}

void setup() {
  // set all pins to output
  pinMode(PIN_D0, OUTPUT);
  pinMode(PIN_D1, OUTPUT);
  pinMode(PIN_D2, OUTPUT);
  pinMode(PIN_D3, OUTPUT);
  pinMode(PIN_D4, OUTPUT);
  pinMode(PIN_D5, OUTPUT);
  pinMode(PIN_D6, OUTPUT);
  pinMode(PIN_D7, OUTPUT);
  pinMode(PIN_NotWE, OUTPUT);
  digitalWrite(PIN_NotWE, HIGH); // don't write yet
  
  // put your setup code here, to run once:
  sendByte(LATCH_MODE | LATCH_CH0 | 0x03);
  sendByte(DATA_MODE | 0x12); // ...set channel 0 tone to 0x123
  sendByte(LATCH_MODE | LATCH_VOLUME | LATCH_CH0 | 0); // set to maximum volume channel 0
  //sendByte(0x9f); // MUTE channel 0

  // 0x3ff - 109hz lowest (MIDI A2)
  // 0x001 - 111,861Hz, highest (MIDI A10 - my not be audible) - 0x006 might be best for max
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for(int channel = 0; channel < 4; ++channel) {
    // Start up the channel

    // Sweep through the available tones

    // Sweep through the available noises (TODO: how to activate noise?)
    
    // Kill this channel so the next one can start
  }

  // TODO: how to set noise?
  
}
