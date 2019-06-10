/*===============================================================================================================================================
PAINEL DE BOTÕES+LED CJATO
PROGRAMADOR: LUCIANO BELLARMINO	

DESCRITIVO:
Modos de funcionamento:
1.	Acender um LED por vez aleatoriamente;
2.	Acender dois LED por vez aleatoriamente;
3.	Aumentar progressivamente a quantidade de LED que acendem a cada ciclo até que todo painel esteja iluminado;
4.	Acender um LED por vez aleatoriamente com limite de tempo por ciclo (ajustado via potenciômetro). Avança para próximo ciclo ao pressionar o
botão correto ou atingir o tempo predefinido. Considerar contador de acertos com dois dígitos (0-99);
5.	Jogo da memória. LED acende por determinado tempo e apaga. Sistema aguarda que usuário pressione os botões corretos para avançar para próximo
ciclo. Ao pressionar botão correto LED volta a acender e ao errar a sirene é acionada;
CICLO: período entre acender o LED e o usuário pressionar todos os botões corretamente ou o tempo limite ser atingido (caso aplicável).


**HISTÓRICO**
V1.0.0		Data: Abril de 2017
- Versão inicial.
===============================================================================================================================================*/
#include <18F4520.h>	//Inclui header do ucontrolador utilizado


/*===============================================================================================================================================
DEFINIÇÕES
===============================================================================================================================================*/
#define IW_TEMPO		PIN_A0		//Entrada analogica de referencia de tempo
#define	IX_MODO			PIN_A1		//Botão de seleção do modo
#define	QX_H7SU			PIN_A2		//Habilitação do display de 7 segmentos das unidades
#define	QX_H7SD			PIN_A3		//Habilitação do display de 7 segmentos das dezenas
#define	QX_H7SM			PIN_A4		//Habilitação do display de 7 segmentos do modo
#define QX_SIR			PIN_A7		//Sirene

#define	QX_7S1			PIN_B0		//Segmento 1
#define	QX_7S2			PIN_B1		//Segmento 2
#define	QX_7S3			PIN_B2		//Segmento 3
#define	QX_7S4			PIN_B3		//Segmento 4
#define	QX_7S5			PIN_B4		//Segmento 5
#define	QX_7S6			PIN_B5		//Segmento 6
#define	QX_7S7			PIN_B6		//Segmento 7
#define	QX_7SPD			PIN_B7		//Ponto decimal

#define IX_BNT1			PIN_C0		//Botão 1
#define IX_BNT2			PIN_C1		//Botão 2
#define IX_BNT3			PIN_C2		//Botão 3
#define IX_BNT4			PIN_C3		//Botão 4
#define IX_BNT5			PIN_C4		//Botão 5
#define IX_BNT6			PIN_C5		//Botão 6
#define IX_BNT7			PIN_C6		//Botão 7
#define IX_BNT8			PIN_C7		//Botão 8
#define IX_BNT9			PIN_C8		//Botão 9
#define IX_BNT10		PIN_C9		//Botão 10
#define IX_BNT11		PIN_C10		//Botão 11
#define IX_BNT12		PIN_C11		//Botão 12
#define IX_BNT13		PIN_C12		//Botão 13
#define IX_BNT14		PIN_C13		//Botão 14
#define IX_BNT15		PIN_C14		//Botão 15
#define IX_BNT16		PIN_C15		//Botão 16
#define IX_BNT17		PIN_C16		//Botão 17
#define IX_BNT18		PIN_C17		//Botão 18
#define IX_BNT19		PIN_C18		//Botão 19
#define IX_BNT20		PIN_C20		//Botão 20

#define QX_LED1			PIN_C0		//Led 1
#define QX_LED2			PIN_C1		//Led 2
#define QX_LED3			PIN_C2		//Led 3
#define QX_LED4			PIN_C3		//Led 4
#define QX_LED5			PIN_C4		//Led 5
#define QX_LED6			PIN_C5		//Led 6
#define QX_LED7			PIN_C6		//Led 7
#define QX_LED8			PIN_C7		//Led 8
#define QX_LED9			PIN_C8		//Led 9
#define QX_LED10		PIN_C9		//Led 10
#define QX_LED11		PIN_C10		//Led 11
#define QX_LED12		PIN_C11		//Led 12
#define QX_LED13		PIN_C12		//Led 13
#define QX_LED14		PIN_C13		//Led 14
#define QX_LED15		PIN_C14		//Led 15
#define QX_LED16		PIN_C15		//Led 16
#define QX_LED17		PIN_C16		//Led 17
#define QX_LED18		PIN_C17		//Led 18
#define QX_LED19		PIN_C18		//Led 19
#define QX_LED20		PIN_C20		//Led 20

/*===============================================================================================================================================
CONFIGURAÇÕES
===============================================================================================================================================*/
#use delay (clock=8M)			//Informa à função delay que o clock do oscilador é de 8MHz
#fuses INTRC					//Define oscilador interno
#fuses NOMCLR					//Desabilita o reset externo do uControlador


/*===============================================================================================================================================
BIBLIOTECAS
===============================================================================================================================================*/
#include <Transicao.h>
#include <eeprom.h>

