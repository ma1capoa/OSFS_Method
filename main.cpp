#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <unistd.h>                    /// getopt
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <limits>
#include <sys/time.h> //para la función gettimeofday 

#include "algoritmo.hpp"
#include "metodoFSW.hpp"
#include "metodoSerieTemporalEMaxOffLine.hpp"
#include "serietemporal.hpp"
#include "matriz.hpp"
#include "clock.hpp"
//#ifdef USE_OPENMP
//#include <omp.h>
//#define CHUNKSIZE 1
//#endif //USE_OPENMP

void metodoEMax(char ficheroSerieTemporal[], SerieTemporal& original, long double  eMax);

using namespace std;


void help(void)
{
    cout << "Usage:" << endl << endl;
    cout << "\t-h:\t\t\tPrints this help." << endl;
    cout << "\t-f <time_series_file>:\time series file used for calculating the optimum segmentation." << endl;
    cout << "\t-e <max_error>:\tMaximum error allowed in the segmentation" << endl;
}


int main(int argc, char *argv[])
{
	/// file to use
  	char* timeSeriesFile = NULL;

  	/// Time series eMax
  	long double  eMax = 0;
	

  ///Menu option
	int c;

  while ((c = getopt(argc, argv, "hf:e:")) != -1)
    switch (c) {
    case 'h':
        help();
        exit(0);
        break;
    case 'f':
        timeSeriesFile = optarg;
        break;
    case 'e':
        eMax = (long double) atof(optarg);
        break;
    default:
        abort();
    }
	if (not timeSeriesFile)
	{
    	help();
    	cerr << "Time series file was not specified" << endl;
    	abort();
	}
	else
	{
		SerieTemporal original(timeSeriesFile);
		metodoEMax(timeSeriesFile, original, eMax);
	}
  return 0;
  
  
}



void metodoEMax(char ficheroSerieTemporal[], SerieTemporal& original, long double  eMax)
{
	Clock time;
	long double tAux;
	long double tAcumulado = 0.0;
	
	long double isePoda = 0.0;
	int numeroLadosPoda = 0;
	int initialNPS = original.numeroPuntosSerieTemporal();
	
	MetodoFSW *m;
	m = new MetodoFSW(ficheroSerieTemporal, eMax, initialNPS);
	

	time.start();
	m->aplicar();
	time.stop();
	tAux = time.elapsed();
	tAcumulado += tAux /1000.0;

	isePoda =  m->errorISE();
	numeroLadosPoda = m->numeroPuntosAproximacion();
	vector <int> numeroLadosAcumuladosPoda = m->ladosAcumulados();

	MetodoMinimoLadosOffLine *p;

	//Definimos el serietemporal a usar
	SerieTemporal actual(m->serieTemporal());  //Este serietemporal ya lleva marcados los dominantes obtenidos por el método FSW
  
 
	p = new MetodoMinimoLadosOffLine(actual, initialNPS, eMax, numeroLadosPoda, numeroLadosAcumuladosPoda, isePoda);
  	vector <int> dominantes;
	time.start();
	p->aplicar();
  	time.stop();
	//calculamos el tiempo en milisegundos
	tAux = time.elapsed();
  	tAcumulado += tAux / 1000.0;
	
	
  	double RSME = sqrt(p->getISE() / initialNPS);
  	dominantes = p->getVectorSerieTemporalOptimo();
	cout << "Results for file " << ficheroSerieTemporal << "\n";
  	cout << "=================================================\n";
  	std::cout << "Number of points of time series: " << initialNPS << "\n";
	std::cout << "RSME = " << RSME  << "\n";
	std::cout << "Maximum error = " << p->getEMax() << "\n";
	std::cout << "Number of cut points = " << p->getNumeroPuntosAproximacion() << "\n";
	std::cout << "Computational time (ms) = " << tAcumulado<< endl;
	
	std::cout << "CUT POINTS (Index or position): ";
	for (unsigned int i = 0; i < dominantes.size(); i++)
		cout << dominantes[i] << ",";
	std::cout << std::endl;
	
	
}

