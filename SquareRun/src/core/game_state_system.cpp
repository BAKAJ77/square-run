#include <core/game_state.h>
#include <core/game_state_system.h>
#include <graphics/renderer.h>

GameStateSystem::GameStateSystem()
{}

GameStateSystem::~GameStateSystem() = default;

void GameStateSystem::SwitchState(GameState* gameState)
{
	// Destroy and pop all game states in the game state stack
	for (GameState* gameState : this->gameStateStack)
		gameState->Destroy();

	this->gameStateStack.clear();

	// Initialize the game state then add it into the game state stack
	gameState->Init();
	this->gameStateStack.emplace_back(gameState);
}

void GameStateSystem::PushState(GameState* gameState)
{
	// Pause the most recent game state
	if (!this->gameStateStack.empty())
		this->gameStateStack.back()->Pause();

	// Initialize the game state then add it into the game state stack
	gameState->Init();
	this->gameStateStack.emplace_back(gameState);
}

void GameStateSystem::PopState()
{
	// Destroy then pop the most recent game state
	if (!this->gameStateStack.empty())
	{
		this->gameStateStack.back()->Destroy();
		this->gameStateStack.pop_back();
	}

	// Resume the game state at the top of the game state stack
	if (!this->gameStateStack.empty())
		this->gameStateStack.back()->Resume();
}

void GameStateSystem::Update(const double& deltaTime)
{
	for (size_t stackIndex = 0; stackIndex < this->gameStateStack.size(); stackIndex++)
	{
		GameState* gameState = this->gameStateStack[stackIndex];
		if ((stackIndex == 0) || (gameState->updateWhilePaused))
			gameState->Update(deltaTime);
	}
}

void GameStateSystem::Render() const
{
	Renderer::GetInstance().SetRenderTarget(RenderTarget::SCENE_FRAMEBUFFER);
	Renderer::GetInstance().Clear({ 0, 0, 255, 255 });

	for (size_t stackIndex = 0; stackIndex < this->gameStateStack.size(); stackIndex++)
	{
		const GameState* gameState = this->gameStateStack[stackIndex];
		if ((stackIndex == 0) || (gameState->renderWhilePaused))
			gameState->Render();
	}

	Renderer::GetInstance().FlushRenderedScene();
}

bool GameStateSystem::IsActive() const
{
	return !this->gameStateStack.empty();
}

GameStateSystem& GameStateSystem::GetInstance()
{
	static GameStateSystem instance;
	return instance;
}

