#include <graphics/renderer.h>
#include <serialization/config.h>
#include <util/logging_system.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() :
	gammaFactor(0.0)
{}

void Renderer::Init(WindowFramePtr window)
{
	this->window = window;

	// Enable blending mode for rendering partial/fully transparent objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load the renderer shaders
	this->geometryShader = Memory::CreateShaderProgram("geometry.glsl.vsh", "geometry.glsl.fsh");
	this->textShader = Memory::CreateShaderProgram("text.glsl.vsh", "text.glsl.fsh");
	this->postProcessShader = Memory::CreateShaderProgram("post_process.glsl.vsh", "post_process.glsl.fsh");

	// Create and setup the rectangle VBO and VAO
	const std::vector<float> rectVertexData =
	{
		// Vertex Coords    // UV Coords
		-0.5f, -0.5f,		0.0f, 0.0f,
		 0.5f, -0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f,		1.0f, 1.0f,

		-0.5f, -0.5f,		0.0f, 0.0f,
		-0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f,  0.5f,		1.0f, 1.0f
	};

	this->rectangleVBO = Memory::CreateVertexBuffer(rectVertexData.data(), (uint32_t)rectVertexData.size() * sizeof(float), 
		GL_STATIC_DRAW);

	this->rectangleVAO = Memory::CreateVertexArray();
	this->rectangleVAO->PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	this->rectangleVAO->PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	this->rectangleVAO->AttachBuffers(this->rectangleVBO);

	// Create and setup the triangle VBO and VA0
	const std::vector<float> triangleVertexData =
	{
		// Vertex Coords    // UV Coords
		-0.5f,  0.5f,		0.0f, 0.0f,
		 0.5f,  0.5f,		1.0f, 0.0f,
		 0.0f, -0.5f,		0.5f, 1.0f
	};

	this->triangleVBO = Memory::CreateVertexBuffer(triangleVertexData.data(), (uint32_t)triangleVertexData.size() * sizeof(float), 
		GL_STATIC_DRAW);

	this->triangleVAO = Memory::CreateVertexArray();
	this->triangleVAO->PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	this->triangleVAO->PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	this->triangleVAO->AttachBuffers(this->triangleVBO);

	// Create and setup the post-processing requisites (the framebuffer, output texture and shader program)
	const int numSamplesMSAA = std::max(Serialization::GetConfigElement<int>("graphics", "numSamplesMSAA"), 2);

	this->gammaFactor = Serialization::GetConfigElement<float>("graphics", "gamma");
	const std::vector<int> resolution = Serialization::GetConfigElement<std::vector<int>>("graphics", "resolution");
	if (resolution.size() < 2)
		LogSystem::GetInstance().OutputLog("The json setting 'resolution' is invalid", Severity::FATAL);

	this->postProcessTexture = Memory::CreateTextureBuffer(GL_TEXTURE_2D_MULTISAMPLE, numSamplesMSAA, GL_SRGB,
		resolution[0], resolution[1]);
	
	this->postProcessFBO = Memory::CreateFrameBuffer();
	this->postProcessFBO->AttachTextureBuffer(GL_COLOR_ATTACHMENT0, this->postProcessTexture);
}

glm::mat4 Renderer::GenerateModelMatrix(const glm::vec2& pos, const glm::vec2& size, float rotationAngle) const
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));

	return modelMatrix;
}

