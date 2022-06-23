#include "Ranged.h"

Ranged::Ranged()
{
	speed = 40;
	charge_duration = 0.6;
	if (!texture.loadFromFile("content/lemon.png"))
	{
		std::cout << "failed to load texture";
	}
	sprite.setOrigin(16, 16);
	sprite.setTexture(texture);
	type = "Ranged";
}

void Ranged::update(double dt, float player_x, float player_y, Map* map, bool& dead)
{
	tileSize = map->tileSize;

	if (pellet.active)
	{
		pellet.launch(tx, ty, 1600, dt, *map);

		if (contact(player_x, player_y))
		{
			std::cout << "shot the player, gosh!";
			dead = true;
		}

		if ((pellet.resolveCollision(*map) && !pellet.contact(x, y)) || (pellet.vx == 0 && pellet.vy == 0))
		{
			pellet.active = false;
		}
		if (getObstructed(tx, ty, map))
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
		else if (cooldown_progress > cooldown_duration)
		{
			charge_progress = 0;
			cooldown_progress = 0;
			pellet.stored = false;
			pellet.setPosition(x, y, 1);
			pellet.active = true;
			pellet.charge_progress = 0;
		}
		else if (cooldown_progress < cooldown_duration - 0.5)
		{
			tx = player_x;
			ty = player_y;
		}
		cooldown_progress += dt;
		if (!pellet.stored)
		{
			launch(tx, ty, -50, dt, *map);
		}
	}
	else if (state == "stunned")
	{
		stunned_progress += dt;
		if (stunned_progress > stunned_duration)
		{
			stunned_progress = 0;
			state = "pathfinding";
		}
	}

	this->resolveCollision(*map);
	if (state == "passive")
	{
		if (getDistance(player_x, player_y) < 220 && !getObstructed(player_x, player_y, map))
		{
			state = "pathfinding";
		};
	}
	x += vx;
	y += vy;
}

void Ranged::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	if (state == "stunned")
	{
		sprite.setColor(sf::Color(100, 100, 100));
	}
	else
	{
		sprite.setColor(sf::Color(255, 255, 255));
	}
	sprite.setOrigin(16, 16);
	target->draw(sprite);
	pellet.render(target);
}