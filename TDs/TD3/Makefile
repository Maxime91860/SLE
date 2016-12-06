
all : td3_sle.pgr

td3_sle.pgr : td3_sle.c
	gcc $< -O3 -lm -o $@

clean : 
	rm -rf *.pgr

envoi : 
	scp * pi@192.168.1.5:/home/pi/mkermarquer/TD3