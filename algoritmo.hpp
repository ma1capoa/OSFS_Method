#ifndef ALGORITMO_H
#define ALGORITMO_H

#include <vector>
#include "serietemporal.hpp"
#include "matriz.hpp"

class Algoritmo
{
	public:
		Algoritmo(){};
		virtual ~Algoritmo()
	    {
			//Liberamos el serieTemporal privado
			serieTemporal().liberarSerieTemporal();
	    };

		inline SerieTemporal & serieTemporal() //Observador
		{
			return _serieTemporal;
		};

		inline void serieTemporal(const SerieTemporal &serieTemporal) //Modificador
		{
			_serieTemporal = serieTemporal;
		};

		//Función virtual pura que se implementará en las clases derivadas
 		virtual void aplicar()=0;
		
		//Funciones virtuales que se implementarán en algunas clases derivadas
		long double & getISE();

		vector <int> & vectorSerieTemporalOptimo();
    
    //Funciones para el cálculo de errores.
    void calcularSumatorios(Matriz <long double> &suma_x, Matriz <long double> &suma_y, Matriz <long double> &suma_xx, Matriz <long double> &suma_yy, Matriz <long double> &suma_xy);
		long double sumaErrores(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
		const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
		const Matriz <long double> &suma_xy); //Errores al aproximar por segmento inicio fin
	long double sumaErroresMinima(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
		const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
		const Matriz <long double> &suma_xy); //Errores al ajustar por el eje de minima inercia.
	long double sumaErrores1(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
		const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
		const Matriz <long double> &suma_xy); //Errores en la regresion de y sobre x
	long double sumaErroresYsobreX(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
		const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
		const Matriz <long double> &suma_xy); //Errores medidos en vertical con respecto a la recta que une inicio y fin
	long double sumaErrores2(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
		const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
		const Matriz <long double> &suma_xy); //Errores en la regresion de x sobre y

	private:
		SerieTemporal _serieTemporal;
    
 
};

#endif
		
