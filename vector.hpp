#ifndef Vector2D_H
#define Vector2D_H
#include "punto.hpp"

class Vector2D
{
	public:
		Vector2D();
		Vector2D(const Punto &origen, const Punto &destino);
		Vector2D(const Vector2D &v);
		inline long double x()const //Observador
    	{
			return _x;
		};

		inline long double y()const //Observador
    	{
			return _y;
		};

		inline void x(const long double &valor) //Modificador
    	{
			_x = valor;
		};

		inline void y(const long double &valor) //Modificador
    	{
			_y = valor;
		};

		Vector2D vectorUnitario()const;
		bool vectoresIguales(const Vector2D &v2);
		Vector2D & operator=(const Vector2D &v2);
		bool operator==(const Vector2D &v2);
		Vector2D operator+(const Vector2D &sumando2);
		long double moduloVector2D()const;

	private:
    long double _x;
    long double _y;
};

#endif
