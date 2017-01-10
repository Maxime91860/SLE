

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main (int argc, char** argv){

	//Variables
	int taille_buffer_image = 640*360*4;
	char image[taille_buffer_image];

	//Récupération de l'image
	printf("CMD CAMERA\n");
	sleep(2);
	// read(0, &image, taille_buffer_image);
	// fprintf(stderr, "Lu sur la stdin : \n%s\n", image);

}