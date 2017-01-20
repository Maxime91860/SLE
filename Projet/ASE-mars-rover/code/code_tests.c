

	/* Bouts de codes ayant servis pour les test */

void calcul_prochain_mouvement (double x, double y, double orientation, char* vue){
	// fprintf(stdout,"CMD FORWARD 5.0\n");
	// fflush(stdout);
}

	//Fichiers de log
	FILE* log = fopen("log.txt","a+");


	// Récupération de l'image
	char image2[taille_buffer_image];
	recup_camera_char(image2);
	fprintf(log, "%s\n", image2);
	fprintf(log, "Lu sur la stdin : \n%s\n", image2);
	fflush(log);

	//Verif fonctions distances
    fprintf(log, "Test calcul distance pour pixel max du robot : %g\n",max_distance_foward(image, rows, cols));
    fprintf(log, "Test calcul distance pos : %g\n",distance_objectif(x,y));


	fprintf(log, "x = %f, y = %f, orientation = %f\n", x, y, orientation);

	//Verif fonctions  PGM
	fprintf(log, "---- TEST PGM ----\n");
	fprintf(log, "Données récupérés :\n\tNb colonnes : %d\n\tNb lignes : %d\n\tMaxGray : %d\n",cols,rows,max);
	fflush(log);

    for (i=0; i<rows; i++){
        for (j=0; j<cols; j++){
            fprintf(log, "%u ", image[rows/2][j]);
        }
        fprintf(log, "\n");
    }

    sleep(1);