#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

#include "State.h"

//CREDIT TO https://github.com/rewrking/sfml-vscode-boilerplate by Andrew King, OggyP, and LucasDoesDev for the build environment and makefile
//Final version will be my own build, I just needed the extra debugging functionality of this build

int main()
{
	RenderWindow window(VideoMode(300, 300), "SFML works!");
	State* state = new GameState();
	sf::Clock deltaClock;
	float dt = 0;
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// window closed
				case sf::Event::Closed:
					window.close();
					break;

					// key pressed
					//case sf::Event::KeyPressed:
					//	... break;
				case Event::MouseButtonPressed:
					int button;
					switch (event.mouseButton.button)
					{
						case Mouse::Left:
							button = 1;
							break;
						case Mouse::Right:
							button = 2;
							break;
						default:
							break;
					}
					state->click(event.mouseButton.x, event.mouseButton.y, button, &window);
					break;

				// we don't process other types of events
				default:
					break;
			}
		}

		dt = deltaClock.restart().asSeconds();
		//std::cout << "dt " << dt << "\n";
		state->update(dt);

		window.clear();
		state->draw(&window);
		window.display();
	}

	return 0;
}