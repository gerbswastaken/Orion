#ifndef MAP_H
#define MAP_H

#include "Intersection.h"

enum MAP_WALL
{
	WALL_EMPTY,		// 0
	WALL_STONE,		// 1
	WALL_BOOKSHELF,	// 2
	WALL_TNT,		// 3
	WALL_ERROR,		// 4

	WALL_COUNT
};

enum MAP_FLOOR
{
	FLOOR_OAK_PLANKS,			// 0
	FLOOR_ACACIA_PLANKS,		// 1
	FLOOR_DARKOAK_PLANKS,		// 2
	FLOOR_CRIMSON_PLANKS,		// 3
	FLOOR_ERROR,				// 4

	FLOOR_COUNT
};


enum MAP_CEILING
{
	CEILING_IRON_BLOCK,			// 0
	CEILING_GOLD_BLOCK,			// 1
	CEILING_DIAMOND_BLOCK,		// 2
	CEILING_EMERALD_BLOCK,		// 3
	CEILING_ERROR,				// 4

	CEILING_COUNT
};


class Map
{
	public:
		int map_width;
		int map_height;
		int map_tile_dimension = 64;

		int* map_data;
		int* floor_data;
		int* ceiling_data;

		Image** wall_textures;
		Image** floor_textures;
		Image** ceiling_textures;

		Map(Image** wall_textures, Image** floor_textures, Image** ceiling_textures);
		~Map();

		bool wall_intersection_movement(int x, int y); 
		bool wall_intersection(int grace_distance, Intersection& intersection);

		int floor_intersection(int x, int y);
		int ceiling_intersection(int x, int y);
};

#endif

