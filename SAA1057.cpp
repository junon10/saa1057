/*
  Lib: PLL SAA1057
  Version: 1.0.8
  Date: 2025/01/13
  Author: Junon M
  License: GPLv3
*/

#include "SAA1057.h"



SAA1057::SAA1057() {

  // Default config for dip switch pins (0 to 7)
  for (int i = 0; i < SW_NUM_BITS; i++) _sw_pins[i] = i;

  _dip_sw_value = 0;

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



void SAA1057::setDipSwPinout(const uint8_t b7, const uint8_t b6, const uint8_t b5, const uint8_t b4, 
const uint8_t b3, const uint8_t b2, const uint8_t b1, const uint8_t b0) 
{
_sw_pins[7] = b7;
_sw_pins[6] = b6;
_sw_pins[5] = b5;
_sw_pins[4] = b4;
_sw_pins[3] = b3;
_sw_pins[2] = b2;
_sw_pins[1] = b1;
_sw_pins[0] = b0;

for (int i = 0; i < SW_NUM_BITS; i++) pinMode(_sw_pins[i], INPUT_PULLUP);

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



void SAA1057::setFrequency(float MHz, uint16_t Speed)
{
  float freq;
  
  freq = MHz;
  freq += _Freq_Shift;
  WordA = round(freq * 100);

  SAA1057::clear(CP_CLEAR, CP_SHL);
  SAA1057::set(Speed, CP_SHL);

  SAA1057::commitConfig();
}


void SAA1057::setFrequencyByDipSw(uint16_t Speed)
{
  for (int i = 0; i < SW_NUM_BITS; i++)
  {
    _dip_sw_value = bitWrite(_dip_sw_value, i, !digitalRead(_sw_pins[i])); 
  }

  // freq = 1080 - 255; // = 825
  float freq = FACTOR - _dip_sw_value;
  freq /= 10; // = 82,5 MHz;
  SAA1057::setFrequency(freq, Speed);
}


void SAA1057::commitConfig()
{
  SAA1057::sendConfig(WordB);
  SAA1057::sendConfig(WordA);
}
