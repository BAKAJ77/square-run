#include <graphics/vertex_array.h>
#include <glad/glad.h>

VertexArray::VertexArray() :
	vaoID(0), hasAttachedIBO(false)
{}

VertexArray::VertexArray(VertexArray&& temp) noexcept :
	vaoID(temp.vaoID), hasAttachedIBO(temp.hasAttachedIBO)
{
	temp.vaoID = 0;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &this->vaoID);
}

VertexArray& VertexArray::operator=(VertexArray&& temp) noexcept
{
	this->vaoID = temp.vaoID;
	this->hasAttachedIBO = temp.hasAttachedIBO;
	temp.vaoID = 0;

	return *this;
}

void VertexArray::InitVertexArray(const VertexBuffer& vbo, const IndexBuffer* ibo)
{
	// Generate the vao and bind it
	glGenVertexArrays(1, &this->vaoID);
	glBindVertexArray(this->vaoID);

	// Bind the vbo and ibo
	vbo.BindBuffer();
	if (ibo)
	{
		ibo->BindBuffer();
		this->hasAttachedIBO = true;
	}

	// Setup the vertex attributes based off the vertex layouts currently stored in the vector
	for (const auto& layout : this->vertexLayouts)
	{
		glEnableVertexAttribArray(layout.index);
		glVertexAttribPointer(layout.index, (int)layout.size, layout.type, layout.normalized, layout.stride, 
			(void*)((uint64_t)layout.offset));

		glVertexAttribDivisor(layout.index, layout.divisor);
	}

	this->vertexLayouts.clear();

	// Unbind the vbo, ibo and vao
	glBindVertexArray(0);

	vbo.UnbindBuffer();
	if (ibo)
		ibo->UnbindBuffer();
}

void VertexArray::BindObject() const
{
	glBindVertexArray(this->vaoID);
}

void VertexArray::UnbindObject() const
{
	glBindVertexArray(0);
}

const uint32_t& VertexArray::GetID() const
{
	return this->vaoID;
}

bool VertexArray::HasAttachedIBO() const
{
	return this->hasAttachedIBO;
}
