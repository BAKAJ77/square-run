#include <core/game_state.h>
#include <core/transition_system.h>
#include <interface/user_interface.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GameState::GameState() :
	camera({ 0, 0 }, { RenderingGlobals::sceneViewWidth, RenderingGlobals::sceneViewHeight }), renderWhilePaused(true), 
	updateWhilePaused(false)
{}

void GameState::Resume() {}

void GameState::Pause() {}

bool GameState::OnExitTransitionUpdate(const double& deltaTime) { return false; }

bool GameState::OnStartTransitionUpdate(const double& deltaTime) { return false; }

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
	pendingGameState(nullptr), changeType(GameStateChange::NONE)
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

		this->changeType = GameStateChange::SWITCH;
		this->pendingGameState = gameState;
	}
}

void GameStateSystem::PushState(GameState* gameState)
{
	if (!this->pendingGameState)
	{
		GameState* lastGameState = this->stateStack.back();
		TransitionSystem::GetInstance().Play(lastGameState, TransitionTrigger::EXIT_GAME_STATE_TRANSITION);

		// Pause the most recent game state
		if (!this->stateStack.empty())
			lastGameState->Pause();

		this->changeType = GameStateChange::PUSH;
		this->pendingGameState = gameState;
	}
}

void GameStateSystem::PopState()
{
	if (!this->pendingGameState)
	{
		if (!this->stateStack.empty())
		{
			GameState* gameState = this->stateStack.back();
			TransitionSystem::GetInstance().Play(gameState, TransitionTrigger::EXIT_GAME_STATE_TRANSITION);

			// Destroy the most recent game state
			if (!this->stateStack.empty())
			{
				this->stateStack.back()->Destroy();
				this->stateStack.pop_back();
			}

			this->changeType = GameStateChange::POP;
			this->pendingGameState = gameState;
		}
	}
}

void GameStateSystem::Update(const double& deltaTime)
{
	TransitionSystem::GetInstance().Update(deltaTime);

	if ((TransitionSystem::GetInstance().ShouldChangeState() || this->stateStack.empty()) && this->changeType == GameStateChange::SWITCH)
	{
		// Destroy all game states currently in the stack
		for (GameState* gameState : this->stateStack)
			gameState->Destroy();

		this->stateStack.clear();

		// Init the new game stack and push it into the stack
		this->pendingGameState->Init();
		this->stateStack.emplace_back(this->pendingGameState);
		this->pendingGameState = nullptr;
		
		this->changeType = GameStateChange::NONE;
		TransitionSystem::GetInstance().NotifyGameStateLoaded();
	}
	else if (!TransitionSystem::GetInstance().IsPlayingCustom() && TransitionSystem::GetInstance().WasRecentTransitionCustom() &&
		this->changeType > GameStateChange::SWITCH)
	{
		if (this->changeType == GameStateChange::PUSH)
		{
			// Init the new game stack and push it into the stack
			this->pendingGameState->Init();
			this->stateStack.emplace_back(this->pendingGameState);
			this->pendingGameState = nullptr;
		}
		else
		{
			// Resume the next game state in the stack
			if (!this->stateStack.empty())
			{
				TransitionSystem::GetInstance().Play(this->stateStack.back(), TransitionTrigger::START_GAME_STATE_TRANSITION);
				this->stateStack.back()->Resume();
			}

			this->pendingGameState = nullptr;
		}

		this->changeType = GameStateChange::NONE;
	}

	for (size_t stateIndex = 0; stateIndex < this->stateStack.size(); stateIndex++)
	{
		// Update the current active game state (and game states which are set to be updated while paused)
		GameState* gameState = this->stateStack[stateIndex];
		if (stateIndex == this->stateStack.size() - 1 || gameState->updateWhilePaused)
			gameState->Update(deltaTime);
	}

	if (!TransitionSystem::GetInstance().IsPlaying() && !TransitionSystem::GetInstance().IsPlayingCustom())
		UserInterfaceManager::GetInstance().UpdateActiveUI(deltaTime);
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

	UserInterfaceManager::GetInstance().RenderActiveUI();

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