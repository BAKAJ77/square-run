#include <states/intro_screen.h>
#include <core/input_system.h>
#include <util/timestamp.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IntroScreen::Init()
{
	// Initialize logic variables
	this->bkgSize = { 0, 50 };
	this->bkgColor = { 255, 255, 255, 255 };
	this->logoPosition = { this->camera.GetSize().x / 2.0f, -300 };
	this->logoSize = { 850, 425 };
	this->borderOpacity = 0.0f;
	this->textOpacity = 0.0f;
	this->timeWhenIntroMusicEnd = 0.0f;

	this->effectPositions[0] = { -700, 1080 + (700 / this->camera.GetAspectRatio()) };
	this->effectPositions[1] = { 2620 - (100 / this->camera.GetAspectRatio()), -100 - (700 / this->camera.GetAspectRatio())};
	this->effectPositions[2] = { 2620 + (100 / this->camera.GetAspectRatio()), 100 - (700 / this->camera.GetAspectRatio()) };

	// Load the game state textures and font
	this->borderTexture = Memory::LoadTextureFromFile("state_border.png");
	this->logoTexture = Memory::LoadTextureFromFile("logo.png", false);
	this->textFont = Memory::LoadFontFromFile("fff_forwa.ttf");
	
	// Load and play the intro music 
	this->introMusic = AudioSystem::GetInstance().LoadAudioFromFile("title_screen.wav");
	this->introMusic->Play();
	this->introMusic->SetVolume(1.0f);
}

void IntroScreen::Destroy() {}

void IntroScreen::Update(const double& deltaTime)
{
	this->UpdateIntroSequence(deltaTime);
	this->UpdateEffects(deltaTime);
	
	// Once the intro is complete and the user presses ENTER, then continue onto the next game state
	if (this->introComplete)
	{
		if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_ENTER))
		{
			//this->SwitchState();
		}
	}
}

void IntroScreen::Render() const
{
	// Render the background
	Renderer::GetInstance().RenderRect(this->camera, this->bkgColor, { this->bkgSize.x / 2.0f, this->camera.GetSize().y / 2.0f }, 
		this->bkgSize);

	// Render the effects
	Renderer::GetInstance().RenderRect(this->camera, { 255, 0, 0, 255 }, this->effectPositions[0], { 700, 50 }, 150);
	Renderer::GetInstance().RenderRect(this->camera, { 0, 0, 255, 255 }, this->effectPositions[1], { 700, 50 }, 150);
	Renderer::GetInstance().RenderRect(this->camera, { 255, 255, 0, 255 }, this->effectPositions[2], { 700, 50 }, 150);

	// Render the game logo
	Renderer::GetInstance().RenderTexturedRect(this->camera, this->logoTexture, this->logoPosition, this->logoSize, 0, 
		{ 255, 225, 255, 255 });

	// Render the border
	Renderer::GetInstance().RenderTexturedRect(this->camera, this->borderTexture, this->camera.GetSize() / 2.0f,
		this->camera.GetSize(), 0, { 0, 255, 0, this->borderOpacity });

	// Render play text
	const glm::vec2 textSize = Renderer::GetInstance().GetTextSize(this->textFont, 100, "Press Enter To Play");
	Renderer::GetInstance().RenderText(this->camera, this->textFont, 100, "Press Enter To Play", { 255, 255, 255, this->textOpacity },
		{ (this->camera.GetSize().x / 2.0f) - (textSize.x / 2.0f), 900 });
}

IntroScreen* IntroScreen::GetGameState()
{
	static IntroScreen gameState;
	return &gameState;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IntroScreen::UpdateIntroSequence(const double& deltaTime)
{
	// Animate the background stretching to fill the screen
	if (this->bkgSize.x < this->camera.GetSize().x)
	{
		this->bkgSize.x = std::min(this->bkgSize.x + (1500 * (float)deltaTime), this->camera.GetSize().x);
	}
	else
	{
		if (this->bkgSize.y < this->camera.GetSize().y)
		{
			this->bkgSize.y = std::min(this->bkgSize.y + (4500 * (float)deltaTime), this->camera.GetSize().y);
		}
		else
		{
			// Make the background change to a green color
			if (this->bkgColor.r > 0 && this->bkgColor.b > 0)
			{
				this->bkgColor.r = std::min(this->bkgColor.r - (200 * (float)deltaTime), this->bkgColor.r);
				this->bkgColor.b = std::min(this->bkgColor.b - (200 * (float)deltaTime), this->bkgColor.b);
			}
			else
			{
				// Make the border gradually opaque (visible)
				if (this->borderOpacity < 255.0f)
				{
					this->borderOpacity = std::min(this->borderOpacity + (400 * (float)deltaTime), 255.0f);
				}
				else
				{
					// Move the logo of the game into view
					if (this->logoPosition.y < 450.0f)
					{
						this->logoPosition.y = std::min(this->logoPosition.y + (750 * (float)deltaTime), 450.0f);
					}
					else
					{
						this->introComplete = true;
					}
				}
			}
		}
	}
}

void IntroScreen::UpdateEffects(const double& deltaTime)
{
	// Update logo bop effect
	this->logoSize.x = 850 + (float)(std::sin(Util::GetSecondsSinceEpoch() * 10.0f) * 100.0f);
	this->logoSize.y = 425 + (float)(std::cos(Util::GetSecondsSinceEpoch() * 10.0f) * 100.0f);


	if (this->introComplete)
	{
		// Update the play text opacity
		this->textOpacity = (float)std::abs(std::sin(Util::GetSecondsSinceEpoch() * 2.0f)) * 255.0f;

		// Update the effect positions
		this->effectPositions[0].x += 700 * (float)deltaTime;
		this->effectPositions[0].y -= (700 / this->camera.GetAspectRatio()) * (float)deltaTime;

		this->effectPositions[1].x -= 700 * (float)deltaTime;
		this->effectPositions[1].y += (700 / this->camera.GetAspectRatio()) * (float)deltaTime;

		this->effectPositions[2].x -= 700 * (float)deltaTime;
		this->effectPositions[2].y += (700 / this->camera.GetAspectRatio()) * (float)deltaTime;

		// Restart effect positions if they are offscreen
		if (this->effectPositions[0].x >= 2620 - (100 / this->camera.GetAspectRatio()))
		{
			this->effectPositions[0] = { -700, 1080 + (700 / this->camera.GetAspectRatio()) };
			this->effectPositions[1] = { 2620 - (100 / this->camera.GetAspectRatio()), -100 - (700 / this->camera.GetAspectRatio()) };
			this->effectPositions[2] = { 2620 + (100 / this->camera.GetAspectRatio()), 100 - (700 / this->camera.GetAspectRatio()) };
		}
	}
}

void IntroScreen::CheckAutoContinue()
{
	if (this->introMusic->isFinished() && this->timeWhenIntroMusicEnd == 0.0f)
	{
		this->timeWhenIntroMusicEnd = Util::GetSecondsSinceEpoch();
	}
	else
	{
		if (Util::GetSecondsSinceEpoch() - this->timeWhenIntroMusicEnd >= 5.0f)
		{
			//this->SwitchState();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////