#include <core/transition_system.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TransitionSystem::TransitionSystem() :
	camera({ 0, 0 }, { RenderingGlobals::sceneViewWidth, RenderingGlobals::sceneViewHeight }), currentlyPlaying(false), 
	changeState(false), speed(10000.0f)
{
	// Load the transition textures
	this->texture = Memory::LoadTextureFromFile("transition_bkg.png");
}

void TransitionSystem::SetSpeed(float speed)
{
	this->speed = speed;
}
 
void TransitionSystem::Play()
{
	this->texturePosition = { 2520, -400 };
	this->effectPositions[0] = { -1000, 1725 };
	this->effectPositions[1] = { 2920, -645 };

	this->currentlyPlaying = true;
	this->changeState = false;
}

void TransitionSystem::Update(const double& deltaTime)
{
	if (this->currentlyPlaying)
	{
		const float sceneAspectRatio = this->camera.GetSize().x / this->camera.GetSize().y;

		// Update the transition background's position
		this->texturePosition.x -= this->speed * (float)deltaTime;
		this->texturePosition.y += (this->speed / sceneAspectRatio) * (float)deltaTime;

		// Update the transition effect positions
		this->effectPositions[0].x += (this->speed * 1.25f) * (float)deltaTime;
		this->effectPositions[0].y -= ((this->speed * 1.25f) / sceneAspectRatio) * (float)deltaTime;

		this->effectPositions[1].x -= (this->speed * 1.25f) * (float)deltaTime;
		this->effectPositions[1].y += ((this->speed * 1.25f) / sceneAspectRatio) * (float)deltaTime;

		// Signal that the game state should change once the transition background is fully covering the screen
		if (this->texturePosition.x <= 0.0f)
			this->changeState = true;

		// The transition is finished once the transition background is out of view
		if (this->texturePosition.x <= -3000.0f)
			this->currentlyPlaying = false;
	}
}

void TransitionSystem::Render() const
{
	if (this->currentlyPlaying)
	{
		// Render the transition background texture
		for (int i = 0; i < 3; i++)
		{
			Renderer::GetInstance().RenderTexturedRect(this->camera, this->texture, { this->texturePosition.x + (800 * i),
				this->texturePosition.y - (400 * i) }, { 2250, 1150 }, 45, { 255, 0, 0, 255 });
		}

		// Render the transition effects
		Renderer::GetInstance().RenderRect(this->camera, { 0, 0, 255, 255 }, this->effectPositions[0], { 600, 50 }, 150);
		Renderer::GetInstance().RenderRect(this->camera, { 255, 0, 255, 255 }, this->effectPositions[1], { 600, 50 }, 150);
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