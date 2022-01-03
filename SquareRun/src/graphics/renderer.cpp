#include <graphics/renderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

Renderer::Renderer()
{
	// Enable blending mode for rendering partial/fully transparent objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create and setup the VBO and VAO for rendering rectangles
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

	this->rectVBO = VertexBuffer(rectVertexData, (uint32_t)rectVertexData.size() * sizeof(float), GL_STATIC_DRAW);

	this->rectVAO = VertexArray();
	this->rectVAO.PushVertexLayout<float>(0, 2, 4 * sizeof(float));
	this->rectVAO.PushVertexLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	this->rectVAO.InitVertexArray(this->rectVBO);
}

Renderer::~Renderer() = default;

void Renderer::Clear(const glm::vec4& color) const
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderData(const ShaderProgram& shader, const VertexArray& vao, uint32_t count) const
{
	// Bind the shader and vao
	shader.BindProgram();
	vao.BindObject();

	// Render the object described by the vertex (and index) data
	vao.HasAttachedIBO() ? glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr) :
		glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderer::RenderRect(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec4& color, const glm::vec2& pos, 
	const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader and vao
	shader.BindProgram();
	this->rectVAO.BindObject();

	// Generate the model matrix
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

	// Assign required shader uniform values
	shader.SetUniform("enableTextureUsage", false);
	shader.SetUniformGLM("mvpMatrix", sceneCamera.GetMatrix() * modelMatrix);
	shader.SetUniformGLM("geometryColor", color / 255.0f);

	// Render the rectangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderTexturedRect(const ShaderProgram& shader, const Camera& sceneCamera, const TextureBuffer& texture, 
	const glm::vec2& pos, const glm::vec2& size, float rotationAngle) const
{
	// Bind the shader, VAO and texture
	shader.BindProgram();
	texture.BindBuffer(0);
	this->rectVAO.BindObject();

	// Generate the model matrix and assign resulting matrix to shader uniform
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

	// Assign required shader uniform values
	shader.SetUniform("geometryTexture", 0);
	shader.SetUniform("enableTextureUsage", true);
	shader.SetUniformGLM("mvpMatrix", sceneCamera.GetMatrix() * modelMatrix);

	// Render the textured rectangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}
