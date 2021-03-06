;constantes
TempoL EQU 0x06
TempoH EQU 0x06
	
;S1_Vermelho EQU P1.0
;S1_Amarelo EQU P1.1
;S1_Verde EQU P1.2

;S2_Vermelho EQU P1.3
;S2_Amarelo EQU P1.4
;S2_Verde EQU P1.5
	
;S3_Vermelho EQU P2.2
;S3_Amarelo EQU P2.1
S3_Verde EQU P2.0

;P3_Vermelho EQU P2.4
P3_Verde EQU P2.3

B3 EQU P3.2

S1_S2 EQU P1
S3_P3 EQU P2	
	
S1_S2_Verdes EQU 00011011b 
S3_Vermelho_P3_Verde EQU 00010011b 
S1_S2_Amarelos EQU 00101101b 
S1_S2_Vermelhos EQU 00110110b 
S3_Verde_P3_Vermelho EQU 00001110b
S3_Amarelo_P3_Vermelho EQU 00001101b

Zero EQU 0 ;para os semáforos indica que o semáforo está ligado
Um EQU 1 ;para os semáforos indica que o semáforo está desligado
Dez EQU 10
Quinze EQU 15
VinteECinco EQU 25
Quarenta EQU 40
Cem EQU 100
	
; A - contador de tempo em segundos dos semaforos para os carros
; R0 - variavel auxiliar para só efetuar mudanças ao semáforos quando A mudar
; R1 - numero de contagens de 250 microsegundos para fazer 10 milisegundos (40)
; R2 - numero de contagens de 10 milisegundos para fazer 1 segundo (100)

CSEG AT 0000h
	JMP Inicio ;depois de um reset efetuar as inicializações

CSEG AT 0003h
	JMP InterrupcaoExterior0
	
CSEG AT 000Bh
	JMP InterrupcaoTempo0
	
CSEG AT 0020h
Inicio: 
	MOV SP, #7
	CALL Inicializacoes
	CALL AtivaInterrupcao
	CALL AtivaTemporizador
	
Principal:
	CJNE R0,#Um,Principal ;se R0 for um, quer dizer que A incrementou por isso faz-se as verificações; se for 0, não se faz as verificações
	JNZ ContaSegundos_10 ;comparar A com 0, saltar se não é zero
	;Pôr S1 e S2 verdes, S3 vermelho e P3 verde
	MOV S1_S2,#S1_S2_Verdes
	MOV S3_P3,#S3_Vermelho_P3_Verde
	JMP ResetR0
ContaSegundos_10: ;comparar A com 10, saltar se não é igual a 10
	CJNE A, #10, ContaSegundos_entre_10e15
	;Pôr S1 e S2 amarelos
	MOV S1_S2,#S1_S2_Amarelos
	JMP ResetR0
ContaSegundos_entre_10e15: ;ver se A é maior a 10 e menor que 15, saltar se não for
	CJNE A, #10, Maior ;comparar A com 10
Maior:
	JC ContaSegundos_15 ;se o carry é 1 então A é menor que 10 e por isso salta-se
	CJNE A, #15, Menorque  ;comparar A com 15
Menorque:
	JNC ContaSegundos_15 ;se o carry é 0 então A é maior ou igual a 15 e por isso salta-se
	CPL P3_Verde ;muda o estado de P3, para pôr P3 intermitente
	JMP ResetR0	
ContaSegundos_15:
	CJNE A, #15, ContaSegundos_25
	;Pôr S1 e S2 vermelhos, S3 verde e P3 vermelho
	MOV S1_S2,#S1_S2_Vermelhos
	MOV S3_P3,#S3_Verde_P3_Vermelho
	JMP ResetR0
ContaSegundos_25:
	CJNE A, #25, ResetR0
	;Pôr S3 amarelo
	MOV S3_P3,#S3_Amarelo_P3_Vermelho
ResetR0:
	MOV R0,#Zero ;reset de R0, para não se efetuar as verificações sem A ser incrementado
ContaSegundos_30:
	CJNE A, #30, Principal
	CLR A ;fazer A=0 para voltar-se ao início do ciclo
	MOV R0, #Um
	JMP Principal

Inicializacoes: 
	CLR A ;fazer reset de A
	MOV R0,#Um ;meter R0 a um para definir os semáforos no estado inicial
	MOV R1, #Quarenta ;meter em R1 o número de contagens necessárias para perfazer 10 milisegundos (40)
	MOV R2, #Cem ;meter em R2 o número de contagens de 10 milisegundos para perfazer 1 segundo (100)
	MOV S1_S2, #S1_S2_Verdes 
	MOV S3_P3, #S3_Vermelho_P3_Verde
	RET

AtivaInterrupcao:
	MOV IE, #10000011b ;ativar as interrupções em geral, a do timer0 e a externa 0
	SETB IT0 ;definir a interrupção externa para ser ativa no falling edge
	RET

AtivaTemporizador:
	MOV TMOD, #00000010b ;definir o timer 0 no modo 2 (8 bit - auto reload)
	MOV TL0, #TempoL
	MOV TH0, #TempoH
	SETB TR0 ;iniciar o timer
	RET

InterrupcaoTempo0:
	DJNZ R1, FimIT0 ;decrementar R1 e verificar se chegou a 0, saltar se não chegou a 0
	MOV R1, #Quarenta ;reset de R1
	DJNZ R2, FimIT0 ;como passou 10 milisegundos, decrementar R2 e verificar se chegou a 0, saltar se não
	INC A ;se R2 chegou a 0, passou um segundo, incrementar A
	MOV R0,#Um ;meter R0 a um, para efetuar as verificações de A em Principal pois A aumentou
	MOV R2, #Cem ;reset de R2	
FimIT0:
	RETI

InterrupcaoExterior0:
	JB S3_Verde, FimIE0 ;verificar se S3_Verde está a 1 (desligado) e saltar se isso ocorrer
	MOV A, #VinteECinco ;se S3_Verde está a 0 (ligado) metemos em A vinte e cinco para mudar S3 para amarelo
	MOV R0,#Um ;meter R0 a um, para efetuar as verificações de A em Principal pois A foi alterado
	MOV R1, #Quarenta ;reset de R1
	MOV R2, #Cem ;reset de R2
FimIE0:
	RETI
	
end