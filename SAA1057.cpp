/*
  Lib: PLL SAA1057
  Version: 1.0.6
  Date: 2024/05/28
  Author: Junon M
*/

#include "SAA1057.h"



SAA1057::SAA1057() {
  WordB = 0;
  WordA = 0;
  _Freq_Shift = 0; 
}



SAA1057::~SAA1057() {

  digitalWrite(_dlen_pin, LOW);
  digitalWrite(_clock_pin, LOW);
  digitalWrite(_data_pin, LOW);

  pinMode(_clock_pin, INPUT);
  pinMode(_data_pin, INPUT);
  pinMode(_dlen_pin, INPUT);
}




void SAA1057::begin(const uint8_t clock_pin, const uint8_t data_pin, const uint8_t dlen_pin) {

  _clock_pin  = clock_pin;
  _data_pin  = data_pin;
  _dlen_pin = dlen_pin;

  pinMode(_clock_pin, OUTPUT);
  pinMode(_data_pin, OUTPUT);
  pinMode(_dlen_pin, OUTPUT);

  digitalWrite(_dlen_pin, LOW);
  digitalWrite(_clock_pin, LOW);
  digitalWrite(_data_pin, LOW);
}




void SAA1057::setFreqShift(float MHz)
{
  _Freq_Shift = MHz;
}



void SAA1057::set(uint16_t Data, uint16_t Shl)
{
  WordB |= (Data << Shl);
}



void SAA1057::clear(uint16_t Data, uint16_t Shl)
{
  WordB &= ~(Data << Shl);
}



void SAA1057::bitDelay()
{
  delay(1);
}




void SAA1057::sendBit(uint16_t b)
{
  if (b != 0)
  {
    digitalWrite(_data_pin, HIGH);
    SAA1057::bitDelay();
    digitalWrite(_clock_pin, HIGH);
    SAA1057::bitDelay();
    digitalWrite(_clock_pin, LOW);
    SAA1057::bitDelay();
    digitalWrite(_data_pin, LOW);
  }
  else
  {
    digitalWrite(_data_pin, LOW);
    digitalWrite(_clock_pin, HIGH);
    SAA1057::bitDelay();
    digitalWrite(_clock_pin, LOW);
  }
  SAA1057::bitDelay();
}



void SAA1057::sendConfig(uint16_t Word)
{
  uint16_t Word_tmp = Word;

  digitalWrite(_dlen_pin, HIGH);

  SAA1057::bitDelay();

  SAA1057::sendBit(0);

  for(int i = 0; i < 16; i++)
  {
    SAA1057::sendBit(Word_tmp & 0b1000000000000000);
    Word_tmp <<= 1;
  }

  digitalWrite(_dlen_pin, LOW);
  SAA1057::bitDelay();

  digitalWrite(_clock_pin, HIGH);
  SAA1057::bitDelay();

  digitalWrite(_clock_pin, LOW);
  SAA1057::bitDelay();
  SAA1057::bitDelay();
  SAA1057::bitDelay();
}





void SAA1057::setDefaultConfig()
{
   // Para CP, PDM e T usar CLEAR

   SAA1057::clear(0xFFFF, 0);        
   SAA1057::set(T_IN_LOCK_CNT, T_SHL);    // Saida Pino de teste = Contador in-lock
   SAA1057::set(BRM, BRM_SHL);  // Corrente no latch reduzida automaticamente
   SAA1057::clear(PDM_CLEAR, PDM_SHL);  // Modo do detector de fase = Automático on/off
   SAA1057::clear(SLA, SLA_SHL);  // Modo de carregamento do LatchA = Assíncrono
   SAA1057::set(SB2, SB2_SHL);  // Habilita os últimos 8 bits da wordB SLA até T0
   SAA1057::set(CP_007, CP_SHL);   // Corrente detector fase = 0,07mA
   SAA1057::clear(REFH, REFH_SHL); // Ref = 1KHz
   SAA1057::set(FM, FM_SHL);   // Modo FM
   SAA1057::set(WORDB, WORDB_SHL);  // Sinaliza WordB

   WordA = 10000; // 100.0MHz

   SAA1057::commitConfig();
}



void SAA1057::setFrequency(float Frequency, uint16_t Speed)
{
  float freq;
  
  freq = Frequency;
  freq += _Freq_Shift;
  WordA = round(freq * 100);

  SAA1057::clear(CP_CLEAR, CP_SHL);
  SAA1057::set(Speed, CP_SHL);

  SAA1057::commitConfig();
}




void SAA1057::commitConfig()
{
  SAA1057::sendConfig(WordB);
  SAA1057::sendConfig(WordA);
}
