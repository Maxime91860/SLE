

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <pgm.h>
#include <string.h>


#define COORD_X_INIT -240.
#define COORD_Y_INIT -240.
#define COORD_X_OBJ 0.
#define COORD_Y_OBJ 0.
#define PI 3.141592653589793238462643383279
#define ORIENTATION_INIT PI/4
#define EPSILON 0.2

// #define min(a,b) (a<=b?a:b) //Déja défini dans /usr/include/pm.h

//Taille robot 50cm x 50 cm x 50 cm
int taille_image = 640*360;
int taille_buffer_image = 640*360*4;
char photo[640*360*4];

//Récupère l'image sous forme d'une chaine de caractère
void recup_camera_char(){
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	read(0, photo, taille_buffer_image);
}

//Parse la photo
int* recup_camera2(int* cols, int* rows, int* max){
	recup_camera_char();
	char str[] ="- This, a sample string.";
	char * pch;
	int* image;
	int i = 0;


	pch = strtok (photo," "); //Magic Number
	pch = strtok (NULL, " "); //Nombre de colonnes
	*cols = atoi (pch);
	pch = strtok (NULL, " "); //Nombre de lignes
	*rows = atoi (pch);
	pch = strtok (NULL, " "); //Niveau de gris max
	*max = atoi (pch);

	image = (int*) malloc ((*rows)*(*cols)*sizeof(int));
	while (pch != NULL)
	{
		pch = strtok (NULL, " ");
		if(pch != NULL){
			image[i] = atoi(pch);
			i++;
		}
	}
	fprintf(stderr, "Avant return\n");
	return image;
}

//Recupère l'image sous forme d'un tableau 2D de gray (unsigned int)
gray** recup_camera(int* cols, int* rows, int* max){
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	return pgm_readpgm(stdin, cols, rows, max);
}

//Calcul la nouvelle position après avoir avancé d'une distance de "pas" mètres
void calcul_nouvelle_position (double* x, double* y, double pas, double orientation){
	*x = *x + pas*cos(orientation);
   	*y = *y + pas*sin(orientation);
}

//Calcul l'angle pour se diriger vers l'objectif
void calcul_orientation_objectif(double x, double y, double* orientation){
	*orientation = atan(y/x);
}

//Convertit une valeur de pixel en mètre
double distance (int pixel){
	return (255-(double)pixel)/255 * 100;
}

//Calcul la distance de la position courante par rapport à l'objectif
double distance_objectif (double x, double y){
	return sqrt( ((COORD_X_OBJ - x)*(COORD_X_OBJ -x)) + ((COORD_Y_OBJ - y)*(COORD_Y_OBJ - y)) );
}

//Calcul la distance max que le robot peut avancer sans se crasher
double max_distance_foward(gray** image, int rows, int cols){
	//On va calculer la distance des pixels de la ligne d'horizon
	double min = distance( image[rows/2][cols/3] );
	FILE* log = fopen("log.txt","a+");
	fprintf(stderr, "bien dans max_distance_foward\n");
	int i;
	for(i=0; i<cols; i++){
		fprintf(stderr, "bien dans max_distance_foward %d\n",i);
		min = min(distance(image[rows/2][i]), distance(image[rows/2][i+1]));
		fprintf(log, "%d, %d, %g\n",i,image[rows/2][i], distance(image[rows/2][i]));
	}
	return  min;
}

double max_distance_foward2(int** image, int rows, int cols){
	//On va calculer la distance des pixels de la ligne d'horizon
	double min = distance( image[rows/2][cols/3] );
	FILE* log = fopen("log.txt","a+");
	fprintf(stderr, "bien dans max_distance_foward\n");
	int i;
	for(i=0; i<cols; i++){
		fprintf(stderr, "bien dans max_distance_foward %d\n",i);
		min = min(distance(image[rows/2][i]), distance(image[rows/2][i+1]));
		fprintf(log, "%d, %d, %g\n",i,image[rows/2][i], distance(image[rows/2][i]));
	}
	return  min;
}

int main (int argc, char** argv){

	//Variables positionnement et deplacement du robot
	double x = COORD_X_INIT;
	double y = COORD_Y_INIT;
	double orientation = ORIENTATION_INIT;
	double pas;

	//Variables images
	gray **image;
	int* image2;
	int cols, rows, max;

	FILE* log = fopen("log.txt","a+");

	int i,j;


	pgm_init(&argc, argv);
	int differences = 0;
	image = recup_camera(&cols, &rows, &max);
	image2 = recup_camera2(&cols, &rows, &max);
	int indice = 0;
	for(i=0; i < rows; i++){
		for(j=0; j<cols; j++){
			if (image[i][j] != image2[indice++]){
				differences++;
			}
		}
	}
	fprintf(stdout, "Nombre de differences : %d\n", differences);
	fflush(stdout);


	//Bug vient d'ici.
	//Pour récupérer l'image la premiere fois aucun problème, mais les fois suivantes,
	//on a l'erreur suivante:
	//mission_rover.pgr: bad magic number - not a pgm or pbm file
	//Alors que ce que devrait lire la fonction pgm_readpgm sur la stdin est bien un format pgm
	//Pour l'illustrer nous avons fait cette boucle
	// for(i=0; i<2; i++){
	// 	image = recup_camera(&cols, &rows, &max);
	// 	fprintf(stderr, "Données récupérés :\n\tNb colonnes : %d\n\tNb lignes : %d\n\tMaxGray : %d\n",cols,rows,max);
	// }
	// sleep(2);

	// Nous avons par la suiter choisi de parser directement la chaine de caractere lu sur le stdin



	//Algorithme de navigation
	while(1){
		image = recup_camera2(&cols, &rows, &max);
		pas = min (max_distance_foward(image, rows, cols) - EPSILON, distance_objectif(x,y));
		fprintf(log, "max distance : %g, distance objectif %g\n",
			max_distance_foward(image,rows,cols),distance_objectif(x,y) );
		//On retire EPSILON car notre calcul de distance peut ne pas etre assez précis
		//On minimise le risque de se prendre un mur en minorant le pas 

		fprintf(stdout,"CMD FORWARD %g\n",pas);
		fflush(stdout);
		calcul_nouvelle_position(&x, &y, pas, orientation);

		//Cas ou on rencontre un obstacle - (pas != (100-EPSILON) aussi dans le cas où le robot va
		// sur l'objectif mais le programme s'arrete dans ce cas là).
		//On tourne à droite, on avance de 5m ou moins, on se ré-oriente vers l'objectif.
		if(pas != (100-EPSILON)){
			//Tourne à droite
			orientation -= PI/4;
			fprintf(stdout,"CMD TURN %g\n",-PI/4);
			fflush(stdout);

			//Avance
			pgm_freearray(image, rows);
			image = recup_camera2(&cols, &rows, &max);
			pas = min (max_distance_foward(image, rows, cols) - EPSILON, 5.0);
			fprintf(stdout,"CMD FORWARD %g\n",pas);
			fflush(stdout);
			calcul_nouvelle_position(&x, &y, pas, orientation);

			//Se tourne vers l'objectif
			calcul_orientation_objectif(x, y, &orientation);
		}

		fclose(log);

		sleep(1);
	}

	sleep(3);
	exit(0);
}