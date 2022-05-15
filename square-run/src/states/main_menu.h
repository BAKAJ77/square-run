#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <core/game_state.h>
#include <core/audio_system.h>

class MainMenu : public GameState
{
private:
	void UpdateEffects(const double& deltaTime);
private:
	// Assets
	TextureBufferPtr borderTexture;
	GlobalAudioPtr menuMusic;

	// Logic variables
	glm::vec2 effectPositions[4];
	float borderOpacity, effectOpacity;
protected:
	void Init() override;
	void Destroy() override;

	void Resume() override;

	bool OnExitTransitionUpdate(const double& deltaTime) override;
	bool OnStartTransitionUpdate(const double& deltaTime) override;

	void Update(const double& deltaTime) override;
	void Render() const override;
public:
	static MainMenu* GetGameState();
};

#endif