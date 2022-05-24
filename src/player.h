#include "ArcSlash.h"
#include "Enemies/Ranged.h"
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

	Player();
	void attack(float dt, Map& map);
	void update(float dt, Map& map, std::vector<Enemy*>& enemies, std::vector<Item>& items);
	void action(int relative_x, int relative_y, int button, std::vector<Enemy*>& enemies);
	void render(sf::RenderTarget* target);
	void checkpoint(Map& map);
};

#endif