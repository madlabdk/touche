#define steps 128

float values[steps];
float alpha;
int maxPos, maxVal;

void setup ()
{
  pinMode (9, OUTPUT); 
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1A |= (1 << COM1A0);        // Toggle OC1A on Compare Match.
  TCCR1B |= (1 << WGM12);         // CTC mode
  Serial.begin(57600);
}

void loop () {
  if (Serial.available()) {
    alpha = (float)Serial.read() / 255.0f;
  }
  maxPos = 0;
  maxVal = 0;
  for (int i = 0; i < steps; i++) {
    TCCR1B &= 0xFE;                       // turns off timer
    TCNT1 = 0;                            // resets timer counter register
    OCR1A = i;                            // sets new frequency step
    TCCR1B |= 0x01;                       // turns on timer
    float curVal = analogRead(0);
    values[i] = values[i] * alpha + curVal * (1 - alpha);  // exponential moving avg
    if (values[i] > maxVal) {                              // finds the signal peak
      maxVal = values[i];
      maxPos = i;
    }
  }
  Serial.print(maxPos, DEC);
  Serial.print(" ");
  Serial.println(maxVal, DEC);
}
