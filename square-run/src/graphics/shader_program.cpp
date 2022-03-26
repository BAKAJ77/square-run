#include <graphics/shader_program.h>
#include <util/directory_system.h>
#include <util/logging_system.h>

#include <glad/glad.h>
#include <sstream>
#include <fstream>

ShaderProgram::ShaderProgram(const std::string_view& vertexFileName, const std::string_view& fragmentFileName,
	const std::string_view& geometryFileName) :
	vertexFileName(vertexFileName), fragmentFileName(fragmentFileName), geometryFileName(geometryFileName)
{
	// Get the game's shader directory path
	const std::string shaderDirectoryPath = Util::GetGameRequisitesDirectory() + "shaders/";
	if (!Util::IsExistingDirectory(shaderDirectoryPath))
		LogSystem::GetInstance().OutputLog("Couldn't find the game's shaders directory", Severity::FATAL);

	// Open the vertex and fragment shader files and load their contents
	std::ifstream vertexFile, fragmentFile;
	std::stringstream vertexFileStream, fragmentFileStream, geometryFileStream;

	vertexFile.open(shaderDirectoryPath + vertexFileName.data());
	fragmentFile.open(shaderDirectoryPath + fragmentFileName.data());

	if (vertexFile.fail()) // Failed to open the vertex shader file
	{
		LogSystem::GetInstance().OutputLog("Failed to open vertex shader file: " + std::string(vertexFileName.data()),
			Severity::FATAL);
	}
	else if (fragmentFile.fail()) // Failed to open the fragment shader file
	{
		LogSystem::GetInstance().OutputLog("Failed to open fragment shader file: " + std::string(fragmentFileName.data()),
			Severity::FATAL);
	}

	vertexFileStream << vertexFile.rdbuf();
	fragmentFileStream << fragmentFile.rdbuf();

	vertexFile.close();
	fragmentFile.close();

	// If a geometry shader path was specified, open and load the geometry file contents
	if (!geometryFileName.empty())
	{
		std::ifstream geometryFile(shaderDirectoryPath + geometryFileName.data());
		if (geometryFile.fail()) // Failed to open the geometry shader file
		{
			LogSystem::GetInstance().OutputLog("Failed to open geometry shader file: " + std::string(geometryFileName.data()),
				Severity::FATAL);
		}

		geometryFileStream << geometryFile.rdbuf();
	}

	// Compile both the loaded vertex and fragment shader source code
	const std::string vertexSourceStr = vertexFileStream.str(), fragmentSourceStr = fragmentFileStream.str();
	const char* vertexSourcePtr = vertexSourceStr.c_str();
	const char* fragmentSourcePtr = fragmentSourceStr.c_str();

	const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSourcePtr, nullptr);
	glCompileShader(vertexShader);

	this->PollShaderErrors(vertexShader, PollCheckType::COMPILATION);

	const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSourcePtr, nullptr);
	glCompileShader(fragmentShader);

	this->PollShaderErrors(fragmentShader, PollCheckType::COMPILATION);

	// Compile the geometry shader source (if a geometry shader file was loaded)
	uint32_t geometryShader = 0;

	if (!this->geometryFileName.empty())
	{
		const std::string geometrySourceStr = geometryFileStream.str();
		const char* geometrySourcePtr = geometrySourceStr.c_str();

		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometrySourcePtr, nullptr);
		glCompileShader(geometryShader);

		this->PollShaderErrors(geometryShader, PollCheckType::COMPILATION);
	}

	// Create shader program, then attach both compiled shaders to the shader program
	this->programID = glCreateProgram();
	glAttachShader(this->programID, vertexShader);
	glAttachShader(this->programID, fragmentShader);
	
	if (!this->geometryFileName.empty())
		glAttachShader(this->programID, geometryShader);

	glLinkProgram(this->programID);

	this->PollShaderErrors(this->programID, PollCheckType::LINKING);

	// Lastly, clean up allocated resources
	glDeleteShader(vertexShader); // We don't need the compiled shader objects anymore, they've been attached to the shader program
	glDeleteShader(fragmentShader);
	
	if (!this->geometryFileName.empty())
		glDeleteShader(geometryShader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->programID);
}

