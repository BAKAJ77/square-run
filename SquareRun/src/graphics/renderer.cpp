#include <graphics/renderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <array>

Renderer::Renderer()
{
	// Enable blending mode for rendering partial/fully transparent objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	this->rectVBO = VertexBuffer(rectVertexData.data(), (uint32_t)rectVertexData.size() * sizeof(float), GL_STATIC_DRAW);
	this->rectVAO.PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	this->rectVAO.PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	this->rectVAO.InitVertexArray(this->rectVBO);

	// Create and setup the triangle VBO and VA0
	const std::vector<float> triangleVertexData =
	{
		// Vertex Coords    // UV Coords
		-0.5f,  0.5f,		0.0f, 0.0f,
		 0.5f,  0.5f,		1.0f, 0.0f,
		 0.0f, -0.5f,		0.5f, 1.0f
	};

	this->triangleVBO = VertexBuffer(triangleVertexData.data(), (uint32_t)triangleVertexData.size() * sizeof(float), GL_STATIC_DRAW);
	this->triangleVAO.PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	this->triangleVAO.PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	this->triangleVAO.InitVertexArray(this->triangleVBO);
}

Renderer::~Renderer() = default;

glm::mat4 Renderer::GenerateModelMatrix(const glm::vec2& pos, const glm::vec2& size, float rotationAngle) const
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

	return modelMatrix;
}

BatchedData Renderer::GenerateBatchedTextData(const Font& font, const std::string_view& text) const
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
		const GlyphData& glyph = font.GetGlyphs().at(character);

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
		topLeftUV.x /= (float)font.GetBitmap().GetWidth();
		topRightUV.x /= (float)font.GetBitmap().GetWidth();
		bottomLeftUV.x /= (float)font.GetBitmap().GetWidth();
		bottomRightUV.x /= (float)font.GetBitmap().GetWidth();

		topLeftUV.y /= (float)font.GetBitmap().GetHeight();
		topRightUV.y /= (float)font.GetBitmap().GetHeight();
		bottomLeftUV.y /= (float)font.GetBitmap().GetHeight();
		bottomRightUV.y /= (float)font.GetBitmap().GetHeight();

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

void Renderer::Clear(const glm::vec4& color) const
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderRect(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos,
	const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader and the rectangle vao
	shader.BindProgram();
	this->rectVAO.BindObject();

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	shader.SetUniform("enableTextureUsage", false);
	shader.SetUniformGLM("geometryColor", color / 255.0f);
	shader.SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	shader.SetUniformGLM("modelMatrix", modelMatrix);

	// Render the rectangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderTriangle(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos,
	const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader and the triangle vao
	shader.BindProgram();
	this->triangleVAO.BindObject();

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	shader.SetUniform("enableTextureUsage", false);
	shader.SetUniformGLM("geometryColor", color / 255.0f);
	shader.SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	shader.SetUniformGLM("modelMatrix", modelMatrix);

	// Render the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::RenderTexturedRect(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture, 
	const glm::vec2& pos, const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader, rectangle's VAO and texture
	shader.BindProgram();
	texture.BindBuffer(0);
	this->rectVAO.BindObject();

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	shader.SetUniform("geometryTexture", 0);
	shader.SetUniform("enableTextureUsage", true);
	shader.SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	shader.SetUniformGLM("modelMatrix", modelMatrix);

	// Render the textured rectangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderTexturedTriangle(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture,
	const glm::vec2& pos, const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader, triangle's VAO and texture
	shader.BindProgram();
	texture.BindBuffer(0);
	this->triangleVAO.BindObject();

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, size, rotationAngle);

	// Assign required shader uniform values
	shader.SetUniform("geometryTexture", 0);
	shader.SetUniform("enableTextureUsage", true);
	shader.SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	shader.SetUniformGLM("modelMatrix", modelMatrix);

	// Render the textured triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::RenderText(const ShaderProgram& shader, const Camera& sceneCamera, const Font& font, uint32_t fontSize,
	const std::string_view& text, const glm::vec4& color, const glm::vec2& pos, float rotationAngle) const
{
	// Generate the text's batched vertex and index data
	const BatchedData renderData = this->GenerateBatchedTextData(font, text);

	// Create and setup the text VBO, IBO and VAO
	VertexBuffer textVertexBuffer(renderData.first.data(), (uint32_t)renderData.first.size() * sizeof(float), GL_STATIC_DRAW);
	IndexBuffer textIndexBuffer(renderData.second.data(), (uint32_t)renderData.second.size() * sizeof(float), GL_STATIC_DRAW);
	
	VertexArray textVertexArray;
	textVertexArray.PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	textVertexArray.PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	textVertexArray.InitVertexArray(textVertexBuffer, &textIndexBuffer);

	// Bind the shader, text's VAO and font bitmap texture
	shader.BindProgram();
	font.GetBitmap().BindBuffer(0);
	textVertexArray.BindObject();

	// Generate the model matrix
	const glm::mat4 modelMatrix = this->GenerateModelMatrix(pos, glm::vec2((float)fontSize / (float)font.GetResolution()), 
		rotationAngle);

	// Assign required shader uniform values
	shader.SetUniform("fontBitmapTexture", 0);
	shader.SetUniformGLM("cameraMatrix", sceneCamera.GetMatrix());
	shader.SetUniformGLM("modelMatrix", modelMatrix);
	shader.SetUniformGLM("textColor", color);

	// Render the text
	glDrawElements(GL_TRIANGLES, (uint32_t)renderData.second.size(), GL_UNSIGNED_INT, nullptr);
}

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}
