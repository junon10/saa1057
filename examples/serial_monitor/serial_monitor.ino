/*
  Lib: SAA1057 PLL
  Version: 1.0.0.14
  Date: 2026/03/15
  Author: Junon M
  Hardware: Arduino Uno or Nano, and Serial Monitor
  Type: No menu
*/

#include "SAA1057.h"

const char * VERSION = "1.0.0.14";

//----------------------------------------------------------------
// Serial menu configuration
//----------------------------------------------------------------
const char MENU_TEXT_FREQ[] = "Frequency";
const float MAX_FREQ = 130.f;
const float MIN_FREQ = 50.f;

const char MENU_TEXT_INT_FREQ[] = "Intermediate Frequency";
const float MAX_INT_FREQ = 10.7f;
const float MIN_INT_FREQ = -10.7f;

const int SEP_COUNT = 60;
//----------------------------------------------------------------

//----------------------------------------------------------------
// Default initial frequency
//----------------------------------------------------------------
float Freq = 98.f; // in MHz
//----------------------------------------------------------------

//----------------------------------------------------------------
// Intermediate frequency for FM receiver +10.7MHz
//----------------------------------------------------------------
float IntFreq = 10.7f; //  0.0 MHz
//----------------------------------------------------------------

//----------------------------------------------------------------
// SAA1057 pins to Arduino
//----------------------------------------------------------------
const int SAA_CLOCK_PIN = 10;
const int SAA_DATA_PIN  = 11;
const int SAA_DLEN_PIN  = 12;
//----------------------------------------------------------------

saa1057_wordB WordB;

SAA1057 pll; // PLL object declaration


void setup() {

  Serial.begin(115200);
   
  // Arduino pins
  pll.begin(SAA_CLOCK_PIN, SAA_DATA_PIN, SAA_DLEN_PIN);

  WordB.ADDR = ADDR_WORDB;
  WordB.FM = MODE_FM;
  WordB.REF = REF_1KHZ;
  WordB.CP = CP_0_23MA;
  WordB.SB2 = SB2_ON;
  WordB.SLA = SLA_ASYNC;
  WordB.PDM = PDM_AUTO;
  WordB.BRM = BRM_ECONOMY;
  WordB.T = T_LOCK_DET;

  pll.set(WordB.raw);
  
  commitConfig();  

  delay(500);

  Serial.println(getConfig());
}


void loop() {
  handleCmd();
}


void commitConfig() {
  pll.setFreqShift(/* Intermediate frequency in MHz */ IntFreq);
  pll.setFrequency(/* Frequency in MHz */ Freq, /* Phase detector current */ SAA1057_RX_FAST_TUNE);
  delay(50); // time to tune in
  pll.setFrequency(/* Frequency in MHz */ Freq, /* Phase detector current */ SAA1057_RX_SLOW_TUNE);
}


String Separator(int len) {
  String s = "\n";
  for (int i = 0; i < len; i++) s += "-";
  s += "\n";
  return s; 
}



String getConfig() {
  String msg = "";
  msg += Separator(SEP_COUNT);
  msg += "SAA1057 PLL\nFIRMWARE VERSION: " + String(VERSION) + "\n";
  msg += String(MENU_TEXT_FREQ) + " = " + String(Freq, 2)  + "MHz\n";
  msg += String(MENU_TEXT_INT_FREQ) + " = " + String(IntFreq, 2)  + "MHz";
  msg += Separator(SEP_COUNT);
  return msg;
}


void handleCmd() {
  if (Serial.available()) {
    String text = Serial.readStringUntil('\n');
    text.trim();
    if (text.length() == 0) {
      Serial.println(getConfig());
      return;
    }
    float value = text.toFloat();
    if (value < MIN_FREQ) {
      IntFreq = constrain(value, MIN_INT_FREQ, MAX_INT_FREQ);
      Serial.println(getConfig());
      commitConfig(); 
    } else {
      Freq = constrain(value, MIN_FREQ, MAX_FREQ);
      Serial.println(getConfig());
      commitConfig();
    }
  }
}
