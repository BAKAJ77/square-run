#include <core/window_frame.h>
#include <util/logging_system.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Callback
{
	int* width = nullptr;
	int* height = nullptr;

	void WindowResizeCallback(GLFWwindow* frame, int width, int height)
	{
		*Callback::width = width;
		*Callback::height = height;
	}
}

WindowFrame::WindowFrame(const std::string_view& title, int width, int height, bool fullscreen, bool resizable, bool enableVsync) :
	width(width), height(height), fullscreen(fullscreen), resizable(resizable)
{
	// Initialize callback member pointers
	Callback::width = &this->width;
	Callback::height = &this->height;

	// Initialize the GLFW library
	if (glfwInit() < 0)
		LogSystem::GetInstance().OutputLog("Failed to initialize the GLFW library.", Severity::FATAL);

	// Configure the window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	// Create the GLFW window
	fullscreen ? 
		this->framePtr = glfwCreateWindow(width, height, title.data(), glfwGetPrimaryMonitor(), nullptr) :
		this->framePtr = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(this->framePtr, (videoMode->width / 2) - (width / 2), (videoMode->height / 2) - (height / 2));
	glfwSetWindowSizeCallback(this->framePtr, Callback::WindowResizeCallback);
	glfwMakeContextCurrent(this->framePtr);

	if (enableVsync)
		glfwSwapInterval(1);

	// Load the OpenGL function implementations via GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		LogSystem::GetInstance().OutputLog("Failed to load the OpenGL function implementations.", Severity::FATAL);

	LogSystem::GetInstance().OutputLog((const char*)glGetString(GL_VERSION), Severity::INFO);
}

WindowFrame::~WindowFrame()
{
	glfwTerminate();
}

void WindowFrame::SetSize(int width, int height)
{
	glfwSetWindowSize(this->framePtr, width, height);
}

void WindowFrame::RequestExit() const
{
	glfwSetWindowShouldClose(this->framePtr, true);
}

void WindowFrame::Refresh() const
{
	glfwPollEvents();
	glfwSwapBuffers(this->framePtr);
}

bool WindowFrame::WasRequestedExit() const
{
	return glfwWindowShouldClose(this->framePtr);
}

bool WindowFrame::IsFullscreen() const
{
	return this->fullscreen;
}

bool WindowFrame::IsResizable() const
{
	return this->resizable;
}

bool WindowFrame::IsVsyncEnabled() const
{
	return this->vsyncEnabled;
}

const int& WindowFrame::GetWidth() const
{
	return this->width;
}

const int& WindowFrame::GetHeight() const
{
	return this->height;
}

GLFWwindow* WindowFrame::GetFramePtr() const
{
	return this->framePtr;
}

WindowFramePtr Memory::CreateWindowFrame(const std::string_view& title, int width, int height, bool fullscreen, bool resizable, 
	bool enableVsync)
{
	return std::make_shared<WindowFrame>(title, width, height, fullscreen, resizable, enableVsync);
}
