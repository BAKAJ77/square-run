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
	bool hasAttachedIBO;
public:
	VertexArray();
	VertexArray(const VertexArray& other) = delete;
	VertexArray(VertexArray&& temp) noexcept;

	~VertexArray();

	VertexArray& operator=(const VertexArray& other) = delete;
	VertexArray& operator=(VertexArray&& temp) noexcept;

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
	void InitVertexArray(const VertexBuffer& vbo, const IndexBuffer* ibo = nullptr);

	// Binds the vertex array object.
	void BindObject() const;

	// Unbinds the vertex array object.
	void UnbindObject() const;

	// Returns the ID of the vertex array object.
	const uint32_t& GetID() const;

	// Returns TRUE if a IBO has been attached to the VAO, else FALSE is returned.
	bool HasAttachedIBO() const;
};

#include <graphics/vertex_array.inl>

#endif
