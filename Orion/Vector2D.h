#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>
#include <iostream>

class Vector2D
{
	public:
		Vector2D(double x, double y);

		double x;
		double y;

		static double dotProduct(const Vector2D* vec_a, const Vector2D* vec_b);
		void normalize();
		void scalar_multiplication(double scalar);
		void add_vector(const Vector2D* vec_add);

		void print();
};

#endif

