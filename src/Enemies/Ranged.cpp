#include "Enemy.h"

Ranged::Ranged(int x, int y) :
	Enemy("Ranged")
{
	speed = 40;
	charge_duration = 0.6;
	//Makes me think, should the enemies array be upgraded into a complete class instead of just an std::vector? A convient place to store textures
	//In fact, I'll do exactly that with the animation update, since it's kind of weird refactoring this and then deleting it
	if (!texture.loadFromFile("content/lemon.png"))
	{
		std::cout << "failed to load texture";
	}
	sprite.setOrigin(16, 16);
	sprite.setTexture(texture);
	setGridPosition(x, y);
}

void Ranged::update(double dt)
{
	float prevx = x;
	float prevy = y;

	if (state == "attacking")
	{
		cooldown_progress = 0;
		projectiles->add(new Projectile(x, y, player->x, player->y, this));
		state = "passive";
	}
	else if (state == "pathfinding")
	{
		pathfinding(dt);

		if (!getObstructed(player->x, player->y))
		{
			state = "passive";
		}
	}
	else if (state == "stunned")
	{
		stunned_progress += dt;
		if (stunned_progress > stunned_duration)
		{
			stunned_progress = 0;
			state = "passive";
		}
	}
	else if (state == "passive")
	{
		cooldown_progress += dt;
		//rework this to use loaded
		//if (!pellet->stored)
		//{
		//	launch(tx, ty, -50, dt);
		//}

		if (cooldown_progress > cooldown_duration)
		{
			cooldown_progress = 0;

			state = "attacking";
		}
		else if (cooldown_progress < cooldown_duration - 0.5)
		{
			tx = player->x;
			ty = player->y;
		}

		if (getDistance(player->x, player->y) < 220 && getObstructed(player->x, player->y))
		{
			state = "pathfinding";
		}
	}
	auto [new_gx, new_gy] = this->getGridPosition();

	if (map->isSolid(new_gx, new_gy))
	{
		x = prevx;
		y = prevy;
	}
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
	target->draw(sprite);
}