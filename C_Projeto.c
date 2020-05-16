#include <reg51.h>

sbit S1_Vermelho = P1^0; //porta de saída para a luz vermelha do semáforo S1
sbit S1_Amarelo = P1^1; //porta de saída para a luz amarela do semáforo S1
sbit S1_Verde = P1^2; //porta de saída para a luz verde do semáforo S1

sbit S2_Vermelho = P1^3; //porta de saída para a luz vermelha do semáforo S2
sbit S2_Amarelo = P1^4; //porta de saída para a luz amarela do semáforo S2
sbit S2_Verde = P1^5;//porta de saída para a luz verde do semáforo S2

sbit S3_Vermelho = P2^2; //porta de saída para a luz vermelha do semáforo S3
sbit S3_Amarelo = P2^1; //porta de saída para a luz amarela do semáforo S3
sbit S3_Verde = P2^0; //porta de saída para a luz verde do semáforo S3

sbit P3_Vermelho = P2^4; //porta de saída para a luz vermelha do semáforo P3
sbit P3_Verde = P2^3; //porta de saída para a luz verde do semáforo P3

sbit B3= P3^2; //porta de entrada para pressionar o botão B3

int contaTempoSemaforosCarros = 0;
int auxContaTempoSemaforosCarros = 0;
int auxIntermitente = 0;

void iniciarTimers(void){
	//Ativar as interrupções globais e dos timers 0 e 1
	EA = 1;
	ET0 = 1;
	EX0 = 1;
	//Configurar os timers no modo 2 (8 bits autoreload)
	TMOD &= 0xF0;
	TMOD |= 0x02;
	//Configurar o tempo de contagem 250 microsegundos
	TH0 = 0x06;
	TL0 = 0x06;
	//Iniciar timer 0 e 1
	TR0 = 1;
	IT0 = 1;
}

void iniciarSemaforos(void){
	S1_Verde = 0;
	S1_Amarelo = 1;
	S1_Vermelho = 1;
	S2_Verde = 0;
	S2_Amarelo = 1;
	S2_Vermelho = 1;
	S3_Verde = 1;
	S3_Amarelo = 1;
	S3_Vermelho = 0;
	P3_Verde = 0;
	P3_Vermelho = 1;
}

void Timer0_ISR(void) interrupt 1{
	if(auxContaTempoSemaforosCarros == 4000){
		contaTempoSemaforosCarros++;
		auxContaTempoSemaforosCarros = 0;
	}else{
		auxContaTempoSemaforosCarros++;
	}
}


void External0_ISR(void) interrupt 0 {
	if (S3_Verde == 0){
		contaTempoSemaforosCarros = 25;
		auxContaTempoSemaforosCarros = 0;
	}
	
}


void main(void){
	iniciarTimers();
	for(;;){
		//Contas = 1 -> 250 microsegundos
		//Contas = 4 -> 1 milisegundo
		//Contas = 20000 -> 5 segundos
		//Contas = 40000 -> 10 segundos
		//Contas = 60000 -> 15 segundos
		if(contaTempoSemaforosCarros == 0){
			iniciarSemaforos();
		}
		
		if(contaTempoSemaforosCarros == 10){
			S1_Amarelo = 0;
			S2_Amarelo = 0;
			S1_Verde = 1;
			S2_Verde = 1;
		}
		if(contaTempoSemaforosCarros >= 10 &&contaTempoSemaforosCarros <15){
			if(contaTempoSemaforosCarros % 2 ==0 && auxIntermitente==0){
				auxIntermitente=1;
				P3_Verde = ~P3_Verde;
			}
			if(contaTempoSemaforosCarros % 2 !=0&& auxIntermitente==1){
				auxIntermitente=0;
				P3_Verde = ~P3_Verde;
			}
		}
		
		if(contaTempoSemaforosCarros == 15){
			S3_Verde = 0;
			S3_Vermelho = 1;
			S1_Vermelho = 0;
			S1_Amarelo = 1;
			S2_Vermelho = 0;
			S2_Amarelo = 1;
			P3_Verde = 1;
			P3_Vermelho = 0;
		}
		
		if(contaTempoSemaforosCarros == 25){
			S3_Amarelo = 0;
			S3_Verde = 1;
		}
		
		if(contaTempoSemaforosCarros == 30){
			contaTempoSemaforosCarros = 0;
		}
	}
}
