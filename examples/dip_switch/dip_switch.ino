/*
  Lib: PLL SAA1057
  Version: 1.0.8
  Date: 2025/01/13
  Author: Junon M
  Hardware: Arduino Uno or Nano with DipSwitch
*/

#include "SAA1057.h"

//---------------------------------------------------
// FI adjust (intermediate frequency adjust)
//---------------------------------------------------
// 0.0 MHz, 10.7 MHz, -10.7 MHz
const float IF_Frequency = 0.0f; 
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
  Serial.println("PLL SAA1057");
  Serial.println();

  // DipSwitch Arduino pinout 
  pll.setDipSwPinout(7,6,5,4,3,2,1,0);
   
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
  pll.setFrequencyByDipSw(/* Corrente no detector de fase */ CP_07);
  delay(2000);
  pll.setFrequencyByDipSw(/* Corrente no detector de fase */ CP_007);
}


void loop() {
}
