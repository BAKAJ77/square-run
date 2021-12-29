#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <core/window.h>
#include <graphics/shader_program.h>

class GameCore
{
private:
	WindowFrame window;
	ShaderProgram shader;
private:
	void MainLoop();
	void Update(const double& deltaTime);
	void Render() const;
public:
	GameCore();
	~GameCore();
};

#endif