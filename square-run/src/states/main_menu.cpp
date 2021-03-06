#include <states/main_menu.h>
#include <interface/user_interface.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainMenu::Init()
{
	// Initialize logic variables
	this->borderOpacity = 0.0f;

	this->effectPositions[0] = { -750, (this->camera.GetSize().y / 2.0f) - 10  };
	this->effectPositions[1] = { 2670, (this->camera.GetSize().y / 2.0f) + 10 };
	this->effectPositions[2] = { (this->camera.GetSize().x / 2.0f) - 10, -750 };
	this->effectPositions[3] = { (this->camera.GetSize().x / 2.0f) + 10, 1830 };

	// Initialize main menu user interface
	UserInterfaceManager::GetInstance().CreateNewUI("main-menu", this->camera);
	UserInterfaceManager::GetInstance().SetActiveUI("main-menu");

	UserInterfaceManager::GetInstance().GetUIObject("main-menu")->AddButtonElement("play", "PLAY", { 255, 255, 255, 255 }, 115, 
		{ 485, 275 }, { 805, 400 }, { 255, 0, 0, 255 }, HoverReactionType::HIGHLIGHT_ENLARGE_ALL_ROUND);
	UserInterfaceManager::GetInstance().GetUIObject("main-menu")->AddButtonElement("settings", "SETTINGS", { 255, 255, 255, 255 }, 115,
		{ 1425, 275 }, { 805, 400 }, { 255, 0, 0, 255 }, HoverReactionType::HIGHLIGHT_ENLARGE_ALL_ROUND);
	UserInterfaceManager::GetInstance().GetUIObject("main-menu")->AddButtonElement("credits", "CREDITS", { 255, 255, 255, 255 }, 115,
		{ 485, 790 }, { 805, 400 }, { 255, 0, 0, 255 }, HoverReactionType::HIGHLIGHT_ENLARGE_ALL_ROUND);
	UserInterfaceManager::GetInstance().GetUIObject("main-menu")->AddButtonElement("exit", "EXIT", { 255, 255, 255, 255 }, 115,
		{ 1425, 790 }, { 805, 400 }, { 255, 0, 0, 255 }, HoverReactionType::HIGHLIGHT_ENLARGE_ALL_ROUND);

	UserInterfaceManager::GetInstance().GetUIObject("main-menu")->GetButtonElement("exit")->SetClickEventCallback([=]() { this->PopState(); });

	// Load the game state textures
	this->borderTexture = Memory::LoadTextureFromFile("state_border.png");

	// Load and play the intro music 
	this->menuMusic = AudioSystem::GetInstance().LoadAudioFromFile("main_menu.wav");
	this->menuMusic->Play();
	this->menuMusic->SetVolume(1.0f);

	// Set background color
	Renderer::GetInstance().SetClearColor({ 0, 255, 0, 255 });
}

void MainMenu::Destroy()
{
	this->borderTexture.reset();
	this->menuMusic.reset();
}

void MainMenu::Update(const double& deltaTime)
{
	this->UpdateEffects(deltaTime);

	// Update the opacity of the border
	if (this->borderOpacity < 255.0f)
	{
		this->borderOpacity = std::min(this->borderOpacity + (150 * (float)deltaTime), 255.0f);
	}

	// Change music play position to create a perfect music track loop
	if (this->menuMusic->GetPlayPosition() >= 56855)
	{
		this->menuMusic->SetPlayPosition(28876);
	}
}

void MainMenu::Render() const
{
	// Render the border
	Renderer::GetInstance().RenderTexturedRect(this->camera, this->borderTexture, this->camera.GetSize() / 2.0f,
		this->camera.GetSize(), 0, { 0, 255, 0, this->borderOpacity });

	// Render the effects
	Renderer::GetInstance().RenderRect(this->camera, { 255, 0, 0, 255 }, this->effectPositions[0], { 1000, 10 });
	Renderer::GetInstance().RenderRect(this->camera, { 255, 0, 255, 255 }, this->effectPositions[2], { 10, 1000 });
	Renderer::GetInstance().RenderRect(this->camera, { 0, 0, 255, 255 }, this->effectPositions[1], { 1000, 10 });
	Renderer::GetInstance().RenderRect(this->camera, { 255, 255, 0, 255 }, this->effectPositions[3], { 10, 1000 });
}

MainMenu* MainMenu::GetGameState()
{
	static MainMenu gameState;
	return &gameState;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainMenu::UpdateEffects(const double& deltaTime)
{
	// Update the effect positions
	float effectSpeed = 200.0f;

	this->effectPositions[0].x += effectSpeed * (float)deltaTime;
	this->effectPositions[1].x -= effectSpeed * (float)deltaTime;
	this->effectPositions[2].y += effectSpeed * (float)deltaTime;
	this->effectPositions[3].y -= effectSpeed * (float)deltaTime;

	// Restart effect positions if they are offscreen
	if (this->effectPositions[0].x >= 2670)
	{
		this->effectPositions[0] = { -750, (this->camera.GetSize().y / 2.0f) - 10 };
		this->effectPositions[1] = { 2670, (this->camera.GetSize().y / 2.0f) + 10 };
	}

	if (this->effectPositions[2].y >= 1830)
	{
		this->effectPositions[2] = { (this->camera.GetSize().x / 2.0f) - 10, -750 };
		this->effectPositions[3] = { (this->camera.GetSize().x / 2.0f) + 10, 1830 };
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////