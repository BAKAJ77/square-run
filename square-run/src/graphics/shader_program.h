#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <string_view>
#include <string>
#include <memory>

class ShaderProgram
{
private:
	enum class PollCheckType
	{
		COMPILATION,
		LINKING
	};
private:
	uint32_t programID;
	std::string vertexFileName, fragmentFileName, geometryFileName;
	mutable std::unordered_map<std::string, uint32_t> uniformCache;
private:
	void PollShaderErrors(const uint32_t& id, PollCheckType type) const;
	uint32_t QueryUniformID(const std::string_view& uniformName) const;
public:
	ShaderProgram(const std::string_view& vertexFileName, const std::string_view& fragmentFileName, 
		const std::string_view& geometryFileName);
	~ShaderProgram();
	
	// Sets the value of shader uniform of type integer.
	void SetUniform(const std::string_view& uniformName, int value) const;

	// Sets the value of shader uniform of type boolean.
	void SetUniform(const std::string_view& uniformName, bool value) const;

	// Sets the value of shader uniform of type float.
	void SetUniform(const std::string_view& uniformName, float value) const;

	// Sets the value of shader uniform of type vector2.
	void SetUniformGLM(const std::string_view& uniformName, const glm::vec2& vector) const;

	// Sets the value of shader uniform of type vector3.
	void SetUniformGLM(const std::string_view& uniformName, const glm::vec3& vector) const;

	// Sets the value of shader uniform of type vector4.
	void SetUniformGLM(const std::string_view& uniformName, const glm::vec4& vector) const;

	// Sets the value of shader uniform of type matrix 3x3.
	void SetUniformGLM(const std::string_view& uniformName, const glm::mat3& matrix) const;

	// Sets the value of shader uniform of type matrix 4x4.
	void SetUniformGLM(const std::string_view& uniformName, const glm::mat4& matrix) const;

	// Binds the shader program.
	void BindProgram() const;

	// Unbinds the shader program.
	void UnbindProgram() const;

	// Returns the ID of the shader program.
	const uint32_t& GetID() const;

	// Returns the vertex shader's file name.
	const std::string& GetVertexFileName() const;

	// Returns the fragment shader's file name.
	const std::string& GetFragmentFileName() const;

	// Returns the geometry shader's file name.
	const std::string& GetGeometryFileName() const;
};

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;

namespace Memory
{
	// Returns a shared pointer to the new created shader program.
	extern ShaderProgramPtr CreateShaderProgram(const std::string_view& vertexFileName, const std::string_view& fragmentFileName,
		const std::string_view& geometryFileName = "");
}

#endif
