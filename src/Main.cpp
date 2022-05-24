#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

#include "State.h"

//CREDIT TO https://github.com/rewrking/sfml-vscode-boilerplate by Andrew King, OggyP, and LucasDoesDev for the build environment and makefile
//Final version will be my own build, I just needed the extra debugging functionality of this build

//Credit to Music Break "Charming Lute Mysterious, Relaxed" under Creative Commons License

int main()
{
	RenderWindow window(VideoMode(300, 300), "SFML works!");
	float music_volume = 0.0f;

	State* state = new GameState(music_volume);
	sf::Clock deltaClock;
	float dt = 0;
	window.setFramerateLimit(60);

	sf::Music music;
	if (!music.openFromFile("content/backgroundmusic.ogg"))
	{
		std::cout << "Failed to load music \n";
	} // error
	music.setLoop(true);

	music.play();

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
		music.setVolume(music_volume);

		state->update(dt, window);

		window.clear();
		state->draw(&window);
		window.display();
	}

	return 0;
}