#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <core/game_state_system.h>
#include <core/input_system.h>
#include <graphics/camera_view.h>
#include <graphics/renderer.h>

class GameState
{
	friend class GameStateSystem;
protected:
	Camera sceneCamera;
	bool renderWhilePaused, updateWhilePaused;
protected:
	GameState();
	GameState(const GameState& other) = delete;
	GameState(GameState&& temp) noexcept = delete;

	virtual ~GameState();
	
	GameState& operator=(const GameState& other) = delete;
	GameState& operator=(GameState&& temp) noexcept = delete;

	// Virtual method for initializing the game state.
	virtual void Init() = 0;

	// Virtual method for cleaning up resources used by the game state.
	virtual void Destroy() = 0;

	// Virtual method for pausing execution of the game state.
	virtual void Pause();

	// Virtual method for resuming execution of the game state.
	virtual void Resume();

	// Virtual method for updating the game state.
	virtual void Update(const double& deltaTime) = 0;

	// Virtual method for rendering objects in the game state.
	virtual void Render() const = 0;

	// Halts then destroys this game state and starts the game state given.
	void SwitchState(GameState* gameState);

	// Pauses this game state and starts the game state given.
	void PushState(GameState* gameState);

	// Halts then destroys this game state.
	void PopState();
public:
	// Returns the scene camera used by the game state.
	const Camera& GetSceneCamera() const;
};

#endif
