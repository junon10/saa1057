/*
  Lib: SAA1057 PLL
  Version: 1.0.0.15
  Date: 2026/03/20
  Author: Junon M
  Hardware: Arduino Uno or Nano, and Display module
*/

#include <LiquidCrystal.h>
#include "SAA1057.h"

const float Frequency = 100.0f;      // 100.0 MHz
const float IntFreq = 0.0f;          //   0.0 MHz

// SAA1057 pins to Arduino
const int SAA_CLOCK_PIN = 10;
const int SAA_DATA_PIN  = 11;
const int SAA_DLEN_PIN  = 12;

//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7;

saa1057_wordB WordB;

SAA1057 pll;

LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);


void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("SAA1057 PLL");
  Serial.println();

  lcd.begin(16, 2);
   
  // Arduino pins
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
  
  pll.setFreqShift(/* intermediate frequency in MHz */ IntFreq);

  pll.setFrequency(/* Frequency in MHz */ Frequency, /* Phase detector current */ SAA1057_TX_FAST_TUNE);
  delay(2000);
  pll.setFrequency(/* Frequency in MHz */ Frequency, /* Phase detector current */ SAA1057_TX_SLOW_TUNE);

  lcd.setCursor(0,0);
  lcd.print("SAA1057 PLL");
  
  lcd.setCursor(0,1);
  lcd.print("Freq: ");
  lcd.print(Frequency, 1);
  lcd.print("MHz");
}



void loop() { 
}
