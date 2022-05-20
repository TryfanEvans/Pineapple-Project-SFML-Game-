#include "player.h"
#include <SFML/Graphics.hpp>

class State
{

public:
	State() {};
	virtual void load() {};
	virtual void update(float) {};
	virtual void draw(sf::RenderWindow*) {};
	virtual void click(int, int, int, sf::RenderWindow*) {};
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
	std::vector<Melee*> enemies;

	float view_x;
	float view_y;

public:
	GameState();
	void load();
	void update(float dt);
	void draw(sf::RenderWindow* win);
	void click(int x, int y, int button, sf::RenderWindow* win);
};

class EditorState : public State
{
	Map map;
	Player player;
	sf::View view;

	float view_x;
	float view_y;

public:
	EditorState();
	void load();
	void update(float dt);
	void draw(sf::RenderWindow* win);
	void click(int x, int y, int button, sf::RenderWindow* win);
};