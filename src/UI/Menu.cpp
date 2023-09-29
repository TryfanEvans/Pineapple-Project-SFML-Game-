#include "Menu.h"

static const float padding = 10;

//TODO: FIX MEMORY LEAK
Menu::Menu(Scripts& scripts) :
	scripts(scripts)
{

	addOption("Resume");
	addOption("Controls");
	addSlider("Volume");
	addOption("Mute");
	addOption("Quit");
}

void Menu::render(sf::RenderWindow* win)
{

	auto [window_width, window_height] = win->getSize();

	float width = option_width + padding * 2;
	float height = options.size() * (option_height + padding) + padding;

	float margins_horizontal = (window_width - width) / 2;
	float margins_vertical = (window_height - height) / 3;

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

void Menu::update(sf::Window&)
{
	if (!scripts.controls)
	{
		for (uint i = 0; i < options.size(); i++)
		{
			options[i]->update(relative_mouse_x, relative_mouse_y);
		}
	}
}

void Menu::toggle()
{
	static bool tap = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		if (tap)
		{
			if (scripts.controls)
			{
				scripts.controls = false;
			}
			else
			{
				scripts.paused = !scripts.paused;
			}
			tap = false;
		}
	}
	else
	{
		tap = true;
	}
}

void Menu::addOption(std::string label)
{
	options.push_back(new MenuOption(label));
}

void Menu::addSlider(std::string label)
{
	options.push_back(new MenuSlider(label));
}