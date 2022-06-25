#ifndef GAME_H
#define GAME_H

#include <vector>
#include <math.h>
#include <iostream>

#include "constants.h"
#include "InputHandler.h"

class Game
{
	public:
		void tick(Input* input);	// Handles the main game loop and such
		void render(void* pixel_array, int buffer_width, int buffer_height);		// Handles the rendering (for now)
		void render_console();
		void clearScreen(void* pixel_array, int buffer_width, int buffer_height);
		Game(int buffer_width, int buffer_height);

	private:
		int buffer_width;
		int buffer_height;

		std::vector<double> tan_values;
		std::vector<double> sin_values;
		std::vector<double> cos_values;

		int map_tile_dimension = 64;

		int map_dimension = 12;
		int map[144] =
		{
			1,1,1,1,1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,1,1,1,1,
			1,0,0,0,0,0,0,0,1,1,1,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1,1,1,1,1
		};

		double player_speed = 8.0;
		double player_x = 244.63;
		double player_y = 203.171;
		double player_direction = 16.0;

		double distance_to_projection_plane;

		double get_horizontal_intersection_distance(int ray_x, int ray_y, int ray_direction);
		double get_horizontal_intersection_distance_debugging(int ray_x, int ray_y, int ray_direction);
		double get_vertical_intersection_distance(int ray_x, int ray_y, int ray_direction);

		bool wall_intersection(int x, int y);
};

#endif
