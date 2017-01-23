
***Amira Akloul M2 CHPS***
***Hamza Belaoura M2 CHPS***
***Maxime Kermarquer M2 CHPS***


Le code n'est pas encore fonctionnel nous avons des soucis sur la recuperation de l'image.
Nous pensions récupérer l'image en utilisant des fonctions de la librairie libnetpbm, mais cela ne fonctionne pas dans notre cas. En effet pour la premiere recupération il n'y a pas de problèmes, néanmoins pour les suivantes la fonction pgm_readpgm ne reconnait plus ce qu'il y sur la stdin.
Nous avons tout de même réfléchi sur les fonctions nessécaires pour trouver un chemin vers l'objectif,
et travailler à une ebauche d'algo de navigation.

Le code est dans le fichier mission_rover.c . Les autres fichiers ne sont que nos premiers essais.

Lancez la commande `make` du répertoire ASE-mars-rover.


Bibliothèque à installer
sudo apt-get install libnetpbm10-dev 

#http://netpbm.sourceforge.net/doc/libpgm.html - libpgm

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
