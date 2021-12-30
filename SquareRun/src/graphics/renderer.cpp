#include <graphics/renderer.h>
#include <glad/glad.h>

Renderer::Renderer() = default;

Renderer::~Renderer() = default;

void Renderer::Clear(const glm::vec4& color) const
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderData(const ShaderProgram& shader, const VertexArray& vao, uint32_t count) const
{
	shader.BindProgram();
	vao.BindObject();

	vao.HasAttachedIBO() ? glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr) :
		glDrawArrays(GL_TRIANGLES, 0, count);
}

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}
