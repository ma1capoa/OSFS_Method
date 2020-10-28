#ifndef FSW_H
#define FSW_H

#include <vector>    // necesario para el contenedor vector
#include "algoritmo.hpp"
#include "serietemporal.hpp"
//#include "matriz.h"

class MetodoFSW:public Algoritmo
{
	public:
		MetodoFSW(char * nombreFicheroSerieTemporal, long double eMax, int inicialNPS);
		~MetodoFSW();

		void aplicar(); //Método heredado que hay que implementar en esta clase
		char nombreFichero[1000];
		
		int & numeroPuntosAproximacion()
		{
			return _numeroPuntosAproximacion;
		};
		
		void numeroPuntosAproximacion(int  n)
		{
			_numeroPuntosAproximacion = n;
		};
		
    //Numero de puntos iniciales de la serie.
		int & initialNPS()
		{
			return _initialNPS;
		};
		
		void initialNPS(int & n)
		{
			_initialNPS = n;
		};
		
		long double & errorISE()
		{
			return _errorISE;
		};
		
		void errorISE(long double & error)
		{
			_errorISE = error;
		};
		
		long double & errorMaximo()
		{
			return _errorMaximo;
		};
		
		void errorMaximo(long double & error)
		{
			_errorMaximo = error;
		};
		
		long double  tiempo()
		{
			return _tiempo;
		};
		
		void tiempo(long double tiempo)
		{
			_tiempo = tiempo;
		};
		
		void serieTemporalDominantes(const SerieTemporal & c)
		{
		  	_serieTemporalDominantes = c;
		}
		
		SerieTemporal serieTemporalDominantes()
		{
		  	return _serieTemporalDominantes;
		}
		
		void vectorDominantes(const vector <int> &v)
	    {
	      	_vectorDominantes = v;
	    }	
			
		//Devuelve un vector que contiene las posiciones de los puntos dominantes o de la aproximación, indexado desde 1 (el 0 no se usa)
		vector <int> vectorDominantes()
	    {
	      	return _vectorDominantes;
	    }

	    void ladosAcumulados(const vector<int> &ladosAcumulados)
	    {
	    	_ladosAcumulados = ladosAcumulados;
	    }

	    vector <int> ladosAcumulados()
	    {
	    	return _ladosAcumulados;
	    }
    
    
    
    
      
	private:
		int _numeroPuntosAproximacion;
		int _initialNPS; //Variable introducida para reflejar el numero de puntos inicial de la serie y poder simular la segmentación off line.
		long double _errorISE;
		long double _errorMaximo;
		long double _tiempo;
		vector <int> _ladosAcumulados; //Este vector guarda el número de lados acumulados de la solución hasta un punto dado de la serie. Se va a usar como posible valor de poda  
		SerieTemporal _serieTemporalDominantes;
    	vector <int> _vectorDominantes;
    	void calcularLadosAcumulados();
    
};
		

#endif
