#ifndef UI_H
#define UI_H

class UI
{
public:
	sf::View view;

	UI() :
		view(sf::FloatRect(0.f, 0.f, 600.f, 600.f)) {

		};
	virtual void render(sf::RenderWindow* win) = 0;
	virtual void update() = 0;
};

#endif