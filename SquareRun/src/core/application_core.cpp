#include <core/application_core.h>
#include <util/timestamp.h>

ApplicationCore::ApplicationCore()
{
	this->window = Memory::CreateWindowFrame("Square Run", 1600, 900);
	this->MainLoop();
}

void ApplicationCore::MainLoop()
{
	constexpr double timeStep = 1.0;
	double accumulatedRenderTime = 0.0, elapsedRenderTime = 0.0;

	while (!this->window->WasRequestedExit())
	{
		// Update the game logic
		accumulatedRenderTime += elapsedRenderTime;
		while (accumulatedRenderTime >= timeStep)
		{
			this->Update(timeStep);
			accumulatedRenderTime -= timeStep;
		}

		// Render game scene
		const double preRenderTime = Util::GetSecondsSinceEpoch();

		this->Render();
		this->window->Refresh();

		const double postRenderTime = Util::GetSecondsSinceEpoch();
		elapsedRenderTime = postRenderTime - preRenderTime;
	}
}

void ApplicationCore::Update(const double& deltaTime)
{
}

void ApplicationCore::Render() const
{
}