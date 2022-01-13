#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <core/game_state.h>
#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>

class SplashScreen : public GameState
{
private:
	ShaderProgramPtr shader;
	TextureBufferPtr texture;
	VertexBufferPtr vbo;
	VertexArrayPtr vao;
protected:
	void Init() override;
	void Destroy() override;

	void Update(const double& deltaTime) override;
	void Render() const override;
public:
	static SplashScreen* GetGameState();
};

#endif
