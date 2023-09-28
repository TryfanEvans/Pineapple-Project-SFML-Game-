#include "Menu.h"

static const float padding = 10;

static sf::Font font;
//TODO: FIX MEMORY LEAK
Menu::Menu(Scripts& scripts) :
	scripts(scripts)
{
	if (!font.loadFromFile("content/impact.ttf"))
	{
		std::cout << "ERROR: Could not load font\n";
	}

	options.push_back(new MenuOption("Resume"));
	options.push_back(new MenuOption("Controls"));
	options.push_back(new MenuSlider("Volume", &scripts.music_volume));
	options.push_back(new MenuOption("Mute"));
	options.push_back(new MenuOption("Quit"));
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

void MenuOption::update(float relative_mouse_x, float relative_mouse_y)
{
	if (getSelected(relative_mouse_x, relative_mouse_y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Scripts::actions_pending.push(label);
	}
}

MenuOption::MenuOption()
{
}

MenuSlider::MenuSlider(std::string label, float* bound_var)
{
	this->bound_var = bound_var;
	this->label = label;
}

void MenuSlider::setSliderPosition(float mouse_x)
{
	auto position = sprite.getPosition();

	float relative_mouse_x = mouse_x - (position.x + padding);

	slider_position = std::min(relative_mouse_x, (width + (3 * padding)));
	slider_position = std::max(0.f, slider_position);
	*bound_var = slider_position / (width + padding);
}

void MenuSlider::update(float relative_mouse_x, float)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		setSliderPosition(relative_mouse_x);
}

void MenuSlider::render(sf::RenderWindow* win, float x, float y, float width, float height)
{
	sprite.setFillColor(sf::Color(200, 200, 200));

	sprite.setPosition(sf::Vector2f(x, y));
	sprite.setSize(sf::Vector2f(width, height));
	win->draw(sprite);

	sf::RectangleShape bar;
	bar.setPosition(sf::Vector2f(x + padding, y + 0.7 * height));
	bar.setSize(sf::Vector2f(width - (2 * padding), 2));
	win->draw(bar);

	sf::RectangleShape dial;
	slider_position = *bound_var * (width - (4 * padding)),
	dial.setPosition(sf::Vector2f(x + padding + slider_position, y + 0.7 * height - padding / 2));
	dial.setSize(sf::Vector2f(2, padding));
	win->draw(dial);

	sf::Text text;
	text.setFont(font);
	text.setString(label);
	text.setCharacterSize(height * 0.4);
	text.setFillColor(sf::Color::White);
	text.setPosition(x + 10, y + 2);
	win->draw(text);
}