#ifndef RENDERER_H
#define RENDERER_H

#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>
#include <graphics/camera_view.h>
#include <graphics/texture_buffer.h>

#include <glm/glm.hpp>

class Renderer
{
private:
	VertexBuffer rectVBO;
	VertexArray rectVAO;
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

	// Renders a rectange of specified size to the position specified on the screen.
	// Note: The origin of transformations, in relation to the rectangle being rendered, is it's centre.
	void RenderRect(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos, 
		const glm::vec2& size, float rotationAngle = 0.0f) const;

	void RenderTexturedRect(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture,
		const glm::vec2& pos, const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Returns singleton instance object of this class.
	static Renderer& GetInstance();
};

#endif 