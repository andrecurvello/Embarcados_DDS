/***********************************************************************
			GERADOR DIGITAL DE SINAIS
			
			Arquivo: DDS.h

			Desc: Interface para inicialização e gerenciamento do
				  gerador digital de sinais
				  
			Autor: FSN

			Sobre licença:
			
			"Usem a vontade, não pretendo ficar rico com isso :)"
			
************************************************************************/

//Previne compilação recursiva:
#ifndef __DDS_H_ 
#define __DDS_H_	1


/***********************************************************************
					Defines
************************************************************************/
//Numero de canais do DDS (o Kinetis possui dois DACS!)
#define DDS_NUMERO_CANAIS 2 

//Define valor da frequencia maxima desejada(HZ):
#define DDS_FREQUENCIA_MAX  20000

//Define numero de amostras na maxima frequencia:
#define N_AMOSTRAS_MIN		4

//Define valor da taxa de amostragem baseada na amostragem:
#define FREQ_AMOSTRAGEM	(DDS_FREQUENCIA_MAX * N_AMOSTRAS_MIN)
/***********************************************************************
					Estruturas:
************************************************************************/
//Estrutura do gerador digital:
typedef struct
{
	uint32_t dAcumulador;
	uint32_t dFrequencia;
	uint8_t  bCanal;
	uint8_t  bEstado;
	uint8_t  bEmUso;
}DDS_t;

/***********************************************************************
					Enumeracoes:
************************************************************************/
typedef enum 
{
	DDS_OK = 0,
	DDS_ERRO,
	
	DDS_MAX_ERROS
}Erro_t;

typedef enum
{
	DDS_DESLIGADO = 0,
	DDS_LIGADO
}Estado_t;

/***********************************************************************
					Prototipos
************************************************************************/
extern DDS_t  *DDS_pxCriaGerador(Erro_t * xErr);
extern Erro_t DDS_xInicia(DDS_t *pxDDS);
extern Erro_t DDS_xMudaFrequencia(DDS_t *pxDDS, uint32_t dFreq);
extern Erro_t DDS_xDesabilita(DDS_t *pxDDS);
extern void DDS_vInterrupcao(void);
/***********************************************************************
					Fim do Arquivo
************************************************************************/

#endif
