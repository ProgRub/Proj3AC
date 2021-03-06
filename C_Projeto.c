#include <reg51.h>

//sbit S1_Vermelho = P1^0; //porta de saída para a luz vermelha do semáforo S1
//sbit S1_Amarelo = P1^1; //porta de saída para a luz amarela do semáforo S1
//sbit S1_Verde = P1^2; //porta de saída para a luz verde do semáforo S1

//sbit S2_Vermelho = P1^3; //porta de saída para a luz vermelha do semáforo S2
//sbit S2_Amarelo = P1^4; //porta de saída para a luz amarela do semáforo S2
//sbit S2_Verde = P1^5;//porta de saída para a luz verde do semáforo S2

//sbit S3_Vermelho = P2^2; //porta de saída para a luz vermelha do semáforo S3
//sbit S3_Amarelo = P2^1; //porta de saída para a luz amarela do semáforo S3
//sbit S3_Verde = P2^0; //porta de saída para a luz verde do semáforo S3

//sbit P3_Vermelho = P2^4; //porta de saída para a luz vermelha do semáforo P3
sbit P3_Verde = P2^3; //porta de saída para a luz verde do semáforo P3

sbit B3= P3^2; //porta de entrada para pressionar o botão B3

int contaSegundos = 0; //esta variável é igual ao número de segundos que passaram desde o início do ciclo
int auxContaSegundos = 0; //esta variável conta o número de vezes que ocorre overflow no timer (que ocorre quando passam 250 microssegundos)
bit auxMudarSemaforos = 1; //variável "booleana" para evitar estar sempre a definir os semáforos

//*****************IMPORTANTE************************
//0 - LED ligado
//1 - LED desligado
int S1_S2_Verdes=0x1B; // 0001 1011 em binário
int S3_Vermelho_P3_Verde=0x13; // 0001 0011 em binário
int S1_S2_Amarelos=0x2D; // 0010 1101 em binário
int S1_S2_Vermelhos=0x36; // 0011 0110 em binário
int S3_Verde_P3_Vermelho=0x0E; // 0000 1110 em binário
int S3_Amarelo_P3_Vermelho=0x0D; // 0000 1101 em binário

void Inicializar(void){
	//Ativar as interrupções globais, do timer 0 e da interrupção externa 0
	EA = 1;
	ET0 = 1;
	EX0 = 1;
	//Configurar o timer no modo 2 (8 bits autoreload)
	TMOD &= 0xF0;
	TMOD |= 0x02;
	//Configurar o tempo de contagem para 250 microssegundos
	TH0 = 0x06;
	TL0 = 0x06;
	//Iniciar timer 0 e definir a interrupção para ser acionada na falling edge
	TR0 = 1;
	IT0 = 1; 
}
void Timer0_ISR(void) interrupt 1{
	//auxContaSegundos = 1 -> 250 microssegundos
	//auxContaSegundos = 4 -> 1 milisegundo
	//auxContaSegundos = 4000 -> 1 segundo
	if(auxContaSegundos == 4000){ //quando esta variável chegar a 4000, significa que fez-se um segundo
		contaSegundos++;
		auxContaSegundos = 0; //reset da variável
		auxMudarSemaforos = 1; //como o contaSegundos aumentou, definimos isto a 1 para efetuar as verificações
	}else{ //se não chegou, incrementa-se a variável
		auxContaSegundos++;
	}
}

void External0_ISR(void) interrupt 0 {
	if (P2 == S3_Verde_P3_Vermelho){ //verifica-se se o sinal verde do semáforo S3 está ligado
		contaSegundos = 25; //se estiver define-se o contasegundos a 25 pois é neste momento que se põe o semáforo 3 a amarelo como pretendido
		auxContaSegundos = 0; //reset da variável que conta os overflows do timer, de modo a passar 5 segundos a partir do botão ser clicado
		auxMudarSemaforos = 1; //como o contaSegundos mudou, definimos isto a 1 para efetuar as verificações
	}	
}

void main(void){
	Inicializar();
	for(;;){
		if (auxMudarSemaforos==1){
			if(contaSegundos == 0){ //parte inicial do ciclo, S1 e S2 verdes, S3 vermelho, P3 verde
				P1 = S1_S2_Verdes;
				P2 = S3_Vermelho_P3_Verde;
			}			
			if(contaSegundos == 10){ //depois de 10 segundos, S1 e S2 ficam amarelos
				P1=S1_S2_Amarelos;
			}
			if(contaSegundos > 10 &&contaSegundos <15){ //entre os 10 e 15 segundos, o semáforo P3 deve ficar verde intermitente, a cada segundo
				P3_Verde = ~P3_Verde;
			}			
			if(contaSegundos == 15){ //depois de 15 segundos, S1 e S2 ficam vermelhos, S3 fica verde e P3 fica vermelho
				P1=S1_S2_Vermelhos;
				P2=S3_Verde_P3_Vermelho;
			}			
			if(contaSegundos == 25){ //depois de 25 segundos, S3 fica amarelo
				P2=S3_Amarelo_P3_Vermelho;
			}			
			auxMudarSemaforos=0; //mudar isto para 0 para não fazer as verificações até o contaSegundos incrementar
			if(contaSegundos == 30){ //depois dos 30 segundos, faz-se reset da variável para voltarmos ao início do ciclo e tratar dos semáforos
				contaSegundos = 0; 
				auxMudarSemaforos = 1;
			}
		}
	}
}