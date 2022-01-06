#include <graphics/buffer_objects.h>
#include <graphics/texture_buffer.h>
#include <util/logging_system.h>

#include <glad/glad.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FrameBuffer::FrameBuffer() :
	fboID(0)
{}

FrameBuffer::FrameBuffer(FrameBuffer && temp) noexcept :
	fboID(temp.fboID)
{
	temp.fboID = 0;
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &this->fboID);
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& temp) noexcept
{
	this->fboID = temp.fboID;
	temp.fboID = 0;
	return *this;
}

void FrameBuffer::GenerateFrameBuffer()
{
	glGenFramebuffers(1, &this->fboID);
}

void FrameBuffer::AttachTextureBuffer(uint32_t attachment, const TextureBuffer& texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture.GetTarget(), texture.GetID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);
}

void FrameBuffer::UnbindBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef _DEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LogSystem::GetInstance().OutputLog("The framebuffer bound is not complete", Severity::WARNING);
#endif
}

const uint32_t& FrameBuffer::GetID() const
{
	return this->fboID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////