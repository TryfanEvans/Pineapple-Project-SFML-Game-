#include "player.h"

sf::Texture character_face;

Player::Player() :
	sprite()
{

	character_face.loadFromFile("content/character_face.png");

	sprite.setTexture(character_face);
}

static bool attacking = false;
static int orientation;
static int range = 64;
static int direction = 1;

void Player::action(int relative_x, int relative_y, int button, std::vector<Melee*> enemies)
{
	std::cout << button;
	attacking = true;
	orientation = -atan2(relative_x, relative_y) + 3.14 / 2;
	direction = -direction;
	for (uint key = 0; key < enemies.size(); key++)
	{
		Enemy* value = enemies[key];
		int angle = -atan2(value->getX() - x, value->getY() - y) + 3.14 / 2;
		bool in_scope = angle > orientation - 3.14 / 4 and angle < orientation + 3.14 / 4;

		int dx = value->getX() - x;
		int dy = value->getX() - y;
		bool in_range = pow(dx, 2) + pow(dy, 2) < pow(range, 2);

		if (in_range && in_scope)
		{
			std::cout << "hit!" << std::endl;
			enemies.erase(enemies.begin() + key);
		}
	}
}

const float duration = 0.3;
static double angle1 = 0;
static double angle2 = 0;

void Player::attack(float dt)
{
	const double arclength = 0.8 * 3.14;

	static float progress = 0;
	// animation
	progress = progress + dt;
	if (progress > duration)
	{
		attacking = false;
		progress = 0;
	}
	std::cout << "proress" << progress;

	if (progress < 0.25)
	{
		angle1 = arclength * progress / duration;
		angle2 = arclength * progress / duration / 2;
	}
}

void Player::update(float dt, bool clipping, Map& map)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		y = y - speed * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		x = x - speed * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		y = y + speed * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		x = x + speed * dt;
	}
	if (clipping)
	{
		resolveCollision(map);
	}
	if (attacking)
	{
		attack(dt);
	}
}

void Player::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	sprite.setOrigin(16, 16);
	target->draw(sprite);
	if (attacking)
	{
		ArcSlash arc(x, y, 24, orientation + direction * (angle2 - 3.14 / 4), orientation + direction * (angle1 - 3.14 / 4));
		arc.render(target);
	}
}