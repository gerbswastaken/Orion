#include "Game.h"

Game::Game(int buffer_width, int buffer_height, Image** wall_textures, Image** floor_textures, Image** ceiling_textures)
{
	this->buffer_width = buffer_width;
	this->buffer_height = buffer_height;
	distance_to_projection_plane = (buffer_width / 2) / tan((constants::field_of_view / 2) * (constants::pi / 180.0));
	for (int i = 0; i < 360; ++i)
	{
		//tan_values.push_back(tan(i));
		//sin_values.push_back(sin(i));
		//cos_values.push_back(cos(i));
	}
	player_height = (map_tile_dimension / 2);

	game_map = new Map(wall_textures, floor_textures, ceiling_textures);
}

Intersection& Game::get_vertical_intersection_distance(int ray_x, int ray_y, int ray_direction, Intersection& intersection_vertical)
{
	intersection_vertical.is_horizontal_intersection = false;
	double ray_direction_radians = (static_cast<double>(ray_direction) / 180.0) * constants::pi;
	int x_step;
	if (ray_direction > 270 || ray_direction < 90)
	{
		intersection_vertical.intersection_x = ((ray_x / map_tile_dimension) * map_tile_dimension) + map_tile_dimension;
		x_step = (map_tile_dimension);
	}
	else
	{
		intersection_vertical.intersection_x = ((ray_x / map_tile_dimension) * map_tile_dimension) - 1;
		x_step = -(map_tile_dimension);
	}

	int y_step = -(x_step * tan(ray_direction_radians));
	intersection_vertical.intersection_y = ray_y - ((intersection_vertical.intersection_x - ray_x) * tan(ray_direction_radians));

	/*
	while (!wall_intersection(grace_value, intersection_vertical))
	{
		intersection_vertical.intersection_x += x_step;
		intersection_vertical.intersection_y += y_step;
	}
	*/

	while (!game_map->wall_intersection(grace_value, intersection_vertical))
	{
		intersection_vertical.intersection_x += x_step;
		intersection_vertical.intersection_y += y_step;
	}


	intersection_vertical.intersection_distance = (abs((ray_x - intersection_vertical.intersection_x) / cos(ray_direction_radians)));
	return intersection_vertical;
}


Intersection& Game::get_horizontal_intersection_distance(int ray_x, int ray_y, int ray_direction, Intersection& intersection_horizontal)
{
	intersection_horizontal.is_horizontal_intersection = true;
	double ray_direction_radians = (static_cast<double>(ray_direction) / 180.0) * constants::pi;
	int y_step;
	if (ray_direction < 180)
	{
		intersection_horizontal.intersection_y = ((ray_y / map_tile_dimension) * map_tile_dimension) - 1;
		y_step = -(map_tile_dimension);
	}
	else
	{
		intersection_horizontal.intersection_y = ((ray_y / map_tile_dimension) * map_tile_dimension) + map_tile_dimension;
		y_step = (map_tile_dimension);
	}

	int x_step = (-y_step) / tan(ray_direction_radians);
	intersection_horizontal.intersection_x = ray_x + (ray_y - intersection_horizontal.intersection_y) / tan(ray_direction_radians);

	while (!game_map->wall_intersection(grace_value, intersection_horizontal))
	{
		intersection_horizontal.intersection_x += x_step;
		intersection_horizontal.intersection_y += y_step;
	}

	intersection_horizontal.intersection_distance = (abs((ray_y - intersection_horizontal.intersection_y) / sin(ray_direction_radians)));

	return intersection_horizontal;
}

Intersection& Game::get_minimum_intersection(Intersection& intersection_horizontal, Intersection& intersection_vertical)
{
	if (intersection_horizontal.intersection_distance < intersection_vertical.intersection_distance) return intersection_horizontal;
	return intersection_vertical;
}

