#include "punto.hpp"
#include <math.h>
/* FUNCIONES DE INTERFAZ PARA EL TIPO PUNTO */


long double Punto::distanciaPuntos(const Punto &p)const
{
	//Variables incremento de x e y de un punto al siguiente
 	long double dx,dy;

 	// Obtencion de los incrementos
 	dx = x() - p.x();
 	dy = y() - p.y();

 	return (pow (dx * dx + dy * dy, 0.5));
}


 

int Punto::codigoCadenaPunto(const Punto &puntoSiguiente)const
{	
 	long double angulo; //Variable para el angulo de la recta que une dos puntos consecutivos 	
 	int codigoCadena; //Variable para el codigo de cadena
 	long double dx,dy; //Variables incremento de x e y de un punto al siguiente

 	// Obtencion de los incrementos
 	dx = puntoSiguiente.x() - x();
 	dy = puntoSiguiente.y() - y();

 	// obtencion del angulo 
 	angulo = atan2(dy,dx);
 
 	// Si el angulo es negativo le sumamos una vuelta entera */
 	if (angulo < 0.0) 
  	angulo = angulo + 2.0 * M_PI;

 	// Pasamos el angulo a grados sexagesimales
 	angulo = (180.0 / M_PI) * angulo;

 	// obtencion del codigo de cadena. Valor 0 para 90 grados
 	if (angulo > 337.5)
  	codigoCadena = 2;
 	else if (angulo > 292.5)
  	codigoCadena = 3;
 	else if (angulo > 247.5)
  	codigoCadena = 4;
 	else if (angulo > 202.5)
  	codigoCadena = 5;
 	else if (angulo > 157.5)
  	codigoCadena = 6;
 	else if (angulo > 112.5)
  	codigoCadena = 7;
 	else if (angulo > 67.5)
  	codigoCadena = 0;
 	else if (angulo > 22.5)
  	codigoCadena = 1;
 	else
  	codigoCadena = 2;

 	return codigoCadena;
}
