

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
#define EPSILON 0.80

// #define min(a,b) (a<=b?a:b) //Déja défini dans /usr/include/pm.h

//Taille robot 50cm x 50 cm x 50 cm
int taille_image = 640*360;
int taille_buffer_image = 640*360*5;
int nb_appel = 1;

void vide_stdin(){
	char buffer[taille_buffer_image];
	read(0, buffer, taille_buffer_image);
}

void read_OK(){
	char buffer[10];
	read(0, buffer, 10);
}

//Récupère l'image sous forme d'une chaine de caractère
char* recup_camera_char(){
	char * photo = (char *) malloc (640*360*4*sizeof(char));
	// fprintf(stdout, "APPEL %f\n", nb_appel);
	sleep(0.5);

	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);

	read(0, photo, taille_buffer_image);

	nb_appel++;
	return photo;
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
	// fprintf(stderr, "Debut parsage #%f\n", nb_appel);
	char* photo = recup_camera_char();
	char * pch;
	int** image;
	int i;
	int ind_ligne = 0;
	int ind_colonne = 0;

	nb_appel--;

	pch = strtok (photo," "); //Magic Number
	pch = strtok (NULL, " "); //Nombre de colonnes
	*cols = atoi (pch);
	pch = strtok (NULL, " "); //Nombre de lignes
	*rows = atoi (pch);
	pch = strtok (NULL, " "); //Niveau de gris max
	*max = atoi (pch);



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
	// fprintf(stderr, "Fin parsage #%f\n", nb_appel);
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
double calcul_orientation_objectif(double x, double y, double* orientation){
	double delta;
	double angle_objectif = atan(y/x);
	delta = angle_objectif - (*orientation);
	*orientation = *orientation + delta;
	return delta;
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
// double max_distance_foward(gray** image, int rows, int cols){
// 	//On va calculer la distance des pixels de la ligne d'horizon
// 	double min = distance( image[rows/2][0] );
// 	int i;
// 	for(i=0; i<cols-1; i++){
// 		min = min( min, distance(image[rows/2][i+1]));
// 	}
// 	return  min;
// }

double max_distance_foward2(int** image, int rows, int cols){
	//On va calculer la distance des pixels de la ligne d'horizon
	double min = distance( image[(rows/2)][0] );
	int i;
	for(i=0; i<cols-1; i++){
		min = min( min, distance(image[(rows/2)][i+1]));
	}
	return  min;
}

void retourne(int** image, int* rows, int* cols, int* max, double* x, double* y, double* orientation){

	//Recule

	fprintf(stdout, "CMD TURN %f\n", -PI);
	fflush(stdout);
	vide_stdin();

	image = recup_camera2(cols, rows, max);
	int pas = min (max_distance_foward2(image, *rows, *cols), 4.0 );
	free_image(image, *rows);

	fprintf(stdout, "CMD FORWARD %g\n", pas - EPSILON);
	fflush(stdout);
	vide_stdin();

	fprintf(stdout, "CMD TURN %f\n", PI);
	fflush(stdout);
	vide_stdin();

	calcul_nouvelle_position(x, y, pas - EPSILON, *orientation);

}

//On va regarder la moyenne des pixels à gauche ou à droite pour prendre notre decision
double gauche_ou_droite (int** image, int rows, int cols){
	double somme_gauche = image[(rows/4)-20][0];
	double somme_droite = image[(rows/4)-20][(cols/3)*2];
	double moyenne_gauche;
	double moyenne_droite;

	int i;
	for(i=1; i<=(cols/3)-1; i++){
		somme_gauche += image[(rows/4)-20][i];
		somme_droite += image[(rows/4)-20][((cols/3)*2)+i];
	}

	moyenne_droite = somme_droite / ((cols/3)-1);
	moyenne_gauche = somme_gauche / ((cols/3)-1);

	fprintf(stderr, "--- MOYENNE GAUCHE : %g, MOYENNE DROITE : %g ---\n", moyenne_gauche, moyenne_droite);
	//On va à droite
	if(moyenne_droite <= moyenne_gauche){
		fprintf(stderr, "--> DROITE\n");
		return -PI/2;
	}
	//On va à gauche
	fprintf(stderr, "--> GAUCHE\n");
	return PI/2;

}

int main (int argc, char** argv){

	//Variables positionnement et deplacement du robot
	double x = COORD_X_INIT;
	double y = COORD_Y_INIT;

	double orientation = ORIENTATION_INIT;
	double delta;
	double pas;

	//Variables images
	gray **image;
	int** image2;
	int cols, rows, max;

	// FILE* log = fopen("log.txt","w+");


	//Algorithme de navigation
	while(1){
		// do {
			image2 = recup_camera2(&cols, &rows, &max);
			pas = min (max_distance_foward2(image2, rows, cols), distance_objectif(x,y));
			free_image(image2, rows);
			pas = min ( pas , 20.0);

			//On retire EPSILON car notre calcul de distance peut ne pas etre assez précis
			//On minimise le risque de se prendre un mur en minorant le pas 

			if(pas < 10.0 && pas != distance_objectif(x,y)){
				fprintf(stderr, "--OBSTACLE trop près, le rover recule--\n");
				retourne(image2, &rows, &cols, &max, &x, &y, &orientation);
			}
			else{
				fprintf(stderr, "--Avance vers l'OBJECTIF--\n");
				fprintf(stdout,"CMD FORWARD %g\n",pas - EPSILON);
				fflush(stdout);
				vide_stdin();

				calcul_nouvelle_position(&x, &y, pas - EPSILON, orientation);

				// fprintf(log, "Après CMD FORWARD OBJ %g\n",pas-EPSILON);
				// fprintf(log, "x=%g, y=%g, orientation=%g\n\n",x,y,orientation);
				// fflush(log);
			}

			if(pas == distance_objectif(x,y)){
				fprintf(stderr, "Robot sur objectif\n");
				exit(0);
			}


		// }while(pas > 6.0);
		sleep(0.5);

		//Cas ou on rencontre un obstacle - (pas != (100-EPSILON) aussi dans le cas où le robot va
		// sur l'objectif mais le programme s'arrete dans ce cas là).
		//On tourne à droite, on avance de 5m ou moins, on se ré-oriente vers l'objectif.
		if(pas < 20.0){

			fprintf(stderr, "\n\n-------------------------------\n-------------------------------\n");
			fprintf(stderr, "-----OBSTACLE DROIT DEVANT-----");
			fprintf(stderr, "\n-------------------------------\n-------------------------------\n\n");
			//Tourne à droite ou à gauche
			image2 = recup_camera2(&cols, &rows, &max);
			double angle_evit = gauche_ou_droite(image2, rows, cols);
			// angle_evit = -PI/2;
			free_image(image2, rows);

			orientation = orientation + angle_evit;
			fprintf(stderr, "--Se tourne pour eviter l'OBSTACLE--\n");
			fprintf(stdout,"CMD TURN %f\n", angle_evit);
			fflush(stdout);
			vide_stdin();

			// fprintf(log, "CMD TURN OBS %f\n", angle_evit);
			// fprintf(log, "x=%g, y=%g, orientation=%g\n\n",x,y,orientation);
			// fflush(log);

			sleep (0.5);


			//Avance
			image2 = recup_camera2(&cols, &rows, &max);
			pas = min (max_distance_foward2(image2, rows, cols) - EPSILON, 25.0);


			if(pas-EPSILON > 1.0){
				fprintf(stderr, "--Avance pour eviter l'OBSTACLE--\n");
				fprintf(stdout,"CMD FORWARD %g\n",pas-EPSILON);
				fflush(stdout);
				vide_stdin();

				calcul_nouvelle_position(&x, &y, pas, orientation);

				// fprintf(log, "Après CMD FORWARD OBS %g\n",pas-EPSILON);
				// fprintf(log, "x=%g, y=%g, orientation=%g\n\n",x,y,orientation);
				// fflush(log);

				sleep(0.5);
			}

			free_image(image2, rows);



			//Se tourne vers l'objectif
			delta = calcul_orientation_objectif(x, y, &orientation);

			fprintf(stderr, "--Se tourne vers l'OBJECTIF--\n");
			fprintf(stdout,"CMD TURN %f\n",delta);
			fflush(stdout);
			vide_stdin();


			fprintf(stderr, "\n\n");

			// fprintf(log, "CMD TURN OBJ %f\n", delta);
			// fprintf(log, "x=%g, y=%g, orientation=%g\n\n",x,y,orientation);
			// fflush(log);
		}

		sleep(0.5);
	}

	// fflush(log);

	sleep(3);
	exit(0);
}