#include <states/splash_screen.h>

void SplashScreen::Init()
{

}

void SplashScreen::Destroy()
{

}

void SplashScreen::Update(const double& deltaTime)
{

}

void SplashScreen::Render() const
{

}

SplashScreen* SplashScreen::GetGameState()
{
	static SplashScreen gameState;
	return &gameState;
}