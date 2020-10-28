
#include "algoritmo.hpp"
#include <vector>
#include "serietemporal.hpp"
#include "matriz.hpp"
#include "recta.hpp"

using namespace std;

void Algoritmo::calcularSumatorios(Matriz <long double> &suma_x, Matriz <long double> &suma_y, Matriz <long double> &suma_xx, Matriz <long double> &suma_yy, Matriz <long double> &suma_xy)
{
	int numeroPuntosSerieTemporal;
  	long double x, y; //Coordenadas de un punto

  	numeroPuntosSerieTemporal = serieTemporal().numeroPuntosSerieTemporal();

	 //Primero se calculan los sumatorios de la primera fila.
	 //Cada elemento j, se obtiene a partir del j - 1, sumándole el sumando correspondiente al punto j - 1
  
	for(int j = 3; j <= numeroPuntosSerieTemporal; j++)
	{
		x = serieTemporal().puntoSerieTemporal(j - 1).x(); //cordenada x del punto j - 1
		y = serieTemporal().puntoSerieTemporal(j - 1).y(); //cordenada y del punto j - 1
		suma_x.elemento(1, j, suma_x.elemento(1, j - 1) + x);
		suma_y.elemento(1, j, suma_y.elemento(1, j - 1) + y);
		suma_xx.elemento(1, j, suma_xx.elemento(1, j - 1) + x * x);
		suma_yy.elemento(1, j, suma_yy.elemento(1, j - 1) + y * y);
		suma_xy.elemento(1, j, suma_xy.elemento(1, j - 1) + x * y);
	}

	//A partir de la segunda fila, los sumatorios se calculan a partir de la fila anterior.
	//Cada elemento i,j se obtiene restándole al elemento (i-1, j) el sumando del punto i
	for(int i = 2; i <= numeroPuntosSerieTemporal - 2; i++)
	{
		x = serieTemporal().puntoSerieTemporal(i).x(); //cordenada x del punto i
		y = serieTemporal().puntoSerieTemporal(i).y(); //cordenada y del punto i  
		for(int j = i + 2; j <= numeroPuntosSerieTemporal; j++)
		{
			suma_x.elemento(i, j, suma_x.elemento(i - 1, j) - x);
			suma_y.elemento(i, j, suma_y.elemento(i - 1, j) - y);
			suma_xx.elemento(i, j, suma_xx.elemento(i - 1, j) - x * x);
			suma_yy.elemento(i, j, suma_yy.elemento(i - 1, j) - y * y);
				suma_xy.elemento(i, j, suma_xy.elemento(i - 1, j) - x * y);
		}
	}
}
long double Algoritmo::sumaErroresMinima(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
const Matriz <long double> &suma_xy)
{
	
	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;

	long double xMedia, yMedia, xVarianza, yVarianza, xyCovarianza;
	int numeroPuntos = fin - inicio - 1;
	long double ux, uy; /* Coordenadas del vector propio correspondiente al maximo valor propio */
	long double sumaErroresCuadrado;

	xMedia = suma_x.elemento(inicio, fin) / numeroPuntos;
	yMedia = suma_y.elemento(inicio, fin) / numeroPuntos;
	xVarianza = suma_xx.elemento(inicio, fin) / numeroPuntos - 
	(suma_x.elemento(inicio, fin) * suma_x.elemento(inicio, fin)) / (numeroPuntos * numeroPuntos);
	yVarianza = suma_yy.elemento(inicio, fin) / numeroPuntos - 
	(suma_y.elemento(inicio, fin) * suma_y.elemento(inicio, fin)) / (numeroPuntos * numeroPuntos);
	xyCovarianza = suma_xy.elemento(inicio, fin) / numeroPuntos - 
	(suma_x.elemento(inicio, fin) * suma_y.elemento(inicio, fin)) / (numeroPuntos * numeroPuntos);

	/** Calculo de valores propios de la matriz de covarianzas */
	long double a1, b1, c1, l1;
	a1 = 1.0;
	b1 = -(xVarianza +yVarianza);
	c1 = xVarianza * yVarianza -xyCovarianza * xyCovarianza ;

	/* Maximo valor propio */
	l1=(-b1+sqrt(b1*b1-4.0*a1*c1))/(2.0*a1);


	if (fabs(xyCovarianza) < 0.00001)
	{
		if (fabs(l1-yVarianza) < 0.00001)
		{
			ux=0.0;
			uy=1.0;
		}
		else  /*(fabs(l1 - xVarianza) < 0.00001)*/
		{
			ux=1.0;
			uy=0.0;
		}
	} 
	else
	{
		ux=1.0;
		uy=(l1 -xVarianza)/xyCovarianza;
	}

	/* La recta tiene de vector (ux, uy) y pasa por (xMedia, yMedia) */
	/* y = a + bx*/
	/*En implicitas la recta es Ax + By + C = 0, donde A = -b, B = 1, C = -a */
	long double a, b;
	long double A, C;
	if (fabs(ux) > 0.00000001)
	{
		b = uy / ux;
		a = yMedia - b * xMedia;
		A = -b;
		C = -a;
		sumaErroresCuadrado = ( numeroPuntos * a * a + 2.0 * a * b * suma_x.elemento(inicio, fin) + 
		b * b * suma_xx.elemento(inicio, fin) - 2.0 * a * suma_y.elemento(inicio, fin) -
		2.0 * b * suma_xy.elemento(inicio, fin) + suma_yy.elemento(inicio, fin)) / (b * b + 1.0);
	}
	else //Recta paralela al eje y -> x = xMedia ya que pasa por el punto (xMedia, yMedia)
	{
		A = 1.0;
		C = -xMedia;
		sumaErroresCuadrado = suma_xx.elemento(inicio, fin) + (2.0 * (C / A)) * suma_x.elemento(inicio, fin) + 
		numeroPuntos * (C/A) * (C/A);
	}
	return sumaErroresCuadrado;
}

