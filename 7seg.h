/*===============================================================================================================================================
DECLARAÇÃO DAS VARIÁVEIS
===============================================================================================================================================*/
//STATIC		SINAL		TIPO	VARIÁVEL				VALOR INICIAL		COMENTÁRIO
//-----------------------------------------------------------------------------------------------------------------------------------------------


/*===============================================================================================================================================
RETORNA VALOR PARA DISPLAY 7 SEGMENTOS
===============================================================================================================================================*/
void Disp7seg(valor)
{
	switch(valor)
	{
		case 0: return(0b01111110);
		case 1: return(0b00110000);
		case 2: return(0b01101101);
		case 3: return(0b01111001);
		case 4: return(0b00110011);
		case 5: return(0b01011011);
		case 6: return(0b00011111);
		case 7: return(0b01110000);
		case 8: return(0b01111111);
		case 9: return(0b01110011);		
		default: return(0);
	}
}			