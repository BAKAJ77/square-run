#ifndef RENDERER_H
#define RENDERER_H

#include <core/window_frame.h>
#include <graphics/shader_program.h>
#include <graphics/vertex_array.h>
#include <graphics/orthogonal_camera.h>
#include <graphics/ttf_font_loader.h>

#include <glm/glm.hpp>
#include <vector>

using BatchedData = std::pair<std::vector<float>, std::vector<uint32_t>>;

namespace RenderingGlobals
{
	constexpr int sceneViewWidth = 1920;
	constexpr int sceneViewHeight = 1080;
}

enum class RenderTarget
{
	DEFAULT_FRAMEBUFFER, // This is what is ultimately displayed to the screen, the post-processed scene texture is rendered here
	SCENE_FRAMEBUFFER, // This is where the scene is rendered to, contains the texture of the rendered scene
	EXTERNAL_FRAMEBUFFER // A framebuffer that's defined outside of the renderer
};

class Renderer
{
private:
	WindowFramePtr window;
	ShaderProgramPtr geometryShader, textShader, postProcessShader;
	VertexBufferPtr rectangleVBO, triangleVBO;
	VertexArrayPtr rectangleVAO, triangleVAO;

	FrameBufferPtr postProcessFBO, externalFBO;
	TextureBufferPtr postProcessTexture;
	float gammaFactor;
private:
	// Returns a generated model matrix.
	glm::mat4 GenerateModelMatrix(const glm::vec2& pos, const glm::vec2& size, float rotationAngle) const;

	// Returns a pair of vectors, one containing vertex data and the other containing index data.
	// The vertex and index data inside the vectors are the result of all the glyphs in the text given having their
	// vertex and index data all batched into their respective vector containers.
	BatchedData GenerateBatchedTextData(const FontPtr font, const std::string_view& text) const;
private:
	Renderer();
public:
	Renderer(const Renderer& other) = delete;
	Renderer(Renderer&& temp) = delete;
	~Renderer() = default;

	Renderer& operator=(const Renderer& other) = delete;
	Renderer& operator=(Renderer&& temp) = delete;

	// Initializes the graphics renderer.
	void Init(WindowFramePtr window);

	// Sets the external render target (aka framebuffer).
	// This allows for framebuffers, defined outside the renderer, to be rendered to by the renderer.
	void SetExternalRenderTarget(FrameBufferPtr fbo);

	// Sets the current render target (aka framebuffer), consequent render calls will only modify the contents of this render target.
	void SetRenderTarget(RenderTarget target) const;

	// Clears the screen and fills the screen with the specified color.
	void Clear(const glm::vec4& color) const;

	// Renders a colored rectangle of specified size to the position specified on the screen.
	void RenderRect(const OrthogonalCamera& sceneCamera, const glm::vec4& color, const glm::vec2& pos,
		const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a colored triangle of specified size to the position specified on the screen.
	void RenderTriangle(const OrthogonalCamera& sceneCamera, const glm::vec4& color, const glm::vec2& pos,
		const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a textured rectangle of specified size to the position specified on the screen.
	void RenderTexturedRect(const OrthogonalCamera& sceneCamera, const TextureBufferPtr texture,
		const glm::vec2& pos, const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a textured triangle of specified size to the position specified on the screen. 
	void RenderTexturedTriangle(const OrthogonalCamera& sceneCamera, const TextureBufferPtr texture,
		const glm::vec2& pos, const glm::vec2& size, float rotationAngle = 0.0f) const;

	// Renders a colored text of specified size to the position specified on the screen.
	void RenderText(const OrthogonalCamera& sceneCamera, const FontPtr font, uint32_t fontSize,
		const std::string_view& text, const glm::vec4& color, const glm::vec2& pos, float rotationAngle = 0.0f) const;

	// Renders and displays the final rendered and post-processed scene.
	void FlushRenderedScene() const;

	// Returns singleton instance object of this class.
	static Renderer& GetInstance();
};

#endif
