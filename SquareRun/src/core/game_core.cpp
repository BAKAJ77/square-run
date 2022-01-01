#include <core/game_core.h>
#include <graphics/renderer.h>
#include <serialization/config.h>
#include <util/directory_system.h>
#include <util/timestamp.h>

#include <glm/gtc/matrix_transform.hpp>
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
	this->shader = ShaderProgram("geometry.glsl.vsh", "geometry.glsl.fsh");

	// Initialize the vbo, ibo and vao
	std::vector<float> vertexData =
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	std::vector<uint32_t> indexData =
	{
		0, 1, 2,
		0, 3, 2
	};

	this->vbo = VertexBuffer(vertexData, sizeof(vertexData), GL_STATIC_DRAW);
	this->ibo = IndexBuffer(indexData, sizeof(indexData), GL_STATIC_DRAW);

	this->vao = VertexArray();
	this->vao.PushVertexLayout<float>(0, 2, 2 * sizeof(float));
	this->vao.InitVertexArray(this->vbo, &this->ibo);

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
	this->window.Update();
}

void GameCore::Render() const
{
	glm::mat4 model;
	model = glm::translate(model, { 200, 200, 0 });
	model = glm::scale(model, { 100, 100, 1 });

	this->shader.BindProgram();
	this->shader.SetUniformGLM("MVPMatrix", this->sceneCamera.GetMatrix() * model);

	Renderer::GetInstance().Clear({ 0, 0, 75, 255 });
	Renderer::GetInstance().RenderData(this->shader, this->vao, 6);
}