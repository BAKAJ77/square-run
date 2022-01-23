#ifndef INTRO_SCREEN_H
#define INTRO_SCREEN_H

#include <core/game_state.h>
#include <core/audio_system.h>

class IntroScreen : public GameState
{
private:
	// Assets
	TextureBufferPtr borderTexture, logoTexture;
	GlobalAudioPtr introMusic;
	FontPtr textFont;

	// Logic variables
	glm::vec2 bkgSize, logoPosition, logoSize, effectPositions[3];
	glm::vec4 bkgColor;
	float borderOpacity, textOpacity, timeWhenIntroMusicEnd, timeWhenTextAppear;
	bool introComplete, abortIntro;
private:
	// Updates the game intro animation sequence.
	void UpdateIntroSequence(const double& deltaTime);

	// Updates the effects in the intro menu screen.
	void UpdateEffects(const double& deltaTime);

	// Checks if the user pressed enter, if so then the intro music (if playing) is ended and the main menu game state is started.
	void CheckUserContinue(const double& deltaTime);

	// Automatically switches to the main menu game state if nothing is pressed for a few seconds after the intro animation ends.
	void CheckAutoContinue();
protected:
	void Init() override;
	void Destroy() override;

	void Update(const double& deltaTime) override;
	void Render() const override;
public:
	static IntroScreen* GetGameState();
};

#endif