all :
	make -C code/

exec: 
	monitor/monitor.py code/mission_rover.pgr

exec2: 
	monitor/monitor.py code/essai.pgr

demo : monitor/monitor.py demo/demo.py
	./$< demo/demo.py

simulator : simulator/simulator 
	./$< maps/map1.tga

clean :
	rm -rf *.txt
	make clean -C code/