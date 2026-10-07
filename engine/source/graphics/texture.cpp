#include "graphics/texture.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

namespace Eng
{
	Texture2D::Texture2D(int width, int height, int numChannels, const unsigned char* data)
		: m_width(width), m_height(height), m_numChannels(numChannels)
	{
		Init(width, height, numChannels, data);
	}

	Texture2D::~Texture2D()
	{
		if (m_textureID > 0)
		{
			glDeleteTextures(1, &m_textureID);
		}
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void Texture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::SetFilter(bool linear)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		GLint filter = linear ? GL_LINEAR : GL_NEAREST;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	}

	void Texture2D::SetWrap(bool repeat)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		GLint wrap = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	}

	void Texture2D::Init(int width, int height, int numChannels, const unsigned char* data)
	{
		m_width = width;
		m_height = height;
		m_numChannels = numChannels;

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		GLenum internalFormat = GL_RGBA8;
		GLenum dataFormat = GL_RGBA;

		if (numChannels == 1)
		{
			internalFormat = GL_R8;
			dataFormat = GL_RED;
		}
		else if (numChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (numChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (width > 4 && height > 4)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
	}

	std::shared_ptr<Texture2D> Texture2D::Create(int width, int height, const unsigned char* data, int numChannels)
	{
		return std::make_shared<Texture2D>(width, height, numChannels, data);
	}

	std::shared_ptr<Texture2D> Texture2D::CreateWhiteTexture()
	{
		uint32_t whitePixel = 0xFFFFFFFF;
		return std::make_shared<Texture2D>(1, 1, 4, reinterpret_cast<const unsigned char*>(&whitePixel));
	}

	std::shared_ptr<Texture2D> Texture2D::CreateRadialGradient(int size)
	{
		std::vector<unsigned char> data(size * size * 4);
		float center = static_cast<float>(size - 1) * 0.5f;
		float maxRadius = center;

		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				float dx = static_cast<float>(x) - center;
				float dy = static_cast<float>(y) - center;
				float dist = std::sqrt(dx * dx + dy * dy);
				float normalized = dist / maxRadius;
				float alpha = 0.0f;

				if (normalized < 1.0f)
				{
					alpha = 0.5f * (1.0f + std::cos(normalized * 3.14159265f));
					alpha = std::pow(alpha, 1.5f);
				}

				int index = (y * size + x) * 4;
				data[index + 0] = 255;
				data[index + 1] = 255;
				data[index + 2] = 255;
				data[index + 3] = static_cast<unsigned char>(alpha * 255.0f);
			}
		}

		return std::make_shared<Texture2D>(size, size, 4, data.data());
	}

	std::shared_ptr<Texture2D> Texture2D::CreateStar(int size)
	{
		std::vector<unsigned char> data(size * size * 4);
		float center = static_cast<float>(size - 1) * 0.5f;

		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				float dx = std::abs(static_cast<float>(x) - center) / center;
				float dy = std::abs(static_cast<float>(y) - center) / center;

				float r = std::sqrt(dx * dx + dy * dy);
				float alpha = 0.0f;
				if (r < 1.0f)
				{
					float cross = std::max(0.0f, 1.0f - dx) * std::pow(std::max(0.0f, 1.0f - dy * 4.0f), 2.0f)
					            + std::max(0.0f, 1.0f - dy) * std::pow(std::max(0.0f, 1.0f - dx * 4.0f), 2.0f);
					float core = std::max(0.0f, 1.0f - r * 2.5f);
					alpha = std::clamp(cross * 0.7f + core * 0.8f, 0.0f, 1.0f);
				}

				int index = (y * size + x) * 4;
				data[index + 0] = 255;
				data[index + 1] = 255;
				data[index + 2] = 255;
				data[index + 3] = static_cast<unsigned char>(alpha * 255.0f);
			}
		}

		return std::make_shared<Texture2D>(size, size, 4, data.data());
	}

	std::shared_ptr<Texture2D> Texture2D::CreateRing(int size)
	{
		std::vector<unsigned char> data(size * size * 4);
		float center = static_cast<float>(size - 1) * 0.5f;
		float ringRadius = center * 0.7f;
		float ringThickness = center * 0.25f;

		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				float dx = static_cast<float>(x) - center;
				float dy = static_cast<float>(y) - center;
				float dist = std::sqrt(dx * dx + dy * dy);
				float diff = std::abs(dist - ringRadius);

				float alpha = 0.0f;
				if (diff < ringThickness)
				{
					float norm = diff / ringThickness;
					alpha = 0.5f * (1.0f + std::cos(norm * 3.14159265f));
				}

				int index = (y * size + x) * 4;
				data[index + 0] = 255;
				data[index + 1] = 255;
				data[index + 2] = 255;
				data[index + 3] = static_cast<unsigned char>(alpha * 255.0f);
			}
		}

		return std::make_shared<Texture2D>(size, size, 4, data.data());
	}

	std::shared_ptr<Texture2D> Texture2D::CreateSmoke(int size)
	{
		std::vector<unsigned char> data(size * size * 4);
		float center = static_cast<float>(size - 1) * 0.5f;

		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				float dx = static_cast<float>(x) - center;
				float dy = static_cast<float>(y) - center;
				float dist = std::sqrt(dx * dx + dy * dy);
				float angle = std::atan2(dy, dx);

				float lobe = 1.0f + 0.15f * std::sin(angle * 3.0f) + 0.10f * std::cos(angle * 5.0f);
				float effectiveRadius = center * 0.85f * lobe;
				float normalized = dist / effectiveRadius;

				float alpha = 0.0f;
				if (normalized < 1.0f)
				{
					alpha = std::pow(std::cos(normalized * 1.570796f), 1.8f);
				}

				int index = (y * size + x) * 4;
				data[index + 0] = 255;
				data[index + 1] = 255;
				data[index + 2] = 255;
				data[index + 3] = static_cast<unsigned char>(alpha * 255.0f);
			}
		}

		return std::make_shared<Texture2D>(size, size, 4, data.data());
	}

	std::shared_ptr<Texture2D> Texture2D::CreateBubble(int size)
	{
		std::vector<unsigned char> data(size * size * 4);
		float center = static_cast<float>(size - 1) * 0.5f;
		float maxRadius = center * 0.9f;

		float specX = center - maxRadius * 0.35f;
		float specY = center - maxRadius * 0.35f;
		float specRadius = maxRadius * 0.25f;

		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				float dx = static_cast<float>(x) - center;
				float dy = static_cast<float>(y) - center;
				float dist = std::sqrt(dx * dx + dy * dy);
				float normalized = dist / maxRadius;

				float alpha = 0.0f;
				if (normalized <= 1.0f)
				{
					float rim = std::pow(normalized, 3.0f) * 0.7f;
					float core = 0.15f * (1.0f - normalized);
					alpha = rim + core;

					float sdx = static_cast<float>(x) - specX;
					float sdy = static_cast<float>(y) - specY;
					float sdist = std::sqrt(sdx * sdx + sdy * sdy);
					if (sdist < specRadius)
					{
						float spec = std::cos((sdist / specRadius) * 1.570796f);
						alpha = std::clamp(alpha + spec * 0.9f, 0.0f, 1.0f);
					}
				}

				int index = (y * size + x) * 4;
				data[index + 0] = 255;
				data[index + 1] = 255;
				data[index + 2] = 255;
				data[index + 3] = static_cast<unsigned char>(alpha * 255.0f);
			}
		}

		return std::make_shared<Texture2D>(size, size, 4, data.data());
	}
}
