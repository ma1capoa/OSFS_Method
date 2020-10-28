#ifndef CONTORNO_H
#define CONTORNO_H
   
#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include "punto.hpp"
#include "matriz.hpp"

using namespace std; //Para usar Vector de la STL

/* Estructura del serieTemporal, que contendr� n puntos */
class SerieTemporal
{
	public:
		SerieTemporal(); //Es necesario en algunos casos.
		SerieTemporal(const int &n);	
		SerieTemporal(char *nombreFichero);
		SerieTemporal(const SerieTemporal &serieTemporal); //Constructor de copia
		~SerieTemporal();
		SerieTemporal & operator=(const SerieTemporal &serieTemporal); //Sobrecarga de la asignacion
		inline int numeroPuntosSerieTemporal()const //Observador
    {
			return _numeroPuntos;
		}
		inline Punto centroSerieTemporal()const //Observador
		{
 			return _centro;
		}

		inline void centroSerieTemporal(Punto p)  //Modificador
		{
 			_centro = p;
		}

		inline void numeroPuntosSerieTemporal(const int &n) //Modificador
		{
 			_numeroPuntos = n;
		}

		inline Punto puntoSerieTemporal(const int &posicion)const //Observador
		{
			return _puntoSerieTemporal[posicion - 1];
		}

		inline void puntoSerieTemporal(const Punto &p, const int &posicion) //Modificador
		{
			_puntoSerieTemporal[posicion - 1] = p; 
		}

		inline int codigoCadenaPunto(const int &posicion)const //Observador
		{
 			return _codigoCadena[posicion - 1];
		}

		inline void codigoCadenaPunto(const int &posicion, const int &codigo)  //Modificador
		{
 			_codigoCadena[posicion - 1] = codigo;
		}

		inline bool dominantePunto(const int &posicion)const //Observador
		{
 			return _dominante[posicion - 1];
		}

		inline void dominantePunto(const int &posicion, const bool &dominante)	//Modificador
		{
 			_dominante[posicion - 1] = dominante;
		}
		
		//Esta función comprueba si el serieTemporal es cerrado. Añadida en julio de 2017
		inline bool esCerrado() const
    		{
      			return _puntoSerieTemporal[0] == _puntoSerieTemporal[numeroPuntosSerieTemporal() -1];
    		}
	
		void reservarPuntosSerieTemporal();
		void reservarCodigoCadenaSerieTemporal();
		bool estaReservadoDominante()const;
		void reservarPuntoDominanteSerieTemporal();
		long double longitudSerieTemporal()const;
		long double longitudSerieTemporalEntrePuntosDominantes()const;
		int contarRegistrosFicheroSerieTemporal(char *nombreFichero)const;
		void inicializarSerieTemporal();
		void leerPuntosFichero(char *nombreFichero);
		void calcularCentroSerieTemporal();
		void guardarSerieTemporal(char *nombre)const;
    	void guardarAproximacion(char *nombre)const;
		void cargarSerieTemporal(char *nombre);
		void crearSerieTemporal();
		void liberarSerieTemporal();
		void coordenadasPolaresCentro();
		void codigoCadenaSerieTemporal();
		void coordenadasCartesianas();
		void coordenadasCartesianasCodigoCadena();
		void marcarTodosPuntosDominantes();
		void marcarTodosPuntosNoDominantes();
		void marcarDominantesIncremento(int incremento);
		int contarPuntosDominantes()const;
		void diferenciaSerieTemporalDominantesReales(long double &sumaErrorCuadrado, long double &errorMaximo)const;
    	void diferenciaVerticalSerieTemporalDominantesReales(long double &sumaErrorCuadrado, long double &errorMaximo)const;
    	void diferenciaVerticalSerieTemporalDominantesReales(const vector <int> &dominantes, long double &sumaErrorCuadrado, long double &errorMaximo)const;
		long double calcularIseEntreDospuntos(int inicial, int final)const;
    	long double calcularIseVerticalEntreDosPuntos(int inicial, int final)const;
		void  calcularEmaxEntreDosPuntos(int inicial, int final, long double &errorMaximo, int &posicion)const;
		long double calcularEmaxVerticalEntreDosPuntos(int inicial, int final) const;
		long double calcularErrorMedioVerticalEntreDosPuntos(int inicial, int final) const;
		long double calcularErrorMedioErroresMediosVerticales(const vector <int> &dominante) const;
		long double calcularErrorMaximoErroresMediosVerticales(const vector <int>  &dominante) const;
		long double calcularErrorMedioErroresMaximosVerticales(const vector <int>  &dominante) const;
		long double calcularErrorMaximoErroresMaximosVerticales(const vector <int>  &dominante) const;
    	void  calcularEmaxVerticalEntreDosPuntos(int inicial, int final, long double &errorMaximo, int &posicion)const;
		void copiarPuntosDominantes(const SerieTemporal &serieTemporal);
		void mostrarPuntosDominantes();
		void guardarPuntosDominantes(vector <int> &vectorPuntosDominantes)const;


	private:
		int _numeroPuntos; /* Numero depuntos del serieTemporal */
		Punto _centro; /* Centro de gravedad */
		Punto *_puntoSerieTemporal; /* Vector para los puntos del serieTemporal */
		int *_codigoCadena; /* Almacena codigo de cadena del punto */
		bool *_dominante; /* Valor booleano que indica si el punto es dominante (1) o no (0) */

};




#endif
