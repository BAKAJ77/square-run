#include <core/game_core.h>
#include <core/input_system.h>
#include <core/collision_detection.h>
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

float x = 0, y = 0;
CollisionRect playerCollider({ x, y }, { 100, 100 });
CollisionRect otherCollider({ 800, 300 }, { 250, 250 });

void GameCore::Update(const double& deltaTime)
{
	if (InputSystem::GetInstance(this->window).WasKeyPressed(KeyCode::KEY_ESCAPE))
		this->window.RequestExit();

	if (InputSystem::GetInstance(this->window).WasKeyPressed(KeyCode::KEY_W))
		y -= 500 * deltaTime;
	else if (InputSystem::GetInstance(this->window).WasKeyPressed(KeyCode::KEY_S))
		y += 500 * deltaTime;
	if (InputSystem::GetInstance(this->window).WasKeyPressed(KeyCode::KEY_A))
		x -= 500 * deltaTime;
	else if (InputSystem::GetInstance(this->window).WasKeyPressed(KeyCode::KEY_D))
		x += 500 * deltaTime;

	playerCollider.SetPosition({ x, y });
	if (playerCollider.IsColliding(otherCollider) == CollidingSide::LEFT)
		std::printf("Left side colliding\n");
	else if (playerCollider.IsColliding(otherCollider) == CollidingSide::RIGHT)
		std::printf("Right side colliding\n");
	else if (playerCollider.IsColliding(otherCollider) == CollidingSide::TOP)
		std::printf("Top side colliding\n");
	else if (playerCollider.IsColliding(otherCollider) == CollidingSide::BOTTOM)
		std::printf("Bottom side colliding\n");

	this->window.Update();
}

void GameCore::Render() const
{
	Renderer::GetInstance().SetRenderTarget(RenderTarget::SCENE_FRAMEBUFFER);
	Renderer::GetInstance().Clear({ 0, 0, 75, 255 });

	Renderer::GetInstance().RenderRect(this->geometryShader, this->sceneCamera, { 255, 0, 0, 255 }, { 800, 300 }, { 250, 250 });

	Renderer::GetInstance().RenderRect(this->geometryShader, this->sceneCamera, { 0, 255, 0, 255 }, { x, y }, { 100, 100 });

	Renderer::GetInstance().FlushRenderedScene();
}