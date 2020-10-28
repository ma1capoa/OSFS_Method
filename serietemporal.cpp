#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <cassert>
#include "serietemporal.hpp"
#include "punto.hpp"
#include "recta.hpp"
#include "vector.hpp"
#include "matriz.hpp"


using namespace std;

/* FUNCIONES DE INTERFAZ PARA EL TIPO CONTORNO */
//Constructor vacío
SerieTemporal::SerieTemporal()	
{
	numeroPuntosSerieTemporal(0);
	_puntoSerieTemporal = NULL; 
	_codigoCadena = NULL; 
	_dominante = NULL; 
}

//Constructor. Crea un serieTemporal vacío con n puntos
SerieTemporal::SerieTemporal(const int &n)	
{
	numeroPuntosSerieTemporal(n);
	//Se reserva memoria para los puntos del serieTemporal 
 	reservarPuntosSerieTemporal();
 	reservarCodigoCadenaSerieTemporal(); 
	inicializarSerieTemporal();
}

//Constructor. Crea un serieTemporal a partir de un fichero
SerieTemporal::SerieTemporal(char *nombreFichero)	
{
	cargarSerieTemporal(nombreFichero);
}

//Destructor
SerieTemporal::~SerieTemporal() 
{
	liberarSerieTemporal();
}

//Constructor de copia.

SerieTemporal::SerieTemporal(const SerieTemporal &serieTemporal)
{
 	Punto centro;
 
	//Asignamos numero de puntos
 	numeroPuntosSerieTemporal(serieTemporal.numeroPuntosSerieTemporal());

 	//Se reserva memoria para los puntos del serieTemporal 
 	reservarPuntosSerieTemporal();
 	reservarCodigoCadenaSerieTemporal();
 
 	//Inicializar a 0 todos los puntos y parametros del serieTemporal
 	inicializarSerieTemporal();

 	//asignamos el punto central
 	centroSerieTemporal(serieTemporal.centroSerieTemporal());

 	// Asignamos todos los puntos del serieTemporal
 	for (int i = 1; i <= numeroPuntosSerieTemporal(); i++)
 	{ 
	   	puntoSerieTemporal(serieTemporal.puntoSerieTemporal(i), i);
	   	codigoCadenaPunto(i, serieTemporal.codigoCadenaPunto(i));
 	} 
 
	/* Si ya se han calculado los puntos dominantes se asignan*/
 
 	if (serieTemporal.estaReservadoDominante())
 	{
		reservarPuntoDominanteSerieTemporal();  
	  	for (int i = 1; i <= numeroPuntosSerieTemporal(); i++)
	  	{ 
	   		dominantePunto(i, serieTemporal.dominantePunto(i));
	  	}
 	}
}

//Sobrecarga de la asignacion.
SerieTemporal & SerieTemporal::operator=(const SerieTemporal &serieTemporal)
{
 	Punto centro;
 
	//Asignamos numero de puntos
 	numeroPuntosSerieTemporal(serieTemporal.numeroPuntosSerieTemporal());

 	//Se reserva memoria para los puntos del serieTemporal 
 	reservarPuntosSerieTemporal();
 	reservarCodigoCadenaSerieTemporal();
 
 	//Inicializar a 0 todos los puntos y parametros del serieTemporal
 	inicializarSerieTemporal();

 	//asignamos el punto central
 	centroSerieTemporal(serieTemporal.centroSerieTemporal());

 	// Asignamos todos los puntos del serieTemporal
 	for (int i = 1; i <= numeroPuntosSerieTemporal(); i++)
 	{ 
	   	puntoSerieTemporal(serieTemporal.puntoSerieTemporal(i), i);
	   	codigoCadenaPunto(i, serieTemporal.codigoCadenaPunto(i));
 	}
	/* Si ya se han calculado los puntos dominantes se asignan*/
 
 	if (serieTemporal.estaReservadoDominante())
 	{
		reservarPuntoDominanteSerieTemporal();  
	  	for (int i = 1; i <= numeroPuntosSerieTemporal(); i++)
	  	{ 
	   		dominantePunto(i, serieTemporal.dominantePunto(i));
	  	}
 	}

	return *this;
}

void SerieTemporal::inicializarSerieTemporal()
{
	Punto punto;

	/* Se inicializa a cero un punto de serieTemporal auxiliar */
	punto.x(0.0);
	punto.y(0.0);
	punto.radio(0.0);
	punto.angulo(0.0);

	/* Se inicializa a cero el centro del serieTemporal */
	centroSerieTemporal(punto);
 
	/* Se inicializan todos los puntos del serieTemporal con el punto de serieTemporal auxiliar */
	for (int i = 1; i <= numeroPuntosSerieTemporal(); i++)
	{
		puntoSerieTemporal(punto, i);
		codigoCadenaPunto(i, 0);
	 }
	 _dominante = NULL;

}

