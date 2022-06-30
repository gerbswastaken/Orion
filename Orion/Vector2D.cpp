#include "Vector2D.h"

Vector2D::Vector2D(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Vector2D::dotProduct(const Vector2D* vec_a, const Vector2D* vec_b)
{
	return ((vec_a->x * vec_b->x) + (vec_a->y * vec_b->y));
}

void Vector2D::scalar_multiplication(double scalar)
{
	x *= scalar;
	y *= scalar;
}

void Vector2D::normalize()
{
	double inv_sqrt = 1.0 / sqrt( (this->x * this->x) + (this->y * this->y) );
	// if the vector is a zero vector, then we can't normalize it, so we exit the function
	if (isinf(inv_sqrt)) return;
	this->x *= inv_sqrt;
	this->y *= inv_sqrt;
}

void Vector2D::add_vector(const Vector2D* vec_add)
{
	this->x += vec_add->x;
	this->y += vec_add->y;
}

void Vector2D::print()
{
	std::cout << "( " << x << ", " << y << " )";
}