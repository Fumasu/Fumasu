#include "Fumasu/Vector.hpp"

namespace Fumasu
{

Vector::Vector()
{
	x = y = z = 0;
}

Vector::Vector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

const Vector Vector::operator+(Vector v)
{
	Vector ret(*this);

	ret +=v;

	return ret;
	/**this +=v;

	return *this;*/
	//return Vector(x + v.GetX(), y + v.GetY(), z + v.GetZ());
}

const Vector Vector::operator-(Vector v)
{
	Vector ret(*this);

	ret -=v;

	return ret;
	//return Vector(x - v.X(), y - v.Y(), z - v.Z());
}

const Vector Vector::operator*(double d)
{
	return Vector(x * d, y * d, z * d);
}

const Vector Vector::operator/(double d)
{
	return Vector(x / d, y / d, z / d);
}

Vector Vector::operator+=(Vector v)
{
	x +=v.X();
	y +=v.Y();
	z +=v.Z();

	return *this;
}

Vector Vector::operator-=(Vector v)
{
	x -=v.X();
	y -=v.Y();
	z -=v.Z();

	return *this;
}

Vector Vector::Cross(Vector v)
{
	return Vector(y * v.GetZ() - z * v.GetY(), z * v.GetX() - x * v.GetZ(), x * v.GetY() - y * v.GetX());
}

/*
 * Berechnet das Skalarprodukt
 */
double Vector::Dot(Vector v)
{
	return x * v.GetX() + y * v.GetY() + z * v.GetZ();
}

Vector Vector::translate(Vector t)
{
	return Vector(x + t.GetX(), y + t.GetY(), z + t.GetZ());
}

Vector Vector::translate(double x, double y, double z)
{
	return Vector(this->x + x, this->y + y, this->z + z);
}

Vector Vector::rotate(double angle, Vector r)
{
	Vector normTrans = r / r.length();
	double alpha = angle * M_PI / 180;
	double n1 = normTrans.GetX(), n2 = normTrans.GetY(), n3 = normTrans.GetZ();
	double cosa = cos(alpha), sina = sin(alpha);
	struct
	{
		Vector v1, v2, v3;
	} RotMatrix;

	RotMatrix.v1 = Vector(cosa + pow(n1, 2) * (1 - cosa), n2 * n1 * (1 - cosa) + n3 * sina, n3 * n1 * (1 - cosa) - n2 * sina);
	RotMatrix.v2 = Vector(n1 * n2 * (1 - cosa) - n3 * sina, cosa + pow(n2, 2) * (1 - cosa), n3 * n2 * (1 - cosa) + n1 * sina);
	RotMatrix.v3 = Vector(n1 * n3 * (1 - cosa) + n2 * sina, n2 * n3 * (1 - cosa) - n1 * sina, cosa + pow(n3, 2) * (1 - cosa));

	normTrans = Vector((RotMatrix.v1.GetX() * this->x) + (RotMatrix.v2.GetX() * this->y) + (RotMatrix.v3.GetX() * this->z),
			(RotMatrix.v1.GetY() * this->x) + (RotMatrix.v2.GetY() * this->y) + (RotMatrix.v3.GetY() * this->z),
			(RotMatrix.v1.GetZ() * this->x) + (RotMatrix.v2.GetZ() * this->y) + (RotMatrix.v3.GetZ() * this->z));

	return normTrans;
}

Vector Vector::rotate(double angle, double x, double y, double z)
{
	Vector r = Vector(x, y, z);
	Vector normTrans = r / r.length();
	double alpha = angle * M_PI / 180;
	double n1 = normTrans.GetX(), n2 = normTrans.GetY(), n3 = normTrans.GetZ();
	double cosa = cos(alpha), sina = sin(alpha);
	struct
	{
		Vector v1, v2, v3;
	} RotMatrix;

	RotMatrix.v1 = Vector(cosa + pow(n1, 2) * (1 - cosa), n2 * n1 * (1 - cosa) + n3 * sina, n3 * n1 * (1 - cosa) - n2 * sina);
	RotMatrix.v2 = Vector(n1 * n2 * (1 - cosa) - n3 * sina, cosa + pow(n2, 2) * (1 - cosa), n3 * n2 * (1 - cosa) + n1 * sina);
	RotMatrix.v3 = Vector(n1 * n3 * (1 - cosa) + n2 * sina, n2 * n3 * (1 - cosa) - n1 * sina, cosa + pow(n3, 2) * (1 - cosa));

	normTrans = Vector((RotMatrix.v1.GetX() * this->x) + (RotMatrix.v2.GetX() * this->y) + (RotMatrix.v3.GetX() * this->z),
			(RotMatrix.v1.GetY() * this->x) + (RotMatrix.v2.GetY() * this->y) + (RotMatrix.v3.GetY() * this->z),
			(RotMatrix.v1.GetZ() * this->x) + (RotMatrix.v2.GetZ() * this->y) + (RotMatrix.v3.GetZ() * this->z));

	return normTrans;
}

double Vector::length()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

bool Vector::operator==(Vector v)
{
	if(v.GetX() == x && v.GetY() == y && v.GetZ() == z) return true;

	return false;
}

bool Vector::operator!=(Vector v)
{
	if(v.GetX() == x && v.GetY() == y && v.GetZ() == z) return false;

	return true;
}

}
