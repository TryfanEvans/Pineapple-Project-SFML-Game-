#include "Enemy.h"

Ranged::Ranged(int x, int y) :
	Enemy("Ranged"),
	pellet()
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

void Ranged::update(double dt, float player_x, float player_y)
{
	float prevx = x;
	float prevy = y;

	pellet.update(dt, tx, ty);

	if (pellet.contact(player_x, player_y))
	{
		std::cout << "Shot the player, Gosh!\n";
	}

	if (state == "attacking")
	{
		cooldown_progress = 0;
		pellet.toss(x, y);
		state = "passive";
	}
	else if (state == "pathfinding")
	{

		pathfinding(dt);

		if (!getObstructed(player_x, player_y))
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
		if (!pellet.stored)
		{
			launch(tx, ty, -50, dt);
		}

		if (cooldown_progress > cooldown_duration)
		{
			cooldown_progress = 0;

			state = "attacking";
		}
		else if (cooldown_progress < cooldown_duration - 0.5)
		{
			tx = player_x;
			ty = player_y;
		}

		if (getDistance(player_x, player_y) < 220 && getObstructed(player_x, player_y))
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
	pellet.render(target);
}