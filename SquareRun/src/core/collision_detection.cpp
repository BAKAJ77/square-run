#include <core/collision_detection.h>

RectangleCollider::RectangleCollider() = default;

RectangleCollider::RectangleCollider(const glm::vec2 & pos, const glm::vec2 & size) :
	position(pos), size(size)
{}

RectangleCollider::~RectangleCollider() = default;

void RectangleCollider::SetPosition(const glm::vec2& pos)
{
	this->position = pos;
}

void RectangleCollider::SetSize(const glm::vec2 & size)
{
	this->size = size;
}

CollidingSide RectangleCollider::IsColliding(const RectangleCollider& otherCollider) const
{
	bool isColliding = false;

	// Check if the collision rectangles are intersecting/colliding
	if (this->GetRightSidePosition() > otherCollider.GetLeftSidePosition() &&
		this->GetLeftSidePosition() < otherCollider.GetRightSidePosition() &&
		this->GetTopSidePosition() < otherCollider.GetBottomSidePosition() &&
		this->GetBottomSidePosition() > otherCollider.GetTopSidePosition())
	{
		isColliding = true;
	}

	// Deduce the colliding side of the rectangle collider
	if (isColliding)
	{
		// Calculate the distances between the sides of this rectangle collider and opposing sides of the rectangle collider
		const float leftDistance = std::abs(this->GetLeftSidePosition() - otherCollider.GetRightSidePosition());
		const float rightDistance = std::abs(this->GetRightSidePosition() - otherCollider.GetLeftSidePosition());
		const float topDistance = std::abs(this->GetTopSidePosition() - otherCollider.GetBottomSidePosition());
		const float bottomDistance = std::abs(this->GetBottomSidePosition() - otherCollider.GetTopSidePosition());

		// Get the lowest distance
		float distances[4] = { rightDistance, leftDistance, topDistance, bottomDistance };
		float lowestDistance = distances[0];

		for (size_t i = 0; i < 4; i++)
		{
			if (distances[i] < lowestDistance)
				lowestDistance = distances[i];
		}

		// Return the side with the lowest distance value
		return
			lowestDistance == rightDistance ? CollidingSide::RIGHT :
			lowestDistance == leftDistance ? CollidingSide::LEFT :
			lowestDistance == topDistance ? CollidingSide::TOP :
			lowestDistance == bottomDistance ? CollidingSide::BOTTOM :
			CollidingSide::NONE;
	}

	return CollidingSide::NONE; // No collision has been detected
}

float RectangleCollider::GetLeftSidePosition() const
{
	return this->position.x - (this->size.x / 2.0f);
}

float RectangleCollider::GetRightSidePosition() const
{
	return this->position.x + (this->size.x / 2.0f);
}

float RectangleCollider::GetTopSidePosition() const
{
	return this->position.y - (this->size.y / 2.0f);
}

float RectangleCollider::GetBottomSidePosition() const
{
	return this->position.y + (this->size.y / 2.0f);
}

const glm::vec2& RectangleCollider::GetPosition() const
{
	return this->position;
}

const glm::vec2& RectangleCollider::GetSize() const
{
	return this->size;
}