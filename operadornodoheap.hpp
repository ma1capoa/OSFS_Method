#ifndef __OPERADORNODO_HPP__
#define __OPERADORNODO_HPP__

#include <iostream>
#include <fstream>  //Para los flujos de entrada/salida en ficheros.
#include <vector>  //Para usar vector de la stl

using namespace std;


template<class G>
class OperadorNodo
{
	public:
		virtual bool aplicar(const G & info) = 0;
};

//Clase para que el tratamiento de cada nodo sea escribir su campo informativo
template<class G>
class EscribirNodo: public OperadorNodo<G>
{
	public:
		EscribirNodo()
		{};
		~EscribirNodo()
		{};
		bool aplicar(const G &info)
		{
			std::cout << info << " ";
			return true;
		}
};

//Clase para que el tratamiento de cada nodo sea guardar en fichero su campo informativo
template<class G>
class GuardarNodo: public OperadorNodo<G>
{
	public:
		GuardarNodo(string fichero)
		{
			//El constructor abre el fichero.
			_flujoSalida.open(fichero.c_str(), ios::out); //Se abre el fichero para escribir.
		};
		~GuardarNodo()
		{
			//El destructor cierra el fichero
			_flujoSalida.close();
		};
		bool aplicar(const G &info)
		{
			//Cada vez que se pasa por un nodo se guarda en el fichero
			_flujoSalida.write((char *) (&info), sizeof(G)); //Guarda el registro. 
			return true;
		}
	private:
		ofstream _flujoSalida; //Se declara el flujo de salida.
};

//Clase para que el tratamiento de cada nodo sea almacenarlo en un vector de la stl
template<class G>
class AlmacenarNodo: public OperadorNodo<G>
{
	public:
		AlmacenarNodo()
		{};
		~AlmacenarNodo()
		{};
		bool aplicar(const G &info)
		{
			_v.push_back(info);  //almacenamos el nodo en el vector
			return true;
		};
		const vector<G> & vectorNodos()
		{
			return _v;
		};
		
	private:
		vector<G> _v;
};

#endif //__OPERADORNODO_HPP__
