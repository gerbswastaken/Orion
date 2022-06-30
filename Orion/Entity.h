#ifndef ENTITY_H
#define ENTITY_H

#include "Image.h"

class Entity
{
	public:
		double x;
		double y;
		double velocity_x;
		double velocity_y;
		double direction;

		Image* sprite;

		virtual void tick() = 0;
		virtual void render(int buffer_width, int buffer_height, void* pixel_array) = 0;
};

#endif

