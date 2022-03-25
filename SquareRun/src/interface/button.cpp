#include <interface/button.h>
#include <graphics/renderer.h>
#include <core/input_system.h>

namespace ButtonGlobal
{
	static FontPtr font = nullptr;
}

Button::Button(const OrthogonalCamera& camera, const std::string_view& text, const glm::vec4& textColor, const uint32_t& fontSize, 
	const glm::vec2& pos, const glm::vec2& size, const glm::vec4& buttonColor, HoverReactionType type, const glm::vec4& shadowColor, 
	float shadowThickness, float opacity) :
	viewportCamera(&camera), position(pos), baseSize(size), currentSize(size), buttonColor(buttonColor), shadowColor(shadowColor), 
	shadowThickness(shadowThickness), hoverType(type), text(text), fontSize(fontSize), textColor(textColor), borderColor({ 0, 0, 0, 255 }),
	opacity(opacity), clicked(false), outOfFocus(false)
{
	// Load the font for the button if it hasn't been loaded yet
	if (!ButtonGlobal::font)
	{
		ButtonGlobal::font = Memory::LoadFontFromFile("fff_forwa.ttf");
	}

	// Calculate the size of the text to be rendered on the button
	this->textSize = Renderer::GetInstance().GetTextSize(ButtonGlobal::font, fontSize, text);
}

void Button::SetPosition(const glm::vec2 & pos)
{
	this->position = pos;
}

void Button::SetSize(const glm::vec2& size)
{
	this->baseSize = size;
	this->currentSize = size;
}

void Button::SetOpacity(float opacity)
{
	this->opacity = opacity;
}

void Button::SetClickEventCallback(std::function<void()> callbackFunc)
{
	this->onClickEventFunction = callbackFunc;
}

void Button::Update(const double& deltaTime)
{
	// Check if the cursor is currently hovering over the button
	const glm::vec2 cursorPos = InputSystem::GetInstance().GetCursorPosition(this->viewportCamera);
	constexpr float hoverAnimationSpeed = 500.0f;

	// Check if i.e the user has stopped interacting with something else
	if (!InputSystem::GetInstance().WasMouseButtonPressed(MouseCode::MOUSE_BUTTON_LEFT) && this->outOfFocus)
	{
		this->outOfFocus = false;
	}

	if (cursorPos.x >= this->position.x - (this->currentSize.x / 2.0f) && cursorPos.x <= this->position.x + (this->currentSize.x / 2.0f) &&
		cursorPos.y >= this->position.y - (this->currentSize.y / 2.0f) && cursorPos.y <= this->position.y + (this->currentSize.y / 2.0f))
	{
		// Handle button hover animation
		if (this->hoverType == HoverReactionType::HIGHLIGHT_ENLARGE_ALL_ROUND)
		{
			// Animate button size
			this->currentSize.x = std::min(this->currentSize.x + (hoverAnimationSpeed * (float)deltaTime), this->baseSize.x + 50.0f);
			this->currentSize.y = std::min(this->currentSize.y + (hoverAnimationSpeed * (float)deltaTime), this->baseSize.y + 50.0f);

			// Animate border color change
			this->borderColor.g = std::min(this->borderColor.g + ((hoverAnimationSpeed * 2.0f) * (float)deltaTime), 255.0f);
			this->borderColor.b = std::min(this->borderColor.b + ((hoverAnimationSpeed * 2.0f) * (float)deltaTime), 255.0f);
		}
		else if (this->hoverType == HoverReactionType::HIGHLIGHT_ENLARGE_TO_THE_RIGHT)
		{
			// TO BE IMPLEMENTED SOON
		}

		// Check if the cursor clicked on the button
		if (InputSystem::GetInstance().WasMouseButtonPressed(MouseCode::MOUSE_BUTTON_LEFT) && this->onClickEventFunction && !this->outOfFocus)
		{
			this->clicked = true;
		}
	}
	else
	{
		// Handle button un-hover animation
		if (this->hoverType == HoverReactionType::HIGHLIGHT_ENLARGE_ALL_ROUND)
		{
			// Animate button size
			this->currentSize.x = std::max(this->currentSize.x - (hoverAnimationSpeed * (float)deltaTime), this->baseSize.x);
			this->currentSize.y = std::max(this->currentSize.y - (hoverAnimationSpeed * (float)deltaTime), this->baseSize.y);

			// Animate border color change
			this->borderColor.g = std::max(this->borderColor.g - ((hoverAnimationSpeed * 2.0f) * (float)deltaTime), 0.0f);
			this->borderColor.b = std::max(this->borderColor.b - ((hoverAnimationSpeed * 2.0f) * (float)deltaTime), 0.0f);
		}
		else if (this->hoverType == HoverReactionType::HIGHLIGHT_ENLARGE_TO_THE_RIGHT)
		{
			// TO BE IMPLEMENTED SOON
		}

		// Check if this button is not in focus (i.e the user is interacting with something else)
		if (InputSystem::GetInstance().WasMouseButtonPressed(MouseCode::MOUSE_BUTTON_LEFT) && !this->clicked)
		{
			this->outOfFocus = true;
		}
	}

	// Run the on-click event callback function after the left mouse button has been released after clicking the button
	if (!InputSystem::GetInstance().WasMouseButtonPressed(MouseCode::MOUSE_BUTTON_LEFT) && this->clicked)
	{
		if (cursorPos.x >= this->position.x - (this->currentSize.x / 2.0f) && cursorPos.x <= this->position.x + (this->currentSize.x / 2.0f) &&
			cursorPos.y >= this->position.y - (this->currentSize.y / 2.0f) && cursorPos.y <= this->position.y + (this->currentSize.y / 2.0f))
		{
			this->onClickEventFunction(); // Run the on-click event callback function
		}
		else
		{
			this->clicked = false;
		}
	}
}

void Button::Render() const
{
	const glm::vec4 opacityMultiplier = { 255, 255, 255, this->opacity };

	// Render the button body
	Renderer::GetInstance().RenderRect(*this->viewportCamera, (this->shadowColor * 255.0f) / 255.0f, 
		this->position + this->shadowThickness * 2.0f, this->currentSize);

	Renderer::GetInstance().RenderRect(*this->viewportCamera, (this->borderColor * 255.0f) / 255.0f, this->position, this->currentSize);
	Renderer::GetInstance().RenderRect(*this->viewportCamera, (this->buttonColor * 255.0f) / 255.0f, this->position, 
		this->currentSize - 20.0f);

	// Render the button text
	Renderer::GetInstance().RenderText(*this->viewportCamera, ButtonGlobal::font, this->fontSize, this->text, 
		(this->textColor * 255.0f) / 255.0f, { this->position.x - (this->textSize.x / 2.0f), this->position.y + (this->textSize.y / 2.0f) });
}

const glm::vec2& Button::GetPosition() const
{
	return this->position;
}

const glm::vec2& Button::GetSize() const
{
	return this->currentSize;
}

const float& Button::GetOpacity() const
{
	return this->opacity;
}