void ShaderProgram::PollShaderErrors(const uint32_t& id, PollCheckType type) const
{
	std::unique_ptr<char> errorLogBuffer;
	int errorLogLength = 0;

	switch (type)
	{
	case PollCheckType::COMPILATION:
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLogLength); // Get the length of potential pending error log
		if (errorLogLength > 0)
		{
			errorLogBuffer = std::unique_ptr<char>(new char[errorLogLength]); // Allocate memory for log buffer
			glGetShaderInfoLog(id, errorLogLength, nullptr, errorLogBuffer.get()); // Write the pending log data to buffer
		}
		break;
	case PollCheckType::LINKING:
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &errorLogLength);
		if (errorLogLength > 0)
		{
			errorLogBuffer = std::unique_ptr<char>(new char[errorLogLength]);
			glGetProgramInfoLog(id, errorLogLength, nullptr, errorLogBuffer.get());
		}
		break;
	}

	if (errorLogLength > 0)
		LogSystem::GetInstance().OutputLog(errorLogBuffer.get(), Severity::FATAL); // Output error log to console/file
}

uint32_t ShaderProgram::QueryUniformID(const std::string_view& uniformName) const
{
	// Attempt to find the uniform ID in the map
	auto mapIterator = this->uniformCache.find(uniformName.data());
	if (mapIterator != this->uniformCache.end())
		return mapIterator->second;

	// The uniform ID is not in the map so we have to query for it
	const uint32_t uniformID = glGetUniformLocation(this->programID, uniformName.data());
	this->uniformCache[uniformName.data()] = uniformID;
	return uniformID;
}

void ShaderProgram::SetUniform(const std::string_view& uniformName, int value) const
{
	glUniform1i(this->QueryUniformID(uniformName), value);
}

void ShaderProgram::SetUniform(const std::string_view& uniformName, bool value) const
{
	glUniform1i(this->QueryUniformID(uniformName), (int)value);
}

void ShaderProgram::SetUniform(const std::string_view& uniformName, float value) const
{
	glUniform1f(this->QueryUniformID(uniformName), value);
}

void ShaderProgram::SetUniformGLM(const std::string_view& uniformName, const glm::vec2& vector) const
{
	glUniform2fv(this->QueryUniformID(uniformName), 1, &vector[0]);
}

void ShaderProgram::SetUniformGLM(const std::string_view& uniformName, const glm::vec3& vector) const
{
	glUniform3fv(this->QueryUniformID(uniformName), 1, &vector[0]);
}

void ShaderProgram::SetUniformGLM(const std::string_view& uniformName, const glm::vec4& vector) const
{
	glUniform4fv(this->QueryUniformID(uniformName), 1, &vector[0]);
}

void ShaderProgram::SetUniformGLM(const std::string_view& uniformName, const glm::mat3& matrix) const
{
	glUniformMatrix3fv(this->QueryUniformID(uniformName), 1, false, &matrix[0][0]);
}

void ShaderProgram::SetUniformGLM(const std::string_view& uniformName, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(this->QueryUniformID(uniformName), 1, false, &matrix[0][0]);
}

void ShaderProgram::BindProgram() const
{
	glUseProgram(this->programID);
}

void ShaderProgram::UnbindProgram() const
{
	glUseProgram(0);
}

const uint32_t& ShaderProgram::GetID() const
{
	return this->programID;
}

const std::string& ShaderProgram::GetVertexFileName() const
{
	return this->vertexFileName;
}

const std::string& ShaderProgram::GetFragmentFileName() const
{
	return this->fragmentFileName;
}

const std::string& ShaderProgram::GetGeometryFileName() const
{
	return this->geometryFileName;
}

ShaderProgramPtr Memory::CreateShaderProgram(const std::string_view& vertexFileName, const std::string_view& fragmentFileName,
	const std::string_view& geometryFileName)
{
	return std::make_shared<ShaderProgram>(vertexFileName, fragmentFileName, geometryFileName);
}
