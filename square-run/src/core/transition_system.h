#ifndef TRANSITION_SYSTEM_H
#define TRANSITION_SYSTEM_H

#include <graphics/renderer.h>

class TransitionSystem
{
private:
	OrthogonalCamera camera;
	TextureBufferPtr texture;
	glm::vec2 rectPositions[2], effectPositions[2];

	bool currentlyPlaying, changeState, finishedLoadingState;
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

	// Updates the transition state.
	void Update(const double& deltaTime);

	// Renders the transition.
	void Render() const;

	// Returns TRUE if the transition is currently playing, else FALSE is returned.
	bool IsPlaying() const;

	// Returns TRUE if the transition has reached a point where the game state should be changed, else FALSE is returned.
	bool ShouldChangeState() const;

	// Returns singleton instance object of this class.
	static TransitionSystem& GetInstance();
};

#endif