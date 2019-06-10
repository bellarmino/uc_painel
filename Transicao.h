/*===============================================================================================================================================
DECLARA��O DAS VARI��VEIS
===============================================================================================================================================*/
//STATIC		SINAL		TIPO	VARI��VEL				VALOR INICIAL		COMENT�RIO
//-----------------------------------------------------------------------------------------------------------------------------------------------
							boolean	MX_PLSP_AUX				=0;					//Vari�vel auxiliar para fun��o plsp
							boolean	MX_PLSP_OUT				=0;					//Vari�vel de saida para fun��o plsp

	
/*===============================================================================================================================================
FLANCO POSITIVO
===============================================================================================================================================*/
//Esta fun��o verifica a transi��o positiva de um bit
plsp(in)
{
	if(in)							//Verifica se a entrada foi acionada
	{
		if(!MX_PLSP_AUX)			//Verifica se j� estava acionada na ultima verifica��o
		{
			MX_PLSP_OUT=TRUE;		//Se n�o, aciona sa�da e marcador auxiliar
			MX_PLSP_AUX=TRUE;		//
		}
		else MX_PLSP_OUT=FALSE;		//Se sim, desliga sa�da
	}
	else
	{
		MX_PLSP_OUT=FALSE;			//Se entrada n�o esta acionada, reseta saida e marcador auxiliar
		MX_PLSP_AUX=FALSE;			//
	}
	return(MX_PLSP_OUT);			//Retorna valor
}