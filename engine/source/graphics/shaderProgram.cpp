#include "graphics/shaderProgram.h"
#include "graphics/texture.h"

namespace Eng
{
	ShaderProgram::~ShaderProgram()
	{
		if (m_programID > 0)
		{
			glDeleteProgram(m_programID);
		}
	}

	ShaderProgram::ShaderProgram(GLuint programID) : m_programID(programID)
	{
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_programID);
	}

	void ShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

	GLint ShaderProgram::GetUniformLocation(const std::string& name)
	{
		auto it = m_uniformLocationCache.find(name);
		if (it != m_uniformLocationCache.end())
		{
			return it->second;
		}

		GLint location = glGetUniformLocation(m_programID, name.c_str());
		m_uniformLocationCache[name] = location;
		return location;
	}

	void ShaderProgram::SetUniform(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1f(location, value);
	}

	void ShaderProgram::SetUniform(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}

	void ShaderProgram::SetUniform(const std::string& name, float v0, float v1)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform2f(location, v0, v1);
	}

	void ShaderProgram::SetUniformVec2(const std::string& name, const glm::vec2& vec)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform2fv(location, 1, glm::value_ptr(vec));
	}

	void ShaderProgram::SetUniformVec3(const std::string& name, const glm::vec3& vec)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform3fv(location, 1, glm::value_ptr(vec));
	}

	void ShaderProgram::SetUniformVec4(const std::string& name, const glm::vec4& vec)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform4fv(location, 1, glm::value_ptr(vec));
	}

	void ShaderProgram::SetUniformMat3(const std::string& name, const glm::mat3& mat)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void ShaderProgram::SetUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void ShaderProgram::SetUniformIntArray(const std::string& name, const int* values, uint32_t count)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1iv(location, count, values);
	}

	void ShaderProgram::SetTexture(const std::string& name, Texture2D* texture)
	{
		if (!texture) return;
		GLint location = GetUniformLocation(name);
		if (location == -1) return;

		glActiveTexture(GL_TEXTURE0 + m_currentTextureUnit);
		glBindTexture(GL_TEXTURE_2D, texture->GetID());
		glUniform1i(location, m_currentTextureUnit);
		++m_currentTextureUnit;
	}
}
