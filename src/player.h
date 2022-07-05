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
	double torch_fuel = 60;
	const int range = 86;

	Player(Map* map);

	//Parses the position of the player from a text file
	void load(std::string level_name);
	//Passes position of the player to a text file
	void save(std::string level_name);
	//Checks whether the coordinates given fit within an arc extending from the player
	bool inHitbox(float tx, float ty);
	//Handles the animation and physics logic of the player attacking
	void attack(float dt);
	//Allows the player to move, pick up items and attack. Would possibly make more sense if items could tell they were being picked up? Then this would cause the pick up function for the player
	void update(float dt, std::vector<Enemy*>& enemies, std::vector<Item>& items);
	//Handles the logic of the player damaging enemies and initiates the attack logic
	void action(int mx, int my, int button, std::vector<Enemy*>& enemies, std::string level_name);
	//Renders the enemy, attack arc and pellet
	void render(sf::RenderTarget* target);
	//Not actually sure if I'll use this, make sure to remember to delete it later if it isn't useful
	void savePosition(std::string level_name);
};

#endif