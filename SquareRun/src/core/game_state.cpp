#include <core/game_state.h>

GameState::GameState() :
	renderWhilePaused(true), updateWhilePaused(true), sceneCamera({ 0, 0 }, { 1920, 1080 })
{}

GameState::~GameState() = default;

void GameState::Pause() {}

void GameState::Resume() {}

void GameState::SwitchState(GameState* gameState)
{
	GameStateSystem::GetInstance().SwitchState(gameState);
}

void GameState::PushState(GameState* gameState)
{
	GameStateSystem::GetInstance().PushState(gameState);
}

void GameState::PopState()
{
	GameStateSystem::GetInstance().PopState();
}

const Camera& GameState::GetSceneCamera() const
{
	return this->sceneCamera;
}