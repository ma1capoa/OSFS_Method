#ifndef MinimoLadosOffLine_H
#define MinimoLadosOffLine_H
#include <limits> //http://www.cplusplus.com/reference/limits/numeric_limits/
#include <vector>
#include "algoritmo.hpp"
#include "nodoMinimoLados.hpp"
#include "heapvectorMinimoLados.hpp"
#include "serietemporal.hpp"


class MetodoMinimoLadosOffLine:public Algoritmo
{
	public:
		MetodoMinimoLadosOffLine(char * nombreFicheroSerieTemporal, int numeroInicialPuntosSerie, long double eMax, int limitePodaNumeroLados, vector<int> &limitePodaLAdosAcumulados,  
										long double limitePodaISE);
    //La variable numeroInicialPuntosSerie refleja el numero de puntos que tendra la serie inicialmente, a estos puntos se le irán añadiendo los siguientes
		//de forma progresiva.
		MetodoMinimoLadosOffLine(const SerieTemporal &c, int numeroInicialPuntosSerie, long double eMax, int limitePodaNumeroLados, vector<int> &limitePodaLAdosAcumulados,  
										long double limitePodaISE);
		
		~MetodoMinimoLadosOffLine();

		void aplicar(); //Método heredado que hay que implementar en esta clase
		char nombreFichero[100];
		
		long double & getISE()
		{
			return _ISE;
		};
		
		void setISE(long double & error)
		{
			_ISE = error;
		};
		
		long double & getTiempo()
		{
			return _tiempo;
		};
		
		void setTiempo(long double &tiempo)
		{
			_tiempo = tiempo;
		};
		
		int & getNumeroPuntosAproximacion()
		{
			return _numeroPuntosAproximacion;
		}
		
		void setNumeroPuntosAproximacion(int n)
		{
			_numeroPuntosAproximacion = n;
		}
		
		int & getNumeroInicialPuntosSerie()
		{
			return _numeroInicialPuntosSerie;
		}
		
		void setNumeroInicialPuntosSerie(int & n)
		{
			_numeroInicialPuntosSerie = n;
		}
		
		long double & getEMax()
		{
			return _eMax;
		}
		
		void setEMax(long double & eMax)
		{
			_eMax = eMax;
		}
		
		vector <int> & getVectorSerieTemporalOptimo()
		{
			return _serieTemporalOptimo;
		}
		
		void setVectorSerieTemporalOptimo(vector <int> & v)
		{
			_serieTemporalOptimo = v;
		}
		
		long double & getLimitePodaISE()
		{
			return _limitePodaISE;
		}
		
		void setLimitePodaISE( long double &limiteISE)
		{
			_limitePodaISE = limiteISE;
		}

		int & getLimitePodaNumeroLados()
		{
			return _limitePodaNumeroLados;
		}
		
		void setLimitePodaNumeroLados( int &limiteNumeroLados)
		{
			_limitePodaNumeroLados = limiteNumeroLados;
		}

		vector <int> & getLimitePodaNumeroLadosAcumulados()
		{
			return _limitePodaNumeroLadosAcumulados;
		}
		
		void setLimitePodaNumeroLadosAcumulados( vector <int> &limiteNumeroLadosAcumulados)
		{
			_limitePodaNumeroLadosAcumulados = limiteNumeroLadosAcumulados;
		}

		vector <long double> & getLimitePodaISEAcumulado()
		{
			return _limitePodaISEAcumulado;
		}
		
		void setLimitePodaISEAcumulado( vector <long double> &limitePodaISEAcumulado)
		{
			_limitePodaISEAcumulado = limitePodaISEAcumulado;
		}
		
		
	private:
		//el error maximo permitido en la segmentación
		long double _eMax;

		// numero de lados óptimo de la segmentación. Este es el valor que hay que minimizar, sin superar el _eMax.
		int _numeroLados;

		// ISE óptimo de la aproximacion poligonal, para el número de lados obtenido, con la condición del eMax.
		long double _ISE;

		//limite de poda para el ISE que se ha obtenido previamente en un método subóptimo.
		long double _limitePodaISE; 

		//límite de poda para el número de lados que se ha obtenido previamente en un método subóptimo
		int _limitePodaNumeroLados; //

		//Limite de poda para el número de lados acumulados que se ha obtenido previamente en un método subóptimo.
		vector<int> _limitePodaNumeroLadosAcumulados;

		//Limite de poda para el ISE acumulado que se ha obtenido para cada punto de la serie en el método subóptimo.
		vector <long double> _limitePodaISEAcumulado;
		
		
		//Variable nueva introducida para simular la segmentación online (Abril 2019)
		int _numeroInicialPuntosSerie;
		
		long double  _tiempo;

		int _numeroPuntosAproximacion; //numero de puntos de cada aproximación.
		
		vector <int> _serieTemporalOptimo; //Vector que almacenará el serieTemporal óptimo para la aproximación poligonal

		void obtenerPuntosSerieTemporalOptimo(SerieTemporal &serieTemporal, int ** padre, const int &numeroPuntosProbados, const int &numeroInicialPuntosSerie);
		
		void insertarCandidatosListaAbiertos(NodoMinimoLados nodoInicial, HeapVectorMinimoLados &listaAbierta, int **posicionNodo/*, HeapVectorMinimoLados &listaAbiertaNueva, int **posicionNodoListaNueva*/,
                                              bool **abierta, bool **cerrada, int &contadorTotalPodados, int &contadorNodosActualizados, int &contadorPodadosISE, int &contadorPodadosNumeroLados, int &ultimoPuntoSerie, long double **costeActual, int **padreActual,
                                              Matriz<long double> &suma_x, Matriz<long double> &suma_y, Matriz<long double> &suma_xx, Matriz<long double> &suma_yy, Matriz<long double> &suma_xy);
};
		

#endif