void SerieTemporal::reservarPuntosSerieTemporal()
{
	_puntoSerieTemporal = new Punto [numeroPuntosSerieTemporal()];	//Reserva de puntos
	if (!_puntoSerieTemporal)
 	{
	  	cout << " FALLO EN LA RESERVA DE MEMORIA \n";
	  	exit(0);
 	}

}

void SerieTemporal::reservarCodigoCadenaSerieTemporal()
{
 	_codigoCadena = new int [numeroPuntosSerieTemporal()];
 	if (!_codigoCadena)
 	{
	  	cout << " FALLO EN LA RESERVA DE MEMORIA \n";
	  	exit(0);
 	}

}


bool SerieTemporal::estaReservadoDominante()const
{
 	if (_dominante )
		return true;
	else
		return false;
}

void SerieTemporal::reservarPuntoDominanteSerieTemporal()
{
 	_dominante = new bool [numeroPuntosSerieTemporal()];
 	if (!_dominante )
	{
	  	cout << " FALLO EN LA RESERVA DE MEMORIA \n";
	  	exit(0);
 	}
}

long double SerieTemporal::longitudSerieTemporal()const
{
 	long double longitud = 0.0;
 	Punto punto1, punto2;
 	

 	for(int i = 1; i < numeroPuntosSerieTemporal(); i++)
 	{
	  	punto1 = puntoSerieTemporal(i);
	  	punto2 = puntoSerieTemporal(i + 1);
	  	Vector2D Vector2D(punto1, punto2);
	  	longitud += Vector2D.moduloVector2D();
 	}

 	/* Se calcula tambien la distancia del último al primero */
 	punto1 = puntoSerieTemporal(numeroPuntosSerieTemporal());
 	punto2 = puntoSerieTemporal(1);
 	Vector2D Vector2D(punto1, punto2);
 	longitud += Vector2D.moduloVector2D();

 	return longitud;
}

long double SerieTemporal::longitudSerieTemporalEntrePuntosDominantes()const
{
 	int inicial, i;
 	long double longitud = 0.0;
 	Punto punto1, punto2;

	/*Primero localizo el punto dominante inicial */
 	i = 1;

 	while(!dominantePunto(i))
  	i++;

 	inicial = i;

 	/*Voy localizando los puntos dominantes siguientes hasta llegar al último */
 	punto1 = puntoSerieTemporal(inicial);
 
 	do
 	{
	  	/* Pasamos al siguiente punto */
	  	i++;
	  	if (i > numeroPuntosSerieTemporal())
	   		i = i - numeroPuntosSerieTemporal();
	  
	  	/*Localizamos el siguiente dominante*/
	  	while(!dominantePunto(i))
	  	{
	   		i++;
	   		if (i > numeroPuntosSerieTemporal())
	    		i = i - numeroPuntosSerieTemporal();
	  	}

	  	punto2 = puntoSerieTemporal(i);
	  	Vector2D vector(punto1, punto2);
	  	longitud += vector.moduloVector2D();
	  	/*Ahora punto2 pasa a ser punto1*/
	  	punto1 = punto2;
  
 	}while (i != inicial);

 return longitud;
}


int SerieTemporal::contarRegistrosFicheroSerieTemporal(char *nombreFichero)const
{
 	int numeroRegistros = 0;
 	long double x, y;
    long double xInicio, yInicio;
        

 	ifstream pf(nombreFichero);
 	if (!pf)
    {
	    cout << "\n Fichero inexistente";
	    exit(0);
    }
    pf >> xInicio >> yInicio;
    numeroRegistros++;

 	while(pf >> x >> y)
    	numeroRegistros++;
        
    Punto inicial(xInicio, yInicio);
    Punto ultimo(x, y);
        
 	pf.close();
    
 	return numeroRegistros;
}


void SerieTemporal::cargarSerieTemporal(char *nombreFichero)
{
 	int numeroRegistros;
 
 	/* Primero se cuentan los registros del fichero */
 	numeroRegistros = contarRegistrosFicheroSerieTemporal(nombreFichero);
  

 	/* Asigna el numero de puntos del serieTemporal */
 	numeroPuntosSerieTemporal(numeroRegistros);
 
 	/* Se reserva memoria para los puntos del serieTemporal */
 	reservarPuntosSerieTemporal();
 	reservarCodigoCadenaSerieTemporal();
	//reservarPuntoDominanteSerieTemporal();
 
 	/* Inicializar a 0 todos los puntos y parametros del serieTemporal */
 	inicializarSerieTemporal();
  
 	/* Se leen los puntos del fichero y se almacenan en el serieTemporal
 	tambien asigna los valores del parametro t*/
 	leerPuntosFichero(nombreFichero);
 
 	/* Se calcula el centro del serieTemporal */
 	calcularCentroSerieTemporal();

 	/* Ahora se obtienen las coordenadas polares respecto al centro */
 	coordenadasPolaresCentro();

 	/* Obtención del código de cadena */
 	codigoCadenaSerieTemporal();


}

