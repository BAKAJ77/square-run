#include <states/splash_screen.h>
#include <core/input_system.h>
#include <glad/glad.h>

void SplashScreen::Init()
{
	this->shader = Memory::CreateShaderProgram("geometry.glsl.vsh", "geometry.glsl.fsh");
	this->texture = Memory::LoadTextureFromFile("transition_bkg.png");

	float vertexData[]
	{
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,

		-0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 1.0f, 1.0f
	};

	this->vbo = Memory::CreateVertexBuffer(vertexData, sizeof(vertexData), GL_STATIC_DRAW);

	this->vao = Memory::CreateVertexArray();
	this->vao->PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	this->vao->PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	this->vao->AttachBuffers(this->vbo);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	this->shader->SetUniform("geometryTexture", 0);
	this->vao->BindObject();
	this->texture->BindBuffer(0);

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