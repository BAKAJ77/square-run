#ifndef TEXTURE_BUFFER_H
#define TEXTURE_BUFFER_H

#include <string_view>
#include <vector>

class TextureBuffer
{
private:
	uint32_t tboID, width, height, target;
private:
	// Sets the default configurations of which filtering and wrapping technique to use in regards to this texture.
	void SetDefaultTextureConfigs() const;
public:
	TextureBuffer();
	TextureBuffer(uint32_t target, int internalFormat, uint32_t width, uint32_t height, uint32_t format, uint32_t type,
		const void* pixelData);
	TextureBuffer(uint32_t target, uint32_t numSamples, uint32_t internalFormat, uint32_t width, uint32_t height);
	TextureBuffer(const TextureBuffer& other) = delete;
	TextureBuffer(TextureBuffer&& temp) noexcept;

	~TextureBuffer();

	TextureBuffer& operator=(const TextureBuffer& other) = delete;
	TextureBuffer& operator=(TextureBuffer&& temp) noexcept;

	// Sets the wrapping technique used when rendering the texture.
	void SetWrappingMode(uint32_t sAxis, uint32_t tAxis);

	// Sets the filtering technique used when rendering the texture.
	void SetFilteringMode(uint32_t minFilter, uint32_t magFilter);

	// Inserts new pixel data into the texture buffer at the buffer offset specified.
	void UpdateBuffer(uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height, uint32_t format, uint32_t type, 
		const void* pixelData);

	// Binds the texture buffer object.
	void BindBuffer() const;

	// Binds the texture buffer object.
	void BindBuffer(int textureIndex) const;

	// Unbinds the texture buffer object.
	void UnbindBuffer() const;

	// Returns the ID of the texture buffer object.
	const uint32_t& GetID() const;

	// Returns the target of the texture buffer object.
	const uint32_t& GetTarget() const;

	// Returns the width of the texture.
	const uint32_t& GetWidth() const;

	// Returns the height of the texture.
	const uint32_t& GetHeight() const;
};

namespace Graphics
{
	extern TextureBuffer LoadTextureFromFile(const std::string_view& fileName, bool flipTextureOnLoad = false);
}

#endif
