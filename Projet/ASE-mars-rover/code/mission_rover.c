

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

//Récupère l'image sous forme d'une chaine de caractère, et l'écrit dans un fichier
char* recup_camera_char(){
	char photo[taille_buffer_image];
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	read(0, photo, taille_buffer_image);
	// FILE* photo_fd = fopen("image.txt","w");
	// fprintf(photo_fd, "%s\n", photo);
	return photo;
}

int** recup_camera2(int* cols, int* rows, int* max){
	char *image_char = recup_camera_char();
	char *pointeur;
	int ** image;
	int i,j;

	pointeur = strtok( image_char, " "); //P2
	pointeur = strtok( NULL, " "); //La colonne 640
	*cols = atoi(pointeur);
	pointeur = strtok( NULL, " "); //Les lignes 340
	*rows = atoi (pointeur);
	pointeur = strtok( NULL, " "); //Le niveau de gris max 255
	*max = atoi (pointeur);

	image = (int**) malloc ((*rows)*sizeof(int *));
	for(i=0; i < (*rows); i++){
		image[i] = (int*) malloc ((*cols)*sizeof(int));
	}
		    
	i=0; j=0;
	while( pointeur != NULL ) 
	{
		pointeur = strtok( NULL, " ");
		if(pointeur != NULL){
			if(i != (*rows))
				image [i][j] = atoi(pointeur);
			j++;
			if(j==(*cols)){
				i++;
				j=0;
			}
		}
	}
	return image;
}

//Recupère l'image sous forme d'un tableau 2D de gray (unsigned int)
gray** recup_camera(int* cols, int* rows, int* max){
	// FILE* photo = recup_camera_char();
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	return pgm_readpgm(stdin, cols, rows, max);
}

// gray** recup_camera(int* cols, int* rows, int* max){
// 	FILE* photo = recup_camera_char();
// 	return pgm_readpgm(photo, cols, rows, max);
// }

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
double max_distance_foward(int** image, int rows, int cols){
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
	// gray **image;
	int **image;
	int cols, rows, max;

	FILE* log = fopen("log.txt","a+");

	int i,j;


	pgm_init(&argc, argv);


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