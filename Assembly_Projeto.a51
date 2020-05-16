TempoL EQU 0x06
TempoH EQU 0x06

S1_Vermelho EQU P1.0
S1_Amarelo EQU P1.1
S1_Verde EQU P1.2

S2_Vermelho EQU P1.3
S2_Amarelo EQU P1.4
S2_Verde EQU P1.5
	
S3_Vermelho EQU P2.2
S3_Amarelo EQU P2.1
S3_Verde EQU P2.0

P3_Vermelho EQU P2.4
P3_Verde EQU P2.3

B3 EQU P3.2

S1_S2 EQU P1
S3_P3 EQU P2	

Zero EQU 0
Um EQU 1
Dez EQU 10
Quinze EQU 15
VinteECinco EQU 25
Quarenta EQU 40
Cem EQU 100
	
; A - contador de tempo em segundos dos semaforos para os carros
; R0 - variavel auxiliar para pôr o semaforo dos peoes intermitente
; R1 - numero de contagens de 250 microsegundos para fazer 10 milisegundos (40)
; R2 - numero de contagens de 10 milisegundos para fazer 1 segundo (100)

CSEG AT 0000h
	JMP Inicio

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
	JNZ IF_1
	CLR S1_Verde
	SETB S1_Amarelo
	SETB S1_Vermelho
	CLR S2_Verde
	SETB S2_Amarelo
	SETB S2_Vermelho
	SETB S3_Verde
	SETB S3_Amarelo
	CLR S3_Vermelho
	CLR P3_Verde
	SETB P3_Vermelho
IF_1:	
	CJNE A, #10, IF_2
	CLR S1_Amarelo
	CLR S2_Amarelo
	SETB S1_Verde
	SETB S2_Verde
IF_2:
	CJNE A, #10, Nada
Nada:
	JC IF_3
	CJNE A, #15, Nada2
Nada2:
	JNC IF_3
	MOV R3, A
	RR A
	JNZ Nada3
	CJNE R0, #Zero, Nada3
	MOV R0, #Um
	CPL P3_Verde
	JMP Nada4
Nada3:
	JZ Nada4
	CJNE R0, #Um, Nada4
	MOV R0, #Zero
	CPL P3_Verde
Nada4:
	MOV A, R3
	
IF_3:
	CJNE A, #15, IF_4
	CLR S3_Verde
	SETB S3_Vermelho
	CLR S1_Vermelho
	SETB S1_Amarelo
	CLR S2_Vermelho
	SETB S2_Amarelo
	SETB P3_Verde
	CLR P3_Vermelho
IF_4:
	CJNE A, #25, IF_5
	CLR S3_Amarelo
	SETB S3_Verde
IF_5:
	CJNE A, #30, Principal
	CLR A
	JMP Principal

Inicializacoes: 
	CLR A
	MOV R0, #Zero
	MOV R1, #Zero
	MOV S1_S2, #Um
	MOV S3_P3, #Um
	RET

AtivaInterrupcao:
	MOV IE, #10000011b
	SETB IT0
	RET

AtivaTemporizador:
	MOV TMOD, #00000010b
	MOV TL0, #TempoL
	MOV TH0, #TempoH
	SETB TR0
	MOV R1, #Quarenta
	MOV R2, #Cem
	RET

InterrupcaoTempo0:
	DJNZ R1, FimIT0
	MOV R1, #Quarenta
	DJNZ R2, FimIT0
	INC A
	MOV R2, #Cem
	
FimIT0:
	RETI

InterrupcaoExterior0:
	JB S3_Verde, FimIE0
	MOV A, #VinteECinco
	MOV R1, #Quarenta
	MOV R2, #Cem
FimIE0:
	RETI


	
end