bool Game::wall_intersection(int grace_distance, Intersection& intersection)
{
	if ((intersection.intersection_x / map_tile_dimension) > map_dimension || (intersection.intersection_x / map_tile_dimension) < 0) return true;
	if ((intersection.intersection_y / map_tile_dimension) > map_dimension || (intersection.intersection_y / map_tile_dimension) < 0) return true;
	if (map[(intersection.intersection_x / map_tile_dimension) + (map_dimension * (intersection.intersection_y / map_tile_dimension))]) return true;
	if (intersection.is_horizontal_intersection)
	{
		if (map[((intersection.intersection_x + grace_distance) / map_tile_dimension) + (map_dimension * (intersection.intersection_y / map_tile_dimension))]) return true;
		if (map[((intersection.intersection_x - grace_distance) / map_tile_dimension) + (map_dimension * (intersection.intersection_y / map_tile_dimension))]) return true;
	}
	if (!intersection.is_horizontal_intersection)
	{
		if (map[(intersection.intersection_x / map_tile_dimension) + (map_dimension * ((intersection.intersection_y + grace_distance) / map_tile_dimension))]) return true;
		if (map[(intersection.intersection_x / map_tile_dimension) + (map_dimension * ((intersection.intersection_y - grace_distance) / map_tile_dimension))]) return true;
	}

	return false;
}

bool Game::wall_intersection_movement(int x, int y)
{
	if (((x / map_tile_dimension) > map_dimension) || (x < 0)) return true;
	if (((y / map_tile_dimension) > map_dimension) || (y < 0)) return true;
	return map[(x / map_tile_dimension) + ((y / map_tile_dimension) * map_dimension)];
}

void Game::tick(Input* input)
{
	player_is_moving = false;

	if (player_direction > 360.0) player_direction -= 360.0;
	if (player_direction < 0.0) player_direction += 360.0;

	if (input->key_input[KEY_LEFT].is_down) player_direction += 2.0;
	if (input->key_input[KEY_RIGHT].is_down) player_direction -= 2.0;
	if (input->key_input[KEY_UP].is_down) projection_plane_offset += 5;
	if (input->key_input[KEY_DOWN].is_down) projection_plane_offset -= 5;

	player_direction -= input->mouse_dx;
	projection_plane_offset -= 10 * input->mouse_dy;

	double velocity_x = 0.0;
	double velocity_y = 0.0;

	if (input->key_input[KEY_W].is_down)
	{
		velocity_x += player_speed * cos(player_direction * (constants::pi / 180.0));
		velocity_y -= player_speed * sin(player_direction * (constants::pi / 180.0));
	}

	if (input->key_input[KEY_A].is_down)
	{
		velocity_x -= player_speed * sin(player_direction * (constants::pi / 180.0));
		velocity_y -= player_speed * cos(player_direction * (constants::pi / 180.0));
	}

	if (input->key_input[KEY_S].is_down)
	{
		velocity_x -= player_speed * cos(player_direction * (constants::pi / 180.0));
		velocity_y += player_speed * sin(player_direction * (constants::pi / 180.0));
	}

	if (input->key_input[KEY_D].is_down)
	{
		velocity_x += player_speed * sin(player_direction * (constants::pi / 180.0));
		velocity_y += player_speed * cos(player_direction * (constants::pi / 180.0));
	}

	if (!game_map->wall_intersection_movement((player_x + (velocity_x * 4)), player_y)) player_x += velocity_x;
	if (!game_map->wall_intersection_movement(player_x, (player_y + (velocity_y * 4)))) player_y += velocity_y;

}

void Game::clearScreen(void* pixel_array, int buffer_width, int buffer_height)
{
	unsigned int* pixel = static_cast<unsigned int*>(pixel_array);
	for (int y = (buffer_height - 1); y >= 0; --y)
	{
		for (int x = 0; x < buffer_width; ++x)
		{
			pixel[x + (y * buffer_width)] = y;
		}
		
	}
	*pixel = 0x0000FF;
}

