#include "Enemy.h"
Melee::Melee(int x, int y) :
	Enemy("Melee")
{
	speed = 40;
	charge_duration = 0.6;
	if (!texture.loadFromFile("content/pineapple.png"))
	{
		std::cout << "failed to load texture";
	}
	sprite.setOrigin(16, 16);
	sprite.setTexture(texture);
	setGridPosition(x, y);
}

void Melee::update(double dt)
{
	float prevx = x;
	float prevy = y;
	if (state == "attacking")
	{
		launch(player->x, player->y, 3200, dt);
		if (resolveCollision())
		{
			state = "stunned";
		}

		if (contact(player->x, player->y))
		{
			//Will kill the player in the final build
			std::cout << "Hit the player, Golly!\n";
			Scripts::actions_pending.push("Death");
		}
		if (charge_progress > 2)
		{
			state = "pathfinding";
		}
	}
	else if (state == "pathfinding")
	{

		if (getObstructed(player->x, player->y))
		{
			pathfinding(dt);
		}
		else
		{
			float dx = player->x - this->x;
			float dy = player->y - this->y;
			float distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

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
			move();

			if (distance < 100 && !resolveCollision())
			{
				state = "attacking";
				charge_progress = 0;
			}
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
	else if (state == "passive")
	{
		if (getDistance(player->x, player->y) < 220 && !getObstructed(player->x, player->y))
		{
			state = "pathfinding";
		};
	}
	auto [new_gx, new_gy] = this->getGridPosition();

	if (map->isSolid(new_gx, new_gy))
	{
		x = prevx;
		y = prevy;
	}
	this->resolveCollision();
}
