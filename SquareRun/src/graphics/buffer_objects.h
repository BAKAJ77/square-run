#ifndef BUFFER_OBJECTS_H
#define BUFFER_OBJECTS_H

#include <string_view>
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

class TextureBuffer
{
private:
	uint32_t tboID, target, width, height;
	int numSamples;
public:
	TextureBuffer(uint32_t target, int level, int internalFormat, int width, int height, uint32_t format, uint32_t type,
		const void* pixelData, bool generateMipmap);
	TextureBuffer(uint32_t target, int numSamples, uint32_t internalFormat, int width, int height);
	~TextureBuffer();

	// Sets the wrapping mode used for the texture.
	void SetWrapMode(uint32_t sAxis, uint32_t tAxis);

	// Sets the filtering mode used for the texture.
	void SetFilterMode(uint32_t min, uint32_t mag);

	// Updates the contents of the texture buffer at the specified offset with the pixel data given.
	// Note that this method is only compatible with non-multisample texture buffers.
	void UpdateBuffer(int level, int offsetX, int offsetY, int width, int height, uint32_t format, uint32_t type,
		const void* pixelData);

	// Binds the texture buffer.
	void BindBuffer() const;

	// Binds the texture buffer.
	void BindBuffer(uint32_t textureIndex) const;

	// Unbinds the texture buffer.
	void UnbindBuffer() const;

	// Returns the ID of the texture buffer.
	const uint32_t& GetID() const;

	// Returns the target of the texture buffer.
	const uint32_t& GetTarget() const;

	// Returns the width of the texture buffer.
	const uint32_t& GetWidth() const;

	// Returns the height of the texture buffer.
	const uint32_t& GetHeight() const;

	// Returns the number of samples per pixel in the texture buffer.
	// Note that in the case of non-multisample texture buffers, -1 will be returned.
	const uint32_t& GetNumSamples() const;
};

using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
using TextureBufferPtr = std::shared_ptr<TextureBuffer>;

namespace Memory
{
	// Returns a shared pointer to the new created vertex buffer.
	extern VertexBufferPtr CreateVertexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage);

	// Returns a shared pointer to the new created index buffer.
	extern IndexBufferPtr CreateIndexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage);

	// Returns a shared pointer to the new created texture buffer.
	extern TextureBufferPtr CreateTextureBuffer(uint32_t target, int level, int internalFormat, int width, int height,
		uint32_t format, uint32_t type, const void* pixelData, bool generateMipmap = false);

	// Returns a shared pointer to the new created texture buffer.
	extern TextureBufferPtr CreateTextureBuffer(uint32_t target, int numSamples, uint32_t internalFormat, int width, int height);

	// Returns a shared pointer to the new created texture buffer filled with pixel loaded from the specified image file.
	extern TextureBufferPtr LoadTextureFromFile(const std::string_view& fileName, bool flipOnLoad = true);
}

#endif
