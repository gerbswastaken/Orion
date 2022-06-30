#include "Player.h"

Player::Player(double x, double y, double direction, int height)
{
	this->x = x;
	this->y = y;
	this->direction = direction;
	this->height = height;

	velocity = new Vector2D(0, 0);
}

void Player::tick()
{
	// We're doing nothing with this function.
	// It will never ever be called.
}


void Player::render(int buffer_width, int buffer_height, void* pixel_array)
{
	// We're doing nothing with this function.
	// It will never ever be called.
}

void Player::tick(Input* input, Map* map)
{
	if (direction > 360.0) direction -= 360.0;
	if (direction < 0.0) direction += 360.0;

	direction -= input->mouse_dx;
	projection_plane_offset -= 10 * input->mouse_dy;

	velocity_x = 0.0;
	velocity_y = 0.0;

	if (input->key_input[KEY_W].is_down)
	{
		velocity_x += speed * cos(direction * (constants::pi / 180.0));
		velocity_y -= speed * sin(direction * (constants::pi / 180.0));

	}

	if (input->key_input[KEY_A].is_down)
	{
		velocity_x -= speed * sin(direction * (constants::pi / 180.0));
		velocity_y -= speed * cos(direction * (constants::pi / 180.0));

	}

	if (input->key_input[KEY_S].is_down)
	{
		velocity_x -= speed * cos(direction * (constants::pi / 180.0));
		velocity_y += speed * sin(direction * (constants::pi / 180.0));
	}

	if (input->key_input[KEY_D].is_down)
	{
		velocity_x += speed * sin(direction * (constants::pi / 180.0));
		velocity_y += speed * cos(direction * (constants::pi / 180.0));
	}

	if (!map->wall_intersection_movement((x + (velocity_x * 4)), y)) x += velocity_x;
	if (!map->wall_intersection_movement(x, (y + (velocity_y * 4)))) y += velocity_y;

}

void Player::tick_new(Input* input, Map* map)
{
	if (direction > 360.0) direction -= 360.0;
	if (direction < 0.0) direction += 360.0;

	direction -= input->mouse_dx;
	projection_plane_offset -= 10 * input->mouse_dy;

	Vector2D* wish_direction = new Vector2D(0.0, 0.0);
	bool key_down = false;

	if (input->key_input[KEY_W].is_down)
	{
		wish_direction->x += speed * cos(direction * (constants::pi / 180.0));
		wish_direction->y -= speed * sin(direction * (constants::pi / 180.0));
	}

	if (input->key_input[KEY_A].is_down)
	{
		wish_direction->x -= speed * sin(direction * (constants::pi / 180.0));
		wish_direction->y -= speed * cos(direction * (constants::pi / 180.0));
	}

	if (input->key_input[KEY_S].is_down)
	{
		wish_direction->x -= speed * cos(direction * (constants::pi / 180.0));
		wish_direction->y += speed * sin(direction * (constants::pi / 180.0));
	}

	if (input->key_input[KEY_D].is_down)
	{
		wish_direction->x += speed * sin(direction * (constants::pi / 180.0));
		wish_direction->y += speed * cos(direction * (constants::pi / 180.0));
	}

	wish_direction->normalize();

	velocity->scalar_multiplication((1 - friction_coefficient));

	double current_speed_in_wish_direction = Vector2D::dotProduct(velocity, wish_direction);
	double add_speed_in_wish_direction = fmin((max_speed - current_speed_in_wish_direction), max_acceleration);

	wish_direction->scalar_multiplication(add_speed_in_wish_direction);
	velocity->add_vector(wish_direction);

	if (!map->wall_intersection_movement((x + (velocity->x * 4)), y)) x += velocity->x;
	if (!map->wall_intersection_movement(x, (y + (velocity->y * 4)))) y += velocity->y;
}