#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

namespace Fumasu
{

class Vector
{
public:
	/** Default constructor */
	Vector();
	Vector(double x, double y, double z);

	double GetX()
	{
		return x;
	}
	double GetY()
	{
		return y;
	}
	double GetZ()
	{
		return z;
	}

	void SetX(double v)
	{
		x = v;
	}
	void SetY(double v)
	{
		y = v;
	}
	void SetZ(double v)
	{
		z = v;
	}

	double X()
	{
		return GetX();
	}
	double Y()
	{
		return GetY();
	}
	double Z()
	{
		return GetZ();
	}

	const Vector operator+(Vector v);
	const Vector operator-(Vector v);
	const Vector operator*(double d);
	const Vector operator/(double d);
	Vector operator+=(Vector v);
	Vector operator-=(Vector v);
	Vector Cross(Vector v);
	double Dot(Vector v);

	Vector translate(Vector t);
	Vector translate(double x, double y, double z);
	Vector rotate(double angle, Vector r);
	Vector rotate(double angle, double x, double y, double z);

	double length();

	bool operator==(Vector v);
	bool operator!=(Vector v);
protected:
private:
	double x;
	double y;
	double z;
};

}

#endif // VECTOR_H
