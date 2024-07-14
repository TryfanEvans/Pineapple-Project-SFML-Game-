#include <SFML/Graphics.hpp>

#ifndef FONTS_H
	#define FONTS_H
class Fonts
{
public:
	static sf::Font font;
	std::string bah = "FAAF";
	Fonts()
	{
		if (!font.loadFromFile("content/impact.ttf"))
		{
			std::cout << "ERROR: Could not load font\n";
		}
	}
};

#endif