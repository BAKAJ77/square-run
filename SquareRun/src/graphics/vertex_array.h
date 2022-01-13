#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <graphics/buffer_objects.h>
#include <vector>

class VertexArray
{
private:
	struct VertexLayout
	{
		uint32_t index, size, type, stride, offset, divisor;
		bool normalized;
	};
private:
	uint32_t vaoID;
	std::vector<VertexLayout> vertexLayouts;
public:
	VertexArray();
	~VertexArray();

	// Pushes given vertex layout which is used to specify to OpenGL how the data in the VBO attached is formatted.
	template<typename Ty> void PushVertexLayout(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset = 0,
		uint32_t divisor = 0, bool normalized = false);

	// Pushes given vertex layout which is used to specify to OpenGL how the data in the VBO attached is formatted.
	template<> void PushVertexLayout<int>(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset,
		uint32_t divisor, bool normalized);

	// Pushes given vertex layout which is used to specify to OpenGL how the data in the VBO attached is formatted.
	template<> void PushVertexLayout<float>(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset,
		uint32_t divisor, bool normalized);

	// Pushes given vertex layout which is used to specify to OpenGL how the data in the VBO attached is formatted.
	template<> void PushVertexLayout<uint8_t>(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset,
		uint32_t divisor, bool normalized);

	// Generates and sets up the VAO, attaching the given buffer objects and configuring the vertex attribute pointers.
	void AttachBuffers(const VertexBufferPtr vbo, const IndexBufferPtr ibo = nullptr);

	// Binds the vertex array object.
	void BindObject() const;

	// Unbinds the vertex array object.
	void UnbindObject() const;

	// Returns the ID of the vertex array object.
	const uint32_t& GetID() const;
};

using VertexArrayPtr = std::shared_ptr<VertexArray>;

namespace Memory
{
	// Returns a shared pointer to the new created vertex array object.
	extern VertexArrayPtr CreateVertexArray();
}

#include <graphics/vertex_array.inl>

#endif