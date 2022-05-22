#include "Enemy.h"

Enemy::Enemy() :
	sprite()
{
}

void Enemy::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	sprite.setRadius(16);
	if (state == "stunned")
	{
		sprite.setFillColor(sf::Color(100, 100, 100));
	}
	else
	{
		sprite.setFillColor(sf::Color(0, 250, 250));
	}
	sprite.setOrigin(16, 16);
	target->draw(sprite);
}

void Enemy::setState(std::string state)
{
	this->state = state;
}

void Enemy::pathfinding(double dt, Map* map)
{
	auto [gx, gy] = this->getGridPosition();

	int tx = gx + 1;
	int ty = gy;

	int cheapest = std::min({ map->getPathTile(gx, gy + 1), map->getPathTile(gx + 1, gy), map->getPathTile(gx - 1, gy), map->getPathTile(gx, gy - 1) });
	if (map->getPathTile(gx, gy + 1) == cheapest)
	{
		tx = gx;
		ty = gy + 1;
	}
	if (map->getPathTile(gx - 1, gy) == cheapest)
	{
		tx = gx - 1;
		ty = gy;
	}
	if (map->getPathTile(gx, gy - 1) == cheapest)
	{
		tx = gx;
		ty = gy - 1;
	}
	tx = tx * tileSize + tileSize / 2;
	ty = ty * tileSize + tileSize / 2;

	double dx = tx - x;
	double dy = ty - y;

	double distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

	vx = dx / distance * dt * speed;
	vy = dy / distance * dt * speed;

	if (vx > 0)
	{
		vx = std::ceil(vx);
	}
	else
	{
		vx = std::floor(vx);
	}
	if (vy > 0)
	{
		vy = std::ceil(vy);
	}
	else
	{
		vy = std::floor(vy);
	}

	move(*map);
}

bool Enemy::getObstructed(float player_x, float player_y, Map* map)
{
	auto [gx, gy] = getGridPosition();
	bool obstructed = false;
	for (int x = std::min(gx, std::floor(player_x / tileSize)); x <= std::max(gx, std::floor(player_x / tileSize)); x++)
	{
		for (int y = std::min(gy, std::floor(player_y / tileSize)); y <= std::max(gy, std::floor(player_y / tileSize)); y++)
		{

			if (map->getTile(x, y) == 1)
			{
				obstructed = true;
			}
		}
	}
	return obstructed;
}

Melee::Melee()
{
	speed = 40;
	charge_duration = 0.6;
}

void Melee::update(double dt, float player_x, float player_y, Map* map)
{
	tileSize = map->tileSize;

	static bool success = false;

	float prevx = x;
	float prevy = y;

	float dx = player_x - this->x;
	float dy = player_y - this->y;
	float distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

	if (state == "attacking")
	{
		launch(player_x, player_y, 36000, dt, *map);
		if (resolveCollision(*map))
		{
			state = "stunned";
		}

		success = contact(player_x, player_y);
		if (success)
		{
			//Will kill the player in the final build
			std::cout << "successful enemy hit!";
		}
		if (charge_progress > 2)
		{
			state = "pathfinding";
		}
	}

	if (state == "pathfinding")
	{

		if (getObstructed(player_x, player_y, map))
		{
			pathfinding(dt, map);
		}
		else
		{
			vx = dx / distance * dt * speed;
			vy = dy / distance * dt * speed;
			if (vx > 0)
			{
				vx = std::ceil(vx);
			}
			else
			{
				vx = std::floor(vx);
			}
			if (vy > 0)
			{
				vy = std::ceil(vy);
			}
			else
			{
				vy = std::floor(vy);
			}
			move(*map);

			if (distance < 100 && !resolveCollision(*map))
			{
				success = false;
				state = "attacking";
				charge_progress = 0;
			}
		}
	}

	if (state == "stunned")
	{
		stunned_progress += dt;
		if (stunned_progress > stunned_duration)
		{
			stunned_progress = 0;
			state = "pathfinding";
		}
	}

	auto [new_gx, new_gy] = this->getGridPosition();

	if (map->getTile(new_gx, new_gy) == 1)
	{
		x = prevx;
		y = prevy;
	}
	this->resolveCollision(*map);
	if (state == "passive")
	{
		if (getDistance(player_x, player_y) < 220 && !getObstructed(player_x, player_y, map))
		{
			state = "pathfinding";
		};
	}
}
