#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

/*
Explicacion detallado de las funciones mas adelante
*/
int escribir_imagen(double **matriz_imagen, char* nombre_salida, int size_imaginario, int size_real);
void paralelo(double **matriz_imagen, int size_imaginario, int size_real, double lim_sup_i, double lim_inf_r, double muestreo, int depth, int hebras);

int main(int argc, char *argv[]){

	//se declaran las funciones necesarias para el getopt
	char c;
	int depth, hebras;
	double muestreo, lim_inf_r, lim_inf_i, lim_sup_r, lim_sup_i;
	char *nombre_salida;


	while((c=getopt(argc,argv,"i:a:b:c:d:s:f:t:"))!=-1)
	{
		//Se genera un switch con case respecto a cada variable identificatoria.
		switch (c)
		{
			case 'i':
				depth = atoi(optarg);
				break;
			case 'a':
				lim_inf_r = atof(optarg);
				break;
			case 'b':
				lim_inf_i = atof(optarg);
				break;
			case 'c':
				lim_sup_r = atof(optarg);
				break;
			case 'd':
				lim_sup_i = atof(optarg);
				break;
			case 's':
				muestreo = atof(optarg);
				break;
			case 'f':
				nombre_salida = optarg;
				break;
			case 't':
				hebras = atoi(optarg);
				break;
		}
	}

	//Se detecta el tamano de la matriz que sera creada
	int size_imaginario = rint(fabs(lim_inf_i-lim_sup_i)/muestreo);
	int size_real = rint(fabs(lim_inf_r-lim_sup_r)/muestreo);

	//Se inicializa la cantidad de filas
	double** matriz_imagen = (double **) malloc (size_imaginario*sizeof(double*));

    //Crear las columnas en cada una de las filas
    for(int i=0;i<size_real;i++){
		matriz_imagen[i] = (double*) malloc (size_real*sizeof(double)); 
    }

    //se realiza el calculo paralelo del proceso mandelbrot
    paralelo(matriz_imagen, size_imaginario, size_real, lim_sup_i, lim_inf_r, muestreo, depth, hebras);
    //Es ecrito el archivo de salida
    escribir_imagen(matriz_imagen, nombre_salida, size_imaginario, size_real);
}

/*
Funcion encargada de realizar la generacion de imagenes Mandelbrot de manera paralela
Entrada:
	-matriz_imagen: matriz que contendra la imagen creada
	-size_imaginario: cantidad de filas en la matriz
	-size_real: cantidad de columnas en la matriz 
	-lim_sup_i: limite superior de parte imaginaria
	-lim_inf_r: limite inferior de parte real
	-muestreo: muestreo utilizado para generar la matriz
	-depth: limite del valor de n
	-hebras: cantidad de hebras utilizadas
*/
void paralelo(double **matriz_imagen, int size_imaginario, int size_real, double lim_sup_i, double lim_inf_r, double muestreo, int depth, int hebras){
	
	//Seteo numero de hebras
    omp_set_num_threads(hebras);

    //se inicializan las variables asociadas al proceso mandelbrot
	double x, y, z0_r, z0_i, zn_r, zn_i, zn_1r, zn_1i;
    int n, i, r;

    //se inicia el proceso paralelo y se indica las variables compartidas y lñas privadas para cada una de las hebras
	#pragma omp parallel shared(matriz_imagen, size_imaginario, size_real, lim_sup_i, lim_inf_r, muestreo, depth) private(x, y, z0_r, z0_i, zn_r, zn_i, zn_1r, zn_1i, n, i, r)
    {
    	//Se indica que el for que recorre la parte imaginaria, es decir, las filas es de manera paralela
    	#pragma omp for
    	//se comienza el proceso para cada uno de los pixeles de la imagen
	    for(i=0;i<size_imaginario;i++){
	    	x = lim_inf_r;
	    	y = lim_sup_i - muestreo*i;
	    	for(r=0;r<size_real;r++){
	    		z0_r = 0;
	    		z0_i = 0;
	    		n = 1;
	    		zn_r = z0_r + x;
	    		zn_i = z0_i + y;
	    		while(sqrt(zn_r*zn_r +  zn_i*zn_i) < 2 && n < depth){
	    			zn_1r = zn_r*zn_r - zn_i*zn_i + x;
	    			zn_1i = 2*zn_r*zn_i + y;
	    			zn_r = zn_1r;
	    			zn_i = zn_1i;
	    			n = n + 1;
	     		}

	     		matriz_imagen[i][r] = log(n) + 1;
	     		x = x + muestreo;
	    	}
	    }
	    //Se espera a que todas las hebras terminen
	    #pragma omp barrier
	}
}

/*
Funcion encargada de realizar la escritura de la imagen en el archivo por
el nombre ingresado por el usuario
Entrada:
	-matriz_imagen: matriz de imagen luego del proceso secuencial
	-nombre_salida: nombre de la imagen a guardar
	-size_imaginario: cantidad de filas en la matriz
	-size_real: cantidad de columnas en la matriz 

salida:
	- 1 si el archivo fue escrito de manera correcta, 0 en otro caso
*/
int escribir_imagen(double **matriz_imagen, char* nombre_salida, int size_imaginario, int size_real){
	
	FILE *imagen_raw;

	imagen_raw = fopen(nombre_salida, "wb");

	if(imagen_raw == NULL){
		printf("Error en escritura de archivo\n");
		return 0;
	}
	float *linea = (float *) malloc(sizeof(float)*size_real);
    //aumentar tamano de columnas
    for(int i=0;i<size_imaginario;i++){
    	for(int j=0;j<size_real;j++){
    		linea[j] = (float)matriz_imagen[i][j];
    	} 
		fwrite(linea, sizeof(float), size_real, imagen_raw);
    }

    fclose(imagen_raw);
    return 1;
}
