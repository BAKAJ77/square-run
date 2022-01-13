#include "game_state.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameState::Resume() {}

void GameState::Pause() {}

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameStateSystem::SwitchState(GameState* gameState)
{
	// Destroy all game states currently in the stack
	for (GameState* gameState : this->stateStack)
		gameState->Destroy();

	this->stateStack.clear();

	// Init the new game stack and push it into the stack
	gameState->Init();
	this->stateStack.emplace_back(gameState);
}

void GameStateSystem::PushState(GameState* gameState)
{
	// Pause the most recent game state
	if (!this->stateStack.empty())
		this->stateStack.back()->Pause();

	// Init the new game stack and push it into the stack
	gameState->Init();
	this->stateStack.emplace_back(gameState);
}

void GameStateSystem::PopState()
{
	// Destroy the most recent game state
	if (!this->stateStack.empty())
	{
		this->stateStack.back()->Destroy();
		this->stateStack.pop_back();
	}

	// Resume the next game state in the stack
	if (!this->stateStack.empty())
		this->stateStack.back()->Resume();
}

void GameStateSystem::Update(const double& deltaTime)
{
	for (size_t stateIndex = 0; stateIndex < this->stateStack.size(); stateIndex++)
	{
		GameState* gameState = this->stateStack[stateIndex];
		if ((stateIndex == 0) || (gameState->updateWhilePaused))
			gameState->Update(deltaTime);
	}
}

void GameStateSystem::Render() const
{
	for (size_t stateIndex = 0; stateIndex < this->stateStack.size(); stateIndex++)
	{
		const GameState* gameState = this->stateStack[stateIndex];
		if ((stateIndex == 0) || (gameState->renderWhilePaused))
			gameState->Render();
	}
}

bool GameStateSystem::IsActive() const
{
	return !this->stateStack.empty();
}

GameStateSystem& GameStateSystem::GetInstance()
{
	static GameStateSystem instance;
	return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////