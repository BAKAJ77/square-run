#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <core/window.h>
#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>
#include <graphics/camera_view.h>
#include <graphics/texture_buffer.h>
#include <graphics/font_loader.h>

class GameCore
{
private:
	WindowFrame window;
	ShaderProgram geometryShader, textShader;
	Camera sceneCamera;

	TextureBuffer texture;
	Font cascadiaFont;
private:
	void MainLoop();
	void Update(const double& deltaTime);
	void Render() const;
public:
	GameCore();
	~GameCore();
};

#endif