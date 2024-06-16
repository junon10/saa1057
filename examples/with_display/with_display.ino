/*
  Lib: PLL SAA1057
  Version: 1.0.6
  Date: 2024/05/28
  Author: Junon M
  Hardware: Arduino Uno and Display module with keypad
*/

#include <LiquidCrystal.h>
#include "SAA1057.h"

const float Frequency = 100.0f;            // 100.0 MHz
const float IF_Frequency = 0.0f;          //  0.0 MHz

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
  Serial.println("PLL SAA1057");
  Serial.println();

  lcd.begin(16, 2);
   
  // Arduino pins
  pll.begin(SAA_CLOCK_PIN, SAA_DATA_PIN, SAA_DLEN_PIN);

  pll.clear(0xFFFF, 0);        
  pll.set(T_IN_LOCK_CNT, T_SHL);    // Saida Pino de teste = Contador in-lock
  pll.set(BRM, BRM_SHL);  // Corrente no latch reduzida automaticamente
  pll.clear(PDM_CLEAR, PDM_SHL);  // Modo do detector de fase = Automático on/off
  pll.clear(SLA, SLA_SHL);  // Modo de carregamento do LatchA = Assíncrono
  pll.set(SB2, SB2_SHL);  // Habilita os últimos 8 bits da wordB SLA até T0
  pll.set(CP_07, CP_SHL);   // Corrente detector fase = 0,07mA
  pll.clear(REFH, REFH_SHL); // Ref = 1KHz
  pll.set(FM, FM_SHL);   // Modo FM
  pll.set(WORDB, WORDB_SHL);  // Sinaliza WordB
  
  pll.setFreqShift(/* Frequência Intermediária em MHz */ IF_Frequency);

  // Correntes dimensionadas para transmissores fm
  //
  pll.setFrequency(/* Frequência em MHz */ Frequency, /* Corrente no detector de fase */ CP_07);
  delay(2000);
  pll.setFrequency(/* Frequência em MHz */ Frequency, /* Corrente no detector de fase */ CP_007);

  lcd.setCursor(0,0);
  lcd.print("PLL SAA1057");
  
  lcd.setCursor(0,1);
  lcd.print("Freq: ");
  lcd.print(Frequency, 1);
  lcd.print("MHz");
}



void loop() { 
}