void SerieTemporal::leerPuntosFichero(char *nombreFichero)
{
 	long double x, y;
 	Punto punto;
 	
	ifstream pf(nombreFichero);
 	if (!pf)
  	{
		cout << "\n Fichero inexistente";
		exit(0);
  	}
  
	//Lee del fichero los puntos y va almacenando coordenadas cartesianas, polares y paramétricas
	for (int i = 1; i <= numeroPuntosSerieTemporal(); i++)
 	{
	    pf >> x >> y;
	    punto.x(x);
	    punto.y(y);
	    punto.radio(0.0);
	    punto.angulo(0.0);
	    puntoSerieTemporal(punto, i);
 	}
 	
  	pf.close();
}

void SerieTemporal::calcularCentroSerieTemporal()
{
	long double xMedia = 0.0, yMedia = 0.0;
	Punto punto;

	for (int i = 1; i <= numeroPuntosSerieTemporal(); i++)
	{
		punto = puntoSerieTemporal(i);
		xMedia += punto.x() / (long double) numeroPuntosSerieTemporal();
		yMedia += punto.y() / (long double) numeroPuntosSerieTemporal();
	}

	punto.x(xMedia);
	punto.y(yMedia);

	/* Las coordenadas polares del centro seran 0,0 ya que se refieren a sí mismo */
	punto.radio(0.0);
	punto.angulo(0.0);

	centroSerieTemporal(punto); 
}


void SerieTemporal::guardarSerieTemporal(char *nombre)const
{
	Punto punto;
 	ofstream pf(nombre);

 	if (!pf) 
  	{
	    cout << "Error en la apertura del fichero \n";
	    exit(0);
  	}
 
 	/* Guardamos las coordenadas cartesianas del serieTemporal */ 
 	for(int i = 1 ;i <= numeroPuntosSerieTemporal() ; i++)
	{
		punto = puntoSerieTemporal(i);
   		pf << punto.x() << " " << punto.y() << "\n";
	}

  	pf.close();
}

//Esta función es igual que la anterior pero solo guarda los puntos dominantes que componen la aproximación

void SerieTemporal::guardarAproximacion(char *nombre)const
{
	Punto punto;
	ofstream pf(nombre);

	if (!pf) 
	{
		cout << "Error en la apertura del fichero \n";
		exit(0);
	}
 
 	/* Guardamos las coordenadas cartesianas del serieTemporal */ 
 	for(int i = 1 ;i <= numeroPuntosSerieTemporal() ; i++)
	{
	    if (dominantePunto(i))
	    {
			punto = puntoSerieTemporal(i);
	        pf << punto.x() << " " << punto.y() << "\n";
	    }
	}

  	pf.close();
}

void SerieTemporal::crearSerieTemporal()
{ 
 /* Se reserva memoria para los puntos del serieTemporal */
 reservarPuntosSerieTemporal();
 reservarCodigoCadenaSerieTemporal();

 /* Inicializar a 0 todos los puntos y parametros del serieTemporal */
 inicializarSerieTemporal();
}

void SerieTemporal::liberarSerieTemporal()
{

	if (_puntoSerieTemporal)
	{
		delete [] _puntoSerieTemporal;
		_puntoSerieTemporal = NULL;

		if (_codigoCadena)
		{
			delete [] _codigoCadena;
			_codigoCadena = NULL;
		}

		if (_dominante )
		{
			delete [] _dominante;
			_dominante = NULL;
		}
	} 
}


void SerieTemporal::coordenadasPolaresCentro()
{
 	long double xPunto, yPunto, xCentro, yCentro, dx, dy, radio, angulo;
  	Punto punto;
 	Punto centro(centroSerieTemporal());
 	xCentro = centro.x();
 	yCentro = centro.y();
 
 	for(int i = 1;i <= numeroPuntosSerieTemporal(); i++)
 	{
	  	punto = puntoSerieTemporal(i);
	  	xPunto = punto.x();
	  	yPunto = punto.y();
	  	dx = xPunto - xCentro;
	  	dy = yPunto - yCentro;
	  	radio = pow(pow(dx, 2.0)+pow(dy, 2.0), 0.5);
	  	punto.radio(radio);
	  	angulo = atan2(dy, dx);
	 
	  	if (angulo < 0.0)
	   		angulo += 2.0 * M_PI;

	  	punto.angulo(angulo);
	  	puntoSerieTemporal(punto, i);
 	}   
}


