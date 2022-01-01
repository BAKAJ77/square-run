#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <core/window.h>
#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>
#include <graphics/camera_view.h>

class GameCore
{
private:
	WindowFrame window;
	ShaderProgram shader;
	Camera sceneCamera;

	VertexBuffer vbo;
	VertexArray vao;
	IndexBuffer ibo;
private:
	void MainLoop();
	void Update(const double& deltaTime);
	void Render() const;
public:
	GameCore();
	~GameCore();
};

#endif