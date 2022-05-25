#include "player.h"

static sf::Texture character_face;

Player::Player() :
	sprite(),
	pellet()
{
	charge_duration = 0;
	charge_progress = 0;
	int tileSize = 32;

	character_face.loadFromFile("content/character_face.png");

	sprite.setTexture(character_face);
	pellet.setPosition(x, y, tileSize);

	std::ifstream loadfile("player.txt");
	std::string line;
	while (std::getline(loadfile, line))
	{
		int space = line.find(" ");
		float x = std::stoi(line.substr(0, space + 1));
		float y = std::stoi(line.substr(space));
		setPosition(x, y, tileSize);
	}
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
			pellet.setPosition(x, y, 1);
			pellet.active = true;
			pellet.charge_progress = 0;
		}
	}
}

static double angle1 = 0;
static double angle2 = 0;

void Player::attack(float dt, Map& map)
{
	const double arclength = 3.14;
	launch(tx, ty, 200, dt, map);
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

void Player::update(float dt, Map& map, std::vector<Enemy*>& enemies, std::vector<Item>& items, bool& gameover)
{
	if (attacking)
	{
		attack(dt, map);
	}
	else
	{
		if (!pellet.stored)
		{
			launch(tx, ty, -200, dt, map);
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

			move(map);
		}
	}
	checkpoint(map);
	win(map, gameover);

	if (pellet.active)
	{
		pellet.launch(tx, ty, 800, dt, map);
		for (uint key = 0; key < enemies.size(); key++)
		{
			Enemy* value = enemies[key];
			if (pellet.contact(value->getX(), value->getY()))
			{
				std::cout << "shot!";
				value->setState("stunned");
			}
		}
		if ((pellet.resolveCollision(map) && !pellet.contact(x, y)) || (pellet.vx == 0 && pellet.vy == 0))
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

void Player::checkpoint(Map& map)
{
	auto [gx, gy] = getGridPosition();
	if (map.getTile(gx, gy) == 2)
	{
		std::cout << "Checkpoint!\n";
		std::ofstream playerfile;
		playerfile.open("player.txt");
		playerfile << gx << " " << gy << "\n";
		playerfile.close();
	}
}

void Player::win(Map& map, bool& gameover)
{
	auto [gx, gy] = getGridPosition();
	if (map.getTile(gx, gy) == 3)
	{
		std::cout << "Win!\n";
		gameover = true;
		std::ofstream playerfile;
		playerfile.open("player.txt");
		playerfile << 1 << " " << 1 << "\n";
		playerfile.close();
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
	pellet.render(target);
}