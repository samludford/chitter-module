
const int ledPin = 1;
const int MAX_STEPS = 16;
const int MIN_STEPS = 3;
const int MIN_BEATS = 1;

int ledState = LOW;
long previousMillis = 0;
int interval = 100;
int blinkTime = 30;

int playhead = 0;

int remainder[MAX_STEPS];
int count[MAX_STEPS];
int level;
int steps;
int pulses;
int pauses;
int patit=0;
char pattern[MAX_STEPS];
int slots;


void setup() {                
  pinMode(ledPin, OUTPUT);
  randomSeed(analogRead(2));
  randomisePattern();
}

void randomisePattern() {
  slots = random(MIN_STEPS, MAX_STEPS);
  int beats = random(MIN_BEATS, slots);
  compute_bitmap(slots, beats);
}

void loop() {

  unsigned long currentMillis = millis();
  
  if(ledState == LOW) {

    if(currentMillis - previousMillis > interval) {

      // this modulo operation is performed at the beginning to account for changes in pattern where the new
      // pattern is shorter than the current playhead position
      playhead = playhead % slots;

      previousMillis = currentMillis;

      if (pattern[playhead] == '1') {
        ledState = HIGH;  
        digitalWrite(ledPin, ledState);
      }

      playhead++;
      
    }
    
    
  } else {

    if(currentMillis - previousMillis > blinkTime) {
      previousMillis = currentMillis;
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
    
  }
}

void compute_bitmap (int num_slots,int num_pulses)  {
    
    for(int i=0 ; i < MAX_STEPS ; i++) {
        pattern[i] = 0;
        remainder[i] = 0;
    }
    
    if (num_pulses > num_slots) {num_pulses = num_slots;}
    int divisor = num_slots - num_pulses;
    steps = num_slots;
    pulses = num_pulses;
    remainder[0] = num_pulses;
    level = 0;
    do {
        count[level] = divisor / remainder[level];
        remainder[level+1] = divisor % remainder[level];
        divisor = remainder[level];
        level = level +1;
    } while (remainder[level] > 1);
    
    count[level] = divisor;
    build_string (level);
    patit = 0;
}

void build_string (int level)  {
    if (level == -1) {
        pattern[patit]='0';
        patit=patit+1;
    }
    else if (level == -2)  {
        pattern[patit]='1';
        patit=patit+1;
    }
    else {
        for (int i = 0; i < count[level]; i++)
            build_string(level-1);
        if (remainder[level] != 0)
            build_string(level-2);
    }
}




