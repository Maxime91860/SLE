
all : led

led : led.o asm.o
	gcc $^ -o $@

led.o : led.c
	gcc $< -c -o $@

asm.o : led.s
	as $< -o $@

led.o : 