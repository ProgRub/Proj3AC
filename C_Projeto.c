#include <reg51.h>

sbit S1_Vermelho = P1^0 //porta de saída para a luz vermelha do semáforo S1
sbit S1_Amarelo = P1^1 //porta de saída para a luz amarela do semáforo S1
sbit S1_Verde = P1^2 //porta de saída para a luz verde do semáforo S1

sbit S2_Vermelho = P1^3 //porta de saída para a luz vermelha do semáforo S2
sbit S2_Amarelo = P1^4 //porta de saída para a luz amarela do semáforo S2
sbit S2_Verde = P1^5 //porta de saída para a luz verde do semáforo S2

sbit S3_Vermelho = P2^2 //porta de saída para a luz vermelha do semáforo S3
sbit S3_Amarelo = P2^1 //porta de saída para a luz amarela do semáforo S3
sbit S3_Verde = P2^0 //porta de saída para a luz verde do semáforo S3

sbit P3_Vermelho = P2^4 //porta de saída para a luz vermelha do semáforo P3
sbit P3_Verde = P2^3 //porta de saída para a luz verde do semáforo P3

sbit B3= P3^2 //porta de entrada para pressionar o botão B3