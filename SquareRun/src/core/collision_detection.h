#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include <glm/glm.hpp>

enum class CollidingSide
{
	NONE,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

class CollisionRect
{
private:
	glm::vec2 position, size;
public:
	CollisionRect();
	CollisionRect(const glm::vec2& pos, const glm::vec2& size);

	~CollisionRect();

	// Sets the position of the rectangle collider.
	void SetPosition(const glm::vec2& pos);

	// Sets the size of the rectangle collider.
	void SetSize(const glm::vec2& size);

	// Returns an enum representing the side of the rectangle collider that is being collided with by the other rectangle collider.
	// If no collision is occurring, no side is returned (aka CollidingSide::NONE).
	CollidingSide IsColliding(const CollisionRect& otherRect) const;
	
	// Returns the X axis position of the left side of the rectangle collider.
	float GetLeftSidePosition() const;

	// Returns the X axis position of the right side of the rectangle collider.
	float GetRightSidePosition() const;

	// Returns the Y axis position of the top side of the rectangle collider.
	float GetTopSidePosition() const;

	// Returns the Y axis position of the bottom side of the rectangle collider.
	float GetBottomSidePosition() const;

	// Returns the position of the rectangle collider.
	const glm::vec2& GetPosition() const;

	// Returns the size of the rectangle collider.
	const glm::vec2& GetSize() const;
};

#endif