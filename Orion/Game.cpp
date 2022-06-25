#include "Game.h"

Game::Game(int buffer_width, int buffer_height)
{
	this->buffer_width = buffer_width;
	this->buffer_height = buffer_height;
	distance_to_projection_plane = (buffer_width / 2) / tan((constants::field_of_view / 2) * (constants::pi / 180.0));
	for (int i = 0; i < 3600; ++i)
	{
		//tan_values.push_back(tan((static_cast<double>(i) / 10.0)));
		//sin_values.push_back(sin((static_cast<double>(i) / 10.0)));
		//cos_values.push_back(cos((static_cast<double>(i) / 10.0)));
	}
}

// This function will return the horizontal intersection distance
double Game::get_vertical_intersection_distance(int ray_x, int ray_y, int ray_direction)
{
	double ray_direction_radians = (static_cast<double>(ray_direction) / 180.0) * constants::pi;
	int x_step;
	int x_intersection;
	if (ray_direction > 270 || ray_direction < 90)
	{
		x_intersection = ((ray_x / map_tile_dimension) * map_tile_dimension) + map_tile_dimension;
		x_step = (map_tile_dimension);
	}
	else
	{
		x_intersection = ((ray_x / map_tile_dimension) * map_tile_dimension) - 1;
		x_step = -(map_tile_dimension);
	}
	
	int y_step = -(x_step * tan(ray_direction_radians));
	int y_intersection = ray_y - ((x_intersection - ray_x) * tan(ray_direction_radians));

	while (!wall_intersection(x_intersection, y_intersection))
	{
		x_intersection += x_step;
		y_intersection += y_step;
	}

	return ( abs( (ray_x - x_intersection) / cos(ray_direction_radians) ) );
}

double Game::get_horizontal_intersection_distance_debugging(int ray_x, int ray_y, int ray_direction)
{
	double ray_direction_radians = (static_cast<double>(ray_direction) / 180.0) * constants::pi;
	int y_step;
	int y_intersection;
	if (ray_direction < 180)
	{
		y_intersection = ((ray_y / map_tile_dimension) * map_tile_dimension) - 1;
		y_step = -(map_tile_dimension);
	}
	else
	{
		y_intersection = ((ray_y / map_tile_dimension) * map_tile_dimension) + map_tile_dimension;
		y_step = (map_tile_dimension);
	}

	int x_step = y_step / tan(ray_direction_radians);
	int x_intersection = ray_x + (ray_y - y_intersection) / tan(ray_direction_radians);

	std::cout << "x_step: " << x_step << " | y_step: " << y_step << '\n';
	std::cout << "x_intersection: " << x_intersection << " | y_intersection: " << y_intersection << '\n';

	while (!wall_intersection(x_intersection, y_intersection))
	{
		x_intersection += x_step;
		y_intersection += y_step;
	}

	return (abs((ray_y - y_intersection) / sin(ray_direction_radians)));
}

double Game::get_horizontal_intersection_distance(int ray_x, int ray_y, int ray_direction)
{
	double ray_direction_radians = (static_cast<double>(ray_direction) / 180.0) * constants::pi;
	int y_step;
	int y_intersection;
	if (ray_direction < 180)
	{
		y_intersection = ((ray_y / map_tile_dimension) * map_tile_dimension) - 1;
		y_step = -(map_tile_dimension);
	}
	else
	{
		y_intersection = ((ray_y / map_tile_dimension) * map_tile_dimension) + map_tile_dimension;
		y_step = (map_tile_dimension);
	}

	int x_step = (-y_step) / tan(ray_direction_radians);
	int x_intersection = ray_x + (ray_y - y_intersection) / tan(ray_direction_radians);

	while (!wall_intersection(x_intersection, y_intersection))
	{
		x_intersection += x_step;
		y_intersection += y_step;
	}

	return (abs((ray_y - y_intersection) / sin(ray_direction_radians)));
}

bool Game::wall_intersection(int x, int y)
{
	//int to_check = (x / map_tile_dimension) + (map_dimension * (y / map_tile_dimension));
	//if (to_check > map_dimension * map_dimension || to_check < 0) return true;
	if ((x / map_tile_dimension) > map_dimension || (x / map_tile_dimension) < 0) return true;
	if ((y / map_tile_dimension) > map_dimension || (y / map_tile_dimension) < 0) return true;
	
	return map[ (x / map_tile_dimension) + (map_dimension * (y / map_tile_dimension)) ];
}

void Game::tick(Input* input)
{
	if (player_direction > 360.0) player_direction -= 360.0;
	if (player_direction < 0.0) player_direction += 360.0;
	if (input->key_input[KEY_LEFT].is_down) player_direction += 2.5;
	if (input->key_input[KEY_RIGHT].is_down) player_direction -= 2.5;
	
	if (input->key_input[KEY_SPACE].is_down)
	{
		std::cout << "(X, Y, Direction): (" << player_x << ", " << player_y << ", " << player_direction << ")\n";
		std::cout << "Horizontal Intersection Distance: " << get_horizontal_intersection_distance(player_x, player_y, static_cast<int>(player_direction)) << "\n";
		std::cout << "Vertical Intersection Distance: " << get_vertical_intersection_distance(player_x, player_y, static_cast<int>(player_direction)) << "\n";
	}
		
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

	player_x += velocity_x;
	player_y += velocity_y;
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


// TODO: CHANGE TO int FOR SPEEDUP!
void Game::render(void* pixel_array, int buffer_width, int buffer_height)
{
	unsigned int* pixel = static_cast<unsigned int*>(pixel_array);
	for (int i = 0; i < buffer_width * buffer_height; ++i) 
	{
		pixel[i] = 0x666666;
	}

	double degrees_per_pixel = constants::field_of_view / static_cast<double>(buffer_width);
	unsigned int color = 0x00FF000;
	for (int x = 0; x < buffer_width; ++x)
	{
		color = 0x00FF00;
		if (x == (buffer_width / 2)) color = 0xFF0000;
		int ray_direction = (player_direction - (constants::field_of_view / 2)) + (x * degrees_per_pixel);
		if (ray_direction > 360) ray_direction -= 360;
		if (ray_direction < 0) ray_direction += 360;
		double min_distance = fmin(get_horizontal_intersection_distance(player_x, player_y, ray_direction), get_vertical_intersection_distance(player_x, player_y, ray_direction));
		double corrected_distance = min_distance * cos(static_cast<double>((ray_direction - player_direction)) * (constants::pi / 180.0));
		int wall_slice_height = (map_tile_dimension / corrected_distance) * distance_to_projection_plane;
		if (wall_slice_height > buffer_height) wall_slice_height = buffer_height;
		for (int i = 0; i < wall_slice_height; ++i)
		{
			pixel[(buffer_width - x) + (((buffer_height / 2) - (wall_slice_height / 2) + i) * buffer_width)] = color;
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