/*
 Project: SAA1057 Arduino Library
 File: serial_monitor_menu.ino
 Version: 1.0.0.20
 Date: 2026/04/12
 License: GNU GPL v3.0
 Developed by: Junon M. (2008-2026)
 Description: Embedded systems and RF signal processing.
*/

#include "SAA1057.h"

//----------------------------------------------------------------
// Serial menu configuration
//----------------------------------------------------------------
const char MENU_TEXT_FREQ[] = "Freq";
const char INDEX_FREQ[] = "1";
const float MAX_FREQ = 130.f;
const float MIN_FREQ = 50.f;

const char MENU_TEXT_INT_FREQ[] = "IF";
const char INDEX_INT_FREQ[] = "2";
const float MAX_INT_FREQ = 10.7f;
const float MIN_INT_FREQ = -10.7f;

const int SEP_COUNT = 60;
//----------------------------------------------------------------

//----------------------------------------------------------------
// Default initial frequency
//----------------------------------------------------------------
float Freq_MHz = 98.f; // 98.0 MHz
//----------------------------------------------------------------

//----------------------------------------------------------------
// Intermediate frequency for FM receiver +10.7MHz
//----------------------------------------------------------------
float IntFreq_MHz = 10.7f; //  0.0 MHz
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

  int32_t IF_KHz = (int32_t)IntFreq_MHz * 1000.f;
  uint32_t Freq_KHz = (uint32_t)Freq_MHz * 1000.f;

  pll.setFreqShift(/* Intermediate frequency in KHz */ IF_KHz);
  pll.setFrequency(/* Frequency in KHz */ Freq_KHz, /* Phase detector current */ SAA1057_RX_FAST_TUNE);
  delay(50); // time to tune in
  pll.setFrequency(/* Frequency in KHz */ Freq_KHz, /* Phase detector current */ SAA1057_RX_SLOW_TUNE);
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
  msg += "SAA1057 PLL\n";
  msg += "Select an option:\n\n";
  msg += String(INDEX_FREQ) + ". " + String(MENU_TEXT_FREQ) + " = " + String(Freq_MHz, 2)  + "MHz\n";
  msg += String(INDEX_INT_FREQ) + ". " + String(MENU_TEXT_INT_FREQ) + " = " + String(IntFreq_MHz, 2)  + "MHz\n";
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
      S += String(number, 2) + unit;
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
      changeParam(lastS, MENU_TEXT_FREQ, INDEX_FREQ, text, Freq_MHz, MIN_FREQ, MAX_FREQ, "MHz");
    }
    else if (S.equalsIgnoreCase(INDEX_INT_FREQ)) {
      changeParam(lastS, MENU_TEXT_INT_FREQ, INDEX_INT_FREQ, text, IntFreq_MHz, MIN_INT_FREQ, MAX_INT_FREQ, "MHz");
    }
    else {
      Serial.println(getCmds());
    }
  }
}
