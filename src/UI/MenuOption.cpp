#include "MenuOption.h"

static const float padding = 10;

Checkbox::Checkbox()
{
	//This might break when I make two
	cross.loadFromFile("content/cross.png");
	sprite.setTexture(cross);
}

void Checkbox::update(float x, float y)
{
	//How u gonna click on two at once? Dummy
	static bool flag = false;

	hover = false;
	//This will execute when the cursor starts outside the zone but otherwise it's good
	if (getSelected(x, y))
	{
		hover = true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			flag = true;
		}
		else if (flag)
		{
			flag = false;
			toggle();
		}
	}
	else
		flag = false;
}

void Checkbox::render(sf::RenderWindow* win, float x, float y, float width, float height)
{
	box.setFillColor(sf::Color(100, 100, 100));

	box.setPosition(sf::Vector2f(x, y));
	box.setSize(sf::Vector2f(width, height));

	win->draw(box);

	box.setOutlineThickness(0);
	if (hover)
	{
		box.setOutlineThickness(-2);
	}
	if (checked)
	{
		//Add an X to be shown when checked
		sprite.setPosition(sf::Vector2f(x, y));
		sprite.setScale(sf::Vector2f(0.5, 0.5));
		win->draw(sprite);
	}
}

void Checkbox::toggle()
{
	checked = !checked;
}

bool Checkbox::getSelected(float x, float y)
{
	auto position = box.getPosition();
	auto size = box.getSize();
	return (x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y);
}

MenuOption::MenuOption(std::string label) :
	label(label)
{
}

void MenuOption::render(sf::RenderWindow* win, float x, float y, float width, float height)
{
	backpanel.render(win, x, y, width, height);
	sf::Text text;
	text.setFont(Fonts::font);
	text.setString(label);
	text.setCharacterSize(height * 0.8);
	text.setFillColor(sf::Color::White);
	text.setPosition(x + 10, y);
	win->draw(text);
}

void MenuOptionBackpanel::render(sf::RenderWindow* win, float x, float y, float width, float height)
{
	//Feels like this stuff could be in the constructor
	sprite.setFillColor(sf::Color(200, 200, 200));

	sprite.setPosition(sf::Vector2f(x, y));
	sprite.setSize(sf::Vector2f(width, height));
	win->draw(sprite);

	sprite.setOutlineThickness(0);
	if (hover)
	{
		sprite.setOutlineThickness(-2);
	}
}

MenuOptionBackpanel::MenuOptionBackpanel()
{
}

void MenuOptionBackpanel::update(float mouse_x, float mouse_y)
{
	hover = false;
	if (getSelected(mouse_x, mouse_y))
	{
		hover = true;
	}
}
bool MenuOptionBackpanel::getSelected(float x, float y)
{
	auto position = sprite.getPosition();
	auto size = sprite.getSize();
	return (x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y);
}

void MenuOption::update(float relative_mouse_x, float relative_mouse_y)
{
	backpanel.update(relative_mouse_x, relative_mouse_y);
	if (backpanel.getSelected(relative_mouse_x, relative_mouse_y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
	auto position = backpanel.sprite.getPosition();

	float relative_mouse_x = mouse_x - (position.x + padding);

	slider_position = std::min(relative_mouse_x, (width + padding + position.x));
	slider_position = std::max(0.f, slider_position);
	float bound_var = slider_position / (width + padding);
	std::cout << bound_var;
	auto tweak = std::make_pair(label, bound_var);
	Scripts::tweaks_pending.push(tweak);
}

void MenuSlider::update(float relative_mouse_x, float relative_mouse_y)
{
	checkbox.update(relative_mouse_x, relative_mouse_y);
	backpanel.update(relative_mouse_x, relative_mouse_y);
	if (!checkbox.getSelected(relative_mouse_x, relative_mouse_y) && backpanel.getSelected(relative_mouse_x, relative_mouse_y) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		setSliderPosition(relative_mouse_x);
		checkbox.checked = false;
	}
	if (checkbox.checked)
		setSliderPosition(0);
}

void MenuSlider::render(sf::RenderWindow* win, float x, float y, float width, float height)
{
	backpanel.render(win, x, y, width, height);

	int checkbox_x = x + width - padding - height / 6;
	int checkbox_y = y + 0.7 * height - padding / 2;
	int checkbox_size = height * 2 / 5;
	int bar_width = width - (2 * padding) - checkbox_size;
	width = bar_width;
	int bar_height = 2;
	sf::RectangleShape bar;
	bar.setPosition(sf::Vector2f(x + padding, y + 0.7 * height));
	bar.setSize(sf::Vector2f(bar_width, bar_height));
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

	checkbox.render(win, checkbox_x, checkbox_y, checkbox_size, checkbox_size);
}