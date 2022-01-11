#include <core/collision_detection.h>

CollisionRect::CollisionRect() = default;

CollisionRect::CollisionRect(const glm::vec2& pos, const glm::vec2& size) :
	position(pos), size(size)
{}

CollisionRect::~CollisionRect() = default;

void CollisionRect::SetPosition(const glm::vec2 & pos)
{
	this->position = pos;
}

void CollisionRect::SetSize(const glm::vec2& size)
{
	this->size = size;
}

CollidingSide CollisionRect::IsColliding(const CollisionRect& otherRect) const
{
	bool isColliding = false;

	// Check if the collision rectangles are intersecting/colliding
	if (this->GetRightSidePosition() > otherRect.GetLeftSidePosition() &&
		this->GetLeftSidePosition() < otherRect.GetRightSidePosition() &&
		this->GetTopSidePosition() < otherRect.GetBottomSidePosition() &&
		this->GetBottomSidePosition() > otherRect.GetTopSidePosition())
	{
		isColliding = true;
	}

	// Deduce the colliding side of the rectangle collider
	if (isColliding)
	{
		// Calculate the distances between the sides of this rectangle collider and opposing sides of the rectangle collider
		const float leftDistance = std::abs(this->GetLeftSidePosition() - otherRect.GetRightSidePosition());
		const float rightDistance = std::abs(this->GetRightSidePosition() - otherRect.GetLeftSidePosition());
		const float topDistance = std::abs(this->GetTopSidePosition() - otherRect.GetBottomSidePosition());
		const float bottomDistance = std::abs(this->GetBottomSidePosition() - otherRect.GetTopSidePosition());

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

float CollisionRect::GetLeftSidePosition() const
{
	return this->position.x - (this->size.x / 2.0f);
}

float CollisionRect::GetRightSidePosition() const
{
	return this->position.x + (this->size.x / 2.0f);
}

float CollisionRect::GetTopSidePosition() const
{
	return this->position.y - (this->size.y / 2.0f);
}

float CollisionRect::GetBottomSidePosition() const
{
	return this->position.y + (this->size.y / 2.0f);
}

const glm::vec2& CollisionRect::GetPosition() const
{
	return this->position;
}

const glm::vec2& CollisionRect::GetSize() const
{
	return this->size;
}