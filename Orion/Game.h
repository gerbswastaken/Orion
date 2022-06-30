#ifndef GAME_H
#define GAME_H

#include <vector>
#include <math.h>
#include <iostream>

#include "constants.h"
#include "InputHandler.h"
#include "Image.h"
#include "Intersection.h"
#include "Map.h"
#include "Player.h"

class Game
{
	public:
		void tick(Input* input);	// Handles the main game loop and such
		void tick_new(Input* input);

		void render(void* pixel_array, int buffer_width, int buffer_height);
		void render_new(void* pixel_array, int buffer_width, int buffer_height);

		void render_console();
		void clearScreen(void* pixel_array, int buffer_width, int buffer_height);
		void renderBMP(void* pixel_array, int buffer_width, int buffer_height, int top_left_x, int top_left_y, Image& image);
		Game(int buffer_width, int buffer_height, Image** wall_textures, Image** floor_textures, Image** ceiling_textures);

	private:
		int buffer_width;
		int buffer_height;

		std::vector<double> tan_values;
		std::vector<double> sin_values;
		std::vector<double> cos_values;

		Map* game_map;
		Player* player;

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

		double player_speed = 6.0;

		double player_max_velocity = 4.0;
		double player_acceleration_x = 0.0;
		double player_acceleration_y = 0.0;
		double player_velocity_x = 0.0;
		double player_velocity_y = 0.0;
		double player_friction_coefficient = 0.2;

		bool player_is_moving = false;

		double player_x = 384.0;
		double player_y = 384.0;
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
