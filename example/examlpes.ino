#include <Arduino.h>

// for 74xx595 
// #define SHIFTREG_MSBFIRST
// #define SHIFTREG_LSBFIRST

#define OUTPUT_COUNT 16

#include "MemoryOptimizedOutputs.h"

MemoryOptimizedOutputs outputs(OUTPUT_COUNT);

void setup() {
  Serial.begin(9600);

  outputs.setAll(true);
  outputs.printStates();                  // Output States [16]: 1111111111111111

  outputs.setAll(false);
  outputs.printStates();                  // Output States [16]: 0000000000000000

  outputs.setOnOff(5, true);
  Serial.println(outputs.getState(5));    // 1
  outputs.setOnOff(5, false);
  Serial.println(outputs.getState(5));    // 0

  Serial.println(outputs.getState(15));   // 0
  outputs.toggle(15);
  Serial.println(outputs.getState(15));   // 1

  outputs.printStates();                  // Output States [16]: 0100000000000000
  outputs.reverseBits();
  outputs.printStates();                  // Output States [16]: 0000000000000010
  
  Serial.println(outputs.getBitfield());          // 0000000000000010
  Serial.println(outputs.getReversedBitfield());  // 0100000000000000


  Serial.println(outputs.getTypeStr());   //uint16_t


#ifdef SHIFTREG_MSBFIRST
  uint8_t rawM[(OUTPUT_COUNT + 7) / 8];
  outputs.getRawDataReversed(rawM, sizeof(rawM));

  digitalWrite(latchPin, LOW);
  for (int i = sizeof(rawM) - 1; i >= 0; i--) {
    shiftOut(dataPin, clockPin, MSBFIRST, rawM[i]);
  }
  digitalWrite(latchPin, HIGH);
#endif

#ifdef SHIFTREG_LSBFIRST
  uint8_t rawL[(OUTPUT_COUNT + 7) / 8];
  outputs.getRawData(rawL, sizeof(rawL));

  digitalWrite(latchPin, LOW);
  for (int i = sizeof(rawL) - 1; i >= 0; i--) {
    shiftOut(dataPin, clockPin, LSBFIRST, rawL[i]);
  }
  digitalWrite(latchPin, HIGH);
#endif



}

void loop() {
  // teszt
}