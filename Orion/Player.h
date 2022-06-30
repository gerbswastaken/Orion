#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "InputHandler.h"
#include "Map.h"
#include "constants.h"
#include "Vector2D.h"

#include <math.h>

class Player : public Entity 
{
	public:
		const double friction_coefficient = 0.1;
		const double max_acceleration = 40.0;
		const double max_speed = 4.0;

		double speed = 8.0;
		int height;
		int projection_plane_offset = 0;
		Player(double x, double y, double direction, int height);

		Vector2D* velocity;

		void tick() override;
		void tick(Input* input, Map* map);
		void tick_new(Input* input, Map* map);
		void render(int buffer_width, int buffer_height, void* pixel_array) override;
};

#endif

