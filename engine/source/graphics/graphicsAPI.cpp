#include "graphics/graphicsAPI.h"
#include "graphics/shaderProgram.h"
#include <iostream>

namespace Eng
{
	void GraphicsAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode)
	{
		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
			std::cerr << "VERTEX SHADER ERROR:\n" << infoLog << "\n";
			glDeleteShader(vertexShader);
			return nullptr;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
			std::cerr << "FRAGMENT SHADER ERROR:\n" << infoLog << "\n";
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return nullptr;
		}

		GLuint shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, vertexShader);
		glAttachShader(shaderProgramID, fragmentShader);
		glLinkProgram(shaderProgramID);

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, sizeof(infoLog), nullptr, infoLog);
			std::cerr << "SHADER LINK ERROR:\n" << infoLog << "\n";
			glDeleteProgram(shaderProgramID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return nullptr;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return std::make_shared<ShaderProgram>(shaderProgramID);
	}

	void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
	{
		if (shaderProgram)
		{
			shaderProgram->Bind();
		}
	}

	GLuint GraphicsAPI::CreateVertexBuffer(const std::vector<float>& vertices)
	{
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vbo;
	}

	GLuint GraphicsAPI::CreateIndexBuffer(const std::vector<uint32_t>& indices)
	{
		GLuint ebo = 0;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return ebo;
	}

	void GraphicsAPI::ClearBuffers()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsAPI::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void GraphicsAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void GraphicsAPI::SetWindowSize(int w, int h)
	{
		m_width = w;
		m_height = h;
	}

	FrameBuffer GraphicsAPI::CreateFrameBuffer(int width, int height, int numColorAttachments)
	{
		FrameBuffer fb;
		fb.width = width;
		fb.height = height;

		glGenFramebuffers(1, &fb.fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);

		std::vector<GLenum> drawBuffers;
		for (int i = 0; i < numColorAttachments; ++i)
		{
			GLuint tex = 0;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);

			fb.colorTextures.push_back(tex);
			drawBuffers.push_back(attachment);
		}

		glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Framebuffer incomplete!\n";
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return fb;
	}

	void GraphicsAPI::BindFrameBuffer(const FrameBuffer& fb)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);
		glViewport(0, 0, fb.width, fb.height);
	}

	void GraphicsAPI::UnbindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_width, m_height);
	}

	void GraphicsAPI::DestroyFrameBuffer(FrameBuffer& fb)
	{
		if (!fb.colorTextures.empty())
		{
			glDeleteTextures(static_cast<GLsizei>(fb.colorTextures.size()), fb.colorTextures.data());
			fb.colorTextures.clear();
		}
		if (fb.fbo)
		{
			glDeleteFramebuffers(1, &fb.fbo);
			fb.fbo = 0;
		}
	}

	void GraphicsAPI::BindTextureUnit(GLuint textureID, int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
}
