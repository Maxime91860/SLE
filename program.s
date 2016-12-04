        .section .rodata        @ déclare une section données lecture seule
        .align 2                @ aligne le début de section sur un mot de 32 bits
                                @ (2^2 = 4 octets)
message:                        @ label, étiquette (permet de conserver l'adresse)
        .string "Hello, world!" @ compile une chaîne de charactères


        .text                   @ déclare une section de code
        .align 2
        .global main            @ déclare un symbole global, visible par le linker
main:                           @ début de la fonction main
        stmfd   sp!, {lr}       @ on sauvegarde l'adresse de retour sur la pile
        ldr     r0, =message    @ on charge dans r0 l'adresse de la chaîne


        set r1, #1
        cmp r0, #0
        beq end
        loop:
                add r1, #1
        	cmp r0, #0
        	bne loop
        end:

        bl      puts            @ on saute au code de la fonction puts
        ldmfd   sp!, {lr}       @ on rétablit l'adresse de retour
        bx      lr              @ on saute à l'adresse de retour