#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <sys/time.h> //para la función gettimeofday
#include <vector>    // necesario para el contenedor vector
#include <list>
#include <queue>
#include <algorithm>
#include "algoritmo.hpp"
#include "metodoSerieTemporalEMaxOffLine.hpp"
#include "nodoMinimoLados.hpp"
#include "matriz.hpp"
#include "punto.hpp"
#include "recta.hpp"
#include "heapvectorMinimoLados.hpp"
#include "operadornodoheap.hpp"
#include "clock.hpp"

using namespace std;

/* FUNCIONES DE INTERFAZ PARA EL TIPO MetodoOptimizado */

//Constructor.

MetodoMinimoLadosOffLine::MetodoMinimoLadosOffLine(char * nombreFicheroSerieTemporal, int numeroInicialPuntosSerie, long double eMax, int limitePodaNumeroLados, vector<int> &limitePodaLAdosAcumulados, 
                                                               long double limitePodaISE)
{	
  //Creamos un serieTemporal temporal donde se carga el serieTemporal
  SerieTemporal c(nombreFicheroSerieTemporal);

  //Lo copiamos en el serieTemporal privado.
  serieTemporal(c);

  //Introducimos el nombre del fichero en la variable 
  strcpy(nombreFichero, nombreFicheroSerieTemporal);
  setNumeroInicialPuntosSerie(numeroInicialPuntosSerie);
  setEMax(eMax);
  setLimitePodaNumeroLados(limitePodaNumeroLados);
  setLimitePodaNumeroLadosAcumulados(limitePodaLAdosAcumulados);
  setLimitePodaISE(limitePodaISE);
}

MetodoMinimoLadosOffLine::MetodoMinimoLadosOffLine(const SerieTemporal &c, int numeroInicialPuntosSerie, long double eMax, int limitePodaNumeroLados, vector<int> &limitePodaLadosAcumulados, 
                                                              long double limitePodaISE)
{
  //Lo copiamos en el serieTemporal privado.
  serieTemporal(c);
  setNumeroInicialPuntosSerie(numeroInicialPuntosSerie);
  setEMax(eMax);
  setLimitePodaNumeroLados(limitePodaNumeroLados);
  setLimitePodaNumeroLadosAcumulados(limitePodaLadosAcumulados);
  setLimitePodaISE(limitePodaISE);
}

//Destructor
MetodoMinimoLadosOffLine::~MetodoMinimoLadosOffLine()
{
}

