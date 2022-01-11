#ifndef GAME_STATE_SYSTEM_H
#define GAME_STATE_SYSTEM_H

#include <vector>

class GameState;

class GameStateSystem
{
	friend class GameState;
private:
	std::vector<GameState*> gameStateStack;
private:
	GameStateSystem();
public:
	GameStateSystem(const GameStateSystem& other) = delete;
	GameStateSystem(GameStateSystem&& temp) noexcept = delete;

	~GameStateSystem();

	GameStateSystem& operator=(const GameStateSystem& other) = delete;
	GameStateSystem& operator=(GameStateSystem&& temp) noexcept = delete;

	// Halts and destroys the most recent game state and starts the game state given.
	void SwitchState(GameState* gameState);

	// Pauses the most recent game state and starts the game state given.
	void PushState(GameState* gameState);

	// Halts and destroys the most recent game state.
	void PopState();

	// Updates the most recent game state (and other game states in the stack which are declared to continue updating 
	// even when paused).
	void Update(const double& deltaTime);

	// Renders the most recent game state (and other game states in the stack which are declared to continue rendering 
	// even when paused).
	void Render() const;

	// Returns TRUE if there are any game states in the game state stack, else FALSE is returned.
	bool IsActive() const;

	// Returns singleton instance object of this class.
	static GameStateSystem& GetInstance();
};

#endif