BatchedData Renderer::GenerateBatchedTextData(const FontPtr font, const std::string_view& text) const
{
	std::vector<float> vertexData;
	std::vector<uint32_t> indexData;

	glm::vec2 originPosition;
	uint32_t indexingOffset = 0;
	bool firstCharacter = false;

	// Allocate enough memory for each vector to store all the vertex and index data
	vertexData.reserve(text.size() * 16);
	indexData.reserve(text.size() * 6);

	for (const char& character : text)
	{
		// Generate the vertex coords
		const GlyphData& glyph = font->GetGlyphs().at(character);

		glm::vec2 topLeftVertex;
		firstCharacter ?
			topLeftVertex = { originPosition.x, originPosition.y - glyph.bearing.y } :
			topLeftVertex = { originPosition.x + glyph.bearing.x, originPosition.y - glyph.bearing.y };

		glm::vec2 topRightVertex = { topLeftVertex.x + glyph.size.x, topLeftVertex.y };
		glm::vec2 bottomLeftVertex = { topLeftVertex.x, topLeftVertex.y + glyph.size.y };
		glm::vec2 bottomRightVertex = { topRightVertex.x, bottomLeftVertex.y };

		// Generate the UV coords
		glm::vec2 topLeftUV = { glyph.textureOffsetX, 0.0f };
		glm::vec2 topRightUV = { glyph.textureOffsetX + glyph.size.x, 0.0f };
		glm::vec2 bottomLeftUV = { topLeftUV.x, glyph.size.y };
		glm::vec2 bottomRightUV = { topRightUV.x, glyph.size.y };

		// Normalize the UV coords, they must be within the bounds of -1.0f to 1.0f.
		topLeftUV.x /= (float)font->GetBitmap()->GetWidth();
		topRightUV.x /= (float)font->GetBitmap()->GetWidth();
		bottomLeftUV.x /= (float)font->GetBitmap()->GetWidth();
		bottomRightUV.x /= (float)font->GetBitmap()->GetWidth();

		topLeftUV.y /= (float)font->GetBitmap()->GetHeight();
		topRightUV.y /= (float)font->GetBitmap()->GetHeight();
		bottomLeftUV.y /= (float)font->GetBitmap()->GetHeight();
		bottomRightUV.y /= (float)font->GetBitmap()->GetHeight();

		// Push the generated vertex data into the vector array
		std::array<float, 16> generatedVertexData =
		{
			topLeftVertex.x, topLeftVertex.y, topLeftUV.x, topLeftUV.y,
			topRightVertex.x, topRightVertex.y, topRightUV.x, topRightUV.y,
			bottomLeftVertex.x, bottomLeftVertex.y, bottomLeftUV.x, bottomLeftUV.y,
			bottomRightVertex.x, bottomRightVertex.y, bottomRightUV.x, bottomRightUV.y
		};

		vertexData.insert(vertexData.end(), generatedVertexData.begin(), generatedVertexData.end());

		// Push the generated index data into the vector array
		std::array<uint32_t, 6> generatedIndexData =
		{
			0 + indexingOffset, 2 + indexingOffset, 3 + indexingOffset,
			0 + indexingOffset, 3 + indexingOffset, 1 + indexingOffset
		};

		indexData.insert(indexData.end(), generatedIndexData.begin(), generatedIndexData.end());

		// Update the necessary counters
		if (firstCharacter)
		{
			originPosition.x += glyph.advanceX - glyph.bearing.x;
			firstCharacter = false;
		}
		else
			originPosition.x += glyph.advanceX;

		indexingOffset += 4;
	}

	return { vertexData, indexData };
}

void Renderer::SetExternalRenderTarget(FrameBufferPtr fbo)
{
	this->externalFBO = fbo;
}

void Renderer::SetRenderTarget(RenderTarget target) const
{
	switch (target)
	{
	case RenderTarget::DEFAULT_FRAMEBUFFER:
		this->postProcessFBO->UnbindBuffer();
		glViewport(0, 0, this->window->GetWidth(), this->window->GetHeight());
		break;
	case RenderTarget::SCENE_FRAMEBUFFER:
		this->postProcessFBO->BindBuffer();
		glViewport(0, 0, this->postProcessTexture->GetWidth(), this->postProcessTexture->GetHeight());
		break;
	case RenderTarget::EXTERNAL_FRAMEBUFFER:
		if (this->externalFBO)
			this->externalFBO->BindBuffer();
		break;
	}
}

