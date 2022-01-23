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
	float borderOpacity, textOpacity, timeWhenIntroMusicEnd;
	bool introComplete;
private:
	void UpdateIntroSequence(const double& deltaTime);
	void UpdateEffects(const double& deltaTime);
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