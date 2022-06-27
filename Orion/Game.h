#ifndef GAME_H
#define GAME_H

#include <vector>
#include <math.h>
#include <iostream>

#include "constants.h"
#include "InputHandler.h"
#include "Image.h"
#include "Intersection.h"

class Game
{
	public:
		void tick(Input* input);	// Handles the main game loop and such
		void render(void* pixel_array, int buffer_width, int buffer_height, Image& texture);

		void render_new(void* pixel_array, int buffer_width, int buffer_height, Image& wall_texture, Image& floor_texture, Image& ceiling_texture);

		void render_console();
		void clearScreen(void* pixel_array, int buffer_width, int buffer_height);
		void renderBMP(void* pixel_array, int buffer_width, int buffer_height, int top_left_x, int top_left_y, Image& image);
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
		double player_x = 372.026;
		double player_y = 240.137;
		double player_direction = 330.5;
		int player_height;
		int projection_plane_offset = 0;

		int grace_value = 2;

		double distance_to_projection_plane;

		Intersection& get_horizontal_intersection_distance(int ray_x, int ray_y, int ray_direction, Intersection& intersection_horizontal);
		Intersection& get_vertical_intersection_distance(int ray_x, int ray_y, int ray_direction, Intersection& intersection_vertical);

		bool wall_intersection(int grace_distance, Intersection& intersection);
		bool wall_intersection_movement(int x, int y);


		Intersection& get_minimum_intersection(Intersection& intersection_horizontal, Intersection& intersection_vertical);
};

#endif
