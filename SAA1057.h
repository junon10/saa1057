/*
  Lib: SAA1057 PLL
  Version: 1.0.0.9
  Date: 2026/02/14
  Author: Junon M
  License: GPLv3
*/

#ifndef SAA1057_h
#define SAA1057_h

#include <Arduino.h>

//==============================================================================
//                             WordB TABLE
//==============================================================================
//  15 14   13  12  11  10   9   8        7    6    5   4  3  2  1  0
//  1  FM REFH CP3 CP2 CP1 CP0 SB2      SLA PDM1 PDM0 BRM T3 T2 T1 T0

#define  WORDB               1
#define  WORDB_SHL          15

//  FM = FM MODE
// ~FM = AM MODE
#define  FM                  1  
#define  FM_SHL             14  

//  REFH = REF 1KHz 
// ~REFH = REF 1,25KHz
#define  REFH                1
#define  REFH_SHL           13

// Example
// High speed fm transmitter = 0,7mA
// Slow speed fm transmitter = 0,07mA
// Default speed fm receiver = 0,07mA

// PHASE DETECTOR CURRENT 
#define  CP_CLEAR       0b1111 // 0,023mA
#define  CP_007         0b0001 // 0,07mA   
#define  CP_023         0b0010 // 0,23mA   
#define  CP_07          0b0110 // 0,7mA   
#define  CP_23          0b1110 // 2,3mA   
#define  CP_SHL              9

// enables last 8 bits (SLA to T0) of data word B; ‘1’ = enables, ‘0’ = disables; 
// when programmed ‘0’, the last 8 bits of data word B will be set to ‘0’ automatically
#define  SB2                 1
#define  SB2_SHL             8 


//  load mode of latch A; ‘1’ = synchronous, ‘0’ = asynchronous 
#define  SLA                 1 
#define  SLA_SHL             7


// phase detector mode
// ~PDM = Automatic on/off
//  PDM = 0b10 = ON
//  PDM = 0b11 = OFF
#define  PDM_CLEAR        0b11
#define  PDM_ON           0b10
#define  PDM_OFF          0b11
#define  PDM_SHL             5

// bus receiver mode bit; in this mode the supply 
// current of the BUS receiver will be switched-off
// automatically after a data transmission (current-draw is reduced); 
// ‘1’ = current switched; 
// ‘0’ = current always on
#define  BRM                 1
#define  BRM_SHL             4      

// Test pin output
#define  T_CLEAR        0b1111 // 1
#define  T_FREQ_REF     0b0100 // reference frequency
#define  T_DIV_PRG      0b0001 // output programmable counter
#define  T_IN_LOCK_CNT  0b0101 // output in-lock counter, ‘0’ = out-lock, ‘1’ = in-lock
#define  T_SHL               0

// NOTE: The test output pin must have a pull-up resistor.
//==============================================================================

// Maximum dip switch calculation frequency
#define FACTOR        1080

// Number of frequency setting switches
#define SW_COUNT      8


class SAA1057
{
  private:
    uint16_t WordA, WordB;

    uint8_t _clock_pin, _data_pin, _dlen_pin;

    uint8_t _dip_sw_value;

    uint8_t _sw_pins[8]; 
    
    float _Freq_Shift;
    
    void bitDelay();
    
    void sendBit(uint16_t b);

    void sendConfig(uint16_t Word);

  public:

    SAA1057();

    virtual ~SAA1057();
    
    void begin(const uint8_t clock_pin, const uint8_t data_pin, const uint8_t dlen_pin);
        
    // Freq_Shift = 10.7    // FM Receiver(F + 10,7MHz)
    // Freq_Shift = 0       // FM Transmitter FM
    // Freq_Shift = -10.7   // FM Receiver(F - 10,7MHz)
    //
    void setFreqShift(float MHz);

    void setDipSwPinout(const uint8_t b7, const uint8_t b6, const uint8_t b5, const uint8_t b4, 
    const uint8_t b3, const uint8_t b2, const uint8_t b1, const uint8_t b0);

    void set(uint16_t Data, uint16_t Shl);

    void clear(uint16_t Data, uint16_t Shl);
    
    // SAA1057 PLL test in 100MHz
    //
    void setDefaultConfig();
    
    void setFrequency(float MHz, uint16_t Speed);

    void setFrequencyByDipSw(uint16_t Speed);

    void commitConfig();
};

#endif /* SAA1057_h */
