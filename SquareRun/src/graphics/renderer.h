#ifndef RENDERER_H
#define RENDERER_H

#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>
#include <graphics/camera_view.h>
#include <graphics/texture_buffer.h>
#include <graphics/font_loader.h>

#include <glm/glm.hpp>

using BatchedData = std::pair<std::vector<float>, std::vector<uint32_t>>;

class WindowFrame;

enum class RenderTarget
{
	DEFAULT_FRAMEBUFFER, // This is what is ultimately displayed to the screen, the post-processed scene texture is rendered here
	SCENE_FRAMEBUFFER, // This is where the scene is rendered to, contains the texture of the rendered scene
	EXTERNAL_FRAMEBUFFER // A framebuffer that's defined outside of the renderer
};

struct GeometryData
{
	glm::vec2 position, size;
	float rotationAngle = 0.0f;
};

class Renderer
{
private:
	const WindowFrame* window;

	VertexBuffer rectVBO, triangleVBO;
	VertexArray rectVAO, triangleVAO;

	ShaderProgram postProcessShader;
	TextureBuffer postProcessSceneTexture;
	FrameBuffer postProcessFBO;

	const FrameBuffer* externalFBO;
	uint32_t numSamplesMSAA;
	float gammaFactor;
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

	// Initializes the graphics renderer.
	void Init(const WindowFrame& window);

	// Sets the external render target (aka framebuffer).
	// This allows for framebuffers, defined outside the renderer, to be rendered to by the renderer.
	void SetExternalRenderTarget(const FrameBuffer& fbo);

	// Sets the current render target (aka framebuffer), consequent render calls will only modify the contents of this render target.
	void SetRenderTarget(RenderTarget target) const;

	// Clears the screen and fills the screen with the specified color.
	void Clear(const glm::vec4& color) const;

	// Uses the vertex (and index) data attached via buffer objects to the vertex array object to render to the screen.
	// The last parameter 'count' specifies the number of vertices/indices in the data being rendered.
	void RenderData(const ShaderProgram& shader, const VertexArray& vao, uint32_t count) const;

	// Renders a colored rectangle of specified size to the position specified on the screen.
	void RenderRect(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos, 
		const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a colored triangle of specified size to the position specified on the screen.
	void RenderTriangle(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos,
		const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a textured rectangle of specified size to the position specified on the screen.
	void RenderTexturedRect(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture,
		const glm::vec2& pos, const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a textured triangle of specified size to the position specified on the screen. 
	void RenderTexturedTriangle(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture,
		const glm::vec2& pos, const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a colored text of specified size to the position specified on the screen.
	void RenderText(const ShaderProgram& shader, const Camera& sceneCamera, const Font& font, uint32_t fontSize,
		const std::string_view& text, const glm::vec4& color, const glm::vec2& pos, float rotationAngle = 0.0f) const;

	// Renders and displays the final rendered and post-processed scene.
	void FlushRenderedScene() const;

	// Returns singleton instance object of this class.
	static Renderer& GetInstance();
};

#endif 