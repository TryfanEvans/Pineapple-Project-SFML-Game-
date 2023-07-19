#include "player.h"

static sf::Texture character_face;

Player::Player() :
	Entity(16),
	sprite(),
	file("player")
{
	this->map = map;

	charge_duration = 0;
	charge_progress = 0;

	character_face.loadFromFile("content/character_face.png");

	sprite.setTexture(character_face);
	sprite.setOrigin(16, 16);
}

void Player::load()
{
	std::ifstream loadfile = file.getFile();
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
void Player::action(int mx, int my, int button, EntityVec& enemies)
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
						save();
						torch_fuel = 60;
					}
				}
			}
		}
		//https://www.youtube.com/watch?v=5DlXGFfoR-g&list=PL6xSOsbVA1ebkU66okpi-KViAO8_9DJKg&index=153

		for (int key = 0; key < enemies.getSize(); key++)
		{
			Entity* value = enemies.getEntity(key);

			if (inHitbox(value->getX(), value->getY()))
			{
				std::cout << "hit!" << std::endl;
				hit = true;
				value->takeHit();
			}
		}
	}
	else
	{
		if (loaded)
		{
			//Add to the list of projectilesVec
			loaded = !loaded;
			projectiles->add(new Projectile(x, y, mx, my, this));
		}
	}
}

static double angle1 = 0;
static double angle2 = 0;

//could be interesting having it such the there is windup, execution and cooldown period. cooldown period specifically is cool
void Player::attack(float dt)
{
	const double arclength = 3.14;

	launch(tx, ty, 200, dt);

	//Change this so it only happens when the player makes contact with the target
	//if (hit)
	//{
	//
	//	vx = 0;
	//	vy = 0;
	//}
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

void Player::update(float dt, EntityVec&, EntityVec&)
{
	torch_fuel = torch_fuel - dt;
	//std::cout << torch_fuel << "\n";
	if (attacking)
	{
		attack(dt);
	}
	else
	{
		if (!loaded)
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

	//Gonna fix this up so if pellet is stored
	if (!loaded)
	{
		for (int key = 0; key < items->getSize(); key++)
		{
			Entity* value = items->getEntity(key);
			if (contact(value->x, value->y))
			{
				loaded = true;
				items->remove(key);
			}
		}
	}
}

void Player::save()
{

	auto [gx, gy] = getGridPosition();
	std::string serialise = std::to_string(gx) + " " + std::to_string(gy) + "\n";
	file.setFile(serialise);
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
}