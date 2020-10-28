#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <sys/time.h> //para la función gettimeofday
#include <vector>    // necesario para el contenedor vector
#include <algorithm>
#include "algoritmo.hpp"
#include "metodoFSW.hpp"
#include "matriz.hpp"
#include "punto.hpp"
#include "recta.hpp"
//#include "operacionescadenas.hpp"
#include "serietemporal.hpp"
//#include "heapvectorMasood.hpp"
//#include "nodoMasood.hpp"



using namespace std;

/* FUNCIONES DE INTERFAZ PARA EL TIPO MetodoPerezVidal */

//Constructor.

MetodoFSW::MetodoFSW(char *nombreFicheroSerieTemporal, long double eMax, int initial)
{
  //Creamos un serieTemporal temporal donde se carga el serieTemporal
	SerieTemporal c(nombreFicheroSerieTemporal);
        
        
  	serieTemporal(c);
	errorMaximo(eMax);
	initialNPS(initial);

	//Introducimos el nombre del fichero en la variable 
  	strcpy(nombreFichero, nombreFicheroSerieTemporal);
}

//Destructor
MetodoFSW::~MetodoFSW()
{
}

void MetodoFSW::aplicar()
{
  //int numeroPuntosDominantes, inicial;
 	long double sumaErroresCuadrado = 0.0, eMaximo = 0.0;
  //int opcion;

 	/* copiamos el serieTemporal en el serieTemporal nuevo */
	SerieTemporal serieTemporalNuevo(serieTemporal());	

	//Reservamos espacio para los puntos dominantes
 	serieTemporalNuevo.reservarPuntoDominanteSerieTemporal();

/* fase de cÃ¡lculo */
	//Marcamos todos los puntos como no dominantes
	serieTemporalNuevo.marcarTodosPuntosNoDominantes();

  	vector <int> dominantes; //Se va a usar un vector para guardar los indices de los puntos de segmentación. No se va a usar la posición 0 del vector.
  
  	dominantes.push_back(-1); //La posición 0 se inicializa con -1.
  	dominantes.push_back(1); //El primer punto de la segementación es el primero de la serie.
            
            
  	//Ahora el primer y ultimo punto del serieTemporal se marcan como dominantes, ya que es una serie temporal y siempre han de pertenecer a la aproximación.
            
  	serieTemporalNuevo.dominantePunto(1, true);
  	serieTemporalNuevo.dominantePunto(initialNPS(), true);
  
	int i = 1;
	int candidatoPuntoSegmentacion = 1;
  	int ultimoPuntoSegmentacion = 1;
	int numeroSegmentos = 0;
	long double lowM, upM;
	lowM = -10000000.0; //Pendiente del limite inferior del espacio factible
	upM = 10000000.0; //Pendiente del limite superior del espacio factible
	Punto puntoInicial = serieTemporalNuevo.puntoSerieTemporal(i); //Punto inicial del segmento a considerar
	Punto puntoFinal;  //Punto final del segmento a considerar
	Punto puntoFinalInferior, puntoFinalSuperior; //Límites inferior y superior del punto final del segmento a considerar.
	long double mInferior, mSuperior; //pendientes para los límites inferior y superior
	//Ahora comenzamos a calcular los segmentos en que se va a descomponer la seri usando el método FSW
  	while (i < initialNPS()) // Mientras que i no llegue al número inicial de puntos de la serie seguimos dividiéndola en segmentos que no superen el eMaximo
	{
		i++;
		puntoFinal = serieTemporalNuevo.puntoSerieTemporal(i);
		//Se calculan los limites inferior y superior del último extremo del segmento.
		puntoFinalInferior = puntoFinal;
		puntoFinalInferior.y(puntoFinal.y() - errorMaximo());
		puntoFinalSuperior = puntoFinal;
		puntoFinalSuperior.y(puntoFinal.y() + errorMaximo());

		//Se calculan las pendientes de las rectas que conforman los límites.
		Recta low(puntoInicial, puntoFinalInferior);
		Recta up(puntoInicial, puntoFinalSuperior);
		mInferior = low.m();
		mSuperior = up.m();

		//Se recalculan los límites superior e inferior al añadir el nuevo punto
		if (mInferior > lowM)
			lowM = mInferior;
		if (mSuperior < upM)
			upM = mSuperior;

		if (upM < lowM) //Hay que añadir un nuevo punto de segmentación.
		{
			numeroSegmentos++;
			serieTemporalNuevo.dominantePunto(candidatoPuntoSegmentacion, true);
			i = candidatoPuntoSegmentacion;
			dominantes.push_back(i); //Se almacena el punto de segmentación al vector de dominantes.
			ultimoPuntoSegmentacion = candidatoPuntoSegmentacion;
			puntoInicial = serieTemporalNuevo.puntoSerieTemporal(ultimoPuntoSegmentacion); //Ahora cambia el punto inicial para el siguiente segmento
			lowM = -10000000.0; //Pendiente del limite inferior del espacio factible
			upM = 10000000.0; 
		}
		else //Ahora se comprueba si el punto i es candidato a punto de segmentación.
		{
			Recta r(serieTemporalNuevo.puntoSerieTemporal(ultimoPuntoSegmentacion), serieTemporalNuevo.puntoSerieTemporal(i));
			if (lowM <= r.m() and r.m() <= upM)
			candidatoPuntoSegmentacion = i;
		}

		//En el caso de que estemos en el último punto y este no sea candidato
		//hay que comenzar de nuevo desde el último candidato, añadiendo éste como punto de segmentación
		if(i == initialNPS() && candidatoPuntoSegmentacion != initialNPS())
		{
			serieTemporalNuevo.dominantePunto(candidatoPuntoSegmentacion, true);
			numeroSegmentos++;
			dominantes.push_back(candidatoPuntoSegmentacion);
			i = candidatoPuntoSegmentacion;
			ultimoPuntoSegmentacion = candidatoPuntoSegmentacion;
			puntoInicial = serieTemporalNuevo.puntoSerieTemporal(ultimoPuntoSegmentacion); //Ahora cambia el punto inicial para el siguiente segmento
			lowM = -10000000.0; //Pendiente del limite inferior del espacio factible
			upM = 10000000.0; 
	    }
  	}
  
  
  
  	//Ahora se comprueba si el último punto de la serie ha sido seleccionado o no. 
 	 //En caso de que no haya sido seleccionado, hay que marcarlo como seleccionado.
  	if(dominantes[dominantes.size()-1] != initialNPS())
    	dominantes.push_back(initialNPS());
  
	//Se guarda el vector de dominantes
	vectorDominantes(dominantes);
	  
	//Se guarda el número de puntos de la aproximación.
	numeroPuntosAproximacion(dominantes.size()-1);
	  
	//Calculamos el error máximo de la serie y el ISE para guardarlos.
	serieTemporalNuevo.diferenciaVerticalSerieTemporalDominantesReales(dominantes, sumaErroresCuadrado, eMaximo);
	errorISE(sumaErroresCuadrado);
	errorMaximo(eMaximo);

  	//Finalmente se calculan los lados acumulados de la solución hasta cualquier punto de la serie
    calcularLadosAcumulados();

}

void MetodoFSW::calcularLadosAcumulados()
{
	vector <int> lados; //vector en el que se guardarán los lados acumulados.
	vector <int> dominantes = vectorDominantes();  //Vector que contiene los puntos de segmentación 
	lados.push_back(1); //Indexamos vector desde 1, y la posición 0 la inicializamos a 1.
	int contadorLados = 1;

	//Ahora recorremos el vector de dominantes para ir calculando el número de lados acumulados
	for(unsigned int i = 1; i < dominantes.size() -1; i++)
	{
		for(int j = dominantes[i]; j <= dominantes[i + 1]; j++)
		{
		  lados.push_back(contadorLados);
		}
		contadorLados++;
	}
	ladosAcumulados(lados); //Finalmente se guardan los lados acumulados.
}

 


