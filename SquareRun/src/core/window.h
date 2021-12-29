#ifndef WINDOW_H
#define WINDOW_H

#include <chrono>

struct GLFWwindow;

class WindowFrame
{
private:
	GLFWwindow* frame;
	int width, height;
	bool fullscreen, resizable, vsyncEnabled;
public:
	WindowFrame();
	WindowFrame(int width, int height, bool fullscreen, bool resizable, bool vsync);
	WindowFrame(const WindowFrame& other) = delete;
	WindowFrame(WindowFrame&& temp) noexcept;

	~WindowFrame();

	WindowFrame& operator=(const WindowFrame& other) = delete;
	WindowFrame& operator=(WindowFrame&& temp) noexcept;

	// Sets the width of the window.
	void SetWidth(int width);

	// Sets the height of the window.
	void SetHeight(int height);

	// Requests for the window to be terminated.
	void RequestExit();

	// Updates the window state i.e updates event polling state and swapping rendering buffers
	void Update();

	// Returns the current width of the window.
	const int& GetWidth() const;

	// Returns the current height of the window.
	const int& GetHeight() const;

	// Returns TRUE if the window has been requested to be terminated, else FALSE is returned.
	bool IsRequestedExit() const;

	// Returns TRUE if window is in fullscreen mode, else FALSE is returned.
	bool IsFullscreen() const;

	// Returns TRUE if the window is resizable, else FALSE is returned.
	bool IsResizable() const;

	// Returns the contained pointer to the GLFW window object created.
	GLFWwindow* GetFramePtr() const;
};

#endif
