struct StateData
{
	//Init
	std::string level_name = "default";

	//Win and loss states
	bool gameover = false;
	bool dead = false;

	//Menu states
	float music_volume = 0.0f;
	float sfx_volume = 0.5f; //Not currently implemented
	bool paused = false;
	//Probably something to do with keybindings, also not implemented
	std::string graphics_quality = "low"; //Will probably never be implemented
};