void SerieTemporal::coordenadasCartesianas()
{
 	long double xCentro, yCentro, xPunto, yPunto;
 	Punto punto;
	Punto centro(centroSerieTemporal());

 	xCentro = centro.x();
 	yCentro = centro.y();
 
 	for(int i = 1;i <= numeroPuntosSerieTemporal(); i++)
 	{
	  	punto = puntoSerieTemporal(i);
	  	xPunto = xCentro + punto.radio() * cos(punto.angulo());
	  	yPunto = yCentro + punto.radio() * sin(punto.angulo());
	  	punto.x(xPunto);
	 	punto.y(yPunto);
	   	puntoSerieTemporal(punto, i);
 	}   
}

void SerieTemporal::codigoCadenaSerieTemporal()
{
	/*Variable para recibir el codigo de cadena de un punto */ 
 	int codigoCadena; 

 	Punto puntoActual, puntoSiguiente;

 	/* Calculo del codigo de cadena */
 	for(int i = 1; i <=  numeroPuntosSerieTemporal(); i++)
 	{
		puntoActual = puntoSerieTemporal(i);
	  	if (i < numeroPuntosSerieTemporal())
	   		puntoSiguiente = puntoSerieTemporal(i + 1);
	  	else /* Ultimo punto */
	  	{
			//Para serieTemporals cerrados hay que tener cuidado, el último punto coincide con el primero.
			//En ese caso el codigo de cadena del ultimo coincide con el del primero. Modificación hecha el 31-10-2014
			if (puntoSerieTemporal(numeroPuntosSerieTemporal()) == puntoSerieTemporal(1))
				puntoSiguiente = puntoSerieTemporal(2);
			else
				puntoSiguiente = puntoSerieTemporal(1);
	  	}
	  	codigoCadena = puntoActual.codigoCadenaPunto(puntoSiguiente);
	  	codigoCadenaPunto(i, codigoCadena);
 	}
}

void SerieTemporal::coordenadasCartesianasCodigoCadena()
{
 	/*Variable para recibir el codigo de cadena de un punto */ 
 	int codigoCadena; 
 	Punto puntoActual;

 	puntoSerieTemporal(puntoActual, 1);

 	/* Calculo del codigo de cadena */
 	for(int i = 1; i <  numeroPuntosSerieTemporal(); i++)
 	{
	  	codigoCadena = codigoCadenaPunto(i);
	  	if (codigoCadena == 2)
	  	{
	   		puntoActual.x(puntoActual.x() + 1);
	   		puntoActual.y(puntoActual.y());
	  	}
	  	else if (codigoCadena == 3)
	  	{
	   		puntoActual.x(puntoActual.x() + 1);
	   		puntoActual.y(puntoActual.y() - 1);
	  	}
	  	else if (codigoCadena == 4)
	  	{
	   		puntoActual.x(puntoActual.x());
	   		puntoActual.y(puntoActual.y() - 1);
	  	}
	  	else if (codigoCadena == 5)
	  	{
	   		puntoActual.x(puntoActual.x() - 1);
	   		puntoActual.y(puntoActual.y() - 1);
	  	}
	  	else if (codigoCadena == 6)
	  	{
	   		puntoActual.x(puntoActual.x() - 1);
	   		puntoActual.y(puntoActual.y());
	  	}
	  	else if (codigoCadena == 7)
	  	{
	   		puntoActual.x(puntoActual.x() - 1);
	   		puntoActual.y(puntoActual.y() + 1);
	  	}
	  	else if (codigoCadena == 0)
	  	{
	   		puntoActual.x(puntoActual.x());
	   		puntoActual.y(puntoActual.y() + 1);
	  	}
	  	else
	  	{
	   		puntoActual.x(puntoActual.x() + 1);
	   		puntoActual.y(puntoActual.y() + 1);
	  	}
	  	puntoSerieTemporal(puntoActual, i + 1);
 	}    
}

void SerieTemporal::marcarTodosPuntosDominantes()
{
 	// Se marcan todos los puntos como dominantes **/
 	for (int t = 1; t <= numeroPuntosSerieTemporal(); t++)
 	{
  		dominantePunto(t, true);
 	}
}

void SerieTemporal::marcarTodosPuntosNoDominantes()
{
 	// Se marcan todos los puntos como no dominantes **/
 	for (int t = 1; t <= numeroPuntosSerieTemporal(); t++)
 	{
  		dominantePunto(t, false);
 	}
}

//Esta funcion marca los dominates con un incremento dado. Sirve para el metodo de Sarker (Bottom-up adaptado y mejorado)
void SerieTemporal::marcarDominantesIncremento(int incremento)
{
	// Se marcan todos los puntos como dominantes **/
	for (int t = 1; t <= numeroPuntosSerieTemporal(); t = t + incremento)
	{
		dominantePunto(t, true);
	}
}

