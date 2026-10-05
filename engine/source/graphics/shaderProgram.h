#ifndef ENG_SHADER_PROGRAM_H
#define ENG_SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Eng
{
	class Texture2D;

	class ShaderProgram
	{
	private:
		std::unordered_map<std::string, GLint> m_uniformLocationCache;
		GLuint m_programID = 0;
		int m_currentTextureUnit = 0;

	public:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		~ShaderProgram();
		explicit ShaderProgram(GLuint programID);

		void Bind() const;
		void Unbind() const;
		GLuint GetProgramID() const { return m_programID; }

		GLint GetUniformLocation(const std::string& name);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, float v0, float v1);
		void SetUniformVec2(const std::string& name, const glm::vec2& vec);
		void SetUniformVec3(const std::string& name, const glm::vec3& vec);
		void SetUniformVec4(const std::string& name, const glm::vec4& vec);
		void SetUniformMat3(const std::string& name, const glm::mat3& mat);
		void SetUniformMat4(const std::string& name, const glm::mat4& mat);
		void SetUniformIntArray(const std::string& name, const int* values, uint32_t count);

		void SetTexture(const std::string& name, Texture2D* texture);
	};
}

#endif
