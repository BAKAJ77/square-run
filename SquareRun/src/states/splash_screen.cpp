#include <states/splash_screen.h>
#include <core/input_system.h>
#include <states/tester_state.h>

void SplashScreen::Init()
{
	
}

void SplashScreen::Destroy()
{

}

void SplashScreen::Update(const double& deltaTime)
{
	if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_SPACE))
		this->SwitchState(TesterState::GetGameState());
}

void SplashScreen::Render() const
{
	Renderer::GetInstance().Clear({ 255, 255, 0, 255 });
}

SplashScreen* SplashScreen::GetGameState()
{
	static SplashScreen gameState;
	return &gameState;
}