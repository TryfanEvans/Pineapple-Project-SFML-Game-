#include "ArcSlash.h"
#include "Enemies/Enemy.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>

#ifndef Player_H
	#define Player_H
class Player : public Solid
{
public:
	const int speed = 90;
	sf::Sprite sprite;
	Pellet pellet;

	Player(Map* map);
	void load(std::string level_name);
	void save(std::string level_name);
	void attack(float dt);
	void update(float dt, std::vector<Enemy*>& enemies, std::vector<Item>& items);
	void action(int relative_x, int relative_y, int button, std::vector<Enemy*>& enemies);
	void render(sf::RenderTarget* target);
	//Not actually sure if I'll use this, make sure to remember to delete it later if it isn't useful
	void savePosition(std::string level_name);
};

#endif