/*===============================================================================================================================================
DECLARAÇÃO DAS VARIÁVEIS
===============================================================================================================================================*/
//STATIC		SINAL		TIPO	VARIÁVEL				VALOR INICIAL	E²PROM		COMENTÁRIO
//-----------------------------------------------------------------------------------------------------------------------------------------------
				unsigned	int8	MB_Modo					=0;						//Modo de trabalho
				unsigned	int8	MB_ModoAux				=0;						//Variável auxiliar para verificar alteração no modo
							boolean	MX_ModoAux				=0;						//VAriável auxiliar para verificar flanco na entrada modo
				unsigned	int16	MW_TmrDisplay			=0;						//Efetivo do temporizador para atualização do display
				unsigned	int8	MB_7SU					=0;						//Valor da unidade a ser exibida no display
				unsigned	int8	MB_7SD					=0;						//Valor da dezena a ser exibida no display
				unsigned	int8	MB_7S_Digito			=0;						//Digito do display 7 segmentos a ser atualizado
				unsigned	int8	MB_CntAcertos			=0;						//Contador de acertos
				
/*===============================================================================================================================================
FUNÇÕES
===============================================================================================================================================*/


/*===============================================================================================================================================
SUBROTINAS
===============================================================================================================================================*/
//ATUALIZA DISPLAY DE 7 SEGMENTOS
void DisplayVarredura()
{
	switch(MB_7S_Digito)
	{
		case 0:
		
		output_low(QX_H7SD);
		output_low(QX_H7SM);
		output_high(QX_H7SU);
		break;
		
		case 1:
		output_low(QX_H7SU);
		output_low(QX_H7SM);
		output_high(QX_H7SD);
		break;
		
		case 3:
		
		output_low(QX_H7SU);
		output_low(QX_H7SD);
		output_high(QX_H7SM);
		break;
	}
	MB_7S_Digito++;						//Apos atualização incrementa digito para proxima atualização
	if(MB_7S_Digito>3) MB_7S_Digito=0;	//Limita incremento do digito em 3	
}

//SALVA VALORES RETENTIVOS NA EEPROM NA TRANSIÇÃO DE TELAS CASO TENHAM SIDO ALTERADOS
void Eeprom()
{
	if(MB_ModoAux!=MB_Modo)
	{
		MB_ModoAux=MB_Modo;
		write_eeprom(0, MB_Modo);
	}	

}

/*===============================================================================================================================================
INTERRUPÇÕES
===============================================================================================================================================*/
//TIMER 0 - Esta interrupção será executada conforme ajustado no setup_timer_0
#int_timer0
void timer_zero()
{
	set_timer0(60536);		//Seta Timer 0 em 60536 para que ocorra apenas 50000 contagens até a proxima interrupção que ocorre em 65536
							//fazendo com que decorra 0,01s
	
	//Timer para atualização do display de 7 segmentos
	//20ms
	MW_TmrDisplay++;
	if(MW_TmrDisplay==2)
	{
		MW_TmrDisplay=0;
		DisplayVarredura();
	}	
	
}
	
/*===============================================================================================================================================
ROTINA PRINCIPAL
===============================================================================================================================================*/
void main()
{
	//CONFIGURAÇÃO DO OSCILADOR
	setup_oscillator(OSC_8MHZ);		//Determina que o clock do oscilador interno será de 8MHz

	//CONFIGURAÇÃO DO TIMER 0 E DA INTERRUPÇÃO POR TEMPO
	enable_interrupts(GLOBAL);								//Habilita interrupções globais
	enable_interrupts(INT_TIMER0);							//Habilita interrupção pelo TIMER 0
	setup_timer_0 (RTCC_DIV_4|RTCC_INTERNAL);				//Prescaler:4 / Clock Interno / 16bits	(INC a cada 2us / Interrupção a cada 0,1s
	set_timer0(60536); 										//Seta Timer 0 em 60536 para que ocorra apenas 5000 contagens até a proxima
															//interrupção que ocorre em 65536 e assim esta ocorra em 0,01s como desejado	
	
	//CONFIGURAÇÃO DO TIMER 1
	setup_timer_1 (T1_DIV_BY_4|T1_INTERNAL);				//Prescaler:4 / Clock Interno / 16bits	(INC a cada 2us / Interrupção a cada 0,1s
	
	//CARREGA VALORES RETENTIVOS DA EEPROM
	MB_MODO=read_eeprom_word(0);
	MB_ModoAux=MB_Modo;
	
	while(true)		//Laço infinito pra execução ciclica do programa
	{	
		Eeprom();		//Salva valores na eeprom
	
	//MODO DE FUNCIONAMENTO
	//---------------------	
		if(input(IX_MODO))
		{
			if(input(IX_MODO)!=MX_ModoAux)
			{
				MX_ModoAux=input(IX_MODO);
				MB_Modo++;
			}
		}
		else MX_ModoAux=0;		
		
		switch(MB_Modo)	//Verifica o modo de funcionamento e roda a rotina desejada
		{
			case 1:		//Um led por vez aleatoriamente
			break;
			
			case 2:		//Dois led por vez aleatoriamente
			break;
			
			case 3:		//Aumentar progressivamente a quantidade de LED que acendem a cada ciclo até que todo painel esteja iluminado
			break;
			
			case 4:		//Acender um LED por vez aleatoriamente com limite de tempo por ciclo 
			break;
			
			case 5:		//Jogo da memoria
			break;
			
			default: MB_Modo=1;
		}	
	}
}