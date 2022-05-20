#include "ArcSlash.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>

class Player : public Solid
{
public:
	const int speed = 90;
	sf::Sprite sprite;

	Player();
	void attack(float dt);
	void update(float dt, bool clipping, Map& map);
	void action(int relative_x, int relative_y, int button, std::vector<Melee*> enemies);
	void render(sf::RenderTarget* target);
};