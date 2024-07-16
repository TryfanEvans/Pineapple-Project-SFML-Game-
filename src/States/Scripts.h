//Had this idea that level could be a namespace/class all of it's own. Would contain level_name, load and save functions
//Potentially everything that is loaded from the files, player, enemies, items and the map
//The gamestate could access the level name just fine, just go Level::name

#ifndef Scripts_H
#define Scripts_H
#include "../SaveManager.h"
#include "../UI/Menu.h"
#include "Collision.h"
#include "EntityVec.h"
#include "File.h"
#include "Player.h"
#include "States/State.h"
#include <bits/stdc++.h>

//Scripts to manage events happening within the level and giving the appropriate response.
//I intend to turn this into a class for recieving messages, and another, or possibly several to manage the response
//I'm thinking each pending loop could be it's own class
//A state manager would be a great way to handle the pause menu
//Then a settings class
//Then a scripts calss strictly for story events
//Then others in each of their own classes
class Scripts
{
public:
	std::vector<State*> states;

	State* state;
	//RED AMD WHITE
	static sf::RenderWindow* window;

	Menu pause_menu;

	//Init

	static bool controls; // Why the fuck isn't this in Menu?

	//Win and loss states
	static bool gameover;
	bool dead = false;

	//UI elements send input to here, possibly refactor so everything comes here
	static std::stack<std::string> actions_pending;
	static std::stack<std::pair<std::string, float>> tweaks_pending;

	//Screens
	Screen* screen;
	Screen death_screen;
	Screen win_screen;
	Screen control_screen;
	Screen default_screen;

	//Settings
	float music_volume = 0.0f;
	float sfx_volume = 0.5f; //Not currently implemented
	bool paused = false;
	//Probably something to do with keybindings, also not implemented
	std::string graphics_quality = "low"; //Will probably never be implemented

	std::stack<UI*> UI_elements;

	Scripts() :
		death_screen("death_screen"),
		win_screen("win_screen"),
		control_screen("controls_screen"),
		default_screen("default_screen")
	{
		states.push_back(new TitleState(window));
		states.push_back(new GameState(window));
		states.push_back(new EditorState(window));
		states.push_back(new EditorMenuState(window));
		states.push_back(new SaveMenuState(window));
		screen = &default_screen;
		state = states[0];

		pause_menu.addOption("Resume");
		pause_menu.addOption("Controls");
		pause_menu.addSlider("Volume");
		pause_menu.addOption("Quit");
	}

	//TODO: boolean return for success/failure

	//This needs to be specific to each level
	bool levelCleared()
	{
		std::cout << "woah";
		return (sf::Keyboard::isKeyPressed(sf::Keyboard::G));
	}

	void death()
	{
		dead = true;
		screen = &death_screen;
	}

	void win()
	{
		gameover = true;
		screen = &win_screen;
		UI_elements.push(&win_screen);
	}

	void update()
	{
		if (levelCleared())
		{
			SaveManager::completeLevel();
			std::cout << SaveManager::level_index;
			if (SaveManager::isWin())
			{
				actions_pending.push("Win");
			}
			else
			{
				states[1]->loadLevel(SaveManager::levels[SaveManager::level_index]);
			}
		}

		if (dead == true)
		{
			death();
		}

		while (!tweaks_pending.empty())
		{
			auto tweak_pending = tweaks_pending.top();
			tweaks_pending.pop();
			if (tweak_pending.first == "Volume")
			{
				music_volume = tweak_pending.second;
				std::cout << music_volume;
			}
		}

		while (!actions_pending.empty())
		{
			std::string action_pending = actions_pending.top();
			actions_pending.pop();
			if (action_pending == "Resume")
			{
				paused = false;
			}
			else if (action_pending == "Controls")
			{
				UI_elements.push(&control_screen);
			}
			else if (action_pending == "Mute")
			{
				music_volume = 0.f;
			}
			else if (action_pending == "Exit Screen")
			{
				//This is disabled for the time being
			}
			else if (action_pending == "Quit")
			{
				window->close();
			}
			else if (action_pending == "death_screen")
			{
				Scripts::actions_pending.push("Exit Screen");

				std::cout << "Respawn!\n";
				//state = new GameState(scripts, window);
				gameover = false;
				dead = false;
				//state->player.x = 50;
				//state->player.y = 50;
				SaveManager::newGame();
			}
			else if (action_pending == "win_screen")
			{
				Scripts::actions_pending.push("Exit Screen");

				std::cout << "Respawn!\n";
				//state = new GameState(scripts, window);
				gameover = false;
				dead = false;
				//state->player.x = 50;
				//state->player.y = 50;
				SaveManager::newGame();
			}
			else if (action_pending == "controls_screen")
			{
				Scripts::actions_pending.push("Exit Screen");
			}
			else if (action_pending == "Win")
			{
				win();
			}
			else if (action_pending == "Pause")
			{
				//Maybe I should be adding the pause menu in here?
			}
			else if (action_pending == "Continue")
			{
				//Make state gamestate
				//Later I will sort out the thing so new game and continue are different
				state = states[1];
				SaveManager::continueGame();
			}
			else if (action_pending == "New Game")
			{
				state = states[1];
				SaveManager::newGame();
			}
			else if (action_pending == "Load Game")
			{
				state = states[3];
			}
			else if (action_pending == "Level Editor")
			{
				state = states[4];
			}
			else if (action_pending == "Pause Menu")
			{
				UI_elements.push(&pause_menu);
			}
		}
	}
};

#endif