long double Algoritmo::sumaErrores(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
const Matriz <long double> &suma_xy)
{
	//Esta funcion calcula los errores entre inicio y fin al aproximar el serieTemporal en ese trozo
	//por el segmento que une los puntos inicio y fin, por eso el numero de puntos es (fin - inicio - 1)

	long double sumaErroresCuadrado = 0.0;
	long double A, B, C; //Coeficientes de la recta en forma general. Ax + By + C = 0
	long double a, b; //Coeficientes de la recta en forma explícita. y = a + bx

	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;
		
	//Los dos puntos son iguales. Los errores se calculan calculando las distancias de los intermedios a ese punto
	if (serieTemporal().puntoSerieTemporal(inicio)== serieTemporal().puntoSerieTemporal(fin))
	{
		for(int i = inicio + 1; i < fin -1; i++)
			sumaErroresCuadrado += pow(serieTemporal().puntoSerieTemporal(i).distanciaPuntos(serieTemporal().puntoSerieTemporal(fin)),2.0);

		return sumaErroresCuadrado;
	}

	Recta recta(serieTemporal().puntoSerieTemporal(inicio), serieTemporal().puntoSerieTemporal(fin));
	
	//Como la recta se calcula en forma general, hay que calcular los coeficientes en forma explicita (a y b)
	A = recta.a();
	B = recta.b();
	C = recta.c();

	if (fabs(B) > 0.0000001) //Recta no paralela al eje y
	{
		a = - C / B;
		b = - A / B;
		sumaErroresCuadrado = ( (fin - inicio - 1.0) * a * a + 2.0 * a * b * suma_x.elemento(inicio, fin) + 
		b * b * suma_xx.elemento(inicio, fin) - 2.0 * a * suma_y.elemento(inicio, fin) -
		2.0 * b * suma_xy.elemento(inicio, fin) + suma_yy.elemento(inicio, fin)) / (b * b + 1.0);

	}
	else //Recta paralela al eje y. En este caso b sería infinito y se hace el cálculo de otra forma. 
	{
		sumaErroresCuadrado = suma_xx.elemento(inicio, fin) + (2.0 * (C / A)) * suma_x.elemento(inicio, fin) +  (fin - inicio -1) * (C/A) * (C/A);
	}

	if (isnan(sumaErroresCuadrado))
	{
		cout << "\ninicio = "<< inicio << endl;
		cout << "fin = " << fin << endl;
		cout << " A = " << A << endl;
		cout << "B = " << B << endl;
		cout << "C = " << C << endl;
	}
	return sumaErroresCuadrado;
}