int SerieTemporal::contarPuntosDominantes()const
{
 	int numeroPuntosDominantes = 0;
 
 	for (int t = 1; t <= numeroPuntosSerieTemporal(); t++)
 	{
	  	if (dominantePunto(t))
	  	{
	   		numeroPuntosDominantes++;
	  	}
 	}

	return numeroPuntosDominantes;
}

void SerieTemporal::diferenciaSerieTemporalDominantesReales(long double &sumaErrorCuadrado, long double &errorMaximo)const
{
 	int i, t = 1;
 	int tInicial;
 	int tIzquierda, tDerecha;
 	long double error;

 	/* Recorrido del serieTemporal, calculando la distancia de cada punto real del serieTemporal
    al serieTemporal aproximado por los puntosdominantes */
 	sumaErrorCuadrado = 0.0;
 	errorMaximo = 0.0;

 	/*Busqueda del primer punto dominante del serieTemporal*/
 	while(!dominantePunto(t)) 
  	t++;

 	tInicial = t;

 	/* se asigna el extremo izquierdo del intervalo a tInicial para los primeros puntos */
 	tIzquierda = t; 
 	do
 	{
	  	t++;
	  	if (t > numeroPuntosSerieTemporal())
	   		t = t - numeroPuntosSerieTemporal();

	  	while (!dominantePunto(t)) /* Se localiza extremo derecho del intervalo */
	  	{
	   		t++;
	   		if (t > numeroPuntosSerieTemporal())
	    		t = t - numeroPuntosSerieTemporal();
	  	}

	  	tDerecha = t;

	  	/* Se calculan las distancias de todos los puntos comprendidos entre tIzquierda y tDerecha hasta
	     el segmento que une los puntos tIzquierda y tDerecha*/

	  	Recta recta(puntoSerieTemporal(tIzquierda), puntoSerieTemporal(tDerecha));

	  	i = tIzquierda + 1;
	  	if (i > numeroPuntosSerieTemporal())
	   		i = i - numeroPuntosSerieTemporal();

	  	while ( i != tDerecha)
	  	{
	   		error = recta.distanciaPuntoRecta(puntoSerieTemporal(i));
	   		sumaErrorCuadrado += error * error;

	   		if (error > errorMaximo)
	    		errorMaximo = error;

	   		i++;
	   		if (i > numeroPuntosSerieTemporal())
	    		i = i - numeroPuntosSerieTemporal();
	  	}

	  	tIzquierda = tDerecha; /*Para el siguiente intervalo tDerecha sera tIzquierda */
 	} while (tIzquierda != tInicial);
}



void SerieTemporal::diferenciaVerticalSerieTemporalDominantesReales(long double &sumaErrorCuadrado, long double &errorMaximo)const
{
 	int i, t = 1;
 	int tInicial;
 	int tIzquierda, tDerecha;
 	long double error;

 	/* Recorrido del serieTemporal, calculando la distancia de cada punto real del serieTemporal
    al serieTemporal aproximado por los puntosdominantes */
 	sumaErrorCuadrado = 0.0;
 	errorMaximo = 0.0;

 	/*Busqueda del primer punto dominante del serieTemporal*/
 	while(!dominantePunto(t)) 
  	t++;

 	tInicial = t;

 	/* se asigna el extremo izquierdo del intervalo a tInicial para los primeros puntos */
 	tIzquierda = t; 
 	do
 	{
	  	t++;
	  	if (t > numeroPuntosSerieTemporal())
	   		t = t - numeroPuntosSerieTemporal();

	  	while (!dominantePunto(t)) /* Se localiza extremo derecho del intervalo */
	  	{
	   		t++;
	   		if (t > numeroPuntosSerieTemporal())
	    		t = t - numeroPuntosSerieTemporal();
	  	}

	  	tDerecha = t;

	  	/* Se calculan las distancias de todos los puntos comprendidos entre tIzquierda y tDerecha hasta el segmento que une los puntos tIzquierda y tDerecha*/
	   

	  	Recta recta(puntoSerieTemporal(tIzquierda), puntoSerieTemporal(tDerecha));

	  	i = tIzquierda + 1;
	  	if (i > numeroPuntosSerieTemporal())
	   		i = i - numeroPuntosSerieTemporal();

	  	while ( i != tDerecha)
	  	{
	   		error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
	   		sumaErrorCuadrado += error * error;

	   		if (error > errorMaximo)
	    		errorMaximo = error;

	   		i++;
	   		if (i > numeroPuntosSerieTemporal())
	    		i = i - numeroPuntosSerieTemporal();
	  	}

	  	tIzquierda = tDerecha; /*Para el siguiente intervalo tDerecha sera tIzquierda */
 	} while (tIzquierda != tInicial);
}

