#include <graphics/camera_view.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() = default;

Camera::Camera(const glm::vec2& pos, const glm::vec2& size) :
	position(pos), size(size)
{
	this->projectionMatrix = glm::ortho(0.0f, size.x, size.y, 0.0f);
}

Camera::~Camera() = default;

void Camera::SetPosition(const glm::vec2 & pos)
{
	this->position = pos;
}

void Camera::SetSize(const glm::vec2& size)
{
	this->size = size;
}

glm::mat4 Camera::GetMatrix() const
{
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(this->position, 0.0f), glm::vec3(this->position, -1.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f));

	return this->projectionMatrix * viewMatrix;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(glm::vec3(this->position, 0.0f), glm::vec3(this->position, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const glm::vec2& Camera::GetPosition() const
{
	return this->position;
}

const glm::vec2& Camera::GetSize() const
{
	return this->size;
}
