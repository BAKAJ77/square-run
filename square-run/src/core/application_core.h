#ifndef APPLICATION_CORE_H
#define APPLICATION_CORE_H

#include <core/window_frame.h>

class ApplicationCore
{
private:
	WindowFramePtr window;
private:
	// The main loop where the game is updated and rendered per loop.
	void MainLoop();

	// Updates the current game logic.
	void Update(const double& deltaTime);

	// Renders the game scene.
	void Render() const;
public:
	ApplicationCore();
	~ApplicationCore() = default;
};

#endif
