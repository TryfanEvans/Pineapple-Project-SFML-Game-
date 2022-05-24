class Screen
{
public:
	sf::Texture texture;
	sf::RectangleShape sprite;
	Screen(std::string image_name);
	void render(sf::RenderTarget* win, sf::View view);
};

class MenuOption
{
public:
	sf::RectangleShape sprite;
	std::string label;
	float width = 100;
	float height = 40;
	MenuOption(std::string label);
	MenuOption();
	virtual void render(sf::RenderWindow* win, float x, float y, float width, float height);
	bool getSelected(float x, float y);
	void update();
	virtual void setSliderPosition(float) {};
};

class MenuSlider : public MenuOption
{
public:
	MenuSlider(std::string label, float* bound_var);
	float* bound_var;
	float slider_position;
	void render(sf::RenderWindow* win, float x, float y, float width, float height);
	void setSliderPosition(float mouse_x);
};

class Menu
{
public:
	sf::RectangleShape sprite;
	std::vector<MenuOption*> options;
	sf::View view;
	Screen control_screen;
	bool paused = false;
	bool controls = false;
	float* music_volume;

	float relative_mouse_x = 0;
	float relative_mouse_y = 0;

	float option_width = 150;
	float option_height = 30;
	Menu(float* volume);
	void render(sf::RenderWindow* win);
	void update(sf::Window& win);
	void checkPaused();

	//Button Functions
	void resume();
};
