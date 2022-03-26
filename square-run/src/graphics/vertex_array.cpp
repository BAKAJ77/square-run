#include <graphics/vertex_array.h>

#include <glad/glad.h>
#include <cassert>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &this->vaoID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &this->vaoID);
}

void VertexArray::AttachBuffers(const VertexBufferPtr vbo, const IndexBufferPtr ibo)
{
	// A valid vertex buffer must be given
	assert(vbo != nullptr);

	// Bind the VAO and buffers
	glBindVertexArray(this->vaoID);

	vbo->BindBuffer();
	if (ibo)
		ibo->BindBuffer();

	// Set the vertex array attribute pointers, then clear the vertex layouts vector array
	for (const VertexLayout& layout : this->vertexLayouts)
	{
		glEnableVertexAttribArray(layout.index);
		glVertexAttribPointer(layout.index, layout.size, layout.type, layout.normalized, layout.stride, 
			(void*)((uint64_t)layout.offset));
		glVertexAttribDivisor(layout.index, layout.divisor);
	}

	this->vertexLayouts.clear();

	// Unbind the buffers and VAO
	glBindVertexArray(0);

	vbo->UnbindBuffer();
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

VertexArrayPtr Memory::CreateVertexArray()
{
	return std::make_shared<VertexArray>();
}