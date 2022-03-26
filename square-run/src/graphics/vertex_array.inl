#include <graphics/vertex_array.h>

template<typename Ty> void VertexArray::PushVertexLayout(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset,
	uint32_t divisor, bool normalized)
{
	static_assert(false);
}

template<> void VertexArray::PushVertexLayout<int>(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset,
	uint32_t divisor, bool normalized)
{
	// 0x1404 is the value of GL_INT
	this->vertexLayouts.push_back({ index, size, 0x1404, stride, offset, divisor, normalized });
}

template<> void VertexArray::PushVertexLayout<float>(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset,
	uint32_t divisor, bool normalized)
{
	// 0x1406 is the value of GL_FLOAT
	this->vertexLayouts.push_back({ index, size, 0x1406, stride, offset, divisor, normalized });
}

template<> void VertexArray::PushVertexLayout<uint8_t>(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset,
	uint32_t divisor, bool normalized)
{
	// 0x1401 is the value of GL_UNSIGNED_BYTE
	this->vertexLayouts.push_back({ index, size, 0x1401, stride, offset, divisor, normalized });
}