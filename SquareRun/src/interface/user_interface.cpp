#include <interface/user_interface.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UserInterface::UserInterface(const OrthogonalCamera& camera) :
	viewportCamera(&camera)
{}

void UserInterface::AddButtonElement(const std::string_view& id, const std::string_view& text, const glm::vec4& textColor, 
	const uint32_t& fontSize, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& buttonColor, HoverReactionType type, 
	const glm::vec4& shadowColor, float shadowThickness, float opacity)
{
	// If the ID is already taken then don't add the button element
	// This is to prevent ID collisions
	for (auto& button : this->buttonElements)
	{
		if (button.first == id)
		{
			return;
		}
	}

	this->buttonElements.push_back({ id.data(), 
		std::move(Button(*this->viewportCamera, text, textColor, fontSize, pos, size, buttonColor, type, shadowColor, shadowThickness, 
			opacity)) });
}

void UserInterface::UpdateInterface(const double& deltaTime)
{
	for (auto& button : this->buttonElements)
	{
		button.second.Update(deltaTime);
	}
}

void UserInterface::RenderInterface() const
{
	for (const auto& button : this->buttonElements)
	{
		button.second.Render();
	}
}

Button* UserInterface::GetButtonElement(const std::string_view& id)
{
	for (auto& button : this->buttonElements)
	{
		if (button.first == id)
		{
			return &button.second;
		}
	}

	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UserInterfaceManager::UserInterfaceManager() :
	activeUserInterface(nullptr)
{}

void UserInterfaceManager::CreateNewUI(const std::string_view& id, const OrthogonalCamera& camera)
{
	// If the ID is already taken then don't create the user interface object
	// This is to prevent ID collisions
	for (auto& userInterface : this->userInterfaceObjects)
	{
		if (userInterface.first == id)
		{
			return;
		}
	}

	this->userInterfaceObjects.push_back({ id.data(), std::move(UserInterface(camera)) });
}

void UserInterfaceManager::SetActiveUI(const std::string_view& id)
{
	for (auto& userInterface : this->userInterfaceObjects)
	{
		if (userInterface.first == id)
		{
			this->activeUserInterface = &userInterface.second;
			return;
		}
	}

	this->activeUserInterface = nullptr;
}

void UserInterfaceManager::UpdateActiveUI(const double& deltaTime)
{
	if (this->activeUserInterface)
	{
		this->activeUserInterface->UpdateInterface(deltaTime);
	}
}

void UserInterfaceManager::RenderActiveUI() const
{
	if (this->activeUserInterface)
	{
		this->activeUserInterface->RenderInterface();
	}
}

UserInterface* UserInterfaceManager::GetUIObject(const std::string_view& id)
{
	for (auto& userInterface : this->userInterfaceObjects)
	{
		if (userInterface.first == id)
		{
			return &userInterface.second;
		}
	}

	return nullptr;
}

UserInterfaceManager& UserInterfaceManager::GetInstance()
{
	static UserInterfaceManager instance;
	return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////