#include <core/input_system.h>
#include <core/window.h>

#include <GLFW/glfw3.h>

InputSystem::InputSystem() :
	window(nullptr)
{}

InputSystem::~InputSystem() = default;

void InputSystem::Init(const WindowFrame& window)
{
	this->window = &window;
}

bool InputSystem::WasKeyPressed(KeyCode key) const
{
	return glfwGetKey(this->window->GetFramePtr(), (int)key);
}

bool InputSystem::WasMouseButtonPressed(MouseCode button) const
{
	return glfwGetMouseButton(this->window->GetFramePtr(), (int)button);
}

glm::vec2 InputSystem::GetMouseCursorPosition() const
{
	double cursorPosX = 0, cursorPosY = 0;
	glfwGetCursorPos(this->window->GetFramePtr(), &cursorPosX, &cursorPosY);

	return { cursorPosX, cursorPosY };
}

InputSystem& InputSystem::GetInstance()
{
	static InputSystem instance;
	return instance;
}
