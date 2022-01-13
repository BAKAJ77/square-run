#include <states/splash_screen.h>
#include <core/input_system.h>
#include <glad/glad.h>

void SplashScreen::Init()
{
	this->shader = Memory::CreateShaderProgram("geometry.glsl.vsh", "geometry.glsl.fsh");

	float vertexData[]
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,

		-0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f,  0.5f
	};

	this->vbo = Memory::CreateVertexBuffer(vertexData, sizeof(vertexData), GL_STATIC_DRAW);

	this->vao = Memory::CreateVertexArray();
	this->vao->PushVertexLayout<float>(0, 2, 2 * sizeof(float));
	this->vao->AttachBuffers(this->vbo);
}

void SplashScreen::Destroy()
{

}

void SplashScreen::Update(const double& deltaTime)
{
	if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_ESCAPE))
		this->PopState();
}

void SplashScreen::Render() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	this->shader->BindProgram();
	this->vao->BindObject();

	if (InputSystem::GetInstance().GetCursorPosition().x <= 800)
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

SplashScreen* SplashScreen::GetGameState()
{
	static SplashScreen gameState;
	return &gameState;
}