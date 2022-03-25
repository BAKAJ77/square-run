#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <graphics/orthogonal_camera.h>
#include <interface/button.h>

#include <vector>
#include <string>
#include <string_view>

class UserInterface
{
	using ButtonElement = std::pair<std::string, Button>;
	friend class UserInterfaceManager;
private:
	const OrthogonalCamera* viewportCamera;
	std::vector<ButtonElement> buttonElements;
private:
	UserInterface(const OrthogonalCamera& camera);

	// Updates all elements in the user interface object.
	void UpdateInterface(const double& deltaTime);

	// Renders all elements in the user interface object.
	void RenderInterface() const;
public:
	~UserInterface() = default;

	// Adds new button element to the user interface object.
	void AddButtonElement(const std::string_view& id, const std::string_view& text, const glm::vec4& textColor, const uint32_t& fontSize,
		const glm::vec2& pos, const glm::vec2& size, const glm::vec4& buttonColor, HoverReactionType type,
		const glm::vec4& shadowColor = { 0, 0, 0, 100 }, float shadowThickness = 7.5f, float opacity = 255.0f);

	// Returns the button element with the corresponding ID given.
	// Note that if no button element matching the ID is found, then nullptr is returned.
	Button* GetButtonElement(const std::string_view& id);
};

class UserInterfaceManager
{
	using UIObject = std::pair<std::string, UserInterface>;
	friend class GameStateSystem;
private:
	std::vector<UIObject> userInterfaceObjects;
	UserInterface* activeUserInterface;
private:
	UserInterfaceManager();

	// Updates the elements in the active user interface.
	void UpdateActiveUI(const double& deltaTime);

	// Renders the elements in the active user interface.
	void RenderActiveUI() const;
public:
	UserInterfaceManager(const UserInterfaceManager& other) = delete;
	UserInterfaceManager(UserInterfaceManager&& temp) noexcept = delete;
	~UserInterfaceManager() = default;

	UserInterfaceManager& operator=(const UserInterfaceManager& other) = delete;
	UserInterfaceManager& operator=(UserInterfaceManager&& temp) noexcept = delete;

	// Creates and stores a new User Interface object.
	void CreateNewUI(const std::string_view& id, const OrthogonalCamera& camera);

	// Sets the user interface object with the corresponding ID given as active.
	// The user interface object set as active will be updated and rendered until another (or no) user interface is set as active.
	// Note that to set no user interfaces objects as active, you just pass an ID which belongs to no user interface object.
	void SetActiveUI(const std::string_view& id);
	
	// Returns the user interface object with the corresponding ID given.
	// Note that if no user interface object matching the ID is found, then nullptr is returned.
	UserInterface* GetUIObject(const std::string_view& id);

	// Returns singleton instance object of this class.
	static UserInterfaceManager& GetInstance();
};

#endif
