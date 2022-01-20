#ifndef INTRO_SCREEN_H
#define INTRO_SCREEN_H

#include <core/game_state.h>
#include <core/audio_system.h>

class IntroScreen : public GameState
{
private:
	GlobalAudioPtr music;
protected:
	void Init() override;
	void Destroy() override;

	void Update(const double& deltaTime) override;
	void Render() const override;
public:
	static IntroScreen* GetGameState();
};

#endif