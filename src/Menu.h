class MenuOption
{
public:
	sf::RectangleShape sprite;
	std::string label;
	float width = 100;
	float height = 40;
	MenuOption(std::string label);
	void render(sf::RenderWindow* win, float x, float y, float width, float height);
	bool getSelected(float x, float y);
	void update();
};

class Menu
{
public:
	sf::RectangleShape sprite;
	std::vector<MenuOption*> options;
	bool paused = false;

	float relative_mouse_x = 0;
	float relative_mouse_y = 0;

	float option_width = 150;
	float option_height = 30;
	float padding = 10;
	Menu();
	void render(sf::RenderWindow* win);
	void update(sf::Window& win);
	void checkPaused();

	//Button Functions
	void resume();
};
