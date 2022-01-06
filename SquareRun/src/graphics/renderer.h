#ifndef RENDERER_H
#define RENDERER_H

#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>
#include <graphics/camera_view.h>
#include <graphics/texture_buffer.h>
#include <graphics/font_loader.h>

#include <glm/glm.hpp>

using BatchedData = std::pair<std::vector<float>, std::vector<uint32_t>>;

struct GeometryData
{
	glm::vec2 position, size;
	float rotationAngle = 0.0f;
};

class Renderer
{
private:
	VertexBuffer rectVBO, triangleVBO;
	VertexArray rectVAO, triangleVAO;
private:
	// Returns a generated model matrix.
	glm::mat4 GenerateModelMatrix(const glm::vec2& pos, const glm::vec2& size, float rotationAngle) const;

	// Returns a pair of vectors, one containing vertex data and the other containing index data.
	// The vertex and index data inside the vectors are the result of all the glyphs in the text given having their
	// vertex and index data all batched into their respective vector containers.
	BatchedData GenerateBatchedTextData(const Font& font, const std::string_view& text) const;
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

	// Renders a colored rectangle of specified size to the position specified on the screen.
	// Note: The origin of transformations, in relation to the rectangle being rendered, is it's centre.
	void RenderRect(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos, 
		const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a colored triangle of specified size to the position specified on the screen.
	// Note: The origin of transformations, in relation to the triangle being rendered, is it's centre.
	void RenderTriangle(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos,
		const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a textured rectangle of specified size to the position specified on the screen.
	// Note: The origin of transformations, in relation to the rectangle being rendered, is it's centre.
	void RenderTexturedRect(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture,
		const glm::vec2& pos, const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a textured triangle of specified size to the position specified on the screen.
	// Note: The origin of transformations, in relation to the triangle being rendered, is it's centre.
	void RenderTexturedTriangle(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture,
		const glm::vec2& pos, const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a colored text of specified size to the position specified on the screen.
	// Note: The origin of transformations, in relation to the triangle being rendered, is it's centre.
	void RenderText(const ShaderProgram& shader, const Camera& sceneCamera, const Font& font, uint32_t fontSize,
		const std::string_view& text, const glm::vec4& color, const glm::vec2& pos, float rotationAngle = 0.0f) const;

	// Returns singleton instance object of this class.
	static Renderer& GetInstance();
};

#endif 