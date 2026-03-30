/*
 * Project: SAA1057 Arduino Library
 * File: dip_switch_to_freq.ino
 * Version: v1.0.0.17
 * Date: 2026/03/29
 * License: GNU GPL v3.0
 * Developed by: Junon M. (2008-2026)
 * Description: Embedded systems and RF signal processing.
 */

#include "SAA1057.h"

//---------------------------------------------------
// FI adjust (intermediate frequency adjust)
//---------------------------------------------------
// 0 KHz, 10700 KHz, -10700 KHz
const int32_t IntFreq = 0;
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

  // Dip Switch Arduino pinout
  pll.setDipSwitchPins(7, 6, 5, 4, 3, 2, 1, 0);

  pll.begin(SAA_CLOCK_PIN, SAA_DATA_PIN, SAA_DLEN_PIN);

  WordB.ADDR = ADDR_WORDB;
  WordB.FM = MODE_FM;
  WordB.REF = REF_1KHZ;
  WordB.CP = CP_0_07MA;
  WordB.SB2 = SB2_ON;
  WordB.SLA = SLA_ASYNC;
  WordB.PDM = PDM_AUTO;
  WordB.BRM = BRM_ECONOMY;
  WordB.T = T_LOCK_DET;

  pll.set(WordB.raw);

  pll.setFreqShift(/* intermediate frequency in KHz */ IntFreq);

  pll.setFrequencyFromDipSwitch(/* Phase detector current */ SAA1057_TX_FAST_TUNE);
  delay(2000);
  pll.setFrequencyFromDipSwitch(/* Phase detector current */ SAA1057_TX_SLOW_TUNE);
}


void loop() {
}
