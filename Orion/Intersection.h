#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Image.h"

class Intersection
{
	public:
		bool is_horizontal_intersection;
		int intersection_x;
		int intersection_y;
		double intersection_distance;

		Image* wall_texture_sprite;
};

#endif