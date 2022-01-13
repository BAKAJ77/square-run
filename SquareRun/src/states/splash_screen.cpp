#include <states/splash_screen.h>
#include <core/input_system.h>

void SplashScreen::Init()
{

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

}

SplashScreen* SplashScreen::GetGameState()
{
	static SplashScreen gameState;
	return &gameState;
}