#include "Map.h"

Map::Map(Image** wall_textures, Image** floor_textures, Image** ceiling_textures)
{
	map_width = 12;
	map_height = 12;
	map_data = new int[144]
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,2,2,0,0,0,0,1,
		1,0,0,0,0,2,2,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,1,
		1,2,0,0,0,0,0,0,2,2,2,1,
		1,2,0,0,0,0,0,0,0,2,2,1,
		1,0,0,0,0,2,2,0,0,0,0,1,
		1,0,0,0,0,2,2,0,0,0,0,1,
		1,0,0,0,0,2,2,0,0,0,0,1,
		1,0,0,0,0,2,2,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1
	};

	floor_data = new int[144]
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,0,0,0,0,2,2,2,1,
		1,1,1,1,0,0,0,0,2,2,2,1,
		1,1,1,1,0,0,0,0,2,2,2,1,
		1,1,1,1,0,3,3,0,2,2,2,1,
		1,1,1,1,3,3,3,3,2,2,2,1,
		1,1,1,1,3,3,3,3,2,2,2,1,
		1,1,1,1,0,3,3,0,2,2,2,1,
		1,1,1,1,0,0,0,0,2,2,2,1,
		1,1,1,1,0,0,0,0,2,2,2,1,
		1,1,1,1,0,0,0,0,2,2,2,1,
		1,1,1,1,1,1,1,1,1,1,1,1
	};

	ceiling_data = new int[144]
	{
		1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,1,2,0,1,2,0,1,2,0,1,
		1,1,2,0,1,2,0,1,2,0,1,1,
		1,2,0,1,2,0,1,2,0,1,2,1,
		1,0,1,2,0,1,2,0,1,2,0,1,
		1,1,2,0,1,2,0,1,2,0,1,1,
		1,2,0,1,2,0,1,2,0,1,2,1,
		1,0,1,2,0,1,2,0,1,2,0,1,
		1,1,2,0,1,2,0,1,2,0,1,1,
		1,2,0,1,2,0,1,2,0,1,2,1,
		1,0,1,2,0,1,2,0,1,2,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1
	};

	this->wall_textures = wall_textures;
	this->floor_textures = floor_textures;
	this->ceiling_textures = ceiling_textures;
}

Map::~Map()
{
	delete[] map_data;
}

bool Map::wall_intersection_movement(int x, int y)
{
	if (((x / map_tile_dimension) > map_width) || (x < 0)) return true;
	if (((y / map_tile_dimension) > map_height) || (y < 0)) return true;
	return map_data[(x / map_tile_dimension) + ((y / map_tile_dimension) * map_width)];
}

bool Map::wall_intersection(int grace_distance, Intersection& intersection)
{
	if ((intersection.intersection_x / map_tile_dimension) >= map_width || (intersection.intersection_x / map_tile_dimension) < 0)
	{
		intersection.wall_texture_sprite = wall_textures[MAP_WALL::WALL_ERROR];
		return true;
	}
	if ((intersection.intersection_y / map_tile_dimension) >= map_height || (intersection.intersection_y / map_tile_dimension) < 0)
	{
		intersection.wall_texture_sprite = wall_textures[MAP_WALL::WALL_ERROR];
		return true;
	}

	int map_wall = map_data[(intersection.intersection_x / map_tile_dimension) + (map_width * (intersection.intersection_y / map_tile_dimension))];
	if (map_wall)
	{
		intersection.wall_texture_sprite = wall_textures[map_wall];
		return true;
	}

	int map_wall_horizontal_plus = map_data[((intersection.intersection_x + grace_distance) / map_tile_dimension) + (map_width * (intersection.intersection_y / map_tile_dimension))];
	int map_wall_horizontal_minus = map_data[((intersection.intersection_x - grace_distance) / map_tile_dimension) + (map_width * (intersection.intersection_y / map_tile_dimension))];
	if (intersection.is_horizontal_intersection)
	{
		if (map_wall_horizontal_plus)
		{
			intersection.wall_texture_sprite = wall_textures[map_wall_horizontal_plus];
			return false;
		}
		if (map_wall_horizontal_minus)
		{
			intersection.wall_texture_sprite = wall_textures[map_wall_horizontal_minus];
			return false;
		}
	}

	int map_wall_vertical_plus = map_data[((intersection.intersection_x) / map_tile_dimension) + (map_width * ((intersection.intersection_y + grace_distance) / map_tile_dimension))];
	int map_wall_vertical_minus = map_data[((intersection.intersection_x) / map_tile_dimension) + (map_width * ((intersection.intersection_y - grace_distance) / map_tile_dimension))];
	if (!intersection.is_horizontal_intersection)
	{
		if (map_wall_vertical_plus)
		{
			intersection.wall_texture_sprite = wall_textures[map_wall_vertical_plus];
			return true;
		}
		if (map_wall_vertical_minus)
		{
			intersection.wall_texture_sprite = wall_textures[map_wall_vertical_minus];
			return true;
		}
	}

	return false;
}

int Map::floor_intersection(int x, int y)
{
	if (((x / map_tile_dimension) >= map_width) || (x < 0)) return MAP_FLOOR::FLOOR_ERROR;
	if (((y / map_tile_dimension) >= map_height) || (y < 0)) return MAP_FLOOR::FLOOR_ERROR;

	return floor_data[(x / map_tile_dimension) + ((y / map_tile_dimension) * map_width)];
}

int Map::ceiling_intersection(int x, int y)
{
	if (((x / map_tile_dimension) >= map_width) || (x < 0)) return MAP_CEILING::CEILING_ERROR;
	if (((y / map_tile_dimension) >= map_height) || (y < 0)) return MAP_CEILING::CEILING_ERROR;

	return ceiling_data[(x / map_tile_dimension) + ((y / map_tile_dimension) * map_width)];
}