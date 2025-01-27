/*
  Lib: PLL SAA1057
  Version: 1.0.8
  Date: 2025/01/13
  Author: Junon M
  Hardware: Arduino Uno or Nano controlled by serial port
*/

#include "SAA1057.h"

const char * VERSION = "1.0.8";

//----------------------------------------------------------------
// Configuração do menu serial
//----------------------------------------------------------------
const char MENU_TEXT_FREQ[] = "Frequency";
const char INDEX_FREQ[] = "1";
const float MAX_FREQ = 130.f;
const float MIN_FREQ = 50.f;

const char MENU_TEXT_INT_FREQ[] = "Intermediate Frequency";
const char INDEX_INT_FREQ[] = "2";
const float MAX_INT_FREQ = 10.7f;
const float MIN_INT_FREQ = -10.7f;

const int SEP_COUNT = 60;
//----------------------------------------------------------------

//----------------------------------------------------------------
// Frequência inicial padrão
//----------------------------------------------------------------
float Freq = 94.1f; // 100.0 MHz
//----------------------------------------------------------------

//----------------------------------------------------------------
// Frequência intermediária para receptor fm +10.7MHz
//----------------------------------------------------------------
float IntFreq = 10.7f; //  0.0 MHz
//----------------------------------------------------------------

//----------------------------------------------------------------
// Corrente dimensionada para sintonia rápida em receptores fm
//----------------------------------------------------------------
const uint16_t FAST_TUNE = CP_023;
//----------------------------------------------------------------

//----------------------------------------------------------------
// Corrente dimensionada para sintonia lenta em receptores fm
//----------------------------------------------------------------
const uint16_t SLOW_TUNE = CP_007;
//----------------------------------------------------------------

//----------------------------------------------------------------
// SAA1057 pins to Arduino
//----------------------------------------------------------------
const int SAA_CLOCK_PIN = 10;
const int SAA_DATA_PIN  = 11;
const int SAA_DLEN_PIN  = 12;
//----------------------------------------------------------------


SAA1057 pll; // Declaração do objeto pll


void setup() {

  Serial.begin(115200);
   
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
  
  commitConfig();
}


void loop() {
  commandInterpreter();
}


void commitConfig()
{
  pll.setFreqShift(/* Frequência Intermediária em MHz */ IntFreq);

  pll.setFrequency(/* Frequência em MHz */ Freq, /* Corrente no detector de fase */ FAST_TUNE);
  delay(50); // tempo para sintonizar
  pll.setFrequency(/* Frequência em MHz */ Freq, /* Corrente no detector de fase */ SLOW_TUNE);
}


String Separator(int len)
{
  String s = "\n";
  for (int i = 0; i < len; i++) s += "-";
  s += "\n";
  return s; 
}



String getCommands() 
{
  String msg = "";
  msg += Separator(SEP_COUNT);
  msg += "PLL SAA1057\nVersion: " + String(VERSION) + "\n\n";
  msg += "Escolha uma opção:\n\n";
  msg += String(INDEX_FREQ) + ". " + String(MENU_TEXT_FREQ) + " = " + String(Freq, 2)  + "MHz\n";
  msg += String(INDEX_INT_FREQ) + ". " + String(MENU_TEXT_INT_FREQ) + " = " + String(IntFreq, 2)  + "MHz\n";
  msg += Separator(SEP_COUNT);
  return msg;
}



void changeParam(String &returned_text, const String menu_label, const String menu_index, String text, float &value, const float min_value, const float max_value,  const String unit)
{
  static int pos = 0;
  String S = "";

  if (pos == 0)
  {
    returned_text = menu_index;
    S += Separator(SEP_COUNT);
    S += "Digite o valor para " + menu_label + " entre " + String(min_value, 2) + unit + " e " + String(max_value, 2) + unit;
    Serial.println(S);
    pos++;
  }
  else
  {
    float number = text.toFloat();

    if ((number >= min_value) && (number <= max_value))
    {
      value = number;
      commitConfig();
      pos = 0;
      returned_text = "";
      S += "Resposta = " + String(number, 2) + unit;
      S += getCommands();
      Serial.println(S);
    }
    else
    {
      S = "Erro, o valor " + String(number, 2) + unit + " está fora do intervalo, tente novamente!";
      Serial.println(S);
    }
  }

}




void commandInterpreter()
{
  static String lastS = "";
  String S = "";

  if (Serial.available())
  {
    String text = Serial.readStringUntil('\n');

    S = text;

    if (!lastS.equals("")) S = lastS;

    if (S.equalsIgnoreCase(INDEX_FREQ))
    {
      changeParam(lastS, MENU_TEXT_FREQ, INDEX_FREQ, text, Freq, MIN_FREQ, MAX_FREQ, "MHz");
    }
    else if (S.equalsIgnoreCase(INDEX_INT_FREQ))
    {
      changeParam(lastS, MENU_TEXT_INT_FREQ, INDEX_INT_FREQ, text, IntFreq, MIN_INT_FREQ, MAX_INT_FREQ, "MHz");
    }
    else
    {
      Serial.println(getCommands());
    }
  }
}
