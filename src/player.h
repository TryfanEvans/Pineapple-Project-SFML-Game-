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
	void attack(float dt);
	void update(float dt, std::vector<Enemy*>& enemies, std::vector<Item>& items, bool& gameover);
	void action(int relative_x, int relative_y, int button, std::vector<Enemy*>& enemies);
	void render(sf::RenderTarget* target);
	void checkpoint();
	void win(bool& gameover);
};

#endif