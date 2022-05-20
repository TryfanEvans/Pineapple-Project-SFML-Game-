#include "Enemy.h"

Enemy::Enemy() :
	sprite()
{
}

void Enemy::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	sprite.setRadius(16);
	sprite.setFillColor(sf::Color(0, 250, 250));
	sprite.setOrigin(16, 16);
	target->draw(sprite);
}

void Enemy::die()
{
	std::cout << "dead\n";
}

Melee::Melee()
{
	speed = 40;
	charge_duration = 1;
}

void Melee::update(double dt, float player_x, float player_y, Map* map)
{
	tileSize = map->tileSize;
	static float vx = 0;
	static float vy = 0;

	static bool success = false;

	float prevx = x;
	float prevy = y;

	float dx = player_x - this->x;
	float dy = player_y - this->y;
	float distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

	if (state == "attacking")
	{
		charge(player_x, player_y, 800, dt);
		success = contact(player_x, player_y);
		std::cout << success;
		if (charge_progress > 3)
		{
			charge_progress = 0;
			state = "pathfinding";
		}
	}
	auto [gx, gy] = this->getGridPosition();

	if (state == "pathfinding")
	{
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

		if (obstructed)
		{
			std::cout << "obstructed";

			int tx = gx + 1;
			int ty = gy;

			int cheapest = std::min({ map->getPathTile(gx, gy + 1), map->getPathTile(gx + 1, gy), map->getPathTile(gx - 1, gy), map->getPathTile(gx, gy - 1) });
			std::cout << cheapest;
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
			dx = tx - x;
			dy = ty - y;

			distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

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
			x += vx;
			y += vy;
		}
		else
		{
			std::cout << "not obstructed";
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
			x = x + vx;
			y = y + vy;

			if (distance < 100)
			{
				charge_progress = 0;
				success = false;
				state = "attacking";
			}
		}
	}
	this->resolveCollision(*map);
	if (map->getTile(gx, gy) == std::numeric_limits<int>::max())
	{
		x = prevx;
		y = prevy;
	}
}
