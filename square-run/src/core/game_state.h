#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <graphics/renderer.h>
#include <vector>

enum class GameStateChange
{
	NONE = 0,
	SWITCH = 1,
	PUSH = 2,
	POP = 3
};

class GameState
{
	friend class GameStateSystem;
protected:
	OrthogonalCamera camera;
	bool updateWhilePaused, renderWhilePaused;
protected:
	GameState();
	virtual ~GameState() = default;

	// For initializing the game state e.g. loading textures.
	virtual void Init() = 0;

	// For destroying the game state e.g. cleaning up resources used.
	virtual void Destroy() = 0;

	// For acting upon when the game state is being resumed.
	virtual void Resume();

	// For acting upon when the game state is being paused.
	virtual void Pause();

	// For updating the game state.
	virtual void Update(const double& deltaTime) = 0;

	// For rendering objects in the game state.
	virtual void Render() const = 0;

	void SwitchState(GameState* gameState, float transitionSpeed = 1000.0f);
	void PushState(GameState* gameState);
	void PopState();
public:
	// For playing and updating a custom transition animation when pausing or popping this game state.
	// The transition is signalled as ended when this function returns FALSE.
	virtual bool OnExitTransitionUpdate(const double& deltaTime);

	// For playing and updating a custom transition animation when resuming this game state.
	// The transition is signalled as ended when this function returns FALSE.
	virtual bool OnStartTransitionUpdate(const double& deltaTime);
};

class GameStateSystem
{
private:
	std::vector<GameState*> stateStack;

	GameStateChange changeType;
	GameState* pendingGameState;
private:
	GameStateSystem();
public:
	GameStateSystem(const GameStateSystem& other) = delete;
	GameStateSystem(GameStateSystem&& temp) noexcept = delete;
	~GameStateSystem() = default;

	GameStateSystem& operator=(const GameStateSystem& other) = delete;
	GameStateSystem& operator=(GameStateSystem&& temp) noexcept = delete;

	// Destroys all game states in the stack and starts up the new game state given.
	void SwitchState(GameState* gameState, float transitionSpeed = 3000.0f);

	// Pauses the most recent game state (if any) and starts up the new game state given.
	void PushState(GameState* gameState);

	// Destroys the most recent game state in the stack.
	void PopState();

	// Updates the most recent game state and game states which are instructed to keep updated even when paused.
	void Update(const double& deltaTime);

	// Renders the most recent game state and game states which are instructed to keep rendering even when paused.
	void Render() const;

	// Returns TRUE if there are 
	bool IsActive() const;
	static GameStateSystem& GetInstance();
};

#endif
