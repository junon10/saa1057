/*
  Lib: SAA1057 PLL
  Version: 1.0.0.9
  Date: 2026/02/14
  Author: Junon M
  Hardware: Arduino Uno or Nano with DipSwitch
*/

#include "SAA1057.h"

//---------------------------------------------------
// FI adjust (intermediate frequency adjust)
//---------------------------------------------------
// 0.0 MHz, 10.7 MHz, -10.7 MHz
const float IntFreq = 0.0f; 
//---------------------------------------------------

//---------------------------------------------------
// SAA1057 pins to Arduino
//---------------------------------------------------
const int SAA_CLOCK_PIN = 10;
const int SAA_DATA_PIN  = 11;
const int SAA_DLEN_PIN  = 12;
//---------------------------------------------------

SAA1057 pll;

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("SAA1057 PLL");
  Serial.println();

  // DipSwitch Arduino pinout 
  pll.setDipSwPinout(7,6,5,4,3,2,1,0);
   
  pll.begin(SAA_CLOCK_PIN, SAA_DATA_PIN, SAA_DLEN_PIN);

  pll.clear(0xFFFF, 0);        
  pll.set(T_IN_LOCK_CNT, T_SHL);  // Test pin output = In-lock counter
  pll.set(BRM, BRM_SHL);  // Current in latch automatically reduced
  pll.clear(PDM_CLEAR, PDM_SHL);  // Phase detector mode = Automatic on/off
  pll.clear(SLA, SLA_SHL);  // LatchA load mode = Asynchronous
  pll.set(SB2, SB2_SHL);  // Enables the last 8 bits of the wordB SLA until T0
  pll.set(CP_07, CP_SHL);   // Phase detector current = 0.07mA
  pll.clear(REFH, REFH_SHL); // Ref = 1KHz
  pll.set(FM, FM_SHL);   // FM mode
  pll.set(WORDB, WORDB_SHL);  // Flag WordB
  
  pll.setFreqShift(/* intermediate frequency in MHz */ IntFreq);

  // Rated current for FM transmitters
  //
  pll.setFrequencyByDipSw(/* Phase detector current */ CP_07);
  delay(2000);
  pll.setFrequencyByDipSw(/* Phase detector current */ CP_007);
}


void loop() {
}
