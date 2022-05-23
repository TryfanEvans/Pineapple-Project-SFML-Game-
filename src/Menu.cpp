#include "Menu.h"

static sf::Font font;

//TODO: FIX MEMORY LEAK
Menu::Menu()
{
	if (!font.loadFromFile("content/Roboto-Regular.ttf"))
	{
		std::cout << "ERROR: Could not load font\n";
	}

	options.push_back(new MenuOption("Resume"));
	options.push_back(new MenuOption("Controls"));
	options.push_back(new MenuOption("Volume"));
	options.push_back(new MenuOption("Main Menu"));
	options.push_back(new MenuOption("Quit"));
}

void Menu::render(sf::RenderWindow* win)
{

	auto [window_width, window_height] = win->getSize();

	float width = option_width + padding * 2;
	float height = options.size() * (option_height + padding) + padding;

	float margins_horizontal = (window_width - width) / 2;
	float margins_vertical = (window_height - height) / 3;

	sf::View view;
	view.reset(sf::FloatRect(0, 0, width, height));
	view.setViewport(sf::FloatRect(margins_horizontal / window_width, margins_vertical / window_height, width / window_width, height / window_height));
	win->setView(view);

	sprite.setPosition(0, 0);
	sprite.setFillColor(sf::Color(100, 100, 100));

	sprite.setOutlineColor(sf::Color(250, 250, 250));
	sprite.setOutlineThickness(-2);

	sprite.setSize(sf::Vector2f(width, height));
	win->draw(sprite);

	sf::Vector2i mouse_position = sf::Mouse::getPosition(*win);
	relative_mouse_x = mouse_position.x - margins_horizontal;
	relative_mouse_y = mouse_position.y - margins_vertical;
	for (uint i = 0; i < options.size(); i++)
	{
		options[i]->sprite.setOutlineThickness(0);
		if (options[i]->getSelected(relative_mouse_x, relative_mouse_y))
		{
			options[i]->sprite.setOutlineThickness(-2);
		}
		options[i]->render(win, padding, i * (option_height + padding) + padding, option_width, option_height);
	}
}

void Menu::update(sf::Window& win)
{
	std::string action_pending = "";

	for (uint i = 0; i < options.size(); i++)
	{
		if (options[i]->getSelected(relative_mouse_x, relative_mouse_y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			action_pending = options[i]->label;
		}
	}

	if (action_pending == "Resume")
	{
		paused = false;
	}
	else if (action_pending == "Quit")
	{
		win.close();
	}
}

void Menu::checkPaused()
{
	static bool tap = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		if (tap)
		{
			tap = false;
			paused = !paused;
		}
	}
	else
	{
		tap = true;
	}
}

MenuOption::MenuOption(std::string label)
{
	this->label = label;
}

void MenuOption::render(sf::RenderWindow* win, float x, float y, float width, float height)
{
	sprite.setFillColor(sf::Color(200, 200, 200));

	sprite.setPosition(sf::Vector2f(x, y));
	sprite.setSize(sf::Vector2f(width, height));
	win->draw(sprite);

	sf::Text text;
	text.setFont(font);
	text.setString(label);
	text.setCharacterSize(height * 0.8);
	text.setFillColor(sf::Color::White);
	text.setPosition(x + 10, y);
	win->draw(text);
}

bool MenuOption::getSelected(float x, float y)
{
	auto position = sprite.getPosition();
	auto size = sprite.getSize();
	return (x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y);
}

void MenuOption::update()
{
}