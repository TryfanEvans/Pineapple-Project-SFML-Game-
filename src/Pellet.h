#include "Collision.h"

//TODO: Fix Memory Leak
class Item : public Solid
{
public:
	Item(Map* map, int x, int y);
	sf::CircleShape sprite;
	void render(sf::RenderTarget* target);
};

//The Player and the ranged enemy will shoot these out
class Pellet : public Solid
{
public:
	Pellet(Map* map);
	int contactRadius = 2;
	bool stored = true;
	bool active = false;
	sf::CircleShape sprite;
	void render(sf::RenderTarget* target);
	void drop(std::vector<Item>& items);
};