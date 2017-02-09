

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
int nb_appel = 1;


void vide_stdin(){
	char buffer[100000];
	read(0, buffer, 100000);
}

void read_OK(){
	char buffer[3];
	read(0, buffer, 3);
}

//Récupère l'image sous forme d'une chaine de caractère
void recup_camera_char(){
	fprintf(stdout, "APPEL %d\n", nb_appel);
	fflush(stdout);
	sleep(0.5);
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	fprintf(stdout, "%d : avant read\n", nb_appel);
	fflush(stdout);
		read(0, photo, taille_buffer_image);
	fprintf(stdout, "%d : après read\n", nb_appel);
	fflush(stdout);
	nb_appel++;
}

void free_image(int** image, int rows){
	int i;
	for (i = 0; i < rows; ++i)
	{
		free(image[i]);
	}
	free(image);
}

//Parse la photo
int** recup_camera2(int* cols, int* rows, int* max){
	fprintf(stdout, "Debut parsage #%d\n", nb_appel);
	fflush(stdout);
	recup_camera_char();
	char * pch;
	int** image;
	int i;
	int ind_ligne = 0;
	int ind_colonne = 0;

	nb_appel--;

	pch = strtok (photo," "); //Magic Number
	fprintf(stdout, "Magic number : %s\n",pch);
	fflush(stdout);
	pch = strtok (NULL, " "); //Nombre de colonnes
	*cols = atoi (pch);
	fprintf(stdout, "Cols : %s\n",pch);
	fflush(stdout);
	pch = strtok (NULL, " "); //Nombre de lignes
	*rows = atoi (pch);
	fprintf(stdout, "rows : %s\n",pch);
	fflush(stdout);
	pch = strtok (NULL, " "); //Niveau de gris max
	*max = atoi (pch);
	fprintf(stdout, "Max : %s\n",pch);
	fflush(stdout);



	image = (int**) malloc ((*rows)*sizeof(int*));
	for(i=0; i<(*rows); i++){
		image[i] = (int*) malloc((*cols)*sizeof(int));
	}
	while (pch != NULL)
	{
		pch = strtok (NULL, " ");
		if(pch != NULL){
			if(ind_ligne == (*rows))
				break;
			image[ind_ligne][ind_colonne++] = atoi(pch);
			if(ind_colonne == (*cols)){
				ind_colonne = 0;
				ind_ligne++;
			}
		}
	}
	fprintf(stdout, "Fin parsage #%d\n", nb_appel);
	fflush(stdout);
	nb_appel++;
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
	double min = distance( image[rows/2][0] );
	int i;
	for(i=0; i<cols-1; i++){
		min = min( min, distance(image[rows/2][i+1]));
	}
	return  min;
}

double max_distance_foward2(int** image, int rows, int cols){
	//On va calculer la distance des pixels de la ligne d'horizon
	double min = distance( image[rows/2][0] );
	int i;
	for(i=0; i<cols-1; i++){
		min = min( min, distance(image[rows/2][i+1]));
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
	int** image2;
	int cols, rows, max;

	FILE* log = fopen("log.txt","a+");

	int i,j;



	//Bug vient d'ici.
	//Pour récupérer l'image la premiere fois aucun problème, mais les fois suivantes,
	//on a l'erreur suivante:
	//mission_rover.pgr: bad magic number - not a pgm or pbm file
	//Alors que ce que devrait lire la fonction pgm_readpgm sur la stdin est bien un format pgm
	//Pour l'illustrer nous avons fait cette boucle
	// for(i=0; i<2; i++){
	// 	image = recup_camera(&cols, &rows, &max);
	// 	fprintf(stdout, "Données récupérés :\n\tNb colonnes : %d\n\tNb lignes : %d\n\tMaxGray : %d\n",cols,rows,max);
	fflush(stdout);
	// }
	// sleep(2);

	// Nous avons par la suiter choisi de parser directement la chaine de caractere lu sur le stdin



	//Algorithme de navigation
	while(1){
		image2 = recup_camera2(&cols, &rows, &max);
		pas = min (max_distance_foward2(image2, rows, cols) - EPSILON, distance_objectif(x,y));
		pas = min ( pas , 20.0);
		//On retire EPSILON car notre calcul de distance peut ne pas etre assez précis
		//On minimise le risque de se prendre un mur en minorant le pas 

		fprintf(stdout,"CMD FORWARD %g\n",pas);
		fflush(stdout);
		read_OK();

		calcul_nouvelle_position(&x, &y, pas, orientation);

		free_image(image2, rows);
		sleep(1);

		//Cas ou on rencontre un obstacle - (pas != (100-EPSILON) aussi dans le cas où le robot va
		// sur l'objectif mais le programme s'arrete dans ce cas là).
		//On tourne à droite, on avance de 5m ou moins, on se ré-oriente vers l'objectif.
		if(pas != (100-EPSILON)){
			//Tourne à droite
			orientation -= PI/3;
			fprintf(stdout,"CMD TURN %g\n",-PI/2);
			fflush(stdout);
			read_OK();

			// fprintf(stdout, "Avant recup_camera2 dans if\n");
			fflush(stdout);

			//Avance
			image2 = recup_camera2(&cols, &rows, &max);
			// fprintf(stdout, "Après recup_camera2 dans if\n");
			fflush(stdout);
			pas = min (max_distance_foward2(image2, rows, cols) - EPSILON, 20.0);
			fprintf(stdout,"CMD FORWARD %g\n",pas-EPSILON);
			fflush(stdout);
			read_OK();
			calcul_nouvelle_position(&x, &y, pas, orientation);

			free_image(image2, rows);

			//Se tourne vers l'objectif
			calcul_orientation_objectif(x, y, &orientation);
			fprintf(stdout,"CMD TURN %g\n",orientation);
			fflush(stdout);
			read_OK();
		}

		sleep(2);
	}

	sleep(3);
	exit(0);
}