void Game::render(void* pixel_array, int buffer_width, int buffer_height, Image& texture)
{
	// Clear the screen
	unsigned int* pixel = static_cast<unsigned int*>(pixel_array);
	for (int i = 0; i < buffer_width * buffer_height; ++i)
	{
		pixel[i] = 0x000000;
	}

	double degrees_per_pixel = constants::field_of_view / static_cast<double>(buffer_width);

	for (int x = 0; x < buffer_width; ++x)
	{
		int ray_direction = (player_direction - (constants::field_of_view / 2)) + (x * degrees_per_pixel);
		if (ray_direction > 360) ray_direction -= 360;
		if (ray_direction < 0) ray_direction += 360;
		
		Intersection intersection_horizontal;
		Intersection intersection_vertical;
		Intersection minimum;
		minimum = get_minimum_intersection(get_horizontal_intersection_distance(player_x, player_y, ray_direction, intersection_horizontal), get_vertical_intersection_distance(player_x, player_y, ray_direction, intersection_vertical));
		double corrected_distance = minimum.intersection_distance * cos(static_cast<double>((ray_direction - player_direction)) * (constants::pi / 180.0));
		int wall_slice_height = (map_tile_dimension / corrected_distance) * distance_to_projection_plane;
		
		// Determining the Offset required for texture_mapping:
		int texture_offset;
		if (minimum.is_horizontal_intersection)
		{
			texture_offset = minimum.intersection_x % map_tile_dimension;
		}
		else
		{
			texture_offset = minimum.intersection_y % map_tile_dimension;
		}
		
		unsigned int color = 0x000000;
		int y_max = fmin(wall_slice_height, buffer_height);
		
		for (int i = 0; i < wall_slice_height; ++i)
		{
			int y = ((buffer_height / 2) - (wall_slice_height / 2) + i);
			if (y >= buffer_height || y < 0) continue;
			color = texture.pixel_array[texture_offset + (static_cast<int>((static_cast<double>(i) / static_cast<double>(wall_slice_height)) * map_tile_dimension) * texture.width)];
			pixel[(buffer_width - x) + (y * buffer_width)] = color;
		}
	}
}

