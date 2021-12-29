#include <core/window.h>
#include <util/logging_system.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

WindowFrame::WindowFrame() :
	frame(nullptr), width(0), height(0), fullscreen(false), resizable(false), vsyncEnabled(false)
{}

WindowFrame::WindowFrame(int width, int height, bool fullscreen, bool resizable, bool vsync) :
	width(width), height(height), fullscreen(fullscreen), resizable(resizable), vsyncEnabled(vsync)
{
	// Initialize callback member pointers
	Callback::width = &this->width;
	Callback::height = &this->height;

	// Initialize GLFW
	if (glfwInit() < 0)
		LogSystem::GetInstance().OutputLog("Failed to initialize GLFW", Severity::FATAL);

	// Configure window hints (we are using OpenGL 3.3 core)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	// Create and setup the window
	fullscreen ? this->frame = glfwCreateWindow(width, height, "Square Run", glfwGetPrimaryMonitor(), nullptr) :
		this->frame = glfwCreateWindow(width, height, "Square Run", nullptr, nullptr);

	const GLFWvidmode* monitorVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(this->frame, (monitorVideoMode->width / 2) - (width / 2), (monitorVideoMode->height / 2) - (height / 2));

	glfwSetWindowSizeCallback(this->frame, Callback::WindowResizeCallback);
	glfwMakeContextCurrent(this->frame);

	if (vsync)
		glfwSwapInterval(1);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		LogSystem::GetInstance().OutputLog("Failed to initialize GLAD", Severity::FATAL);

	LogSystem::GetInstance().OutputLog((const char*)glGetString(GL_VERSION), Severity::INFO);
}

WindowFrame::WindowFrame(WindowFrame&& temp) noexcept :
	frame(temp.frame), width(temp.width), height(temp.height), fullscreen(temp.fullscreen), resizable(temp.resizable),
	vsyncEnabled(temp.vsyncEnabled)
{
	temp.frame = nullptr;
}

WindowFrame::~WindowFrame()
{
	if (this->frame)
		glfwTerminate();
}

WindowFrame& WindowFrame::operator=(WindowFrame&& temp) noexcept
{
	this->frame = temp.frame;
	this->width = temp.width;
	this->height = temp.height;
	this->fullscreen = temp.fullscreen;
	this->resizable = temp.resizable;
	this->vsyncEnabled = temp.vsyncEnabled;
	
	temp.frame = nullptr;
	return *this;
}

void WindowFrame::SetWidth(int width)
{
	glfwSetWindowSize(this->frame, width, this->height);
	this->width = width;
}

void WindowFrame::SetHeight(int height)
{
	glfwSetWindowSize(this->frame, this->width, height);
	this->height = height;
}

void WindowFrame::RequestExit()
{
	glfwSetWindowShouldClose(this->frame, true);
}

void WindowFrame::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(this->frame);
}

const int& WindowFrame::GetWidth() const
{
	return this->width;
}

const int& WindowFrame::GetHeight() const
{
	return this->height;
}

bool WindowFrame::IsRequestedExit() const
{
	return glfwWindowShouldClose(this->frame);
}

bool WindowFrame::IsFullscreen() const
{
	return this->fullscreen;
}

bool WindowFrame::IsResizable() const
{
	return this->resizable;
}

GLFWwindow* WindowFrame::GetFramePtr() const
{
	return this->frame;
}