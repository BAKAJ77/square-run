#include <serialization/config.h>

namespace Serialization
{
	void GenerateConfigFile()
	{
		// Make sure that the game data directory exists
		std::string directory = Util::GetGameRequisitesDirectory() + "data/";
		if (!Util::IsExistingDirectory(directory))
			LogSystem::GetInstance().OutputLog("Could not find the game's data directory", Severity::FATAL);

		// Create and setup the json object
		nlohmann::json jsonObject = 
		{
			{ "window", 
				{ 
					{ "width", 1600 },
					{ "height", 900 },
					{ "fullscreen", false },
					{ "resizable", false },
					{ "vsync", false }
				}
			},
			{ "graphics",
				{
					{ "resolution", { 1600, 900 } },
					{ "numSamplesMSAA", 1 },
					{ "gamma", 2.2f }
				}
			}
		};

		// Write the json data to the new config file
		std::ofstream configFile(directory + "config.json", std::ios::trunc);
		configFile << std::setw(4) << jsonObject;
	}
}