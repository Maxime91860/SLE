

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













	// Bug vient d'ici.
	// Pour récupérer l'image la premiere fois aucun problème, mais les fois suivantes,
	// on a l'erreur suivante:
	// mission_rover.pgr: bad magic number - not a pgm or pbm file
	// Alors que ce que devrait lire la fonction pgm_readpgm sur la stdin est bien un format pgm
	// Pour l'illustrer nous avons fait cette boucle
	for(i=0; i<2; i++){
		image = recup_camera(&cols, &rows, &max);
		fprintf(stdout, "Données récupérés :\n\tNb colonnes : %d\n\tNb lignes : %d\n\tMaxGray : %d\n",cols,rows,max);
		fflush(stdout);
	}
	sleep(2);

	// Nous avons par la suiter choisi de parser directement la chaine de caractere lu sur le stdin




	while(strcmp(pch, "P2") != 0){
		pch = strtok (photo," "); //Magic Number
		vide_stdin();
		free(photo);
		photo = recup_camera_char();
		pch = strtok (photo," "); //Magic Number
		fprintf(stderr, "Magic number : _%s_\n",pch);
		fflush(stderr);
	}


	fprintf(stderr, "Magic number : _%s_\n",pch);
	fflush(stderr);
	fprintf(stderr, "Cols : %s\n",pch);
	fflush(stderr);
	fprintf(stderr, "rows : %s\n",pch);
	fflush(stderr);
	fprintf(stderr, "Max : %s\n",pch);
	fflush(stderr);	