void SerieTemporal::diferenciaVerticalSerieTemporalDominantesReales(const vector <int> &dominantes, long double &sumaErrorCuadrado, long double &errorMaximo)const
{
	long double eMaxGlobal = 0.0;
	long double eMax = 0.0;
	int posicion = 0;
	for (unsigned int i = 1; i < dominantes.size() - 1; i++)
	{
		sumaErrorCuadrado += calcularIseVerticalEntreDosPuntos(dominantes[i],dominantes[ i + 1]);
		calcularEmaxVerticalEntreDosPuntos(dominantes[i],dominantes[ i + 1], eMax, posicion);
		
		if (eMax > eMaxGlobal)
			eMaxGlobal = eMax;
	}
	errorMaximo = eMaxGlobal;
}

long double SerieTemporal::calcularIseEntreDospuntos(int inicial, int final) const
{
	assert(inicial >= 1 and inicial <= numeroPuntosSerieTemporal());
	assert(final >= 1 and final <= numeroPuntosSerieTemporal());
	
	long double ise = 0.0;
	long double error = 0.0;
	int begin, end, limite;
	
	if (inicial == final)
		return 0.0;
        
    Punto p;
    begin = inicial;
    end = final;
	
	Recta recta(puntoSerieTemporal(inicial), puntoSerieTemporal(final));
	
    if (puntoSerieTemporal(inicial) != puntoSerieTemporal(final)) //No es bucle o lazo
    { //Los errores se calculan con distancias de puntos a rectas
        if (inicial < final)
        {
            for (int i = begin; i <= end; i++)
            {
                    error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
                    ise += error * error;
            }
        }
        else 
        {
            int limite = numeroPuntosSerieTemporal();
            if (puntoSerieTemporal(1) == puntoSerieTemporal(limite)) //Primer punto duplicado por ser serieTemporal cerrado y no se contabiliza
                limite--;
            
            for (int i = begin; i <= limite; i++)
            {
                error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
                ise += error * error;
            }
            for (int i = 1; i <= end; i++)
            {
                error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
                ise += error * error;
            }
        }
    }
    else //Es un bucle o lazo 
    { //Los errores se calculan con distancias entre puntos
        if (inicial < final)
        {
            for (int i = begin; i <= end; i++)
            {
                    error = puntoSerieTemporal(inicial).distanciaPuntos(puntoSerieTemporal(i));
                    ise += error * error;
            }
        }
        else 
        {
            limite = numeroPuntosSerieTemporal();
            if (puntoSerieTemporal(1) == puntoSerieTemporal(limite)) //Primer punto duplicado por ser serieTemporal cerrado y no se contabiliza
                limite--;
            
            for (int i = begin; i <= limite; i++)
            {
                    error = puntoSerieTemporal(inicial).distanciaPuntos(puntoSerieTemporal(i));
                    ise += error * error;
            }
            for (int i = 1; i <= end; i++)
            {
                    error = puntoSerieTemporal(inicial).distanciaPuntos(puntoSerieTemporal(i));
                    ise += error * error;
            }
        }
    }
	
	return ise;	
}

long double SerieTemporal::calcularIseVerticalEntreDosPuntos(int inicial, int final) const
{
	assert(inicial >= 1 and inicial <= numeroPuntosSerieTemporal());
	assert(final >= 1 and final <= numeroPuntosSerieTemporal());

	long double ise = 0.0;
	long double error = 0.0;
	int begin, end, limite;

	if (inicial == final)
	return 0.0;
	    
	Punto p;
	//Para no tener problemas con el siguiente a inicial o el anterior a final, begin y end se inicializan con inicial y final
	begin = inicial;
	end = final;

	Recta recta(puntoSerieTemporal(inicial), puntoSerieTemporal(final));


    if (puntoSerieTemporal(inicial) != puntoSerieTemporal(final)) //No es bucle o lazo
    { //Los errores se calculan con distancias de puntos a rectas
        if (inicial < final)
        {
            for (int i = begin; i <= end; i++)
            {
                error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
                ise += error * error;
            }
        }
        else 
        {
            int limite = numeroPuntosSerieTemporal();
            if (puntoSerieTemporal(1) == puntoSerieTemporal(limite)) //Primer punto duplicado por ser serieTemporal cerrado y no se contabiliza
                limite--;
            
            for (int i = begin; i <= limite; i++)
            {
                error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
                ise += error * error;
            }
            for (int i = 1; i <= end; i++)
            {
                error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
                ise += error * error;
            }
        }
    }
    else //Es un bucle o lazo 
    { //Los errores se calculan viendo la diferencia entre ordenadas de los puntos
        if (inicial < final)
        {
            for (int i = begin; i <= end; i++)
            {
                error = puntoSerieTemporal(inicial).y() - puntoSerieTemporal(i).y();
                ise += error * error;
            }
        }
        else 
    	{
            limite = numeroPuntosSerieTemporal();
            if (puntoSerieTemporal(1) == puntoSerieTemporal(limite)) //Primer punto duplicado por ser serieTemporal cerrado y no se contabiliza
                limite--;
            
            for (int i = begin; i <= limite; i++)
            {
                error = puntoSerieTemporal(inicial).y() - puntoSerieTemporal(i).y();
                ise += error * error;
            }
            for (int i = 1; i <= end; i++)
            {
                error = puntoSerieTemporal(inicial).y() - puntoSerieTemporal(i).y();
                ise += error * error;
            }
        }
    }

	return ise; 
}


