

#include <stdlib.h>
#include <math.h>


void init_tableau (float* tableau, int N){
	int i;
	float rand;
	for (i=0; i<N; i++){
		rand = (float) rand();
		tableau[i] =  rand%(2*M_PI);
	}
}

int main (int argc, char **argv){
	float* tableau;
	tableau = (float *) malloc (10000*sizeof(float));

	init_tableau(tableau,10000);


	exit(0);
}
