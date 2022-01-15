#include <states/splash_screen.h>
#include <core/input_system.h>

void SplashScreen::Init()
{
	this->texture = Memory::LoadTextureFromFile("transition_bkg.png");
	this->font = Memory::LoadFontFromFile("cascadia_code_bold.ttf", 100);
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
	if (InputSystem::GetInstance().GetCursorPosition().x <= 800)
	{
		Renderer::GetInstance().RenderTriangle(this->camera, { 255, 255, 0, 255 }, { 400, 400 }, { 100, 100 });
		Renderer::GetInstance().RenderText(this->camera, this->font, 100, "You can see me???", { 0, 255, 255, 255 }, { 300, 700 });
	}
}

SplashScreen* SplashScreen::GetGameState()
{
	static SplashScreen gameState;
	return &gameState;
}