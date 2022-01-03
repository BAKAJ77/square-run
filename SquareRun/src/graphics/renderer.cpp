#include <graphics/renderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

Renderer::Renderer()
{
	// Create and setup the VBO and VAO for rendering rectangles
	const std::vector<float> rectVertexData =
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,

		-0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f,  0.5f
	};

	this->rectVBO = VertexBuffer(rectVertexData, (uint32_t)rectVertexData.size() * sizeof(float), GL_STATIC_DRAW);

	this->rectVAO = VertexArray();
	this->rectVAO.PushVertexLayout<float>(0, 2, 2 * sizeof(float));
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

void Renderer::RenderRect(const ShaderProgram& shader, const Camera& sceneCamera, const glm::vec2& pos, const glm::vec2& size,
	float rotationAngle) const
{
	// Bind the shader and vao
	shader.BindProgram();
	this->rectVAO.BindObject();

	// Generate the model matrix and assign resulting matrix to shader uniform
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.SetUniformGLM("MVPMatrix", sceneCamera.GetMatrix() * modelMatrix);

	// Render the rectangle
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}
