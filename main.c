#include "stm32f4xx.h"

/*
	Serão 4 efeitos visuais, tendo 2 duplas: a dupla 1 (led vermelho e branco) e a dupla 2 (led verde e azul), tendo 1s de intervalo entre cada efeito e 2s para reiniciar a sequencia de efeitos

	efeito 1: a dupla 1 pisca durante um periodo determinado com uma frequencia determinada sendo que ambos (frequencia e periodo) mudam conforme o tempo.
			A dupla 2 pisca apenas uma vez entre os intervalos de tempos da dupla 1 com frequencia fixa.

	efeito 1 invertido: funciona do mesmo jeito porém inverte as duplas que ligam e desligam

	efeito 2: as 2 duplas piscam juntas com periodos decrescentes conforme o tempo passa

	efeito 3: as 2 duplas piscam 3 vezes com periodo de 400ms cada piscada, depois, todos piscam a 16hz durante meio segundo
			 todo esse processo se repete 3 vezes ao todo

	Sequencia dos efeitos: efeito 2 -> efeito 1 -> efeito 1 invertido -> efeito 3
*/

	int main(void)
{
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

	// variaveis e funções:
	void efeito1 (int PiscadasDupla1, int PiscadasDupla2, int divisor);
	void efeito2 (int divisor);
	void Efeito1Invertido (int PiscadasDupla1, int PiscadasDupla2, int divisor);
	void efeito3 (int divisor1, int divisor2);
	int i=0, arr, PiscadasDupla1, PiscadasDupla2;

	while (1)
	{

		if (GPIOC->IDR & tomada){

			// resetando variaveis:
			PiscadasDupla1 = 2;
			PiscadasDupla2 = 2;

			// iniciando efeitos visuais:

			// executando efeito 2
			arr = 10000; // resetando para 1s de periodo
				for (i=0; i<98; i++){
					efeito2(arr);
					if (i >=9){
						arr -= 10;
				}
					else {arr -= 1000;}
			}

				arr = 5000; // resetando para 2 hz

				// executando efeito 1
				for(i=0; i<5; i++){
					efeito1(PiscadasDupla1, 1, arr); // arr = 5000, frequencia inicial de 2 hz
					arr = arr/2;
					PiscadasDupla1 += 2;
			}

				arr = 5000;

				// executando efeito 1 invertido
				for(i=0; i<5; i++){
					Efeito1Invertido (1, PiscadasDupla2, arr);
					arr = arr/2;
					PiscadasDupla2 += 2;
			}

				// executando efeito 3
				for (i=0; i<3; i++){
					efeito3(4000, 10000);
			}
		} // if da tomada

		else {GPIOB->ODR &= ~(dupla1 | dupla2);} // garantindo que os leds estarão apagados se a tomada não estiver ligada

	} // loop infinito (while)
} // main

	// FUÇÕES:

	void efeito1 (int PiscadasDupla1, int PiscadasDupla2, int divisor){
		TIM10->ARR = (divisor); // a frequencia aumenta (divisor diminui) conforme o tempo
		TIM10->CNT = 0; // garantindo que tudo esteja resetado
		TIM10->SR &= ~TIM_SR_UIF;

		PiscadasDupla1 = PiscadasDupla1*2;
		PiscadasDupla2 = PiscadasDupla2*2;

		while(PiscadasDupla1 != 0){ // pisca a dupla 1 conforme o "PiscadasDupla1"
			if (TIM10->SR & TIM_SR_UIF){

				GPIOB->ODR ^= dupla1;
				PiscadasDupla1--;
				TIM10->SR &= ~TIM_SR_UIF;
				}
			}

		TIM10->ARR = (2000-1); // 5 hz e 100ms led acesso
		TIM10->CNT = 0;
		TIM10->SR &= ~TIM_SR_UIF;

		while(PiscadasDupla2 != 0){
			if (TIM10->SR & TIM_SR_UIF){ // pisca dupla 2 uma vez

				TIM10->SR &= ~TIM_SR_UIF;
				GPIOB->ODR ^= dupla2;
				PiscadasDupla2--;
				}
			}
		}

	void efeito2 (int divisor){
		TIM10->ARR = (divisor); //  o periodo vai diminuindo por piscada, periodo inicial de 1,2s
		TIM10->CNT = 0; // garantindo que tudo esteja resetado
		TIM10->SR &= ~TIM_SR_UIF;

		GPIOB->ODR |= dupla2; // garantindo que os leds estarão nas posições certas
		GPIOB->ODR &= ~dupla1;

		while((TIM10->SR & TIM_SR_UIF) != 1){ // sai do loop quando chegar na metade do periodo
				GPIOB->ODR ^= dupla2; // liga todos os leds por metade do periodo
				GPIOB->ODR ^= dupla1;
			}

		TIM10->SR &= ~TIM_SR_UIF;

		while((TIM10->SR & TIM_SR_UIF) != 1){
			GPIOB->ODR &= ~(dupla1 | dupla2); // desliga os leds pelo resto do periodo
		}
	}

	void Efeito1Invertido (int PiscadasDupla1, int PiscadasDupla2, int divisor){
		TIM10->ARR = (divisor); // a frequencia aumenta (divisor diminui) conforme o tempo
		TIM10->CNT = 0; // garantindo que tudo esteja resetado
		TIM10->SR &= ~TIM_SR_UIF;

		PiscadasDupla1 = PiscadasDupla1*2;
		PiscadasDupla2 = PiscadasDupla2*2;

		while(PiscadasDupla2 != 0){
			if (TIM10->SR & TIM_SR_UIF){ // pisca a dupla 2 conforme o "PiscadasDupla2"

				GPIOB->ODR ^= dupla2;
				PiscadasDupla2--;
				TIM10->SR &= ~TIM_SR_UIF;
			}
		}

		TIM10->ARR = (2000-1); // 5 hz e 100ms led acesso
		TIM10->CNT = 0;
		TIM10->SR &= ~TIM_SR_UIF;

		while(PiscadasDupla1 != 0){ // dupla 1 pisca uma vez
			if (TIM10->SR & TIM_SR_UIF){

				TIM10->SR &= ~TIM_SR_UIF;
				GPIOB->ODR ^= dupla1;
				PiscadasDupla1--;
			}
		}
	}

	void efeito3(int divisor1, int divisor2){
		TIM10->ARR = (divisor1); // 2,5 hz
		TIM11->ARR = (divisor2); // 1 hz
		TIM10->CNT = 0; // garantindo que tudo esteja resetado
		TIM10->SR &= ~TIM_SR_UIF;

		GPIOB->ODR |= dupla1; // garantindo que os leds estarão nas posições certas
		GPIOB->ODR &= ~dupla2;

		int cont=0;

		// piscando 3 vezes a 2,5 hz
		while(cont != 4){
			while((TIM10->SR & TIM_SR_UIF) != 1){ // sai do loop quando chegar na metade do periodo
				GPIOB->ODR ^= dupla2; // liga todos os leds por metade do periodo
				GPIOB->ODR ^= dupla1;
			}

			TIM10->SR &= ~TIM_SR_UIF;

			while((TIM10->SR & TIM_SR_UIF) != 1){
				GPIOB->ODR &= ~(dupla1 | dupla2); // desliga os leds pelo resto do periodo
			}
			TIM10->SR &= ~TIM_SR_UIF;
			cont++;
		}

		// piscando a 16 hz por meio segundo
		TIM10->ARR = (625-1); // 16 hz
		TIM11->SR &= ~TIM_SR_UIF;
		TIM11->CNT = 0; // garantindo que tudo esteja resetado
		TIM10->SR &= ~TIM_SR_UIF;

		GPIOB->ODR |= dupla1; // garantindo que os leds estarão nas posições certas
		GPIOB->ODR &= ~dupla2;

		while((TIM11->SR & TIM_SR_UIF) != 1){
			if (TIM10->SR & TIM_SR_UIF){
				GPIOB->ODR ^= dupla1; // liga todos os leds por metade do periodo
				GPIOB->ODR ^= dupla2;
				TIM10->SR &= ~TIM_SR_UIF;
			}
		}
	} // fim função
