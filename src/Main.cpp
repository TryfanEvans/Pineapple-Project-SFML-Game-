#include "./UI/Fonts.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

#include "States/Scripts.h"
//CREDIT TO https://github.com/rewrking/sfml-vscode-boilerplate by Andrew King, OggyP, and LucasDoesDev for the build environment and makefile

//Credit to Music Break "Charming Lute Mysterious, Relaxed" under Creative Commons License
class EntityFactory;
class EntityVec;

//TODO: Fix memory leak

Map* Solid::map;
EntityVec* Solid::projectiles;
EntityVec* Solid::enemies;
EntityVec* Solid::items;
Solid* Solid::player;

std::string File::level_name;

bool Scripts::gameover = false;
std::stack<std::string> Scripts::actions_pending;
std::stack<std::pair<std::string, float>> Scripts::tweaks_pending;

sf::Font Fonts::font;
RenderWindow* Scripts::window;
RenderWindow* State::win;

int main()
{

	Fonts yum = Fonts();
	std::cout << yum.bah;
	//test();
	try
	{
		RenderWindow* window = new RenderWindow(VideoMode(600, 400), "Pineapple Project!");
		State::win = window;
		Scripts::window = window;

		Scripts scripts;

		sf::Clock deltaClock;
		float dt = 0;
		//Ok this is really weird getting rid of this breaks the physics system? wtf
		window->setFramerateLimit(60);

		sf::Music music;
		if (!music.openFromFile("content/backgroundmusic.ogg"))
		{
			std::cout << "Failed to  music \n";
		} // error
		music.setLoop(true);

		music.play();

		while (window->isOpen())
		{
			Event event;
			while (window->pollEvent(event))
			{
				switch (event.type)
				{
					// window closed
					case sf::Event::Closed:
						window->close();
						break;

						// key pressed
						//case sf::Event::KeyPressed:
						//	... break;
					case Event::KeyPressed:
						scripts.state->keyPress(event.key.code);
						break;
					case Event::MouseButtonPressed:
						if (scripts.state->UI_elements.empty())
						{
							scripts.state->click(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
						}
						break;

					// we don't process other types of events
					default:
						break;
				}
			}

			dt = deltaClock.restart().asSeconds();
			//std::cout << "dt " << dt << "\n";

			music.setVolume(scripts.music_volume * scripts.music_volume * 8);

			scripts.update();
			scripts.state->update(dt);


			window->clear();

			scripts.state->draw();

			window->display();
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error! " << e.what() << "\n";
	}
	return 0;
}