void SerieTemporal::calcularEmaxEntreDosPuntos(int inicial, int final, long double &errorMaximo, int &posicion) const
{
	if (inicial == final)
	{
	    posicion = inicial;
	    errorMaximo = 0.0;
	    return;
	}
  
	assert(inicial >= 1 and inicial <= numeroPuntosSerieTemporal());
	assert(final >= 1 and final <= numeroPuntosSerieTemporal());
	
	errorMaximo = 0.0;
	posicion = inicial;
	long double error = 0.0;
	int begin, end;
    
    begin = inicial;
    end = final;
	
	Recta recta(puntoSerieTemporal(inicial), puntoSerieTemporal(final));
	if (puntoSerieTemporal(inicial) != puntoSerieTemporal(final)) //No es bucle o lazo
	{ //Los errores se calculan con distancias de puntos a rectas
	//cout << "No es bucle \n";
	    if (inicial < final)
	    {
            for (int i = begin; i <= end; i++)
            {
                error = recta.distanciaPuntoRecta(puntoSerieTemporal(i));
                if (error > errorMaximo)
                {
                        errorMaximo = error;
                        posicion = i;
                }
            }
	    }
	    else if (inicial > final)
	    {
            int limite = numeroPuntosSerieTemporal();
            if (puntoSerieTemporal(1) == puntoSerieTemporal(limite)) //Primer punto duplicado por ser serieTemporal cerrado y no se contabiliza
                limite--;
            for (int i = begin; i <= limite; i++)
            {
                error = recta.distanciaPuntoRecta(puntoSerieTemporal(i));
                if (error > errorMaximo)
                {
                    errorMaximo = error;
                    posicion = i;
                }
            }
            for (int i = 1; i <= end; i++)
            {
                error = recta.distanciaPuntoRecta(puntoSerieTemporal(i));
                if (error > errorMaximo)
                {
                    errorMaximo = error;
                    posicion = i;
                }
            }
	    }
	}
	else //Es un bucle o lazo 
	{ //Los errores se calculan con distancias entre puntos
	//cout << "Es bucle \n";
	    if (inicial < final)
	    {
            for (int i = begin; i <= end; i++)
            {
                error = puntoSerieTemporal(inicial).distanciaPuntos(puntoSerieTemporal(i));
                if (error > errorMaximo)
                {
                    errorMaximo = error;
                    posicion = i;
                }
            }
	    }
	    else if (inicial > final)
	    {
            int limite = numeroPuntosSerieTemporal();
            if (puntoSerieTemporal(1) == puntoSerieTemporal(limite)) //Primer punto duplicado por ser serieTemporal cerrado y no se contabiliza
                limite--;
            for (int i = begin; i <= limite; i++)
            {
                error = puntoSerieTemporal(inicial).distanciaPuntos(puntoSerieTemporal(i));
                if (error > errorMaximo)
                {
                    errorMaximo = error;
                    posicion = i;
                }
            }
            for (int i = 1; i <= end; i++)
            {
                error = puntoSerieTemporal(inicial).distanciaPuntos(puntoSerieTemporal(i));
                if (error > errorMaximo)
                {
                    errorMaximo = error;
                    posicion = i;
                }
            }
	    }
	    
	}
	}


long double SerieTemporal::calcularEmaxVerticalEntreDosPuntos(int inicial, int final) const
{
	if (inicial == final)
	return 0;

	assert(inicial >= 1 and inicial <= numeroPuntosSerieTemporal());
	assert(final >= 1 and final <= numeroPuntosSerieTemporal());

	long double errorMaximo = 0.0;
	long double error = 0.0;

	Recta recta(puntoSerieTemporal(inicial), puntoSerieTemporal(final));
	   
	for (int i = inicial + 1; i < final; i++)
	{
		error = recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i));
		if (error > errorMaximo)
		  errorMaximo = error;
	}
	return errorMaximo;
}



