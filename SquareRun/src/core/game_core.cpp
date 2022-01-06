#include <core/game_core.h>
#include <core/input_system.h>
#include <graphics/renderer.h>
#include <serialization/config.h>
#include <util/directory_system.h>
#include <util/timestamp.h>

#include <glad/glad.h>

GameCore::GameCore()
{
	// Check if the game config file exists, if not then create new default config file
	if (!Util::IsExistingFile(Util::GetGameRequisitesDirectory() + "data/configs.json"))
		Serialization::GenerateConfigFile();

	// Load required game window config settings, then initialize the window
	const int width = Serialization::GetConfigElement<int>("window", "width");
	const int height = Serialization::GetConfigElement<int>("window", "height");
	const bool fullscreen = Serialization::GetConfigElement<bool>("window", "fullscreen");
	const bool resizable = Serialization::GetConfigElement<bool>("window", "resizable");
	const bool vsync = Serialization::GetConfigElement<bool>("window", "vsync");

	this->window = WindowFrame(width, height, fullscreen, resizable, vsync);

	// Load and initialize the shaders
	this->geometryShader = ShaderProgram("geometry.glsl.vsh", "geometry.glsl.fsh");
	this->textShader = ShaderProgram("text.glsl.vsh", "text.glsl.fsh");

	// Load the test texture
	this->texture = Graphics::LoadTextureFromFile("test_texture.png");

	// Load the cascadia font
	this->cascadiaFont = Font("cascadia_code_bold.ttf", 100);

	// Initialize the camera
	this->sceneCamera = Camera({ 0, 0 }, { 1600, 900 });
	
	// Execute the main game loop
	this->MainLoop();
}

GameCore::~GameCore() = default;

void GameCore::MainLoop()
{
	double previousTime = 0.0;
	while (!this->window.IsRequestedExit())
	{
		const double currentTime = Util::GetSecondsSinceEpoch();
		const double elapsedTime = currentTime - previousTime;
		previousTime = currentTime;

		this->Update(elapsedTime);
		this->Render();
	}
}

void GameCore::Update(const double& deltaTime)
{
	if (InputSystem::GetInstance(this->window).WasKeyPressed(KeyCode::KEY_ESCAPE))
		this->window.RequestExit();

	this->window.Update();
}

void GameCore::Render() const
{
	Renderer::GetInstance().Clear({ 0, 0, 75, 255 });
	Renderer::GetInstance().RenderRect(this->geometryShader, this->sceneCamera, { 0, 255, 0, 255 }, { 200, 200 }, { 100, 100 });
	Renderer::GetInstance().RenderTexturedRect(this->geometryShader, this->sceneCamera, this->texture, { 500, 500 }, { 250, 250 }, 70);
	Renderer::GetInstance().RenderTriangle(this->geometryShader, this->sceneCamera, { 255, 0, 0, 255 }, { 900, 300 }, { 175, 175 });
	Renderer::GetInstance().RenderTexturedTriangle(this->geometryShader, this->sceneCamera, this->texture, { 1300, 700 }, 
		{ 300, 300 }, 30);

	Renderer::GetInstance().RenderText(this->textShader, this->sceneCamera, this->cascadiaFont, 100, "Testing...",
		{ 0, 255, 0, 255 }, { 400, 400 });
}