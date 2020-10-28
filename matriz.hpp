#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
#include <cstdio>
#include <cassert>

using namespace std;

template <class T>
class Matriz
{
  public:
		Matriz(const int &f, const int &c)
		{
			filas(f);
			columnas(c);
		
			reservarMemoria();

			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				{
				   elemento(i, j, 0.0);
				}
			}  
		}

		Matriz()
		{
			_m = NULL;
			filas(0);
			columnas(0);
		}

		//Constructor de copia.
		Matriz(const Matriz &m)
		{
			filas(m.filas());
			columnas(m.columnas());

			reservarMemoria();
			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				{
				   elemento(i, j, m.elemento(i, j));
				}
			}  
		}

		~Matriz()
		{
			liberarMatriz();
		}

		void liberarMatriz()
		{
			if (_m == NULL)
				return;
			
			for(int i = 0; i < filas(); ++i)
				delete [] _m[i];

			delete [] _m;
			_m = NULL;
		}

		inline int filas() const
		{
			return _f;
		}

		inline int columnas() const
		{
			return _c;
		}

		inline void filas(const int &f)
		{
			_f = f;
		}

		inline void columnas(const int &c)
		{
			_c = c;
		}

		inline T elemento(const int &f, const int &c) const
		{
			assert(f <= filas());
      		assert(c <= columnas());
			assert(f >= 1 && c >= 1);
			return _m[f - 1][c - 1];
		}

		inline void elemento(const int &f, const int &c, const T &x)
		{
			assert(f <= filas() && c <= columnas());
			_m[f - 1][c - 1] = x;
		}

		void verMatriz() const
		{
			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				  std::cout << elemento(i, j) << " ";
				std::cout << "\n";
			}
		}

		void rellenarMatriz()
		{
			T x;
			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				{
				  std::cout << " elemento(" << i << "," << j << "): ";
				  std::cin >> x;
				  elemento(i, j, x);
				}
			}
		}

		bool vacia()const
		{
			if (_m == NULL)
				return true;
			return false;
		}

		Matriz& operator=(const Matriz &m2)
		{
			filas(m2.filas());
			columnas(m2.columnas());
		
			if (vacia()) //Se comprueba si la matriz del primer miembro esta vacia.
			{
				reservarMemoria();
				for(int i = 1; i <= filas(); i++)
				{
				  for(int j = 1; j <= columnas(); j++)
				  {
				    elemento(i, j, m2.elemento(i, j));
				  }
				}
			}
			else //La matriz del primer miembro ya estaba creada.
			{
				assert (filas() == m2.filas() && columnas() == m2.columnas());
				//Al tener las mismas dimensiones se pueden asignar.
				for(int i = 1; i <= filas(); i++)
				{
				  for(int j = 1; j <= columnas(); j++)
				  {
				    elemento(i, j, m2.elemento(i, j));
				  }
				}
			}

			return *this;
		}
		 
  private:
    T **_m;
    int _f;
    int _c;
    void reservarMemoria()
		{
			_m = new T * [filas()];

			for(int i = 0; i < filas(); ++i)
				_m[i] = new T [columnas()];
		}
};


#endif
