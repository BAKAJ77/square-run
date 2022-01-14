#include <states/splash_screen.h>
#include <core/input_system.h>
#include <glad/glad.h>

void SplashScreen::Init()
{
	this->shader = Memory::CreateShaderProgram("geometry.glsl.vsh", "geometry.glsl.fsh");
	this->postProcessShader = Memory::CreateShaderProgram("post_process.glsl.vsh", "post_process.glsl.fsh");

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

	this->fboTexture = Memory::CreateTextureBuffer(GL_TEXTURE_2D_MULTISAMPLE, 2, GL_RGB, 1600, 900);
	this->fbo = Memory::CreateFrameBuffer();
	this->fbo->AttachTextureBuffer(GL_COLOR_ATTACHMENT0, this->fboTexture);

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
	this->fbo->BindBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->shader->BindProgram();
	this->shader->SetUniform("geometryTexture", 0);
	this->vao->BindObject();
	this->texture->BindBuffer(0);

	if (InputSystem::GetInstance().GetCursorPosition().x <= 800)
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	this->fbo->UnbindBuffer();
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->postProcessShader->BindProgram();
	this->postProcessShader->SetUniform("processedTexture", 0);
	this->postProcessShader->SetUniform("numSamples", 2);
	this->postProcessShader->SetUniformGLM("framebufferSize", { 1600, 900 });
	this->fboTexture->BindBuffer(0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

SplashScreen* SplashScreen::GetGameState()
{
	static SplashScreen gameState;
	return &gameState;
}