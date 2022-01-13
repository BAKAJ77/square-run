#ifndef BUFFER_OBJECTS_H
#define BUFFER_OBJECTS_H

#include <memory>

class VertexBuffer
{
private:
	uint32_t vboID;
public:
	VertexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage);
	~VertexBuffer();

	// Inserts data given into the buffer at the specified offset.
	void UpdateBuffer(const void* data, uint32_t dataSize, uint32_t bufferOffset);

	// Binds the vertex buffer.
	void BindBuffer() const;

	// Unbinds the vertex buffer.
	void UnbindBuffer() const;

	// Returns the ID of the vertex buffer.
	const uint32_t& GetID() const;
};

class IndexBuffer
{
private:
	uint32_t iboID;
public:
	IndexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage);
	~IndexBuffer();

	// Inserts data given into the buffer at the specified offset.
	void UpdateBuffer(const void* data, uint32_t dataSize, uint32_t bufferOffset);

	// Binds the index buffer.
	void BindBuffer() const;

	// Unbinds the index buffer.
	void UnbindBuffer() const;

	// Returns the ID of the index buffer.
	const uint32_t& GetID() const;
};

using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;

namespace Memory
{
	// Returns a shared pointer to the new created vertex buffer.
	extern VertexBufferPtr CreateVertexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage);

	// Returns a shared pointer to the new created index buffer.
	extern IndexBufferPtr CreateIndexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage);
}

#endif
