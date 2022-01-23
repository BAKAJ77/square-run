#include <core/game_state.h>
#include <core/transition_system.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GameState::GameState() :
	camera({ 0, 0 }, { RenderingGlobals::sceneViewWidth, RenderingGlobals::sceneViewHeight }), renderWhilePaused(true), 
	updateWhilePaused(true)
{}

void GameState::Resume() {}

void GameState::Pause() {}

void GameState::SwitchState(GameState* gameState, float transitionSpeed)
{
	GameStateSystem::GetInstance().SwitchState(gameState, transitionSpeed);
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

GameStateSystem::GameStateSystem() :
	pendingGameState(nullptr)
{}

void GameStateSystem::SwitchState(GameState* gameState, float transitionSpeed)
{
	if (!this->pendingGameState)
	{
		if (!this->stateStack.empty())
		{
			TransitionSystem::GetInstance().SetSpeed(transitionSpeed);
			TransitionSystem::GetInstance().Play();
		}

		this->pendingGameState = gameState;
	}
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
	TransitionSystem::GetInstance().Update(deltaTime);

	if ((TransitionSystem::GetInstance().ShouldChangeState() || this->stateStack.empty()) && this->pendingGameState)
	{
		// Destroy all game states currently in the stack
		for (GameState* gameState : this->stateStack)
			gameState->Destroy();

		this->stateStack.clear();

		// Init the new game stack and push it into the stack
		this->pendingGameState->Init();
		this->stateStack.emplace_back(this->pendingGameState);
		this->pendingGameState = nullptr;
	}

	for (size_t stateIndex = 0; stateIndex < this->stateStack.size(); stateIndex++)
	{
		GameState* gameState = this->stateStack[stateIndex];
		if ((stateIndex == 0) || (gameState->updateWhilePaused))
			gameState->Update(deltaTime);
	}
}

void GameStateSystem::Render() const
{
	Renderer::GetInstance().SetRenderTarget(RenderTarget::SCENE_FRAMEBUFFER);
	Renderer::GetInstance().Clear();

	for (size_t stateIndex = 0; stateIndex < this->stateStack.size(); stateIndex++)
	{
		const GameState* gameState = this->stateStack[stateIndex];
		if ((stateIndex == 0) || (gameState->renderWhilePaused))
			gameState->Render();
	}

	TransitionSystem::GetInstance().Render();
	Renderer::GetInstance().FlushRenderedScene();
}

bool GameStateSystem::IsActive() const
{
	return !this->stateStack.empty() || this->pendingGameState;
}

GameStateSystem& GameStateSystem::GetInstance()
{
	static GameStateSystem instance;
	return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////