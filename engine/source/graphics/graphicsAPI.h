#ifndef ENG_GRAPHICS_API_H
#define ENG_GRAPHICS_API_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>
#include <array>

namespace Eng
{
	struct FrameBuffer
	{
		GLuint fbo = 0;
		std::vector<GLuint> colorTextures;
		int width = 0;
		int height = 0;
	};

	class ShaderProgram;

	class GraphicsAPI
	{
	public:
		void Init();
		void ClearBuffers();
		void SetClearColor(float r, float g, float b, float a);
		void SetViewport(int x, int y, int width, int height);

		GLuint CreateVertexBuffer(const std::vector<float>& vertices);
		GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);

		std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode);
		void BindShaderProgram(ShaderProgram* shaderProgram);

		FrameBuffer CreateFrameBuffer(int width, int height, int numColorAttachments = 1);
		void BindFrameBuffer(const FrameBuffer& fb);
		void UnbindFrameBuffer();
		void DestroyFrameBuffer(FrameBuffer& fb);
		void BindTextureUnit(GLuint textureID, int unit);

		void SetWindowSize(int w, int h);
		int GetWindowWidth() const { return m_width; }
		int GetWindowHeight() const { return m_height; }

	private:
		int m_width = 1280;
		int m_height = 720;
	};
}

#endif
