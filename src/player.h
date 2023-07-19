#include "ArcSlash.h"
#include "Enemies/Enemy.h"
#include "Entity.h"
#include "File.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>

#ifndef Player_H
	#define Player_H
class Player : public Entity
{
public:
	const int speed = 90;
	sf::Sprite sprite;
	double torch_fuel = 60;
	const int range = 86;
	bool loaded = true;

	File file;

	Player();

	//Parses the position of the player from a text file
	void load();
	//Passes position of the player to a text file
	void save();
	//Checks whether the coordinates given fit within an arc extending from the player
	bool inHitbox(float tx, float ty);
	//Handles the animation and physics logic of the player attacking
	void attack(float dt);
	//Allows the player to move, pick up items and attack. Would possibly make more sense if items could tell they were being picked up? Then this would cause the pick up function for the player
	void update(float dt, EntityVec& enemies, EntityVec& items);
	//Handles the logic of the player damaging enemies and initiates the attack logic
	void action(int mx, int my, int button, EntityVec& enemies);
	//Renders the enemy, attack arc and pellet
	void render(sf::RenderTarget* target);

	std::string serialise()
	{
		return "";
	};
	void setState(std::string) {};
};

#endif