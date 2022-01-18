#include <states/tester_state.h>
#include <states/splash_screen.h>
#include <core/input_system.h>

void TesterState::Init()
{
}

void TesterState::Destroy()
{
}

void TesterState::Update(const double& deltaTime)
{
	if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_SPACE))
		this->SwitchState(SplashScreen::GetGameState());
}

void TesterState::Render() const
{
	Renderer::GetInstance().Clear({ 0, 255, 0, 255 });
}

TesterState* TesterState::GetGameState()
{
	static TesterState gameState;
	return &gameState;
}
