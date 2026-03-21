/*
  Lib: SAA1057 PLL
  Version: 1.0.0.15
  Date: 2026/03/20
  Author: Junon M
  Hardware: Arduino Uno or Nano, and Serial Monitor
  Type: With menu
*/

#include "SAA1057.h"

const char * VERSION = "1.0.0.15";

//----------------------------------------------------------------
// Serial menu configuration
//----------------------------------------------------------------
const char MENU_TEXT_FREQ[] = "Frequency";
const char INDEX_FREQ[] = "1";
const float MAX_FREQ = 130.f;
const float MIN_FREQ = 50.f;

const char MENU_TEXT_INT_FREQ[] = "Intermediate Frequency";
const char INDEX_INT_FREQ[] = "2";
const float MAX_INT_FREQ = 10.7f;
const float MIN_INT_FREQ = -10.7f;

const int SEP_COUNT = 60;
//----------------------------------------------------------------

//----------------------------------------------------------------
// Default initial frequency
//----------------------------------------------------------------
float Freq = 98.f; // 100.0 MHz
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



String getCmds() {
  String msg = "";
  msg += Separator(SEP_COUNT);
  msg += "SAA1057 PLL\nFW v" + String(VERSION) + "\n\n";
  msg += "Select an option:\n\n";
  msg += String(INDEX_FREQ) + ". " + String(MENU_TEXT_FREQ) + " = " + String(Freq, 2)  + "MHz\n";
  msg += String(INDEX_INT_FREQ) + ". " + String(MENU_TEXT_INT_FREQ) + " = " + String(IntFreq, 2)  + "MHz\n";
  msg += Separator(SEP_COUNT);
  return msg;
}



void changeParam(String &returned_text, const String menu_label, const String menu_index, String text, float &value, const float min_value, const float max_value,  const String unit) {
  static int pos = 0;
  String S = "";

  if (pos == 0) {
    returned_text = menu_index;
    S += Separator(SEP_COUNT);
    S += "Enter " + menu_label + " (" + String(min_value, 2) + unit + " - " + String(max_value, 2) + unit + " )";
    Serial.println(S);
    pos++;
  } else {
    float number = text.toFloat();

    if ((number >= min_value) && (number <= max_value)) {
      value = number;
      commitConfig();
      pos = 0;
      returned_text = "";
      S += "Response = " + String(number, 2) + unit;
      S += getCmds();
      Serial.println(S);
    } else {
      S = "Error, the value " + String(number, 2) + unit + " is outside the acceptable range! Please try again...";
      Serial.println(S);
    }
  }

}




void handleCmd() {
  static String lastS = "";
  String S = "";

  if (Serial.available()) {
    String text = Serial.readStringUntil('\n');
    S = text;

    if (!lastS.equals("")) S = lastS;

    if (S.equalsIgnoreCase(INDEX_FREQ)) {
      changeParam(lastS, MENU_TEXT_FREQ, INDEX_FREQ, text, Freq, MIN_FREQ, MAX_FREQ, "MHz");
    }
    else if (S.equalsIgnoreCase(INDEX_INT_FREQ)) {
      changeParam(lastS, MENU_TEXT_INT_FREQ, INDEX_INT_FREQ, text, IntFreq, MIN_INT_FREQ, MAX_INT_FREQ, "MHz");
    }
    else {
      Serial.println(getCmds());
    }
  }
}
