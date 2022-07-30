#include "Entity.h"

//TODO: Fix Memory Leak
//What memory leak u fucking liar
#ifndef PELLET_H
	#define PELLET_H
class Item : public Entity
{
public:
	Item(int x, int y);
	sf::CircleShape sprite;
	//Right now the item is just a white circle, would be good to give it a few different textures for flavour
	void render(sf::RenderTarget* target);
	std::string serialise();
};

//The Player and the ranged enemy will shoot these out
class Pellet : public Solid
{
public:
	Pellet();
	int contactRadius = 2;
	bool stored = true;
	bool active = false;
	sf::CircleShape sprite;
	//Right now the pellet is just a white circle, would be good to give it a few different textures for flavour. Matching the item picked up
	void render(sf::RenderTarget* target);
	//Replaces the pellet with an item at it's location
	void drop(EntityVec& items);
	//Removes the pellet from the inventory and adds it to the world
	void toss(int x, int y);
	//Keeps the pellet in motion if it is active, otherwise nothing
	void update(float dt, int tx, int ty);

	void pickUp();
};

#endif