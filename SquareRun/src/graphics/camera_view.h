#ifndef CAMERA_VIEW_H
#define CAMERA_VIEW_H

#include <glm/glm.hpp>

class Camera
{
private:
	glm::vec2 position, size;
	glm::mat4 projectionMatrix;
public:
	Camera();
	Camera(const glm::vec2& pos, const glm::vec2& size);

	~Camera();

	// Sets the position of the camera.
	void SetPosition(const glm::vec2& pos);

	// Sets the size of the camera.
	void SetSize(const glm::vec2& size);

	// Returns the product of the camera's projection and view matrix.
	glm::mat4 GetMatrix() const;

	// Returns the camera's view matrix.
	glm::mat4 GetViewMatrix() const;

	// Returns the camera's projection matrix.
	const glm::mat4& GetProjectionMatrix() const;

	// Returns the position of the camera.
	const glm::vec2& GetPosition() const;

	// Returns the size of the camera.
	const glm::vec2& GetSize() const;
};

#endif
