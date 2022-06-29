#include "player.h"

static sf::Texture character_face;

Player::Player(Map* map) :
	Solid(16),
	sprite(),
	pellet(map)
{
	this->map = map;
	charge_duration = 0;
	charge_progress = 0;

	character_face.loadFromFile("content/character_face.png");

	sprite.setTexture(character_face);
}

void Player::load(std::string level_name)
{
	std::ifstream loadfile("./levels/" + level_name + "/player.txt");
	std::string line;
	while (std::getline(loadfile, line))
	{
		int space = line.find(" ");
		float x = std::stoi(line.substr(0, space + 1));
		float y = std::stoi(line.substr(space));
		setGridPosition(x, y);
	}
	loadfile.close();
}

static bool attacking = false;
static int orientation;
static int range = 86;
static int direction = 1;

static float tx;
static float ty;

void Player::action(int relative_x, int relative_y, int button, std::vector<Enemy*>& enemies)
{
	tx = x + relative_x;
	ty = y + relative_y;

	if (button == 1)
	{

		attacking = true;
		charge_progress = 0;
		orientation = -atan2(relative_x, relative_y) + 3.14 / 2;
		direction = -direction;
		for (uint key = 0; key < enemies.size(); key++)
		{
			Enemy* value = enemies[key];
			int angle = -atan2(value->getX() - x, value->getY() - y) + 3.14 / 2;
			bool in_scope = angle > orientation - 3.14 / 4 and angle < orientation + 3.14 / 4;

			bool in_range = value->getDistance(x, y) < range;

			if (in_range && in_scope)
			{
				std::cout << "hit!" << std::endl;
				enemies.erase(enemies.begin() + key);
			}
		}
	}
	else
	{
		if (pellet.stored)
		{
			charge_progress = 0;
			pellet.stored = false;
			pellet.setPosition(x, y);
			pellet.active = true;
			pellet.charge_progress = 0;
		}
	}
}

static double angle1 = 0;
static double angle2 = 0;

void Player::attack(float dt)
{
	const double arclength = 3.14;
	launch(tx, ty, 200, dt);
	// animation
	if (charge_progress > 0.3)
	{
		attacking = false;
	}

	if (charge_progress < 0.25)
	{
		angle1 = arclength * charge_progress / 0.3;
		angle2 = arclength * charge_progress / 0.3 / 2;
	}
}

void Player::update(float dt, std::vector<Enemy*>& enemies, std::vector<Item>& items)
{
	if (attacking)
	{
		attack(dt);
	}
	else
	{
		if (!pellet.stored)
		{
			launch(tx, ty, -200, dt);
		}
		if (vx == 0 && vy == 0)
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				vy = -speed * dt;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				vx = -speed * dt;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				vy = speed * dt;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				vx = speed * dt;
			}

			move();
		}
	}

	if (pellet.active)
	{
		pellet.launch(tx, ty, 800, dt);
		for (uint key = 0; key < enemies.size(); key++)
		{
			Enemy* value = enemies[key];
			if (pellet.contact(value->getX(), value->getY()))
			{
				std::cout << "shot!";
				value->setState("stunned");
			}
		}
		if ((pellet.resolveCollision() && !pellet.contact(x, y)) || (pellet.vx == 0 && pellet.vy == 0))
		{
			pellet.drop(items);
		}
	}
	else if (!pellet.stored)
	{
		for (uint key = 0; key < items.size(); key++)
		{
			Item& value = items[key];
			if (contact(value.x, value.y))
			{
				pellet.stored = true;
				items.erase(items.begin() + key);
			}
		}
	}
}

void Player::savePosition(std::string level_name)
{
	std::ofstream playerfile;
	playerfile.open("./levels/" + level_name + "/player.txt");
	auto [gx, gy] = getGridPosition();
	playerfile << gx << " " << gy << "\n";
	playerfile.close();
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
	pellet.render(target);
}