#define steps 128

void setup ()
{
  pinMode (9, OUTPUT);

#if (defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__))
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= (1 << COM2B0);   // Toggle OC2A on Compare Match.
  TCCR2A |= (1 << WGM21);    // CTC mode
#else
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1A |= (1 << COM1A0);   // Toggle OC1A on Compare Match.
  TCCR1B |= (1 << WGM12);    // CTC mode
#endif

  Serial.begin(57600);
}

void loop () {
  for (int i = 0; i < steps; i++) {

#if (defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__))
    TCCR2B &= 0xFE; // turns off timer
    TCNT2 = 0;      // resets timer counter register
    OCR2A = i;      // changes frequency step
    TCCR2B |= 0x01; // turns on timer
#else
    TCCR1B &= 0xFE; // turns off timer
    TCNT1 = 0;      // resets timer counter register
    OCR1A = i;      // changes frequency step
    TCCR1B |= 0x01; // turns on timer
#endif

    Serial.print(i,DEC);
    Serial.print(" ");
    Serial.println(analogRead(0), DEC);
  }
}
