#include <core/application_core.h>
#include <core/game_state.h>
#include <core/input_system.h>
#include <graphics/renderer.h>
#include <serialization/config.h>
#include <util/directory_system.h>
#include <util/timestamp.h>
#include <states/intro_screen.h>

ApplicationCore::ApplicationCore()
{
	// Generate a new default config file if it doesn't exist
	if (!Util::IsExistingFile(Util::GetGameRequisitesDirectory() + "data/config.json"))
		Serialization::GenerateConfigFile();

	// Retrieve the window config settings
	const int width = Serialization::GetConfigElement<int>("window", "width");
	const int height = Serialization::GetConfigElement<int>("window", "height");
	const bool enableVsync = Serialization::GetConfigElement<int>("window", "vsync");
	const bool resizable = Serialization::GetConfigElement<int>("window", "resizable");
	const bool fullscreen = Serialization::GetConfigElement<int>("window", "fullscreen");

	// Create the game window
	this->window = Memory::CreateWindowFrame("Square Run", width, height, fullscreen, resizable, enableVsync);
	
	// Initialize the input system
	InputSystem::GetInstance().Init(this->window);
	Renderer::GetInstance().Init(this->window);

	// Continue onto the game's main loop with the splash screen game state being the first game state ran
	GameStateSystem::GetInstance().SwitchState(IntroScreen::GetGameState());
	this->MainLoop();
}

void ApplicationCore::MainLoop()
{
	constexpr double timeStep = 0.001;
	double accumulatedRenderTime = 0.0, elapsedRenderTime = 0.0;

	while (!this->window->WasRequestedExit() && GameStateSystem::GetInstance().IsActive())
	{
		// Update the game logic
		accumulatedRenderTime += elapsedRenderTime;
		while (accumulatedRenderTime >= timeStep)
		{
			this->Update(timeStep);
			accumulatedRenderTime -= timeStep;
		}

		// Render game scene
		const double preRenderTime = Util::GetSecondsSinceEpoch();

		this->Render();
		this->window->Refresh();

		const double postRenderTime = Util::GetSecondsSinceEpoch();
		elapsedRenderTime = postRenderTime - preRenderTime;
	}
}

void ApplicationCore::Update(const double& deltaTime)
{
	GameStateSystem::GetInstance().Update(deltaTime);
}

void ApplicationCore::Render() const
{
	GameStateSystem::GetInstance().Render();
}