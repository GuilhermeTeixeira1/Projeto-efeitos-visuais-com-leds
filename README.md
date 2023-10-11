# Projeto efeitos visuais com leds
 4 tipos de efeitos visuais um seguido do outro. Programado em C na platforma Attolic TrueStudio usando o microcontrolador STM32 nucleo 446RE
 Obs: as configurações de entradas, saida, timers e etc estarão no final deste arquivo

 QUAL A LOGICA DO PROJETO?
o projeto foi divido entre 4 efeitos diferentes com a seguinte ordem: efeito 2 -> efeito 1 -> efeito 1 invertido -> efeito 3. O projeto possui 4 leds ao todo sendo dividos em 2 duplas diferentes cada um, sendo denominados "dupla1" e "dupla2"

COMO FUNCIONA A LINHA DE RACICIONIO DO CODIGO?
- main:
  dentro da main esta as funções que chamam para cada evento, as variaveis de controle de cada evento e variaveis que manipulam o efeito conforme o tempo. Por exemplo o efeito 2, ele é o primeiro a ser executado, entrando em um laço for, a função "efeito2" possui como argumento a temporização do timer10 (arr) que decresce cada vez que a função é executada novamente parando somente quando os leds ficam ligando 100% do tempo.
dentro do "efeito2", ele recebe o periodo do timer, e faz as duplas piscarem, conforme o tempo passa, os leds piscam cada vez mais rapido justamente porque o laço for dentro da main esta sendo executado varias vezes.

- efeito 2
 dentro do "efeito2", ele recebe o periodo do timer, põe as 2 duplas nas posições certas para piscar (dupla2 ligado e dupla1 desligado), depois entra em um loop while que só acaba quando o periodo chegar na metade (flag estourando), fazendo com que troquem de estados os leds. Logo após, entra em um outro loop onde desliga as 2 duplas pelo resto da metade do periodo (flag estourando), fechando assim, um periodo completo.
 
- efeito 1:
  esse efeito é um pouco mais complexo, a dupla 1 ira piscar inicialmente 2x a 2 Hz, porém a cada vez que é executado a função "efeito1" o numero de piscadas aumenta em 2 unidades até o limite de 10x. A dupla 2 piscam apenas 1x sempre a 5 Hz.
a função esta dentro de um laço for que a cada vez que executa, divide pela metade o periodo (arr) e aumenta em 2 o numero de piscadas da dupla 1. A função "efeito1" recebe como parametros o numero de piscadas da dupla 1, da dupla 2 e o temporizador, respectivamente.

- efeito 1 invertido:
  funciona do mesmo jeito, porém, agora, aumenta o numero de piscadas conforme o tempo é a dupla 2, e a 1 fica fixa. A função "efeito1 invertido" recebe os mesmo parametros na mesma ordem que o efeito 1 original.

- efeito 3:
  a ideia dele é fazer todas duplas piscarem 3x a 2,5 Hz, depois piscarem a freneticamente a 16 Hz por 0,5s. A função "efeito3" esta dentro de um laço for que executa 3x a função. Ela recebe como parametro o temporizador 1 e o temporizador 2 porque dentro da função usei 2 timers difenretes (timer 10 e 11), um responvel para controlar a frequencia de 2,5 Hz (timer10) e o outro para contar 0,5s.
  
Dentro do "efeito3", eu coloco as duplas em estados diferentes para poder começar a piscar, depois, entro em um loop while que so para quando os leds piscarem 2x (cont != 4), na pratica eles piscam 3x porque inicialmente as duplas ja estão nas posições corretas, o que faz com que tenham que fazer metade trabalho para piscar, por isso cont != 4 e nao != 6. Dentro desse loop tem mais 2 loops while, um encarregado de trocar os estados e o outro loop para desligar todos.
  obs: as piscadas tem que estarem dentro dos loops porque o processador do microprocessador é muito mais veloz que o tempo do timer, entao quando ele entra no loop, ele fica la até estourar o tempo do timer (estourar o flag).
por último, saimos do loop anterior e entramos no último que consiste um while dentro do outro, o loop maior so expira quando o timer de 0,5s acaba (timer11), o outro loop executa as piscadas a 16 Hz (a cada 100ms) (timer10).
