/*
  Lib: SAA1057 PLL
  Version: 1.0.0.9
  Date: 2026/02/14
  Author: Junon M
  Hardware: Arduino Uno and Display module
*/

#include <LiquidCrystal.h>
#include "SAA1057.h"

const float Frequency = 100.0f;            // 100.0 MHz
const float IntFreq = 0.0f;          //  0.0 MHz

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
  pll.setFrequency(/* Frequency in MHz */ Frequency, /* Phase detector current */ CP_07);
  delay(2000);
  pll.setFrequency(/* Frequency in MHz */ Frequency, /* Phase detector current */ CP_007);

  lcd.setCursor(0,0);
  lcd.print("SAA1057 PLL");
  
  lcd.setCursor(0,1);
  lcd.print("Freq: ");
  lcd.print(Frequency, 1);
  lcd.print("MHz");
}



void loop() { 
}
