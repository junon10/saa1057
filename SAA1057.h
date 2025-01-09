/*
  Lib: PLL SAA1057
  Version: 1.0.7
  Date: 2025/01/09
  Author: Junon M
*/

#ifndef SAA1057_h
#define SAA1057_h

#include <Arduino.h>

//==============================================================================
//                              Constantes
//==============================================================================

//==============================================================================
//                Tabela WordB de configurações possíveis
//==============================================================================
//  15 14   13  12  11  10   9   8        7    6    5   4  3  2  1  0
//  1  FM REFH CP3 CP2 CP1 CP0 SB2      SLA PDM1 PDM0 BRM T3 T2 T1 T0

#define  WORDB               1
#define  WORDB_SHL          15

//  FM = MODO FM
// ~FM = MODO AM
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
#define  CP_CLEAR       0b1111 // Corrente detector fase = 0,023mA
#define  CP_007         0b0001 // Corrente detector fase = 0,07mA   
#define  CP_023         0b0010 // Corrente detector fase = 0,23mA   
#define  CP_07          0b0110 // Corrente detector fase = 0,7mA   
#define  CP_23          0b1110 // Corrente detector fase = 2,3mA   
#define  CP_SHL              9

// ~SB2 = Zera os ultimos 8 bits da wordB SLA até T0
//  SB2 = Habilita os últimos 8 bits da wordB SLA até T0
#define  SB2                 1
#define  SB2_SHL             8 


//  Modo de carregamento do LatchA 
// ~SLA = Assíncrono
//  SLA = Síncrono
#define  SLA                 1 
#define  SLA_SHL             7


// Modo do detector de fase
// ~PDM = Automático on/off
//  PDM = 0b10 = ON
//  PDM = 0b11 = OFF
#define  PDM_CLEAR        0b11
#define  PDM_ON           0b10
#define  PDM_OFF          0b11
#define  PDM_SHL             5

// ~BRM = Corrente sempre ligada
//  BRM = Corrente no latch reduzida automaticamente
#define  BRM                 1
#define  BRM_SHL             4      

#define  T_CLEAR        0b1111 // Saida Pino de teste = 1
#define  T_FREQ_REF     0b0100 // Saida Pino de teste = Frequência de referência
#define  T_DIV_PRG      0b0001 // Saida Pino de teste = Saída do divisor progr.
#define  T_IN_LOCK_CNT  0b0101 // Saida Pino de teste = Contador in-lock
#define  T_SHL               0

//OBS: O pino de saída de teste deve ter resistor de pull-up.
//Estados do contador in-lock: '0' = out-lock   '1' = in-lock.
//==============================================================================


#define FACTOR        1080
#define SW_NUM_BITS      8


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
        
    // Freq_Shift = 10.7    // Receptor FM (F + 10,7MHz)
    // Freq_Shift = 0       // Transmissor FM
    // Freq_Shift = -10.7   // Receptor FM (F - 10,7MHz)
    //
    void setFreqShift(float MHz);

    void setDipSwPinout(const uint8_t b7, const uint8_t b6, const uint8_t b5, const uint8_t b4, 
    const uint8_t b3, const uint8_t b2, const uint8_t b1, const uint8_t b0);

    void set(uint16_t Data, uint16_t Shl);

    void clear(uint16_t Data, uint16_t Shl);
    
    // PLL teste SAA1057 em 100MHz
    //
    void setDefaultConfig();
    
    void setFrequency(float Frequency, uint16_t Speed);

    void setFrequencyByDipSW(uint16_t Speed);

    void commitConfig();
};

#endif /* SAA1057_h */
