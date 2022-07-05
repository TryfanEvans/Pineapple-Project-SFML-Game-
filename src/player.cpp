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
	sprite.setOrigin(16, 16);
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

void Player::save(std::string level_name)
{
	auto [gx, gy] = getGridPosition();
	std::ofstream playerfile;
	playerfile.open("./levels/" + level_name + "/player.txt");
	playerfile << gx << " " << gy << "\n";
	playerfile.close();
}

static bool attacking = false;
static int orientation;
static int direction = 1;

static float tx;
static float ty;

bool Player::inHitbox(float tx, float ty)
{
	int angle = getOrientation(tx, ty);
	bool in_scope = angle > orientation - 3.14 / 3 and angle < orientation + 3.14 / 3;

	bool in_range = getDistance(tx, ty) < range;
	return (in_range && in_scope);
}

static bool hit = false;
void Player::action(int mx, int my, int button, std::vector<Enemy*>& enemies, std::string level_name)
{
	//Refactor:Could be split into two functions for left and right clicks
	tx = mx;
	ty = my;
	if (button == 1)
	{
		attacking = true;
		charge_progress = 0;
		orientation = getOrientation(tx, ty);
		direction = -direction;

		auto [gx, gy] = getGridPosition();
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				auto [x, y] = map->GridtoAbsolutePos(gx + i, gy + j);
				if (inHitbox(x, y) && map->isSolid(gx + i, gy + j))
				{
					std::cout << map->getTile(gx + i, gy + j) << "\n";
					hit = true;
					if (map->getTile(gx + i, gy + j) == 4)
					{
						save(level_name);
						torch_fuel = 60;
					}
				}
			}
		}
		for (uint key = 0; key < enemies.size(); key++)
		{
			Enemy* value = enemies[key];

			if (inHitbox(value->getX(), value->getY()))
			{
				std::cout << "hit!" << std::endl;
				hit = true;
				delete value;
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

	//Change this so it only happens when the player makes contact with the target
	if (hit)
	{

		vx = 0;
		vy = 0;
	}
	// animation
	if (charge_progress > 0.3)
	{
		attacking = false;
		hit = false;
	}

	if (charge_progress < 0.25)
	{
		angle1 = arclength * charge_progress / 0.3;
		angle2 = arclength * charge_progress / 0.3 / 2;
	}
}

void Player::update(float dt, std::vector<Enemy*>& enemies, std::vector<Item>& items)
{
	torch_fuel = torch_fuel - dt;
	//std::cout << torch_fuel << "\n";
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
	target->draw(sprite);
	if (attacking)
	{
		ArcSlash arc(x, y, 24, orientation + direction * (angle2 - 3.14 / 4), orientation + direction * (angle1 - 3.14 / 4));
		arc.render(target);
	}
	pellet.render(target);
}