#include "Ranged.h"

Ranged::Ranged()
{
	speed = 40;
	charge_duration = 0.6;
}

void Ranged::update(double dt, float player_x, float player_y, Map* map)
{
	tileSize = map->tileSize;

	float prevx = x;
	float prevy = y;

	if (state == "attacking")
	{
	}
	else if (state == "pathfinding")
	{
		if (getObstructed(player_x, player_y, map))
		{
			pathfinding(dt, map);
		}
		else
		{
			state = "attacking";
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
