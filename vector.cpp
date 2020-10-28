#include <math.h>
#include <cassert>
#include "vector.hpp"
#include "punto.hpp"

Vector2D::Vector2D()
{
	x(0.0);
	y(0.0);
}

Vector2D::Vector2D(const Punto &origen, const Punto &destino)
{
 	assert(origen != destino);
  x(destino.x() -origen.x());
 	y(destino.y() -origen.y());
}
 
Vector2D::Vector2D(const Vector2D &v)
{
	x(v.x());
	y(v.y());
}

Vector2D Vector2D::vectorUnitario()const
{
 	long double modulo = 0.0;
 	Vector2D w;
 
 	modulo = pow((pow(x(),2.0) + pow(y(),2.0)), 0.5);
 	w.x(x() / modulo);
 	w.y(y() / modulo);

 	return w;
}


Vector2D & Vector2D::operator=(const Vector2D &v2)
{
	x(v2.x());
	y(v2.y());

	return *this;
}

bool Vector2D::operator==(const Vector2D &v2)
{
 	if (fabs(x() - v2.x()) < 0.0000000001 && fabs(y() - v2.y()) < 0.0000000001)
  	return true;
 	else 
  	return false;
}
Vector2D Vector2D::operator+(const Vector2D &sumando2)
{
 	Vector2D suma;
 
 	suma.x(x() + sumando2.x());
 	suma.y(y() + sumando2.y());
 
 	return suma;
}

long double Vector2D::moduloVector2D()const
{
 return (pow(pow(x(),2.0) + pow(y(),2.0), 0.5));
}


