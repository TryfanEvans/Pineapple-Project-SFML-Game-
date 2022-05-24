#include "Collision.h"

//TODO: Fix Memory Leak
class Item : public Solid
{
public:
	sf::CircleShape sprite;
	void render(sf::RenderTarget* target);
};

//The Player and the ranged enemy will shoot these out
class Pellet : public Solid
{
public:
	int contactRadius = 2;
	bool stored = true;
	bool active = false;
	sf::CircleShape sprite;
	void render(sf::RenderTarget* target);
	void drop(std::vector<Item>& items);
};