/***********************************************************************
			GERADOR DIGITAL DE SINAIS
			
			Arquivo: DDS.c

			Desc: Modulo de configuração e operação do 
				  gerador digital de sinais
				  
			Autor: FSN

			Sobre licença:
			
			"Usem a vontade, não pretendo ficar rico com isso :)"
			
************************************************************************/
#include <stdint.h>
#include "derivative.h"
#include "DDS.h"
#include "Tabela.h"
/***********************************************************************
					Defines
************************************************************************/
//limite de salto do acumulador de fase
#define TAM_PALAVRA_FREQ 0x3FFFFE80 

//DDS nao usado:
#define DDS_LIVRE 0x00  

//DDS sendo usado:
#define DDS_EM_USO 0x01
/***********************************************************************
					Variaveis locais
************************************************************************/
//Temos até 2 geradores de sinais
static DDS_t axTblDDS[DDS_NUMERO_CANAIS];

//Controla os canais usados
static uint8_t bCanaisUsados = 0;
/***********************************************************************
					Funcoes:
************************************************************************/
 
/***********************************************************************
		DDS_pxCriaGerador()
		
		Desc: Esta funcao aloca um dos dois canais para o
			  gerador digital.
			  
		Parametros: TODO
		
		Retorna: TODO
					
************************************************************************/
DDS_t  *DDS_pxCriaGerador(Erro_t * xErr)
{
	DDS_t *pGerador = (DDS_t *)0;
	uint8_t bI = 0;

	//Checa parametros:
	
	//em caso de ponteiro de erro nulo:
	if((Erro_t *)0 == xErr)
	{
		//sai da rotina
		return (0);
	}
	
	//checa se temos canais disponiveis:
	if( DDS_NUMERO_CANAIS < bCanaisUsados)
	{
		//deposita o erro:
		*xErr = DDS_ERRO;
	
		//sai da rotina
		return(0);
	}
	
	//se temos canais disponiveis entao podemos
	//passar um bloco livre ao usuario:
	for(bI = 0; bI < DDS_NUMERO_CANAIS; bI++)
	{
		if(DDS_LIVRE == axTblDDS[bI].bEmUso) break;
		
	}
	
	//Da o bI-esimo bloco ao usuario
	pGerador = (DDS_t *)&axTblDDS[bI];
	
	//ocupa o bloco:
	pGerador->bEmUso = DDS_EM_USO;
	
	//Desabilita ele por segurança:
	pGerador->bEstado = DDS_DESLIGADO;
	
	//Diz qual o canal que ele pertence:
	pGerador->bCanal = bI;
	
	//Deposita ok:
	*xErr = DDS_OK;
	
	//incrementa o numero de canais em uso:
	bCanaisUsados++;
	
	//retorna o ponteiro:
	return(pGerador);
}

/***********************************************************************
		DDS_xInicia()
		
		Desc: Inicia a operação do gerador digital
			  
		Parametros: TODO
		
		Retorna: TODO
					
************************************************************************/
Erro_t DDS_xInicia(DDS_t *pxDDS)
{
	//Checa parametros:
	
	//ponteiro nulo?
	if((DDS_t *)0 == pxDDS)
	{
		//Sai da rotina com erro:
		return(DDS_ERRO);
		
	}
	
	//Zera acumulador de fase:
	pxDDS->dAcumulador = 0;
	
	//Ajusta para frequencia default (fo/2) = 10KHz:
	pxDDS->dFrequencia = TAM_PALAVRA_FREQ >> 10;
	
	//Liga o gerador:
	pxDDS->bEstado = DDS_LIGADO;
	
	//sinaliza tudo ok:
	return(DDS_OK);
	
}
/***********************************************************************
		DDS_xMudaFrequencia()
		
		Desc: Modifica a frequencia de saida do DDS no range
		      entre 0 e 20KHz
			  
		Parametros: TODO
		
		Retorna: TODO
					
************************************************************************/
Erro_t DDS_xMudaFrequencia(DDS_t *pxDDS, uint32_t dFreq)
{
	//Checa parametros:
	
	//ponteiro nulo?
	if((DDS_t *)0 == pxDDS)
	{
		//Sai da rotina com erro:
		return(DDS_ERRO);
		
	}
	
	//Checa valor da frequencia:
	if(TAM_PALAVRA_FREQ < dFreq)
	{
		//Satura no maximo valor disponivel:
		dFreq = TAM_PALAVRA_FREQ;
	}
	
	//Modifica o valor:
	pxDDS->dFrequencia = dFreq;
	
	//Sinaliza tudo ok:
	return(DDS_OK);
	
}
/***********************************************************************
		DDS_xDesabilita()
		
		Desc: Desabilita o canal desejado do gerador
			  
		Parametros: TODO
		
		Retorna: TODO
					
************************************************************************/
Erro_t DDS_xDesabilita(DDS_t *pxDDS)
{
	//Checa parametros:

	//ponteiro nulo?
	if((DDS_t *)0 == pxDDS)
	{
		//Sai da rotina com erro:
		return(DDS_ERRO);
		
	}
	
	//libera o bloco para uso:
	pxDDS->bEmUso = DDS_LIVRE;
	
	//Desliga o DDS:
	pxDDS->bEstado = DDS_DESLIGADO;
	
	//Destroi a referencia a esse bloco:
	pxDDS = (DDS_t *)0;
	
	//Temos um canal a mais livre:
	bCanaisUsados--;

	//Sinaliza tudo ok:
	return(DDS_OK);
}
/***********************************************************************
		DDS_vInterrupcao()
		
		Desc: Interrupcao de processamento do gerador de sinais
			  (e aqui que a magica ocorre)
		Parametros: TODO
		
		Retorna: TODO
					
************************************************************************/
void DDS_vInterrupcao(void)
{
	uint8_t  bCanal = 0;
	uint32_t dTemp  = 0;
	uint16_t static awAmplitude[DDS_NUMERO_CANAIS] = {0,0};

	//Limpa flag de interrupção:
	
	//Debug:
	GPIOB_BASE_PTR->PDOR |= (0x01 << 7);
	
	//para minimizar a latencia passamos os valores direto aos D/A:
	dTemp = GPIOA_BASE_PTR->PDOR;
	
	//Mascara dados:
	dTemp &= 0xFF00FFFF;
	
	
	//Transfere para o D/A:
	dTemp |= (uint32_t)((awAmplitude[0] >> 4) << 16);
	
	GPIOA_BASE_PTR->PDOR = dTemp;
	
	
	//Repetindo as 4 linhas acima porem mudando o GPIO e adicionando
	//mais um D/A permite o uso de um segundo canal.
			
	
	//Processamos ambos os canais:
	for(bCanal = 0; bCanal < DDS_NUMERO_CANAIS; bCanal++)
	{
		//checa se esse DDS esta ativo:
		if(DDS_LIGADO == axTblDDS[bCanal].bEstado)
		{
			//acumula a fase:
			axTblDDS[bCanal].dAcumulador += axTblDDS[bCanal].dFrequencia;

			//Mascara valor:
			axTblDDS[bCanal].dAcumulador &= 0x3FFFFFFF;
			
			//Escala valor para 8bits e extrai amplitude da tabela:
			awAmplitude[bCanal] = awTblSeno[(axTblDDS[bCanal].dAcumulador >> 24)];
		}
	}
	
	//Habilita nova contagem do timer
	//Debug:
	GPIOB_BASE_PTR->PDOR &= ~(0x01 << 7);

}
/***********************************************************************
					Fim do Arquivo
************************************************************************/

