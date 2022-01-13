#include <graphics/buffer_objects.h>
#include <util/directory_system.h>
#include <util/logging_system.h>

#include <stb_image.h>
#include <glad/glad.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage)
{
	glGenBuffers(1, &this->vboID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
	glBufferData(GL_ARRAY_BUFFER, bufferAllocSize, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &this->vboID);
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

IndexBuffer::IndexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage)
{
	glGenBuffers(1, &this->iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferAllocSize, data, usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &this->iboID);
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

TextureBuffer::TextureBuffer(uint32_t target, int level, int internalFormat, int width, int height, uint32_t format, uint32_t type, 
	const void* pixelData, bool generateMipmap) :
	target(target), width(width), height(height), numSamples(-1)
{
	// Generate and bind the texture buffer
	glGenTextures(1, &this->tboID);
	glBindTexture(target, this->tboID);

	// Set the wrap and filter modes
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Allocate the texture buffer, and generate mipmap if specified to do so
	glTexImage2D(target, level, internalFormat, width, height, 0, format, type, pixelData);
	if (generateMipmap)
		glGenerateMipmap(target);

	// Unbind the texture buffer
	glBindTexture(target, 0);
}

TextureBuffer::TextureBuffer(uint32_t target, int numSamples, uint32_t internalFormat, int width, int height) :
	target(target), numSamples(numSamples), width(width), height(height)
{
	// Generate and bind the texture buffer
	glGenTextures(1, &this->tboID);
	glBindTexture(target, this->tboID);

	// Set the wrap and filter modes
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Allocate the multisample texture buffer
	glTexImage2DMultisample(target, numSamples, internalFormat, width, height, true);

	// Unbind the texture buffer
	glBindTexture(target, 0);
}

TextureBuffer::~TextureBuffer()
{
	glDeleteTextures(1, &this->tboID);
}

void TextureBuffer::SetWrapMode(uint32_t sAxis, uint32_t tAxis)
{
	glBindTexture(this->target, this->tboID);
	glTexParameteri(this->target, GL_TEXTURE_WRAP_S, sAxis);
	glTexParameteri(this->target, GL_TEXTURE_WRAP_T, tAxis);
	glBindTexture(this->target, 0);
}

void TextureBuffer::SetFilterMode(uint32_t min, uint32_t mag)
{
	glBindTexture(this->target, this->tboID);
	glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, mag);
	glBindTexture(this->target, 0);
}

void TextureBuffer::UpdateBuffer(int level, int offsetX, int offsetY, int width, int height, uint32_t format, uint32_t type, 
	const void* pixelData)
{
	glBindTexture(this->target, this->tboID);
	glTexSubImage2D(this->target, level, offsetX, offsetY, width, height, format, type, pixelData);
	glBindTexture(this->target, 0);
}

void TextureBuffer::BindBuffer() const
{
	glBindTexture(this->target, this->tboID);
}

void TextureBuffer::BindBuffer(uint32_t textureIndex) const
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(this->target, this->tboID);
}

void TextureBuffer::UnbindBuffer() const
{
	glBindTexture(this->target, 0);
}

const uint32_t& TextureBuffer::GetID() const
{
	return this->tboID;
}

const uint32_t& TextureBuffer::GetTarget() const
{
	return this->target;
}

const uint32_t& TextureBuffer::GetWidth() const
{
	return this->width;
}

const uint32_t& TextureBuffer::GetHeight() const
{
	return this->height;
}

const uint32_t& TextureBuffer::GetNumSamples() const
{
	return this->numSamples;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VertexBufferPtr Memory::CreateVertexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage)
{
	return std::make_shared<VertexBuffer>(data, bufferAllocSize, usage);
}

IndexBufferPtr Memory::CreateIndexBuffer(const void* data, uint32_t bufferAllocSize, uint32_t usage)
{
	return std::make_shared<IndexBuffer>(data, bufferAllocSize, usage);
}

TextureBufferPtr Memory::CreateTextureBuffer(uint32_t target, int level, int internalFormat, int width, int height, uint32_t format, 
	uint32_t type, const void* pixelData, bool generateMipmap)
{
	return std::make_shared<TextureBuffer>(target, level, internalFormat, width, height, format, type, pixelData, generateMipmap);
}

TextureBufferPtr Memory::CreateTextureBuffer(uint32_t target, int numSamples, uint32_t internalFormat, int width, int height)
{
	return std::make_shared<TextureBuffer>(target, numSamples, internalFormat, width, height);
}

TextureBufferPtr Memory::LoadTextureFromFile(const std::string_view& fileName, bool flipOnLoad)
{
	int width = 0, height = 0, channels = 0;
	uint32_t textureFormat = 0;

	// Construct the full path to the texture file
	std::string filePath = Util::GetGameRequisitesDirectory() + "assets/" + fileName.data();

	// Load the texture image pixel data
	stbi_set_flip_vertically_on_load(flipOnLoad);
	const uint8_t* pixelData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	if (!pixelData)
		LogSystem::GetInstance().OutputLog("Failed to load texture from path: " + filePath, Severity::WARNING);

	// Deduce the texture format
	if (channels > 3)
		textureFormat = GL_RGBA;
	else
		textureFormat = GL_RGB;

	return Memory::CreateTextureBuffer(GL_TEXTURE_2D, 0, textureFormat, width, height, textureFormat, GL_UNSIGNED_BYTE, pixelData,
		true);
}
