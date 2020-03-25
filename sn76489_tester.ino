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
#define LATCH_NOISE 0x60 // set bit 6 to 1, 5 to 1
#define LATCH_VOLUME 0x10 // set bit 4 to 1
#define LATCH_TONE 0x0 // set bit 4 to 0
#define DATA_MODE 0x0 // set bit 7 to 0

#define NOISE_PERIODIC 0x4 // set bit 3 high
#define NOISE_WHITE 0x0 // set bit 3 low

#define MIN_VOLUME 0xf // is this true?
#define MAX_VOLUME 0x0

#define MAX_TONE 0x006
#define MIN_TONE 0x3ff

#define DELAY_BETWEEN_TONES 50
#define DELAY_BETWEEN_NOISES 100

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

  digitalWrite(PIN_NotWE, LOW);
  delay(1); // this is the magic part right here
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

  // 0x3ff - 109hz lowest (MIDI A2)
  // 0x001 - 111,861Hz, highest (MIDI A10 - may not be audible) - 0x006 might be best for max

  // mute channels
  sendByte(LATCH_MODE | LATCH_VOLUME | LATCH_CH0 | MAX_VOLUME);
  sendByte(LATCH_MODE | LATCH_VOLUME | LATCH_CH1 | MIN_VOLUME);
  sendByte(LATCH_MODE | LATCH_VOLUME | LATCH_CH2 | MIN_VOLUME);
  sendByte(LATCH_MODE | LATCH_VOLUME | LATCH_NOISE | MIN_VOLUME);

  Serial.begin(9600);

  Serial.write("Muted all lines.\n");

  delay(25);
}

int getChannelFlag(int channelNumber) {
  switch(channelNumber) {
    case 0: return LATCH_CH0;
    case 1: return LATCH_CH1;
    case 2: return LATCH_CH2;
    default:
      // raise error
      Serial.write("Error: Asked for impossible channel number ");
      Serial.print(channelNumber);
      Serial.write("\n");
      return LATCH_CH0;
  }
}

void loop() {  
  for(int channel = 0; channel < 3; ++channel) {
    Serial.write("Testing tone channel ");
    Serial.print(channel);
    Serial.write("\n");
    int channelFlag = getChannelFlag(channel); // FIXME: there is probably a better way to do this but i'm tired
    // Start up the channel
    sendByte(LATCH_MODE | LATCH_VOLUME | channelFlag | MAX_VOLUME);

    // Sweep through the available tones
    for(int i = MAX_TONE; i < MIN_TONE; i += 10) {
      sendByte(LATCH_MODE | channelFlag | (i & 0xf)); // bottom 4 bits
      sendByte(DATA_MODE | ((i & 0x3f0) >> 4)); // top 6 bits
      delay(DELAY_BETWEEN_TONES); // wait a bit for the human to hear it
    }
    
    // Kill this channel so the next one can start
    sendByte(LATCH_MODE | LATCH_VOLUME | channelFlag | MIN_VOLUME);
    delay(100);
  }

  // now test the noise channel
  Serial.write("Testing noise channel\n");
  sendByte(LATCH_MODE | LATCH_VOLUME | LATCH_NOISE | MAX_VOLUME);
  for(int mode = 0; mode <= 1; ++mode) {
    int noiseMode = mode == 0 ? NOISE_PERIODIC : NOISE_WHITE;
    if(noiseMode == NOISE_PERIODIC) {
      Serial.write("\t...periodic noise\n");
    }
    else {
      Serial.write("\t...white noise\n");
    }

    // try all the 2-bit combinations of noise
    for(int i = 0; i <= 0x3; ++i) {
      sendByte(LATCH_MODE | LATCH_NOISE | noiseMode | i);
      delay(DELAY_BETWEEN_NOISES);
    }
  }
  // turn it back off
  sendByte(LATCH_MODE | LATCH_VOLUME | LATCH_NOISE | MIN_VOLUME);
  delay(100);
}