long double Algoritmo::sumaErrores1(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
const Matriz <long double> &suma_xy)
{
	/*Esta funcion calcula la suma de los errores de la regresión de y sobre x */

	long double sumaErroresCuadrado = 0.0;
	long double a, b; //Coeficientes de la recta en forma explícita. y = a + bx


	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;
	

	//Calculamos los coeficientes b y a a partir de los sumatorios 
	long double numeradorB, denominadorB;
	int numeroPuntos = fin - inicio - 1;

	numeradorB = suma_x.elemento(inicio, fin) * suma_y.elemento(inicio, fin) - 
	numeroPuntos * suma_xy.elemento(inicio, fin);
	denominadorB = suma_x.elemento(inicio, fin) * suma_x.elemento(inicio, fin) - 
	numeroPuntos * suma_xx.elemento(inicio, fin);

	//Si todos los valores de x son iguales b es infinito y el error es 0
	if (fabs(denominadorB) < 0.0000001)
		return 0.0;

	b = numeradorB / denominadorB;
	a = (suma_y.elemento(inicio, fin) / numeroPuntos) - (b * suma_x.elemento(inicio, fin) / numeroPuntos);

	sumaErroresCuadrado = numeroPuntos * a * a + suma_yy.elemento(inicio, fin) + b * b * suma_xx.elemento(inicio, fin) 
	- 2.0 * a * suma_y.elemento(inicio, fin) - 2.0 * b * suma_xy.elemento(inicio, fin) + 2.0 * a * b * suma_x.elemento(inicio, fin);

	return sumaErroresCuadrado;
}

long double Algoritmo::sumaErroresYsobreX(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
const Matriz <long double> &suma_xy)
{
	/*Esta funcion calcula la suma de los errores medidos en vertical de los puntos comprendidos entre inicio y fin*/
	/* Con respecto a la recta que une los puntos inicio y fin */

	long double sumaErroresCuadrado = 0.0;
	long double A, B, C; //Coeficientes de la recta en forma general. Ax + By + C = 0
	long double a, b; //Coeficientes de la recta en forma explícita. y = a + bx

	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;

	int numeroPuntos = fin - inicio - 1;

	Recta recta(serieTemporal().puntoSerieTemporal(inicio), serieTemporal().puntoSerieTemporal(fin));
	


	//Como la recta se calcula en forma general, hay que calcular los coeficientes en forma explicita (a y b)
	A = recta.a();
	B = recta.b();
	C = recta.c();

	if (fabs(B) > 0.0000001) //Recta no paralela al eje y
	{
		a = - C / B;
		b = - A / B;
		sumaErroresCuadrado = numeroPuntos * a * a + suma_yy.elemento(inicio, fin) + b * b * suma_xx.elemento(inicio, fin) 
		- 2.0 * a * suma_y.elemento(inicio, fin) - 2.0 * b * suma_xy.elemento(inicio, fin) + 2.0 * a * b * suma_x.elemento(inicio, fin);

	}
	else //Recta paralela al eje y. En este caso b sería infinito y se hace el cálculo de otra forma. 
	{
		sumaErroresCuadrado = 0;
	}

	if (isnan(sumaErroresCuadrado))
	{
		cout << "\ninicio = "<< inicio << endl;
		cout << "fin = " << fin << endl;
		cout << " A = " << A << endl;
		cout << "B = " << B << endl;
		cout << "C = " << C << endl;
	}
	return sumaErroresCuadrado;
}

long double Algoritmo::sumaErrores2(const int &inicio, const int &fin, const Matriz <long double> &suma_x, 
const Matriz <long double> &suma_y, const Matriz <long double> &suma_xx, const Matriz <long double> &suma_yy, 
const Matriz <long double> &suma_xy)
{
	/*Esta funcion calcula la suma de los errores de la regresión de x sobre y */

	long double sumaErroresCuadrado = 0.0;
	long double a, b; //Coeficientes de la recta en forma explícita. x = a + by

	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;
		

	//Calculamos los coeficientes b y a a partir de los sumatorios 
	long double numeradorB, denominadorB;
	int numeroPuntos = fin - inicio - 1;

	numeradorB = suma_y.elemento(inicio, fin) * suma_x.elemento(inicio, fin) - 
	numeroPuntos * suma_xy.elemento(inicio, fin);
	denominadorB = suma_y.elemento(inicio, fin) * suma_y.elemento(inicio, fin) - 
	numeroPuntos * suma_yy.elemento(inicio, fin);

	//Si todos los valores de y son iguales b es infinito y el error es 0
	if (fabs(denominadorB) < 0.0000001)
		return 0.0;

	b = numeradorB / denominadorB;
	a = (suma_x.elemento(inicio, fin) / numeroPuntos) - (b * suma_y.elemento(inicio, fin) / numeroPuntos);

	//Ahora se calculan los errores punto a punto en la dirección y
		
	sumaErroresCuadrado = numeroPuntos * a * a + suma_xx.elemento(inicio, fin) + b * b * suma_yy.elemento(inicio, fin) 
	- 2.0 * a * suma_x.elemento(inicio, fin) - 2.0 * b * suma_xy.elemento(inicio, fin) + 2.0 * a * b * suma_y.elemento(inicio, fin);
	return sumaErroresCuadrado;
}
