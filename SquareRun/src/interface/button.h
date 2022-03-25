#ifndef BUTTON_H
#define BUTTON_H

#include <glm/glm.hpp>
#include <functional>
#include <string_view>
#include <string>

class OrthogonalCamera;

enum class HoverReactionType
{
	HIGHLIGHT_ENLARGE_ALL_ROUND,
	HIGHLIGHT_ENLARGE_TO_THE_RIGHT
};

class Button
{
	friend class UserInterface;
private:
	const OrthogonalCamera* viewportCamera;

	std::string text;
	glm::vec2 position, baseSize, currentSize, textSize;
	glm::vec4 textColor, buttonColor, shadowColor, borderColor;
	float shadowThickness, opacity;
	uint32_t fontSize;

	HoverReactionType hoverType;
	std::function<void()> onClickEventFunction;
	bool clicked, outOfFocus;
private:
	Button(const OrthogonalCamera& viewportCamera, const std::string_view& text, const glm::vec4& textColor, const uint32_t& fontSize,
		const glm::vec2& pos, const glm::vec2& size, const glm::vec4& buttonColor, HoverReactionType type,
		const glm::vec4& shadowColor = { 0, 0, 0, 100 }, float shadowThickness = 7.5f, float opacity = 255.0f);

	// Polls button events and updates button graphics accordingly.
	void Update(const double& deltaTime);

	// Renders the button.
	void Render() const;
public:
	~Button() = default;

	// Sets the position of the button.
	void SetPosition(const glm::vec2& pos);

	// Sets the size of the button.
	void SetSize(const glm::vec2& size);

	// Sets the opacity of the button.
	void SetOpacity(float opacity);

	// Sets the function called when the button is clicked.
	void SetClickEventCallback(std::function<void()> callbackFunc);

	// Returns the position of the button.
	const glm::vec2& GetPosition() const;

	// Returns the size of the button.
	const glm::vec2& GetSize() const;

	// Returns the opacity of the button.
	const float& GetOpacity() const;

	// Returns whether the user is interacting with the button (i.e in focus)
	bool IsFocused() const;
};

#endif