void Renderer::Clear(const glm::vec4& color) const
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderRect(const OrthogonalCamera& sceneCamera, const glm::vec4& color, const glm::vec2& pos, 
	const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader and the rectangle vao
	this->geometryShader->BindProgram();
	this->rectangleVAO->BindObject();

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	this->geometryShader->SetUniform("material.useTexture", false);
	this->geometryShader->SetUniformGLM("material.color", color / 255.0f);
	this->geometryShader->SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	this->geometryShader->SetUniformGLM("modelMatrix", modelMatrix);

	// Render the rectangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderTriangle(const OrthogonalCamera& sceneCamera, const glm::vec4& color, const glm::vec2& pos, 
	const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader and the triangle vao
	this->geometryShader->BindProgram();
	this->triangleVAO->BindObject();

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	this->geometryShader->SetUniform("material.useTexture", false);
	this->geometryShader->SetUniformGLM("material.color", color / 255.0f);
	this->geometryShader->SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	this->geometryShader->SetUniformGLM("modelMatrix", modelMatrix);

	// Render the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::RenderTexturedRect(const OrthogonalCamera& sceneCamera, const TextureBufferPtr texture, const glm::vec2& pos,
	const glm::vec2& size, float rotationAngle, const glm::vec4& colorMod) const
{
	// Bind the shader, rectangle's VAO and texture
	this->geometryShader->BindProgram();
	this->rectangleVAO->BindObject();
	texture->BindBuffer(0);

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	this->geometryShader->SetUniform("material.texture", 0);
	this->geometryShader->SetUniform("material.useTexture", true);
	this->geometryShader->SetUniformGLM("material.color", colorMod / 255.0f);
	this->geometryShader->SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	this->geometryShader->SetUniformGLM("modelMatrix", modelMatrix);

	// Render the textured rectangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderTexturedTriangle(const OrthogonalCamera& sceneCamera, const TextureBufferPtr texture, const glm::vec2& pos, 
	const glm::vec2& size, float rotationAngle, const glm::vec4& colorMod) const
{
	// Bind the shader, triangle's VAO and texture
	this->geometryShader->BindProgram();
	this->triangleVAO->BindObject();
	texture->BindBuffer(0);

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	this->geometryShader->SetUniform("material.texture", 0);
	this->geometryShader->SetUniform("material.useTexture", true);
	this->geometryShader->SetUniformGLM("material.color", colorMod / 255.0f);
	this->geometryShader->SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	this->geometryShader->SetUniformGLM("modelMatrix", modelMatrix);

	// Render the textured triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::RenderText(const OrthogonalCamera& sceneCamera, const FontPtr font, uint32_t fontSize, const std::string_view& text, 
	const glm::vec4& color, const glm::vec2& pos, float rotationAngle) const
{
	// Generate the text's batched vertex and index data
	const BatchedData renderData = this->GenerateBatchedTextData(font, text);

	// Create and setup the text VBO, IBO and VAO
	VertexBufferPtr textVertexBuffer = Memory::CreateVertexBuffer(renderData.first.data(), 
		(uint32_t)renderData.first.size() * sizeof(float), GL_STATIC_DRAW);

	IndexBufferPtr textIndexBuffer = Memory::CreateIndexBuffer(renderData.second.data(), 
		(uint32_t)renderData.second.size() * sizeof(float), GL_STATIC_DRAW);

	VertexArrayPtr textVertexArray = Memory::CreateVertexArray();
	textVertexArray->PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	textVertexArray->PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	textVertexArray->AttachBuffers(textVertexBuffer, textIndexBuffer);

	// Bind the shader, text's VAO and font bitmap texture
	this->textShader->BindProgram();
	textVertexArray->BindObject();
	font->GetBitmap()->BindBuffer(0);

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, glm::vec2((float)fontSize / (float)font->GetResolution()),
		rotationAngle);

	// Assign required shader uniform values
	this->textShader->SetUniform("fontBitmapTexture", 0);
	this->textShader->SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	this->textShader->SetUniformGLM("modelMatrix", modelMatrix);
	this->textShader->SetUniformGLM("textColor", color);

	// Render the text
	glDrawElements(GL_TRIANGLES, (uint32_t)renderData.second.size(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::FlushRenderedScene() const
{
	// Bind the default framebuffer and clear it
	this->SetRenderTarget(RenderTarget::DEFAULT_FRAMEBUFFER);
	Renderer::GetInstance().Clear({ 0, 0, 0, 255 });

	// Bind the post-process shader, texture and rectangle VAO
	this->postProcessShader->BindProgram();
	this->postProcessTexture->BindBuffer(0);
	this->rectangleVAO->BindObject();

	// Assign required shader uniform values
	this->postProcessShader->SetUniform("renderedTexture", 0);
	this->postProcessShader->SetUniform("gammaFactor", this->gammaFactor);
	this->postProcessShader->SetUniform("numSamples", this->postProcessTexture->GetNumSamples());

	this->postProcessShader->SetUniformGLM("framebufferSize", { this->postProcessTexture->GetWidth(),
		this->postProcessTexture->GetHeight() });

	// Render the processed texture
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// Returns singleton instance object of this class.
Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}