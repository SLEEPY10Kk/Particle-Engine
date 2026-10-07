#ifndef ENG_TEXTURE_H
#define ENG_TEXTURE_H

#include <glad/glad.h>
#include <memory>
#include <string>

namespace Eng
{
	class Texture2D
	{
	public:
		Texture2D(int width, int height, int numChannels, const unsigned char* data);
		~Texture2D();

		GLuint GetID() const { return m_textureID; }
		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
		int GetChannels() const { return m_numChannels; }

		void Bind(uint32_t slot = 0) const;
		void Unbind() const;

		void SetFilter(bool linear);
		void SetWrap(bool repeat);

		static std::shared_ptr<Texture2D> Create(int width, int height, const unsigned char* data, int numChannels = 4);
		static std::shared_ptr<Texture2D> CreateWhiteTexture();
		static std::shared_ptr<Texture2D> CreateRadialGradient(int size = 64);
		static std::shared_ptr<Texture2D> CreateStar(int size = 64);
		static std::shared_ptr<Texture2D> CreateRing(int size = 64);
		static std::shared_ptr<Texture2D> CreateSmoke(int size = 64);
		static std::shared_ptr<Texture2D> CreateBubble(int size = 64);

	private:
		void Init(int width, int height, int numChannels, const unsigned char* data);

	private:
		int m_width = 0;
		int m_height = 0;
		int m_numChannels = 0;
		GLuint m_textureID = 0;
	};

	using Texture = Texture2D;
}

#endif
