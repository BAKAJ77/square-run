#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <graphics/texture_buffer.h>

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

struct FT_LibraryRec_;
using FT_Library = FT_LibraryRec_*;

struct GlyphData
{
	glm::vec2 bearing, size;
	uint32_t advanceX = 0, textureOffsetX = 0;
};

using GlyphMap = std::unordered_map<char, GlyphData>;

class Font
{
private:
	std::string fileName;
	TextureBuffer bitmapTexture;
	GlyphMap glyphs;
	uint32_t resolution;
public:
	Font();
	Font(const std::string_view& fileName, uint32_t resolution);
	Font(Font&& temp) noexcept;

	~Font();

	Font& operator=(Font&& temp) noexcept;

	// Returns the file name of the loaded font.
	const std::string& GetFileName() const;

	// Returns the data of each glyph loaded from the font file.
	const GlyphMap& GetGlyphs() const;

	// Returns the generated bitmap texture loaded from the font file.
	const TextureBuffer& GetBitmap() const;

	// Returns the resolution of the the loaded glyphs in the font bitmap texture.
	const uint32_t& GetResolution() const;
};

#endif
