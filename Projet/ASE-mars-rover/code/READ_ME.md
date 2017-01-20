
***Amira Akloul M2 CHPS***
***Hamza Belaoura M2 CHPS***
***Maxime Kermarquer M2 CHPS***



**Pré-requis**
===================

Paquets à installer :
> `sudo apt-get install mesa-common-dev freeglut3-dev libglu1-mesa-dev libmicrohttpd-dev libglew-dev`
> `sudo apt-get install python-matplotlib python-numpy`


**Lancement du Rover**
===================

###Installer le simulateur
Dans le dossier ASE-mars-rover
> `make -C simulator/`

###Lancer le simulateur 

> `simulator/simulator maps/map1.tga`

###Lancer le moniteur avec la démo
> ` monitor/monitor.py --server http://localhost:8888 ./demo/demo.py`

###Lancer le moniteur avec son programme
> ` monitor/monitor.py --server http://localhost:8888 ./mon-programme`

###Lancer le moniteur sur une adresse particulière 
> ` monitor/monitor.py --server http://192.168.1.1:8888 ./mon-programme`


Bibliothèque à installer
sudo apt-get install libnetpbm10-dev