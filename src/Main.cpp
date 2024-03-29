//#include "tests.cpp"
#include "./UI/Fonts.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

#include "States/Scripts.h"
//CREDIT TO https://github.com/rewrking/sfml-vscode-boilerplate by Andrew King, OggyP, and LucasDoesDev for the build environment and makefile

//Credit to Music Break "Charming Lute Mysterious, Relaxed" under Creative Commons License
class EntityFactory;
class EntityVec;

std::vector<std::string> Scripts::levels = { "Arena", "Ballerina", "Dungeon" };
std::string File::level_name = Scripts::levels[0];
//TODO: Fix memory leak

Map* Solid::map;
EntityVec* Solid::projectiles;
EntityVec* Solid::enemies;
EntityVec* Solid::items;
Solid* Solid::player;

bool Scripts::gameover = false;
bool Scripts::controls = false;
std::stack<std::string> Scripts::actions_pending;
std::stack<std::pair<std::string, float>> Scripts::tweaks_pending;

sf::Font Fonts::font;
RenderWindow* Scripts::window;
int main()
{
	Fonts yum = Fonts();
	std::cout << yum.bah;
	//test();
	try
	{
		RenderWindow* window = new RenderWindow(VideoMode(600, 600), "Pineapple Project!");
		Scripts::window = window;

		Scripts scripts;
		Menu pause_menu;

		pause_menu.addOption("Resume");
		pause_menu.addOption("Controls");
		pause_menu.addSlider("Volume");
		pause_menu.addOption("Quit");

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
						if (event.key.code == Keyboard::Escape)
						{
							if (scripts.show_screen)
							{
								scripts.screen->update();
							}
							else if (scripts.menu_enabled)
							{
								//Refactor this to use the events system
								scripts.paused = !scripts.paused;
							}
						}
						break;
					case Event::MouseButtonPressed:
						if (!scripts.paused)
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

			//Really need to overhaul this
			if (!scripts.paused && !scripts.dead && !scripts.gameover)
			{
				scripts.state->update(dt);
			}
			scripts.update();

			if (scripts.paused and !scripts.controls)
			{
				pause_menu.update();
			}

			window->clear();
			scripts.state->draw();
			if (scripts.paused and !scripts.show_screen)
			{
				pause_menu.render(window);
			}
			if (scripts.show_screen)
			{

				scripts.screen->render(window, scripts.state->camera.view);
			}

			window->display();
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error! " << e.what() << "\n";
	}
	return 0;
}