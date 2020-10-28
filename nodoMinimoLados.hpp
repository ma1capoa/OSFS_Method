#ifndef __NodoMinimoLados_HPP__
#define __NodoMinimoLados_HPP__
#include "punto.hpp"

		class NodoMinimoLados
		{
			private:
				//Punto _punto;
				int _indice;
				int _rango;
				int _padre;
				long double _coste;
				
			public:
				NodoMinimoLados()
				{
					//Punto p(0,0);
					//setPunto(p);
					setIndice(0);
					setRango(0);
					setPadre(0);
					setCoste(0.0);
				}
				
				NodoMinimoLados(/*const Punto punto,*/ const int indice, const int rango, const int padre, const long double coste)
				{
					//setPunto(punto);
					setIndice(indice);
					setRango(rango);
					setPadre(padre);
					setCoste(coste);
				}
				
				~NodoMinimoLados()
				{
				}
				
				NodoMinimoLados & operator=(const NodoMinimoLados &n)
				{
					if (this != &n)
					{
						//setPunto(n.getPunto());
						setIndice(n.getIndice());
						setRango(n.getRango());
						setPadre(n.getPadre());
						setCoste(n.getCoste());
					}
					return *this;
				}
	      
	      		//Dos NodoMinimoLadoss serán iguales cuando se correspondan con el mismo punto y tengan el mismo rango
				bool operator==(const NodoMinimoLados &n) const
				{
					return (getIndice() == n.getIndice() && getRango() == n.getRango());
				}

				//Sobrecarga del operador < para la ordenación.
				bool operator<(const NodoMinimoLados &n) const
				{
					long double aux = getCoste() - n.getCoste();

					if (getRango() < n.getRango())
						return true;
					else if (getRango() == n.getRango())
						return (aux < 0.0);
					else
						return false;
				}
				
				//Sobrecarga del operador < para la ordenación.
				bool operator>=(const NodoMinimoLados &n) const
				{
					long double aux = getCoste() - n.getCoste();

					if (getRango() < n.getRango())
						return false;
					else if (getRango() == n.getRango())
						return not (aux < 0.0);
					else
						return true;
					
				}

				
				const int &getIndice() const
				{
					return _indice;
				}
				
				const int &getRango() const
				{
					return _rango;
				}
				
				const int &getPadre() const
				{
					return _padre;
				}
				
				const long double &getCoste() const
				{
					return _coste;
				}
				
				
				void setIndice(const int indice)
				{
					_indice = indice;
				}
				
				void setRango(const int rango)
				{
					_rango = rango;
				}
				
				void setPadre(const int padre)
				{
					_padre = padre;
				}
				
				void setCoste(const long double coste)
				{
					_coste = coste;
				}
		};
		
#endif