long double SerieTemporal::calcularErrorMedioVerticalEntreDosPuntos(int inicial, int final) const
{
	if (inicial == final)
		return 0.0;

	int numeroPuntosIntermedios = final -inicial -1;
	assert(inicial >= 1 and inicial <= numeroPuntosSerieTemporal());
	assert(final >= 1 and final <= numeroPuntosSerieTemporal());

	long double errorMedio = 0.0;
	    
	//Recta que une final e inicial
	Recta recta(puntoSerieTemporal(inicial), puntoSerieTemporal(final));

	for (int i = inicial + 1; i < final; i++)
		errorMedio += recta.distanciaVerticalPuntoRecta(puntoSerieTemporal(i)) / numeroPuntosIntermedios;

	return errorMedio;
}


long double SerieTemporal::calcularErrorMedioErroresMediosVerticales(const vector <int> &dominante) const
{
	int numeroPuntosDominantes = dominante.size();
	long double errorMedioTotal = 0.0;

	for (int i = 0; i < numeroPuntosDominantes - 1; i++)
		errorMedioTotal += calcularErrorMedioVerticalEntreDosPuntos(dominante[i], dominante[i + 1]) / (numeroPuntosDominantes - 1);

	return errorMedioTotal;
}


long double SerieTemporal::calcularErrorMaximoErroresMediosVerticales(const vector <int> &dominante) const
{
	int numeroPuntosDominantes = dominante.size();
	long double errorMaximoTotal = 0.0;
	double aux;

	for (int i = 0; i < numeroPuntosDominantes -1; i++)
	{
		aux = calcularErrorMedioVerticalEntreDosPuntos(dominante[i], dominante[i + 1]);
		if (aux > errorMaximoTotal)
		  	errorMaximoTotal = aux;
		}

	return errorMaximoTotal;
}




long double SerieTemporal::calcularErrorMedioErroresMaximosVerticales(const vector <int> &dominante) const
{
	int numeroPuntosDominantes = dominante.size();
	long double errorMedioTotal = 0.0;

	for (int i = 0; i < numeroPuntosDominantes - 1; i++)
		errorMedioTotal += calcularEmaxVerticalEntreDosPuntos(dominante[i], dominante[i + 1]) / (numeroPuntosDominantes - 1);

	return errorMedioTotal;
}


long double SerieTemporal::calcularErrorMaximoErroresMaximosVerticales(const vector <int> &dominante) const
{
	int numeroPuntosDominantes = dominante.size();
	long double errorMaximoTotal = 0.0;
	double aux;

	for (int i = 0; i < numeroPuntosDominantes -1; i++)
	{
		aux = calcularEmaxVerticalEntreDosPuntos(dominante[i], dominante[i + 1]);
		if (aux > errorMaximoTotal)
		  errorMaximoTotal = aux;
	}

	return errorMaximoTotal;
}


void SerieTemporal::calcularEmaxVerticalEntreDosPuntos(int inicial, int final, long double &errorMaximo, int &posicion) const
{
	long double alfa = atan((puntoSerieTemporal(final).y() - puntoSerieTemporal(inicial).y())/(puntoSerieTemporal(final).x() - puntoSerieTemporal(inicial).x()));
	calcularEmaxEntreDosPuntos(inicial, final, errorMaximo, posicion);
	errorMaximo = errorMaximo / fabs(cos(alfa));
}
	
void SerieTemporal::copiarPuntosDominantes(const SerieTemporal &serieTemporal)
{
 	int contadorDominantes = 0;
 
 	for (int t = 1; t <= serieTemporal.numeroPuntosSerieTemporal(); t++)
 	{
	  	if (serieTemporal.dominantePunto(t) == true)
	  	// Si el punto es dominante se traspasa al serieTemporal de puntos dominantes
	  	{
	   		contadorDominantes++;   		
	   		puntoSerieTemporal(serieTemporal.puntoSerieTemporal(t), contadorDominantes);
	  	}
 	}
  printf("Habia %d puntos dominantes\n", contadorDominantes);
}

void SerieTemporal::mostrarPuntosDominantes()
{
 	Punto p;
  
  for (int t = 1; t <= numeroPuntosSerieTemporal(); t++)
 	{
  	if (dominantePunto(t) == 1)
  	{
   		p = puntoSerieTemporal(t);  
   		cout << "El punto " << t << " de coordenadas x = " << p.x() << " y = " << p.y() <<" \n";
  	}
 	}
}

void SerieTemporal::guardarPuntosDominantes(vector <int> &vectorPuntosDominantes)const
{
    for(int i = 1; i <= numeroPuntosSerieTemporal(); i++)
    {
      if (dominantePunto(i))
	vectorPuntosDominantes[i] = 1;
      else
	vectorPuntosDominantes[i] = 0;
    }
}

	





