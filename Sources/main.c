/***********************************************************************
			GERADOR DIGITAL DE SINAIS
			
			Arquivo: main.c

			Desc: Modulo principal
				  
			Autor: FSN

			Sobre licenca:
			
			"Usem a vontade, nao pretendo ficar rico com isso :)"
			
************************************************************************/
#include <stdint.h>
#include "derivative.h" 
#include "DDS.h"

/***********************************************************************
					Defines
************************************************************************/
#define SYSTEM_CLOCK 16000000 //clock do sistema

#define TIMER_LOAD   (SYSTEM_CLOCK / FREQ_AMOSTRAGEM)

/***********************************************************************
					Variaveis locais
************************************************************************/
//aloca um canal para o DDS:
DDS_t *pxDemoDDS = (DDS_t*)0;

/***********************************************************************
					Prototipos locais
************************************************************************/
void vIniciaHardware(void);

/***********************************************************************
					Funcoes
************************************************************************/

/***********************************************************************
		vIniciaHardware()
		
		Desc: Inicializa o hardware utilizado do
			  microcontrolador
			  
		Parametros: TODO
		
		Retorna: TODO
					
************************************************************************/
void vIniciaHardware(void)
{
	
	uint16_t wI;
	
	//Inicializa clock do sistema:
	OSC_BASE_PTR->CR |= 0x96;
			
	//Aguarda inicializacao do oscilador:
	for(wI = 0; wI < 0xFFFF; wI++);		
			
	//Clock externo de 8MHz do kit
	ICS_BASE_PTR->C1 |= 0xD8;
	
	//aguarda o FLL travar:
	for(wI = 0; wI < 0xFFFF; wI++);	
	
	//configura o divisor do FLL (FLLout = 32MHz)
	ICS_BASE_PTR->C2 &= ~(0x07 << 5);
	
	//Divide por 2, nossa fonte de clock e 16MHz
	ICS_BASE_PTR->C2 |= 0x20;
	
	//Troca para o FLL agora configurado:
	ICS_BASE_PTR->C1 &= ~(0x03 << 6);
	
	
	//Configura agora o systick:
	
	//Derruba o systick:
	SysTick_BASE_PTR->CSR = 0x00;

	//Valor de carga:
	SysTick_BASE_PTR->RVR = TIMER_LOAD;
	
	//Habilita systick e seu exception:
	SysTick_BASE_PTR->CSR |= 0x07;
	
	//Configura PORTC para comunicacao com o D/A
	
	//PTC como saida
	GPIOA_BASE_PTR->PDDR |= (0xFF << 16);
	
	//ZERa port do D/A:
	GPIOA_BASE_PTR->PDOR &= ~(0xFF << 16);
	
	//PTC como saida
	GPIOB_BASE_PTR->PDDR |= (0x01 << 7);
	
	//ZERa port do D/A:
	GPIOB_BASE_PTR->PDOR &= ~(0x01 << 7);
	
	
	
}

/***********************************************************************
		main()
		
		Desc: Funcao principal
			  
		Parametros: TODO
		
		Retorna: TODO
					
************************************************************************/
int main(void)
{
	//Variavel de erro:
	uint8_t bErro;
	
	//Inicia o sistema do microcontrolador:
	vIniciaHardware();
	
	//Aloca um DDS:
	pxDemoDDS = (DDS_t *)DDS_pxCriaGerador(&bErro);
	
	//Inicia gerador:
	bErro = DDS_xInicia(pxDemoDDS);
	
	for(;;) {	   

	}
	
	return 0;
}
/***********************************************************************
				Fim do Arquivo
************************************************************************/
