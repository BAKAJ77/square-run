#ifndef TRANSITION_SYSTEM_H
#define TRANSITION_SYSTEM_H

#include <graphics/renderer.h>
#include <functional>

enum class TransitionTrigger
{
	EXIT_GAME_STATE_TRANSITION,
	START_GAME_STATE_TRANSITION
};

class GameState;

class TransitionSystem
{
private:
	OrthogonalCamera camera;
	TextureBufferPtr texture;
	glm::vec2 rectPositions[2], effectPositions[2];

	GameState* triggeredGameState;
	TransitionTrigger triggerType;

	bool customCurrentlyPlaying, currentlyPlaying, changeState, finishedLoadingState, wasRecentTransitionCustom;
	float speed;
public:
	TransitionSystem();
	~TransitionSystem() = default;

	// Sets the speed of transitions between game state switches.
	void SetSpeed(float speed);

	// Notifies the transition system that the game state has loaded, allowing for the transition to end.
	void NotifyGameStateLoaded();

	// Starts the transition.
	void Play();

	// Starts the custom transition given assigned to the given game state.
	void Play(GameState* gameState, TransitionTrigger gameStateTrigger);

	// Updates the transition state.
	void Update(const double& deltaTime);

	// Renders the transition.
	void Render() const;

	// Returns TRUE if the transition is currently playing, else FALSE is returned.
	bool IsPlaying() const;

	// Returns TRUE if the custom transition from game state is currently playing, else FALSE is returned.
	bool IsPlayingCustom() const;

	// Returns TRUE if the transition has reached a point where the game state should be changed, else FALSE is returned.
	bool ShouldChangeState() const;

	// Returns TRUE if the last transition was a custom game state transition, else returns FALSE.
	bool WasRecentTransitionCustom() const;

	// Returns the transition trigger type of the current transitioning game state.
	TransitionTrigger GetTriggerType() const;

	// Returns singleton instance object of this class.
	static TransitionSystem& GetInstance();
};

#endif