void MetodoMinimoLadosOffLine::aplicar()
{
	Clock time;
	//Variables para el numero de puntos del serieTemporal y el numero a optimizar
 	int numeroPuntosSerieTemporal;
 
 	SerieTemporal serieTemporalPuntosDominantes;

	//Variables para guardar los errores que se van obteniendo
 	long double errorMaximo, sumaErroresCuadrado;
 	//long double error;


 	//Obtenemos numero de puntos del serieTemporal cargado y reservar así el nuevo serieTemporal 
 	numeroPuntosSerieTemporal = serieTemporal().numeroPuntosSerieTemporal();
  
 	/* Matrices de los sumatorios */
  if (serieTemporal().esCerrado() == false)
  {
    Matriz<long double> suma_x(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1 );
    Matriz<long double> suma_y(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1 );
    Matriz<long double> suma_xx(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1 );
    Matriz<long double> suma_yy(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1 );
    Matriz<long double> suma_xy(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1 );
  }
  
  Matriz<long double> suma_x(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1 );
 	Matriz<long double> suma_y(numeroPuntosSerieTemporal + 1, numeroPuntosSerieTemporal + 1 );
 	Matriz<long double> suma_xx(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1);
 	Matriz<long double> suma_yy(numeroPuntosSerieTemporal + 1, numeroPuntosSerieTemporal + 1 );
 	Matriz<long double> suma_xy(numeroPuntosSerieTemporal + 1 , numeroPuntosSerieTemporal + 1 );

	/* Ahora vamos a calcular los sumatorios */
 	/* Este cálculo se realiza una sola vez, así se evita el tener que calcular sumatorios repetidas veces */
 	calcularSumatorios(suma_x, suma_y, suma_xx, suma_yy, suma_xy);
	
	
	/*Reservamos la matriz para ver la evolución de los puntos dominantes.
 	 Tendrá tantas filas como iteraciones y tantas columnas como puntos tenga el serieTemporal.
 	 si el elemento i,j es 1 indicará que cuando se obtiene el serieTemporal optimo con i puntos, el punto j pertenece a 	 ese serieTemporal si el elemento i,j es 0 indicará que cuando se obtiene el serieTemporal optimo con i puntos, el punto 	  j no pertenece a ese serieTemporal*/

  //vector para ver los puntos dominantes 
  if (serieTemporal().esCerrado() == false)
  {
    vector <int> vectorPuntosDominantes((unsigned int) numeroPuntosSerieTemporal + 1);
  }
    
  vector <int> vectorPuntosDominantes((unsigned int) numeroPuntosSerieTemporal + 1);
	
	//Estas matrices que se declaran a continuación facilitarán las búsquedas, aunque no son estrictamente necesarias.
	//El primer índice indica número de nodo y el segundo indica rango.
	long double **costeActual = NULL; //matriz para guardar costes actuales
	bool **abierta=NULL; //Controla en que lista está el nodo junto con su rango
	bool **cerrada=NULL; //Controla en que lista está el nodo junto con su rango
	costeActual = new long double * [numeroPuntosSerieTemporal + 1];
	for(int i = 0; i <= numeroPuntosSerieTemporal; i++)
		costeActual[i] = new long double [getLimitePodaNumeroLados() + 1];


  
	abierta = new bool * [numeroPuntosSerieTemporal + 1];
		for(int i = 0; i <= numeroPuntosSerieTemporal; i++)
			abierta[i] = new bool [getLimitePodaNumeroLados() + 1];
  
  cerrada = new bool * [numeroPuntosSerieTemporal + 1];
	for(int i = 0; i <= numeroPuntosSerieTemporal; i++)
		cerrada[i] = new bool [getLimitePodaNumeroLados() + 1];

	
	//Matriz para guardar el padre del nodo seleccionado y reconstruir aproximación
	int **padreActual = NULL;
	
  //Matriz para saber la posición de cada nodo con su rango en el heap
	int **posicionNodo = NULL;
	
	

	//Contador para ver las podas porque la solucion no es viable ya que no mejora a la mejor solución del momento.
	static int podadosPrematuros = 0;
	
	
	padreActual = new int * [numeroPuntosSerieTemporal + 1];
	for(int i = 0; i <= numeroPuntosSerieTemporal; i++)
		padreActual[i] = new int [getLimitePodaNumeroLados() + 1];
	
	posicionNodo = new int * [numeroPuntosSerieTemporal + 1];
	for(int i = 0; i <= numeroPuntosSerieTemporal; i++)
		posicionNodo[i] = new int [getLimitePodaNumeroLados() + 1];
	
	int contadorTotalPodados = 0;
  int contadorPodadosISE = 0;
  int contadorPodadosNumeroLados = 0;
  int contadorNodosActualizados = 0;

  //Estructuras necesarias para el A*
  list <NodoMinimoLados> listaCerrada; //Lista que albergará los nodos ya seleccionados
  HeapVectorMinimoLados listaAbierta; //heap que albergará los nodos candidatos
  //HeapVectorMinimoLados listaAbiertaNueva; //heap que albergará los nodos candidatos para la siguiente pasada

  
  contadorTotalPodados = 0;
  //Todas las iteraciones utilizan la misma matriz, pero en cada iteración el trozo de matriz usado aumenta en una columna
  //Inicialización de los trozos de matriz que se usan en cada iteración.
  costeActual[1][0] = 0.0;
  for(int i = 0; i <= numeroPuntosSerieTemporal; i++)
    for(int j = 0; j <= getLimitePodaNumeroLados(); j++)
    {
      costeActual[i][j] = 0.0;
      padreActual[i][j] = -1;
      cerrada[i][j] = false;
      abierta[i][j] = false;
      posicionNodo[i][j] = 0;
    }
  
  NodoMinimoLados minimo; //Nodo para almacenar el minimo en cada momento de la lista abierta
  
  //Comienza el algoritmo A*
  
  
  //Medimos tiempos al principio.
  time.start();

	
  NodoMinimoLados nInicial(1, 1, 0, costeActual[1][0]);
  
  //Se inserta nodo inicial en la lista abierta.
  assert(listaAbierta.insertar(nInicial, posicionNodo)); 
  
  //En la matriz de la lista abierta se marca el primer nodo con rango 0
  abierta[1][0] = true; 
  int pasada = 0; //Para controlar las pasadas.
          
  
  while (1)
  {
    pasada++;
    
    //Comprobamos que la lista abierta no esta vacia
    if (listaAbierta.estaVacio())
    {
      cout << "Lista Vacia en pasada " <<  pasada << "El ultimo extraido es el punto "<< minimo.getIndice() << " y se llevan " << minimo.getRango() << "Puntos. Numero de podas por ISE= " << contadorPodadosISE << " Numero de podas por lados = " << contadorPodadosNumeroLados << endl;
      podadosPrematuros++;
      break; //Nos salimos ya que por aquí no mejoramos.
    }
    
    //Sacamos el nodo mínimo de la lista abierta.
    minimo = listaAbierta.minimo();

    
    //borramos el mínimo de la lista abierta
    listaAbierta.borrarMinimo(posicionNodo);
    
    //Marcamos en la matriz que el nodo ha salido de la lista abierta.
    abierta[minimo.getIndice()][minimo.getRango()] = false;
    
    //Introducimos el nodo minimo en la cerrada
    listaCerrada.push_back(minimo);
    
    //Marcamos en la matriz que el nodo ha entrado en la lista cerrada.
    //También marcamos que no podrá entrar en la solución con un rango superior. (for)
    for(int i = minimo.getRango(); i <= getLimitePodaNumeroLados(); i++)
      cerrada[minimo.getIndice()][i] = true;

    if (minimo.getIndice() == getNumeroInicialPuntosSerie()) //Se ha seleccionado el último punto de la serie en la primera etapa y por tanto se ha completado la segmentación en dicha etapa.
    {
      break;
    }
   
    //Se evalúan ahora todos los posibles sucesores a entrar en la lista de abiertos.
    insertarCandidatosListaAbiertos(minimo, listaAbierta, posicionNodo/*, listaAbiertaNueva, posicionNodoListaNueva*/, abierta, cerrada, contadorTotalPodados, contadorNodosActualizados, contadorPodadosISE, contadorPodadosNumeroLados,
                                     getNumeroInicialPuntosSerie(), costeActual, padreActual, suma_x, suma_y, suma_xx, suma_yy, suma_xy);
  }

  //Ahora vamos a generar el serieTemporal optimo
  SerieTemporal serieTemporalOptimo(serieTemporal());

  //Reservamos espacio para los puntos dominantes del serieTemporal optimo
  serieTemporalOptimo.reservarPuntoDominanteSerieTemporal();

  /*Marcamos todos los puntos como no dominantes*/
  serieTemporalOptimo.marcarTodosPuntosNoDominantes();
    
  //Obtenemos los puntos que conforman el serieTemporal optimo.
  obtenerPuntosSerieTemporalOptimo(serieTemporalOptimo, padreActual, minimo.getRango(), getNumeroInicialPuntosSerie());
  vector <int> dominantes;
  dominantes.push_back(0);

  for(int i = 1; i <= getNumeroInicialPuntosSerie(); i++)
    if (serieTemporalOptimo.dominantePunto(i))
      dominantes.push_back(i);
    
  //Calculamos el error máximo cometido.
  errorMaximo = 0.0;
  sumaErroresCuadrado = 0.0;
  serieTemporalOptimo.diferenciaVerticalSerieTemporalDominantesReales(dominantes, sumaErroresCuadrado, errorMaximo);
  dominantes.erase(dominantes.begin()); //Se borra el primero ya que es 0, de cara a no guardarlo
  setVectorSerieTemporalOptimo(dominantes);
  setEMax(errorMaximo);
  setISE(sumaErroresCuadrado);
  setNumeroPuntosAproximacion(dominantes.size());
  
  //Guardamos los puntos dominantes de la iteración en la matriz.
  serieTemporalOptimo.guardarPuntosDominantes(vectorPuntosDominantes);

}

