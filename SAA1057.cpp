/*
 * Project: SAA1057 Arduino Library
 * File: SAA1057.cpp
 * Version: v1.0.0.17
 * Date: 2026/03/29
 * License: GNU GPL v3.0
 * Developed by: Junon M. (2008-2026)
 * Description: Embedded systems and RF signal processing.
 */

#include "SAA1057.h"


SAA1057::SAA1057() {
  // Default config for dip switch pins (0 to 7)
  for (int i = 0; i < SW_COUNT; i++) _sw_pins[i] = i;
  _dip_sw_value = 0;
  WordB.ADDR = ADDR_WORDB;
  WordB.FM = MODE_FM;
  WordB.REF = REF_1KHZ;
  WordB.CP = CP_0_07MA;
  WordB.SB2 = SB2_ON;
  WordB.SLA = SLA_ASYNC;
  WordB.PDM = PDM_AUTO;
  WordB.BRM = BRM_ECONOMY;
  WordB.T = T_LOCK_DET;
  WordA.ADDR = ADDR_WORDA;
  WordA.N = 9800; // 98000 KHz
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


void SAA1057::setDipSwitchPins(const uint8_t b7, const uint8_t b6, const uint8_t b5, const uint8_t b4,
                             const uint8_t b3, const uint8_t b2, const uint8_t b1, const uint8_t b0) {
  _sw_pins[7] = b7;
  _sw_pins[6] = b6;
  _sw_pins[5] = b5;
  _sw_pins[4] = b4;
  _sw_pins[3] = b3;
  _sw_pins[2] = b2;
  _sw_pins[1] = b1;
  _sw_pins[0] = b0;
  for (int i = 0; i < SW_COUNT; i++) pinMode(_sw_pins[i], INPUT_PULLUP);
}


void SAA1057::setFreqShift(int32_t KHz) {
  _Freq_Shift = KHz;
}


void SAA1057::bitDelay() {
  delay(1);
}


void SAA1057::sendBit(uint16_t b) {
  if (b) {
    digitalWrite(_data_pin, HIGH);
    SAA1057::bitDelay();
    digitalWrite(_clock_pin, HIGH);
    SAA1057::bitDelay();
    digitalWrite(_clock_pin, LOW);
    SAA1057::bitDelay();
    digitalWrite(_data_pin, LOW);
  } else {
    digitalWrite(_data_pin, LOW);
    digitalWrite(_clock_pin, HIGH);
    SAA1057::bitDelay();
    digitalWrite(_clock_pin, LOW);
  }
  SAA1057::bitDelay();
}



void SAA1057::sendConfig(uint16_t Word) {
  uint16_t Word_tmp = Word;
  digitalWrite(_dlen_pin, HIGH);
  SAA1057::bitDelay();
  SAA1057::sendBit(0);
  for (int i = 0; i < 16; i++) {
    // Test bit 15
    SAA1057::sendBit(Word_tmp & 0x8000);
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


void SAA1057::set(uint16_t Word) {
  if (Word & 0x8000) WordB.raw = Word;
  else WordA.raw = Word;
}


void SAA1057::setFrequency(uint32_t KHz, uint16_t Speed) {
  uint32_t freq;
  freq = KHz;
  freq += _Freq_Shift;
  WordA.N = freq / 10;
  WordA.ADDR = ADDR_WORDA;
  WordB.CP = Speed;
  SAA1057::commitConfig();
}


void SAA1057::setFrequencyFromDipSwitch(uint16_t Speed) {
  for (int i = 0; i < SW_COUNT; i++) {
    _dip_sw_value = bitWrite(_dip_sw_value, i, !digitalRead(_sw_pins[i]));
  }
  
  // MHz
  // freq = 1080 - 255
  // freq = 825
  // freq /= 10;
  // freq = 82.5 MHz
  
  // KHz
  // freq = 1080 - 255
  // freq = 825
  // freq *= 100;
  // freq = 82500 KHz
  
  uint32_t freq = FACTOR - _dip_sw_value;
  freq *= 100; // = 82500 KHz;
  SAA1057::setFrequency(freq, Speed);
}


void SAA1057::commitConfig() {
  SAA1057::sendConfig(WordB.raw);
  SAA1057::sendConfig(WordA.raw);
}
