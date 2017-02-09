

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










//Essai 1 strtok
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






	//Récupère l'image sous forme d'une chaine de caractère, et l'écrit dans un fichier
void recup_camera_char(){
	fprintf(stdout,"CMD CAMERA\n");
	fflush(stdout);
	read(0, photo, taille_buffer_image);
	// FILE* photo_fd = fopen("image.txt","w");
	// fprintf(photo_fd, "%s\n", photo);
	// return photo;
}




		fprintf(log, "max distance : %g, distance objectif %g\n",
			max_distance_foward(image,rows,cols),distance_objectif(x,y) );


	
	pgm_init(&argc, argv);
	int differences = 0;
	image = recup_camera(&cols, &rows, &max);
	image2 = recup_camera2(&cols, &rows, &max);
	for(i=0; i < rows; i++){
		for(j=0; j<cols; j++){
			if (image[i][j] != image2[i][j]){
				differences++;
			}
		}
	}
	fprintf(stdout, "Nombre de differences : %d\n", differences);
	fflush(stdout);

	double dist = max_distance_foward(image, rows, cols);
	double dist2 = max_distance_foward2(image2, rows, cols);
	fprintf(stderr, "Distance = %g, Distance2 = %g\n",dist, dist2);