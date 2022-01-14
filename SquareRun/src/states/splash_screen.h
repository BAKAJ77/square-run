#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <core/game_state.h>
#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>
#include <graphics/orthogonal_camera.h>

class SplashScreen : public GameState
{
private:
	ShaderProgramPtr shader, postProcessShader;
	TextureBufferPtr texture, fboTexture;
	VertexBufferPtr vbo;
	VertexArrayPtr vao;

	FrameBufferPtr fbo;
	OrthogonalCamera camera;
protected:
	void Init() override;
	void Destroy() override;

	void Update(const double& deltaTime) override;
	void Render() const override;
public:
	static SplashScreen* GetGameState();
};

#endif
