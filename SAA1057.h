/*
  Lib: SAA1057 PLL
  Version: 1.0.0.10
  Date: 2026/03/14
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

// Register Address / Endereço do Registrador (Bit 15)
#define ADDR_WORDA          0
#define ADDR_WORDB          1

// Operation Mode / Modo de Operação (Bit 14)
#define MODE_AM             0
#define MODE_FM             1

// Reference Frequency / Frequência de Referência (Bit 13)
#define REF_1_25KHZ         0
#define REF_1KHZ            1

// Phase Detector Current / Corrente do Detector de Fase (Bits 9-12)
#define CP_0_023MA          0b0000 
#define CP_0_07MA           0b0001    
#define CP_0_23MA           0b0010    
#define CP_0_7MA            0b0110    
#define CP_2_3MA            0b1110    

// Latch B Control / Controle do Latch B (Bit 8)
#define SB2_OFF             0 // Bits 0-7 set to '0' automatically
#define SB2_ON              1 // Enables last 8 bits

// Latch A Load Mode / Modo de Carga do Latch A (Bit 7)
#define SLA_ASYNC           0 // Asynchronous load
#define SLA_SYNC            1 // Synchronous load

// Phase Detector Mode / Modo do Detector de Fase (Bits 5-6)
#define PDM_AUTO            0b00 // Automatic ON/OFF
#define PDM_ON              0b10 // Always ON
#define PDM_OFF             0b11 // Always OFF (High impedance)

// Bus Receiver Mode / Economia de Energia do Barramento (Bit 4)
#define BRM_ALWAYS_ON       0 // Current always on
#define BRM_ECONOMY         1 // Current switched off after transmission

// Test Pin Output / Saída do Pino de Teste (Bits 0-3)
#define T_OFF               0b0000 
#define T_FREQ_REF          0b0100 // Reference frequency
#define T_DIV_PRG           0b0001 // Programmable counter output
#define T_LOCK_DET          0b0101 // In-lock counter (1=Locked)

// NOTE: The test output pin must have a pull-up resistor.
//==============================================================================

// Maximum dip switch calculation frequency
#define FACTOR        1080

// Number of frequency setting switches
#define SW_COUNT      8


typedef union {
    struct {
        uint16_t N    : 14; // Divisor (Bits 0 to 13) - The core frequency math!
        uint16_t MODE : 1;  // Bit 14: Reserved/Function
        uint16_t ADDR : 1;  // Bit 15: Word Address (WORDA = 0)
    } refined;
    uint16_t raw;
} saa1057_wordA;


typedef union
{
    struct
    {
        // --- Low Byte (Bits 0 to 7) ---
        uint16_t T   : 4; // Test pin output (T0-T3)
        uint16_t BRM : 1; // Bus receiver mode: 1=switched, 0=always on
        uint16_t PDM : 2; // Phase detector mode: 00=auto, 10=ON, 11=OFF
        uint16_t SLA : 1; // Load mode latch A: 1=sync, 0=async

        // --- High Byte (Bits 8 to 15) ---
        uint16_t SB2  : 1; // Enable last 8 bits of Word B: 1=enable
        uint16_t CP   : 4; // Phase detector current (CP0-CP3)
        uint16_t REF : 1; // Reference frequency: 1=1kHz, 0=1.25kHz
        uint16_t FM   : 1; // Operation mode: 1=FM, 0=AM
        uint16_t ADDR : 1; // Register address: 1=Word B, 0=Word A
    } refined;
    uint16_t raw;
} saa1057_wordB;


class SAA1057
{
  private:
    saa1057_wordA WordA;
    saa1057_wordB WordB;

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
    
    void set(uint16_t Word);
    
    void setFrequency(float MHz, uint16_t Speed);

    void setFrequencyByDipSw(uint16_t Speed);

    void commitConfig();
};

#endif /* SAA1057_h */
