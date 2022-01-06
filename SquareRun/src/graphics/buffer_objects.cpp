#include <graphics/buffer_objects.h>
#include <glad/glad.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer() :
	vboID(0)
{}

VertexBuffer::VertexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t bufferUsage)
{
	glGenBuffers(1, &this->vboID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
	glBufferData(GL_ARRAY_BUFFER, bufferAllocSize, data, bufferUsage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::VertexBuffer(VertexBuffer&& temp) noexcept :
	vboID(temp.vboID)
{
	temp.vboID = 0;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &this->vboID);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& temp) noexcept
{
	this->vboID = temp.vboID;
	temp.vboID = 0;
	return *this;
}

void VertexBuffer::UpdateBuffer(const void* data, uint32_t dataSize, uint32_t bufferOffset)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, dataSize, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BindBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
}

void VertexBuffer::UnbindBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const uint32_t& VertexBuffer::GetID() const
{
	return this->vboID;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer() :
	iboID(0)
{}

IndexBuffer::IndexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t bufferUsage)
{
	glGenBuffers(1, &this->iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferAllocSize, data, bufferUsage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(IndexBuffer&& temp) noexcept :
	iboID(temp.iboID)
{
	temp.iboID = 0;
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &this->iboID);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& temp) noexcept
{
	this->iboID = temp.iboID;
	temp.iboID = 0;
	return *this;
}

void IndexBuffer::UpdateBuffer(const void* data, uint32_t dataSize, uint32_t bufferOffset)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, bufferOffset, dataSize, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::BindBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboID);
}

void IndexBuffer::UnbindBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

const uint32_t& IndexBuffer::GetID() const
{
	return this->iboID;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////