//Esta funcion inserta los candidatos posibles teniendo en cuenta el espacio factible y además usando variuos criterios de poda.
//Entre los criterios de poda que se usan o se han usado están los siguientes.
//1. Que el ISE acumulado en la optimización no supere el ISE total de la segmantación global del método no óptimo. Esta parece que no poda nada.
//2. Que el número de segmentos acumulado en la optimización no supere al número de segmentos acumulados en el método no óptimo más un margen. Esta si poda pero hay que subir bastante el margen, lo cual no parece lógico.


void MetodoMinimoLadosOffLine::insertarCandidatosListaAbiertos(NodoMinimoLados nodoInicial, HeapVectorMinimoLados &listaAbierta, int ** posicionNodo/*, HeapVectorMinimoLados &listaAbiertaNueva, int ** posicionNodoListaNueva*/, 
                                                                    bool **abierta, bool **cerrada, int &contadorTotalPodados, int &contadorNodosActualizados, int &contadorPodadosISE, int &contadorPodadosNumeroLados, int &ultimoPuntoSerie, long double **costeActual, 
                                                                    int **padreActual, Matriz<long double> &suma_x, Matriz<long double> &suma_y, Matriz<long double> &suma_xx, Matriz<long double> &suma_yy, Matriz<long double> &suma_xy)
{
  int i = nodoInicial.getIndice(); //Posicion que ocupa el nodo en la serie
  long double lowM, upM;
  lowM = -10000000.0; //Pendiente del limite inferior del espacio factible
  upM = 10000000.0; //Pendiente del limite superior del espacio factible
  Punto puntoInicial = serieTemporal().puntoSerieTemporal(i); //Punto inicial del segmento a considerar
  Punto puntoFinal;  //Punto final del segmento a considerar
  Punto puntoFinalInferior, puntoFinalSuperior; //Límites inferior y superior del punto final del segmento a considerar.
  long double mInferior, mSuperior; //pendientes para los límites inferior y superior
  long double costeAdicional, costeTotal; //Variables para calcular el incremento de coste al incluir nodos nuevos
  //int heuristico; //Heuristico para el A* que sera un % de los lados del método suboptimo
  if (nodoInicial.getIndice() == ultimoPuntoSerie - 1 /*&& cerrada[ultimoPuntoSerie][nodoInicial.getRango() + 1] == false*/) //El punto Inicial es el penúltimo de la serie. Entonces se añade el último sin comprobación
  {
    //Se crea un nodo nuevo, con los datos nuevos
    NodoMinimoLados nuevo(ultimoPuntoSerie, nodoInicial.getRango() + 1, nodoInicial.getIndice(), nodoInicial.getCoste());
    costeActual[ultimoPuntoSerie][nodoInicial.getRango() + 1] = nodoInicial.getCoste();
    padreActual[ultimoPuntoSerie][nodoInicial.getRango() + 1] = nodoInicial.getIndice(); //El padre será el nodo inicial
    assert(listaAbierta.insertar(nuevo, posicionNodo)); //Se inserta el nuevo en la lista abierta
    //assert(listaAbiertaNueva.insertar(nuevo, posicionNodoListaNueva)); //Se inserta también en la lista nueva para la siguiente iteración
    abierta[ultimoPuntoSerie][nodoInicial.getRango() + 1] = true;
    //cout << "Entra en la lista de candidatos el ultimo punto de la serie \n";
    return;
  }
  vector <int> posicionesCandidatos;

  //Ahora comenzamos a calcular los segmentos en que se va a descomponer la serie usando el método FSW
  while (lowM < upM and i < ultimoPuntoSerie) // Mientras que no se cierre el espacio factible, y no lleguemos al último punto de la serie
  {
    i++;
    //cout << "Analizando el segmento " << ultimoPuntoSegmentacion << " ->" << i << endl;
   // getchar();
    puntoFinal = serieTemporal().puntoSerieTemporal(i);
    //Se calculan los limites inferior y superior del último extremo del segmento.
    puntoFinalInferior = puntoFinal;
    puntoFinalInferior.y(puntoFinal.y() - getEMax());
    puntoFinalSuperior = puntoFinal;
    puntoFinalSuperior.y(puntoFinal.y() + getEMax());
    
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
    
   
    Recta r(puntoInicial, puntoFinal);
    if (lowM <= r.m() and r.m() <= upM and cerrada[i][nodoInicial.getRango() + 1] == false) //Hemos encontrado un posible punto de segmentación que será candidato a la lista de abiertos
    {
      posicionesCandidatos.push_back(i);
    }
  }

  for(int j = 0; j < (int)posicionesCandidatos.size(); j++)
  {
      i = posicionesCandidatos[j];
      //Primero comprobamos que el candidato no esté en la lista de cerrados y que además sea dominante en el método subóptimo
      if (cerrada[i][nodoInicial.getRango() + 1] == false /*&& serieTemporal().dominantePunto(i)*/) //La segunda parte del predicado es para usar solo los puntos del método subóptimo
      {
          //Calculamos coste desde el minimo al nodo i.
          //Este coste es el de la suma de los errores en vertical
          costeAdicional = sumaErroresYsobreX(nodoInicial.getIndice(), i, suma_x, suma_y, suma_xx, suma_yy, suma_xy);
          costeTotal = nodoInicial.getCoste() + costeAdicional;
        //Ahora evaluamos si el candidato ya estaba en la lista
        if (abierta[i][nodoInicial.getRango() + 1] == false) //No estaba en la lista de abiertos
        {
          NodoMinimoLados nuevo(i, nodoInicial.getRango() + 1, nodoInicial.getIndice(), costeTotal);
          costeActual[i][nodoInicial.getRango() + 1] = costeTotal;
          padreActual[i][nodoInicial.getRango() + 1] = nodoInicial.getIndice(); //El padre será el nodo inicial
          listaAbierta.insertar(nuevo, posicionNodo); //Se inserta el nuevo en la lista abierta
          abierta[i][nodoInicial.getRango() + 1] = true;
        }
        else //Ya estaba en la lista de abiertos. Hay que ver si se actualiza o no.
        {
          if (costeTotal < costeActual[i][nodoInicial.getRango() + 1]) //El nuevo coste es menor. Se actualiza
          {
            NodoMinimoLados antiguo(i, nodoInicial.getRango() + 1, nodoInicial.getIndice(), costeActual[i][nodoInicial.getRango() + 1]);
            NodoMinimoLados nuevo(i, nodoInicial.getRango() + 1, nodoInicial.getIndice(), costeTotal);
            costeActual[i][nodoInicial.getRango() + 1] = costeTotal;
            padreActual[i][nodoInicial.getRango() + 1] = nodoInicial.getIndice(); //El padre será el nodo inicial
            contadorNodosActualizados++;
            listaAbierta.actualizar(antiguo, nuevo, posicionNodo);
          }
        }
      }    
    }  
}


void MetodoMinimoLadosOffLine::obtenerPuntosSerieTemporalOptimo(SerieTemporal &serieTemporal, int ** padre, const int & numeroPuntosProbados, const int & numeroInicialPuntosSerie)
{
  int i, j, contador = numeroPuntosProbados;
  i = numeroInicialPuntosSerie; //Esto se ha actualizado para adaptarlo al número de puntos que tenga la serie en cada momento.
  j = numeroPuntosProbados;

  //El primer punto dominante que se asigna es el punto inicial del serieTemporal. Esto viene impuesto por el método 	de Pérez y Vidal.
  serieTemporal.dominantePunto(1, true);
  serieTemporal.dominantePunto(i, true);
	do
  {
  	serieTemporal.dominantePunto(padre[i][j], true);

    contador--;
    i = padre[i][j];
    --j;
  }while(padre[i][j] != 1);
}



