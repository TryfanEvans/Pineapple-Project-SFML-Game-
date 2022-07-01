#include "Collision.h"

//TODO: Fix Memory Leak
//What memory leak u fucking liar
class Item : public Solid
{
public:
	Item(Map* map, int x, int y);
	sf::CircleShape sprite;
	//Right now the item is just a white circle, would be good to give it a few different textures for flavour
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
	//Right now the pellet is just a white circle, would be good to give it a few different textures for flavour. Matching the item picked up
	void render(sf::RenderTarget* target);
	//Replaces the pellet with an item at it's location
	void drop(std::vector<Item>& items);
};