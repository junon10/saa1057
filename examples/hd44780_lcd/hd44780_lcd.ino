/*
 Project: SAA1057 Arduino Library
 File: hd44780_lcd.ino
 Version: 1.0.0.20
 Date: 2026/04/12
 License: GNU GPL v3.0
 Developed by: Junon M. (2008-2026)
 Description: Embedded systems and RF signal processing.
*/

#include <LiquidCrystal.h>
#include "SAA1057.h"

const uint32_t Frequency = 100000;  // 100000 KHz
const int32_t IntFreq = 0;          //      0 KHz

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
  lcd.print(Frequency, 0);
  lcd.print("KHz");
}



void loop() { 
}
