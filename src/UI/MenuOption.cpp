#include "MenuOption.h"

static const float padding = 10;

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
	text.setFont(Fonts::font);
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

MenuSlider::MenuSlider(std::string label)
{
	this->label = label;
}

void MenuSlider::setSliderPosition(float mouse_x)
{
	auto position = sprite.getPosition();

	float relative_mouse_x = mouse_x - (position.x + padding);

	slider_position = std::min(relative_mouse_x, (width + (3 * padding)));
	slider_position = std::max(0.f, slider_position);
	float bound_var = slider_position / (width + padding);
	std::cout << bound_var;
	auto tweak = std::make_pair(label, bound_var);
	Scripts::tweaks_pending.push(tweak);
}

void MenuSlider::update(float relative_mouse_x, float relative_mouse_y)
{
	if (getSelected(relative_mouse_x, relative_mouse_y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
	//This doesn't seem to do anything
	//slider_position = *bound_var * (width - (4 * padding)),
	dial.setPosition(sf::Vector2f(x + padding + slider_position, y + 0.7 * height - padding / 2));
	dial.setSize(sf::Vector2f(2, padding));
	win->draw(dial);

	sf::Text text;
	text.setFont(Fonts::font);
	text.setString(label);
	text.setCharacterSize(height * 0.4);
	text.setFillColor(sf::Color::White);
	text.setPosition(x + 10, y + 2);
	win->draw(text);
}