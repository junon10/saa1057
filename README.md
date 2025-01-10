## SAA1057 PLL Controller Library

## Descrição

- Biblioteca Arduino para controlar o circuito integrado PLL SAA1057.<br>
Fornece funções para configurar e operar o dispositivo.<br>

## Requisitos

- Arduino IDE
- Esta biblioteca é compatível com a maioria das placas Arduino.<br>
- Circuito integrado SAA1057.<br>

## Instalação

- Baixe a biblioteca.<br>
- Instale via Arduino IDE: Sketch > Incluir Biblioteca > Adicionar Biblioteca...<br>

## Uso

```c++
#include <SAA1057.h>

/* Declaração do objeto pll */
SAA1057 pll;

/* Define os 3 pinos de conexão do Arduino com o PLL */
pll.begin(clock_pin, data_pin, dlen_pin);
        
/* Receptor FM (F + 10,7MHz)
   Transmissor FM (F + 0)
   Receptor FM (F - 10,7MHz) */
pll.setFreqShift(float MHz);

/* Força os bits da variável (Data) para 1(um), e a envia à WordB, iniciando no bit determinado por Shl = ShiftLeft (deslocamento para a esquerda), conforme as informações do arquivo .h */
pll.set(uint16_t Data, uint16_t Shl);

/* Força os bits da variável (Data) para 0(zero) */
pll.clear(uint16_t Data, uint16_t Shl);
    
/* Teste do PLL SAA1057 em 100MHz */
pll.setDefaultConfig();
    
/* Define a frequência de operação e consequentemente a velocidade de travamento do pll (corrente no Loop Filter).*/
pll.setFrequency(float Frequency, uint16_t Speed);

/* Efetiva as mudanças de configuração. */
pll.commitConfig();

/* Configuração de pinagem da DipSwitch. */
pll.setDipSwPinout(b7, b6, b5, b4, b3, b2, b1, b0);

/* Define a frequência baseada na DipSwitch. */
pll.setFrequencyByDipSW(Speed);</p>
```

## Limitações

- Consulte o datasheet do SAA1057 para detalhes técnicos.<br>

## Autor
- Junon M.

## Repositório
[https://github.com/junon10/saa1057](https://github.com/junon10/saa1057)

## Licença

- GPL-3.0

## Contribuição

- Contribuições são bem-vindas! Envie um pull request.

## Referências

- Datasheet SAA1057<br>
- Documentação Arduino<br>

## Log de Alterações

### 2024/06/15 - v1.0.6  
- Commit inicial.<br>

### 2025/01/09 - v1.0.7  
- Definição de frequência por DipSwitch já integrada nesta biblioteca.<br>
