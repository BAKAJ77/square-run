#ifndef RENDERER_H
#define RENDERER_H

#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>

#include <glm/glm.hpp>

class Renderer
{
private:
	Renderer();
public:
	Renderer(const Renderer& other) = delete;
	Renderer(Renderer&& temp) = delete;

	~Renderer();

	Renderer& operator=(const Renderer& other) = delete;
	Renderer& operator=(Renderer&& temp) = delete;

	// Clears the screen and fills the screen with the specified color.
	void Clear(const glm::vec4& color) const;

	// Renders to screen using the vertex data bound to the given vertex array object.
	// Note: The last parameter 'count' specifies the amount of vertices in the VBO's data (if you're not using an IBO) or indices 
	// in the IBO's data (if you're using an IBO).
	void RenderData(const ShaderProgram& shader, const VertexArray& vao, uint32_t count) const;

	// Returns singleton instance object of this class.
	static Renderer& GetInstance();
};

#endif 