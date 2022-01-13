#ifndef APPLICATION_CORE_H
#define APPLICATION_CORE_H

#include <core/window_frame.h>

class ApplicationCore
{
private:
	WindowFramePtr window;
private:
	void MainLoop();
	void Update(const double& deltaTime);
	void Render() const;
public:
	ApplicationCore();
	~ApplicationCore() = default;
};

#endif
