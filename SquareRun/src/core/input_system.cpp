#include <core/input_system.h>
#include <GLFW/glfw3.h>

InputSystem::InputSystem(const WindowFrame& window) :
	window(window)
{}

InputSystem::~InputSystem() = default;

bool InputSystem::WasKeyPressed(KeyCode key) const
{
	return glfwGetKey(this->window.GetFramePtr(), (int)key);
}

bool InputSystem::WasMouseButtonPressed(MouseCode button) const
{
	return glfwGetMouseButton(this->window.GetFramePtr(), (int)button);
}

glm::vec2 InputSystem::GetMouseCursorPosition() const
{
	double cursorPosX = 0, cursorPosY = 0;
	glfwGetCursorPos(this->window.GetFramePtr(), &cursorPosX, &cursorPosY);

	return { cursorPosX, cursorPosY };
}

InputSystem& InputSystem::GetInstance(const WindowFrame& window)
{
	static InputSystem instance(window);
	return instance;
}
