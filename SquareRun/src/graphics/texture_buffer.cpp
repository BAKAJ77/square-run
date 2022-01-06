#include <graphics/texture_buffer.h>
#include <util/directory_system.h>
#include <util/logging_system.h>

#include <glad/glad.h>
#include <stb_image.h>

TextureBuffer::TextureBuffer() :
	tboID(0), width(0), height(0), target(0)
{}

TextureBuffer::TextureBuffer(uint32_t target, int internalFormat, uint32_t width, uint32_t height, uint32_t format, uint32_t type, 
	const void* pixelData) :
	width(width), height(height), target(target)
{
	// Generate a new texture buffer object and bind it
	glGenTextures(1, &this->tboID);
	glBindTexture(target, this->tboID);

	// Set texture wrap and filter configs then fill the buffer with the provided pixel data
	this->SetDefaultTextureConfigs();
	glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, pixelData);
	glBindTexture(target, 0);
}

TextureBuffer::TextureBuffer(uint32_t target, uint32_t numSamples, uint32_t internalFormat, uint32_t width, uint32_t height)
{
	// Generate a new texture buffer object and bind it
	glGenTextures(1, &this->tboID);
	glBindTexture(target, this->tboID);

	// Set texture wrap and filter configs then fill the buffer with the provided pixel data
	this->SetDefaultTextureConfigs();
	glTexImage2DMultisample(target, numSamples, internalFormat, width, height, true);
	glBindTexture(target, 0);
}

TextureBuffer::TextureBuffer(TextureBuffer&& temp) noexcept :
	tboID(temp.tboID), target(temp.target), width(temp.width), height(temp.height)
{
	temp.tboID = 0;
}

TextureBuffer::~TextureBuffer()
{
	glDeleteTextures(1, &this->tboID);
}

TextureBuffer& TextureBuffer::operator=(TextureBuffer&& temp) noexcept
{
	this->tboID = temp.tboID;
	this->width = temp.width;
	this->height = temp.height;
	this->target = temp.target;

	temp.tboID = 0;
	return *this;
}

void TextureBuffer::SetDefaultTextureConfigs() const
{
	glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void TextureBuffer::SetWrappingMode(uint32_t sAxis, uint32_t tAxis)
{
	glBindTexture(this->target, this->tboID);
	glTexParameteri(this->target, GL_TEXTURE_WRAP_S, sAxis);
	glTexParameteri(this->target, GL_TEXTURE_WRAP_T, tAxis);
	glBindTexture(this->target, 0);
}

void TextureBuffer::SetFilteringMode(uint32_t minFilter, uint32_t magFilter)
{
	glBindTexture(this->target, this->tboID);
	glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, magFilter);
	glBindTexture(this->target, 0);
}

void TextureBuffer::UpdateBuffer(uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height, uint32_t format,
	uint32_t type, const void* pixelData)
{
	glBindTexture(this->target, this->tboID);
	glTexSubImage2D(this->target, 0, offsetX, offsetY, width, height, format, type, pixelData);
	glBindTexture(this->target, 0);
}

void TextureBuffer::BindBuffer() const
{
	glBindTexture(this->target, this->tboID);
}

void TextureBuffer::BindBuffer(int textureIndex) const
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

namespace Graphics
{
	extern TextureBuffer LoadTextureFromFile(const std::string_view& fileName, bool flipTextureOnLoad)
	{
		int width = 0, height = 0, channels = 0;
		uint32_t textureFormat = 0;
		
		// Construct the full path to the texture file
		std::string filePath = Util::GetGameRequisitesDirectory() + "assets/" + fileName.data();

		// Load the texture image pixel data
		stbi_set_flip_vertically_on_load(flipTextureOnLoad);
		const uint8_t* pixelData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		if (!pixelData)
			LogSystem::GetInstance().OutputLog("Failed to load texture from path: " + filePath, Severity::WARNING);

		// Deduce the texture format
		if (channels > 3)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_RGB;

		return TextureBuffer(GL_TEXTURE_2D, textureFormat, width, height, textureFormat, GL_UNSIGNED_BYTE, pixelData);
	}
}
