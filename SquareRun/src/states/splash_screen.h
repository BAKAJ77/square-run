#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <core/game_state.h>
#include <core/collision_detection.h>

class SplashScreen : public GameState
{
private:
	TextureBufferPtr texture;
	FontPtr font;

	glm::vec2 pos;
	RectangleCollider collider1, collider2;
protected:
	void Init() override;
	void Destroy() override;

	void Update(const double& deltaTime) override;
	void Render() const override;
public:
	static SplashScreen* GetGameState();
};

#endif