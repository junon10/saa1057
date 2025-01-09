<h2>SAA1057 PLL Controller Library</h2>

<h2>Descrição</h2>

<p>Biblioteca Arduino para controlar o circuito integrado PLL SAA1057.<br>
Fornece funções para configurar e operar o dispositivo.</p>

<h2>Requisitos</h2>

<p>- Arduino IDE<br>
- Qualquer placa Arduino é compatível com essa biblioteca.<br>
- Circuito integrado SAA1057.</p>

<h2>Instalação</h2>

<p>
1. Baixe a biblioteca.<br>
2. Instale via Arduino IDE: Sketch > Incluir Biblioteca > Adicionar Biblioteca...</p>

<h2>Uso</h2>

<p>Exemplo</p>

<p>#include <SAA1057.h></p>

<p>SAA1057 pll;</p>

<p>Métodos e Configurações</p>

<p>
Define os 3 pinos de conexão do Arduino com o PLL<br>
pll.begin(clock_pin, data_pin, dlen_pin);</p>
        
<p>
1. pll.setFreqShift(10.7); // = Receptor FM (F + 10,7MHz)<br>
2. pll.setFreqShift(0); // = Transmissor FM (F + 0)<br>
3. pll.setFreqShift(-10.7); // = Receptor FM (F - 10,7MHz)<br>
pll.setFreqShift(float MHz);</p>

<p>
Força os bits da variável Data para 1,<br> 
escrevendo a constante na WordB,<br>
iniciando no número de bit determinado por Shl,<br>
que significa ShiftLeft (deslocamento para a<br> 
esquerda), conforme as constantes no arquivo .h<br>
pll.set(uint16_t Data, uint16_t Shl);</p>

<p>
Força os bits da variável Data para zero,<br>
escrevendo os bits zero na WordB<br> 
iniciando no número de bit determinado por Shl,<br>
que significa ShiftLeft (deslocamento para a<br> 
esquerda), conforme as constantes no arquivo .h<br>
pll.clear(uint16_t Data, uint16_t Shl);</p>
    
<p>
Teste do PLL SAA1057 em 100MHz<br>
pll.setDefaultConfig();</p>
    
<p>
Define a frequência de operação e a velocidade<br> 
de travamento do pll (corrente no Loop Filter)<br>
pll.setFrequency(float Frequency, uint16_t Speed);</p>

<p>
Efetiva as mudanças de configuração.<br>
pll.commitConfig();</p>

<p>
Configuração de pinagem da DipSwitch<br>
pll.setDipSwPinout(b7, b6, b5, b4, b3, b2, b1, b0);</p>

<p>
Baseada na DipSwitch define a frequência<br>
pll.setFrequencyByDipSW(Speed);</p>

<h2>Limitações</h2>

<p>- Consulte o datasheet do SAA1057 para detalhes técnicos.</p>

<h2>Autor</h2>
<p>Junon M.</p>

<h2>Repositório</h2>
<p>https://github.com/junon10/saa1057</p>

<h2>Contribuição</h2>

<p>Contribuições são bem-vindas!<br>
Envie um pull request.</p>

<h2>Licença</h2>

<p>GPL-3.0</p>

<h2>Referências</h2>

<p>- Datasheet SAA1057<br>
- Documentação Arduino</p>

<h2>Changelog</h2>

<p>-1.0.6 - Commit inicial.</p>

<p>-1.0.7 - Definição de frequência baseada numa DipSwitch já<br> 
integrada à biblioteca.</p>
