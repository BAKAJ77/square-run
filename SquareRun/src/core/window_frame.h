#ifndef WINDOW_FRAME_H
#define WINDOW_FRAME_H

#include <string_view>
#include <memory>

struct GLFWwindow;

class WindowFrame
{
private:
	GLFWwindow* framePtr;
	int width, height;
	bool fullscreen, resizable, vsyncEnabled;
public:
	WindowFrame(const std::string_view& title, int width, int height, bool fullscreen, bool resizable, bool enableVsync);
	~WindowFrame();

	// Sets the size of the window.
	void SetSize(int width, int height);

	// Requests for the window to be closed.
	void RequestExit() const;

	// Polls for new pending events from the window, and swaps the rendering buffers.
	void Refresh() const;

	// Returns TRUE if the window was requested to be closed, else FALSE is returned.
	bool WasRequestedExit() const;

	// Returns TRUE if the window is in fullscreen mode, else FALSE is returned.
	bool IsFullscreen() const;

	// Returns TRUE if the window is resizable, else FALSE is returned.
	bool IsResizable() const;

	// Returns TRUE if the window is in vsync mode, else FALSE is returned.
	bool IsVsyncEnabled() const;

	// Returns the width of the window.
	const int& GetWidth() const;

	// Returns the height of the window.
	const int& GetHeight() const;

	// Returns a pointer to the created GLFW window object.
	GLFWwindow* GetFramePtr() const;
};

using WindowFramePtr = std::shared_ptr<WindowFrame>;

namespace Memory
{
	// Returns a shared pointer to the new created window frame.
	extern WindowFramePtr CreateWindowFrame(const std::string_view& title, int width, int height, bool fullscreen = false, 
		bool resizable = false, bool enableVsync = false);
}

#endif
