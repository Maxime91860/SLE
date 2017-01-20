

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <pgm.h>


#define COORD_X_INIT -240.
#define COORD_Y_INIT -240.
#define COORD_X_OBJ 0.
#define COORD_Y_OBJ 0.
#define PI 3.141592653589793238462643383279
#define ORIENTATION_INIT PI/4
#define EPSILON 1

// #define min(a,b) (a<=b?a:b) //Déja défini dans /usr/include/pm.h

//Taille robot 50cm x 50 cm x 50 cm
int taille_image = 640*360;
int taille_buffer_image = 640*360*4;

//Récupère l'image sous forme d'une chaine de caractère, et l'écrit dans un fichier
FILE* recup_camera_char(){
	char photo[taille_buffer_image];
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	read(0, photo, taille_buffer_image);
	FILE* photo_fd = fopen("tampon.txt","w");
	fprintf(photo_fd, "%s\n", photo);
	return photo_fd;
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
double distance (unsigned int pixel){
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

	//Mais pas tous les pixels encore une fois, uniquement ceux à prendre en compte pour la largeur du robot
	//Cela reste encore à determiner, intuitivement nous pensons que cela correspond à 1/3 de cette ligne.
	//Ici de 213 à 426
	int i;
	for(i=(cols/3); i<(cols/3)*2; i++){
		min = min(distance(image[rows/2][i]), distance(image[rows/2][i+1]));
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
	int cols, rows, max;

	int i,j;


	pgm_init(&argc, argv);


	// for(i=0; i<5; i++)
		// image = recup_camera(&cols, &rows, &max);
		// image = recup_camera(&cols, &rows, &max);
	// sleep(2);

		// fprintf(stderr, "ici\n");

	//Algorithme de navigation
	while(1){
		image = recup_camera(&cols, &rows, &max);
		pas = min (max_distance_foward(image, rows, cols) - EPSILON, distance_objectif(x,y));
		//On retire EPSILON car notre calcul de distance peut ne pas etre assez précis
		//On minimise le risque de se prendre un mur en minorant le pas 

		fprintf(stdout,"CMD FORWARD %g\n",pas);
		fflush(stdout);
		calcul_nouvelle_position(&x, &y, pas, orientation);

		//Cas ou on rencontre un obstacle 
		//On tourne à droite, on avance de 5m ou moins, on se ré-oriente vers l'objectif.
		if(pas != (100-EPSILON)){
			//Tourne à droite
			orientation -= PI/2;
			fprintf(stdout,"CMD TURN %g\n",-PI/4);
			fflush(stdout);

			//Avance
			image = recup_camera(&cols, &rows, &max);
			pas = min (max_distance_foward(image, rows, cols) - EPSILON, 5.0);
			fprintf(stdout,"CMD FORWARD %g\n",pas);
			fflush(stdout);
			calcul_nouvelle_position(&x, &y, pas, orientation);

			//Se tourne vers l'objectif
			calcul_orientation_objectif(x, y, &orientation);
		}


		// sleep(1);
	}

	pgm_freearray(image, rows);
	sleep(3);
	exit(0);
}