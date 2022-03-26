#include <core/transition_system.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TransitionSystem::TransitionSystem() :
	camera({ 0, 0 }, { RenderingGlobals::sceneViewWidth, RenderingGlobals::sceneViewHeight }), currentlyPlaying(false), 
	changeState(false), finishedLoadingState(false), speed(1000.0f)
{}

void TransitionSystem::SetSpeed(float speed)
{
	this->speed = speed;
}

void TransitionSystem::NotifyGameStateLoaded()
{
	this->finishedLoadingState = true;
}
 
void TransitionSystem::Play()
{
	this->rectPositions[0] = { this->camera.GetSize().x / 2.0f, -(this->camera.GetSize().y / 2.0f) };
	this->rectPositions[1] = { this->camera.GetSize().x / 2.0f, this->camera.GetSize().y * 1.5f };

	this->effectPositions[0] = { -300, (this->camera.GetSize().y / 2.0f) - 25.0f };
	this->effectPositions[1] = { this->camera.GetSize().x + 300, (this->camera.GetSize().y / 2.0f) + 25.0f };

	this->currentlyPlaying = true;
	this->finishedLoadingState = false;
	this->changeState = false;
}

void TransitionSystem::Update(const double& deltaTime)
{
	if (this->currentlyPlaying)
	{
		// Update the transition rectangles positions
		if (!this->changeState)
		{
			this->rectPositions[0].y += this->speed * (float)deltaTime;
			this->rectPositions[1].y -= this->speed * (float)deltaTime;
		}
		else if (this->finishedLoadingState)
		{
			// Update the transition effect positions
			this->effectPositions[0].x += (this->speed * 4.0f) * (float)deltaTime;
			this->effectPositions[1].x -= (this->speed * 4.0f) * (float)deltaTime;

			if (this->effectPositions[0].x > (this->camera.GetSize().x / 2.0f) + 1000.0f)
			{
				this->rectPositions[0].y -= this->speed * (float)deltaTime;
				this->rectPositions[1].y += this->speed * (float)deltaTime;
			}
		}

		// Signal that the game state should change once the transition background is fully covering the screen
		if (this->rectPositions[0].y >= this->camera.GetSize().y / 4.0f)
			this->changeState = true;

		// The transition is finished once the transition background is out of view
		if (this->rectPositions[0].y < -(this->camera.GetSize().y / 2.0f))
			this->currentlyPlaying = false;
	}
}

void TransitionSystem::Render() const
{
	if (this->currentlyPlaying)
	{
		// Render the transition rectangles
		Renderer::GetInstance().RenderRect(this->camera, { 0, 0, 0, 255 }, this->rectPositions[0], this->camera.GetSize());
		Renderer::GetInstance().RenderRect(this->camera, { 0, 0, 0, 255 }, this->rectPositions[1], this->camera.GetSize());

		// Render the transition effects
		Renderer::GetInstance().RenderRect(this->camera, { 0, 0, 255, 255 }, this->effectPositions[0], { 600, 25 });
		Renderer::GetInstance().RenderRect(this->camera, { 0, 255, 0, 255 }, this->effectPositions[1], { 600, 25 });
	}
}

bool TransitionSystem::IsPlaying() const
{
	return this->currentlyPlaying;
}

bool TransitionSystem::ShouldChangeState() const
{
	return this->changeState;
}

TransitionSystem& TransitionSystem::GetInstance()
{
	static TransitionSystem instance;
	return instance;
}