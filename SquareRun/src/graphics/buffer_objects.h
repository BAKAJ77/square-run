#ifndef BUFFER_OBJECTS_H
#define BUFFER_OBJECTS_H

#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VertexBuffer
{
private:
	uint32_t vboID;
public:
	VertexBuffer();
	VertexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t bufferUsage);
	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer(VertexBuffer&& temp) noexcept;

	~VertexBuffer();

	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(VertexBuffer&& temp) noexcept;

	// Inserts new data into the vertex buffer at the buffer offset specified.
	void UpdateBuffer(const void* data, uint32_t dataSize, uint32_t bufferOffset);

	// Binds the vertex buffer.
	void BindBuffer() const;

	// Unbinds the vertex buffer.
	void UnbindBuffer() const;

	// Returns the ID of the vertex buffer.
	const uint32_t& GetID() const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class IndexBuffer
{
private:
	uint32_t iboID;
public:
	IndexBuffer();
	IndexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t bufferUsage);
	IndexBuffer(const IndexBuffer& other) = delete;
	IndexBuffer(IndexBuffer&& temp) noexcept;

	~IndexBuffer();

	IndexBuffer& operator=(const IndexBuffer& other) = delete;
	IndexBuffer& operator=(IndexBuffer&& temp) noexcept;

	// Inserts new data into the index buffer at the buffer offset specified.
	void UpdateBuffer(const void* data, uint32_t dataSize, uint32_t bufferOffset);

	// Binds the index buffer.
	void BindBuffer() const;

	// Unbinds the index buffer.
	void UnbindBuffer() const;

	// Returns the ID of the index buffer.
	const uint32_t& GetID() const;
};


#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TextureBuffer;

class FrameBuffer
{
private:
	uint32_t fboID;
public:
	FrameBuffer();
	FrameBuffer(const FrameBuffer& other) = delete;
	FrameBuffer(FrameBuffer&& temp) noexcept;

	~FrameBuffer();

	FrameBuffer& operator=(const FrameBuffer& other) = delete;
	FrameBuffer& operator=(FrameBuffer&& temp) noexcept;

	void GenerateFrameBuffer();
	void AttachTextureBuffer(uint32_t attachment, const TextureBuffer& texture);
	void BindBuffer() const;
	void UnbindBuffer() const;

	const uint32_t& GetID() const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////