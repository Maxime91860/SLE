

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define COORD_X_INIT -240
#define COORD_Y_INIT -240
#define COORD_X_OBJ 0
#define COORD_Y_OBJ 0

//Taille robot 50cm x 50 cm x 50 cm
int taille_buffer_image = 640*360*4;

void recup_photo(char* photo){
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	read(0, photo, taille_buffer_image);
}

void calcul_nouvelle_position (double* x, double y, double pas, double orientation){

}

void calcul_prochain_mouvement (double x, double y, double orientation, char* vue){

}

int main (int argc, char** argv){

	//Variables
	double x = COORD_X_INIT;
	double y = COORD_Y_INIT;
	char image[taille_buffer_image];
	FILE* log = fopen("log.txt","a+");

	// fprintf(stdout,"CMD FORWARD 5.0\n");
	// fflush(stdout);

	//Récupération de l'image
	recup_photo(&image);

	fprintf(log, "Lu sur la stdin : \n%s\n", image);

}