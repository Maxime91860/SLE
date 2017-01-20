

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <pgm.h>


#define COORD_X_INIT -240
#define COORD_Y_INIT -240
#define COORD_X_OBJ 0
#define COORD_Y_OBJ 0
#define PI 3.141592653589793238462643383279
#define ORIENTATION_INIT PI/4

//Taille robot 50cm x 50 cm x 50 cm
int taille_buffer_image = 640*360*4;
int taille_image = 640*360;

void recup_photo(char* photo){
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	read(0, photo, taille_buffer_image);
}

void calcul_nouvelle_position (double* x, double* y, double pas, double orientation){
	*x = *x + pas*cos(orientation);
   	*y = *y + pas*sin(orientation);
}

void calcul_prochain_mouvement (double x, double y, double orientation, char* vue){
	// fprintf(stdout,"CMD FORWARD 5.0\n");
	// fflush(stdout);
}

int main (int argc, char** argv){

	//Variables
	double x = COORD_X_INIT;
	double y = COORD_Y_INIT;
	double orientation = ORIENTATION_INIT;
	char image2[taille_buffer_image];
	FILE* log = fopen("log.txt","a+");
	FILE* image_file = fopen("image.txt","a+");
	gray **image;
	int cols, rows, max;

	// Récupération de l'image
	recup_photo(image2);
	fprintf(image_file, "%s\n", image2);
	fflush(image_file);
	// fprintf(log, "x = %f, y = %f, orientation = %f\n", x, y, orientation);

	pgm_init(&argc, argv);

	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);

	fprintf(log, "avant pgm_read\n");
	fflush(log);

	sleep(2);
	
	image = pgm_readpgm(image_file, &cols, &rows, &max);

	fprintf(log, "---- TEST PGM ----\n");
	fprintf(log, "Données récupérés :\n\tNb colonnes : %d\n\tNb lignes : %d\n\tMaxGray : %d",cols,rows,max);
	fflush(log);

	sleep(10);

    // for (y=0; y<rows; y++)
    //     for (x=0; x<cols; x++)
    //         fprintf(log, "%d\n", );


	fprintf(stdout,"CMD FORWARD 5.0\n");
	fflush(stdout);
	calcul_nouvelle_position(&x, &y, 5.0, orientation);
	fprintf(log, "x = %f, y = %f, orientation = %f\n", x, y, orientation);

	// fprintf(log, "Lu sur la stdin : \n%s\n", image);
	sleep(3);
	exit(0);
}