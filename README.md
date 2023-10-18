RESUMO DO PROJETO:

 4 tipos de efeitos visuais, um seguido do outro. Programado em C, na platforma Attolic - TrueStudio, usando o microcontrolador STM32 núcleo 446RE
 Obs: as configurações de entradas, saída, timers etc, estarão no final deste arquivo
 Obs 2: na imagem que contém o esquemático tem o microcontrolador com o núcleo diferente do usado, porém funciona do mesmo jeito normalmente

 QUAL A LÓGICA DO PROJETO?
 
o projeto foi dividido entre 4 efeitos diferentes com a seguinte ordem: efeito 2 -> efeito 1 -> efeito 1 invertido -> efeito 3. O projeto possui 4 leds ao todo, sendo dividos em 2 duplas diferentes, sendo denominados "dupla1" e "dupla2". Os efeitos só acontecem se o interruptor no hardware estiver ligado (sw0), caso contrário, todos desligam

COMO FUNCIONA A LINHA DE RACICÍONIO DO CÓDIGO?
- Main:
  Dentro da main, está as funções que chamam para cada efeito, as variáveis de controle de cada evento e variáveis que manipulam o efeito conforme o tempo. Por exemplo, o efeito 2, ele é o primeiro a ser executado, entrando em um laço for. Já função "efeito2" possui como argumento a temporização do timer10 (arr), que decresce cada vez que a função é executada novamente, parando somente quando os leds ligam 100% do tempo.

Dentro do "efeito2", ele recebe o período do timer e faz as duplas piscarem. Conforme o tempo passa, os leds piscam cada vez mais rápido, justamente porque o laço for dentro da main esta sendo executado várias vezes.

- Efeito 2
 Dentro do "efeito2", recebe o período do timer como argumento, põe as 2 duplas nas posições certas para piscar (dupla2 ligado e dupla1 desligado), depois, entra em um loop while que só acaba quando o intervalo chega na metade (flag estourando), fazendo com que troque de estados dos leds. Logo após, entra em um outro loop, onde desliga as 2 duplas pelo resto da metade do período (flag estourando), fechando, assim, um invervalo completo.
 
- Efeito 1:
  Esse efeito é um pouco mais complexo, a dupla 1 irá piscar inicialmente 2x a 2 Hz. Porém, a cada vez que é executada a função "efeito1", o número de piscadas aumenta em 2 unidades até o limite de 10x. A dupla 2 pisca apenas 1x sempre a 5 Hz.
  
A função está dentro de um laço for que, a cada vez que executa, divide pela metade o periodo (arr), aumenta em 2 o número de piscadas da dupla 1. A função "efeito1" recebe como pârametros o número de piscadas da dupla 1, da dupla 2 e o temporizador, respectivamente.

- Efeito 1 invertido:
  Funciona do mesmo jeito, porém, agora, aumenta o número de piscadas da dupla 2 conforme o tempo, a dupla 1 permanece fixa. A função "efeito1 invertido" recebe os mesmos pârametros na mesma ordem que o efeito 1 original.

- Efeito 3:
  A ideia dele é fazer todas duplas piscarem 3x à 2,5 Hz, depois, começam a piscar frenéticamente à 16 Hz por 0,5s. A função "efeito3" está dentro de um laço for que executa 3x a função. Ela recebe como parâmetro o temporizador 1 e o temporizador 2, porque dentro da função, usei 2 timers diferentes (timer 10 e 11), um responsável para controlar a frêquencia de 2,5 Hz (timer10) e o outro para contar 0,5s.
  
Dentro do "efeito3", duplas são postas em estados diferentes para poder começar a piscar, depois, entra em um loop while que só para quando os leds piscarem 2x (cont != 4), na prática, eles piscam 3x, porque inicialmente as duplas já estão nas posições corretas, o que faz com que tenham que leve metade do tempo para piscar, por isso cont != 4 e não != 6. Dentro desse loop, existe mais 2 loops while, um encarregado de trocar os estados dos leds e o outro loop para desligar todos.

  Obs: as piscadas tem que estarem dentro dos loops porque o processador do microprocessador é muito mais veloz que o tempo do timer, então, quando ele entra no loop, ele fica executando o while até estourar o tempo do timer (estourar o flag).
Por último, saí do loop anterior e entra no último laço que consiste em um while dentro do outro, o loop maior só expira quando o timer de 0,5s acaba (timer11), o outro loop executa as piscadas a 16 Hz (a cada 100ms) (timer10).

CONFIGURAÇÃO DAS ENTRADAS, SAIDAS, TIMERS E ETC:

 	// defines:
	#define dupla1 GPIO_ODR_ODR_3
	#define dupla2 GPIO_ODR_ODR_4
	#define tomada GPIO_IDR_IDR_0

	// configurando GPIO
	// habilitando clock para port C e B
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN);

	// entradas;
	// PC0 -> SW0
	GPIOC->MODER &= ~GPIO_MODER_MODER0;

	// saidas:
	// PB3 -> dupla 1 (vm e br), PB4 -> dupla 2 (vd e az)
	GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4);
	GPIOB->MODER |= (GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0);

	// configuando TIM10
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	TIM10->PSC = (1600-1);

	//habilitando o contador do TIIM10
	TIM10->CR1 |= TIM_CR1_CEN;
	TIM10->CR1 |= TIM_CR1_ARPE;

	// configurando TIM11:
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
	TIM11->PSC = (1600-1);

	//habilitando o contador do TIIM11
	TIM11->CR1 |= TIM_CR1_CEN;
	TIM11->CR1 |= TIM_CR1_ARPE;
