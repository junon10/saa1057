/*
  Lib: SAA1057 PLL
  Version: 1.0.0.11
  Date: 2026/03/15
  Author: Junon M
  Hardware: Arduino Uno or Nano, and DipSwitch
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

saa1057_wordB WordB;

SAA1057 pll;

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("SAA1057 PLL");
  Serial.println();

  // DipSwitch Arduino pinout
  pll.setDipSwPinout(7, 6, 5, 4, 3, 2, 1, 0);

  pll.begin(SAA_CLOCK_PIN, SAA_DATA_PIN, SAA_DLEN_PIN);

  WordB.refined.ADDR = ADDR_WORDB;
  WordB.refined.FM = MODE_FM;
  WordB.refined.REF = REF_1KHZ;
  WordB.refined.CP = CP_0_07MA;
  WordB.refined.SB2 = SB2_ON;
  WordB.refined.SLA = SLA_ASYNC;
  WordB.refined.PDM = PDM_AUTO;
  WordB.refined.BRM = BRM_ECONOMY;
  WordB.refined.T = T_LOCK_DET;

  pll.set(WordB.raw);

  pll.setFreqShift(/* intermediate frequency in MHz */ IntFreq);

  // Rated current for FM transmitters
  //
  pll.setFrequencyByDipSw(/* Phase detector current */ CP_0_7MA);
  delay(2000);
  pll.setFrequencyByDipSw(/* Phase detector current */ CP_0_07MA);
}


void loop() {
}