void Game::render_new(void* pixel_array, int buffer_width, int buffer_height)
{
	// Clear the screen
	unsigned int* pixel = static_cast<unsigned int*>(pixel_array);
	for (int i = 0; i < buffer_width * buffer_height; ++i)
	{
		pixel[i] = 0x000000;
	}

	double degrees_per_pixel = constants::field_of_view / static_cast<double>(buffer_width);

	for (int x = 0; x < buffer_width; ++x)
	{
		int ray_direction = (player_direction - (constants::field_of_view / 2)) + (x * degrees_per_pixel);
		if (ray_direction > 360) ray_direction -= 360;
		if (ray_direction < 0) ray_direction += 360;

		Intersection intersection_horizontal;
		Intersection intersection_vertical;
		Intersection minimum;
		minimum = get_minimum_intersection(get_horizontal_intersection_distance(player_x, player_y, ray_direction, intersection_horizontal), get_vertical_intersection_distance(player_x, player_y, ray_direction, intersection_vertical));
		double corrected_distance = minimum.intersection_distance * cos(static_cast<double>((ray_direction - player_direction)) * (constants::pi / 180.0));
		int wall_slice_height = (map_tile_dimension / corrected_distance) * distance_to_projection_plane;

		// Determining the Offset required for texture mapping the walls:
		int wall_texture_offset;
		if (minimum.is_horizontal_intersection)
		{
			wall_texture_offset = minimum.intersection_x % map_tile_dimension;
		}
		else
		{
			wall_texture_offset = minimum.intersection_y % map_tile_dimension;
		}

		unsigned int color = 0x000000;
		for (int i = 0; i < wall_slice_height; ++i)
		{
			int y = (((buffer_height / 2) - projection_plane_offset) - (wall_slice_height / 2) + i);
			if (y >= buffer_height || y < 0) continue;
			color = minimum.wall_texture_sprite->pixel_array[wall_texture_offset + (static_cast<int>((static_cast<double>(i) / static_cast<double>(wall_slice_height)) * map_tile_dimension) * (minimum.wall_texture_sprite)->width)];
			pixel[(buffer_width - x) + (y * buffer_width)] = color;
		}

		// Texture-mapping the Floors:

		int wall_bottom_y = (((buffer_height / 2) - projection_plane_offset) - (wall_slice_height / 2));
		// First, check whether we need to draw any floors in the first place!
		if (!((((buffer_height / 2) - projection_plane_offset) - (wall_slice_height / 2)) < 0))
		{
			for (int y = wall_bottom_y - 1; y >= 0; --y)
			{
				if (y >= buffer_height || y < 0) continue;
				int perpendicular_distance_floor = ( (distance_to_projection_plane * player_height) / (((buffer_height / 2) - projection_plane_offset) - y) );
				int corrected_perpendicular_distance_floor = (perpendicular_distance_floor / cos(static_cast<double>((ray_direction - player_direction)) * (constants::pi / 180.0)) );

				int x_intersection_floor = player_x + (corrected_perpendicular_distance_floor * cos(static_cast<double>(ray_direction) * (constants::pi / 180.0)) );
				int y_intersection_floor = player_y - (corrected_perpendicular_distance_floor * sin(static_cast<double>(ray_direction) * (constants::pi / 180.0)));

				int floor_texture_offset_x = x_intersection_floor % map_tile_dimension;
				int floor_texture_offset_y = y_intersection_floor % map_tile_dimension;

				
				//color = floor_texture.pixel_array[floor_texture_offset_x + (floor_texture_offset_y * floor_texture.width)];
				//int debug = game_map->floor_intersection(x_intersection_floor, y_intersection_floor);
				Image* floor_texture = game_map->floor_textures[game_map->floor_intersection(x_intersection_floor, y_intersection_floor)];
				color = floor_texture->pixel_array[floor_texture_offset_x + (floor_texture_offset_y * floor_texture->width)];

				pixel[(buffer_width - x) + (y * buffer_width)] = color;
			}
		}

		// Texture-mapping the Ceilings:
		int wall_top_y = (((buffer_height / 2) - projection_plane_offset) + (wall_slice_height / 2));
		// First, check whether we need to draw any floors in the first place!
		if (!((((buffer_height / 2) - projection_plane_offset) + (wall_slice_height / 2)) >= buffer_height))
		{
			for (int y = wall_top_y; y < buffer_height; ++y)
			{
				if (y >= buffer_height || y < 0) continue;
				int perpendicular_distance_ceiling = ((distance_to_projection_plane * (map_tile_dimension - player_height)) / (y - ((buffer_height / 2) - projection_plane_offset)));
				int corrected_perpendicular_distance_ceiling = (perpendicular_distance_ceiling / cos(static_cast<double>((ray_direction - player_direction)) * (constants::pi / 180.0)));

				int x_intersection_ceiling = player_x + (corrected_perpendicular_distance_ceiling * cos(static_cast<double>(ray_direction) * (constants::pi / 180.0)));
				int y_intersection_ceiling = player_y - (corrected_perpendicular_distance_ceiling * sin(static_cast<double>(ray_direction) * (constants::pi / 180.0)));

				int ceiling_texture_offset_x = x_intersection_ceiling % map_tile_dimension;
				int ceiling_texture_offset_y = y_intersection_ceiling % map_tile_dimension;

				//color = ceiling_texture.pixel_array[ceiling_texture_offset_x + (ceiling_texture_offset_y * ceiling_texture.width)];
				//int debug = game_map->ceiling_intersection(x_intersection_ceiling, y_intersection_ceiling);
				Image* ceiling_texture = game_map->ceiling_textures[game_map->ceiling_intersection(x_intersection_ceiling, y_intersection_ceiling)];
				color = ceiling_texture->pixel_array[ceiling_texture_offset_x + (ceiling_texture_offset_y * ceiling_texture->width)];
				
				pixel[(buffer_width - x) + (y * buffer_width)] = color;
			}
		}
	}
}

void Game::render_console()
{
	system("CLS");
	for (int map_y = 0; map_y < map_dimension; ++map_y)
	{
		for (int map_x = 0; map_x < map_dimension; ++map_x)
		{
			if ( (map_x == static_cast<int>((player_x / map_tile_dimension))) && (map_y == static_cast<int>((player_y / map_tile_dimension)))) std::cout << "P ";
			else if (map[map_x + (map_y * map_dimension)]) std::cout << "X ";
			else std::cout <<"  ";
			
		}
		std::cout << '\n';
	}
	std::cout << "(X, Y, Direction): (" << player_x << ", " << player_y << ", " << player_direction << ")\n";

}

void Game::renderBMP(void* pixel_array, int buffer_width, int buffer_height, int top_left_x, int top_left_y, Image& image)
{
	unsigned int* pixel = static_cast<unsigned int*>(pixel_array);
	for (int y = 1; y <= image.height; ++y)
	{
		for (int x = 0; x < image.width; ++x)
		{
			if ((top_left_x + x) > buffer_width) continue;
			if ((top_left_y + y) > buffer_height) continue;

			pixel[(top_left_x + x) + ((buffer_height - (top_left_y + y)) * buffer_width)] = image.pixel_array[x + ((image.height - y) * image.width)];
		}

	}
}



