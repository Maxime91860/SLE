
.text
.global main_asm
.align 2

main_asm :

	MOV r5, r0  	@On stocke le pointeur gpio_map dans le registre 5
	MOV r6 , #1 	@Dans r6, on va stocker 1<<18
	MOV r7 , #1		@Dans r7, on va stocker 1<<16

	@Calcul des décalages 

	MOV r6 , r6 , lsl #18
	MOV r7 , r7 , lsl #16

	@Calcul des décalages par rapport gpio_map

	ADD r8 , r0,#4   @gpio_map+4  Config
	ADD r9 , r0,#40  @gpio_map+40 Allumer
	ADD r10 , r0,#28 @gpio_map+28 Eteindre

	STR r6, [r8]


	clignote:
		STMFD   sp!, {lr}
		BT allumer
		LDMFD   sp!, {lr}
		STMFD   sp!, {lr}
		BT attendre
		LDMFD   sp!, {lr}
		STMFD   sp!, {lr}
		BT attendre
		LDMFD   sp!, {lr}
	BT clignote

	allumer:
		STR r7, [r9]

	eteindre:
		STR r7, [r10]

	attendre:
		MOV r0, #500
		MUL r0, r0, #2
		STMFD   sp!, {lr}
		USLEEP
		LDMFD   sp!, {lr}