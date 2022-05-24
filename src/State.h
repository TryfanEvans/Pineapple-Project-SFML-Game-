#include "Menu.h"
#include "player.h"
#include <SFML/Graphics.hpp>

#ifndef State_H
	#define State_H
class State
{

public:
	State(float& music_volume);
	State() {};
	virtual void update(float, sf::Window& win) = 0;
	virtual void draw(sf::RenderWindow*) = 0;
	virtual void click(int, int, int, sf::RenderWindow*) = 0;
	std::tuple<float, float> ViewPosition(float px, float py, int window_width, int window_height, int grid_width, int grid_height, int tileSize)
	{
		float view_x = std::max(px, window_width / 2.f - 32);
		view_x = std::min(view_x, (grid_width + 1) * tileSize - (window_width / 2.f));
		float view_y = std::max(py, window_height / 2.f - 32);
		view_y = std::min(view_y, (grid_height + 1) * tileSize - (window_height / 2.f));

		return { view_x, view_y };
	};
};

class GameState : public State
{

	Map map;
	Player player;
	sf::View view;
	std::vector<Enemy*> enemies;
	std::vector<Item> items;
	Menu menu;

public:
	GameState(float& music_volume);
	void update(float dt, sf::Window& win);
	void draw(sf::RenderWindow* win);
	void click(int x, int y, int button, sf::RenderWindow* win);
};

class EditorState : public State
{

	Map map;
	Player player;
	sf::View view;
	Menu menu;

	std::vector<Enemy*> enemies;
	std::vector<Item> items;

	float view_x;
	float view_y;

public:
	EditorState(float&);
	void update(float dt, sf::Window&);
	void draw(sf::RenderWindow* win);
	void click(int x, int y, int button, sf::RenderWindow* win);
};

#endif