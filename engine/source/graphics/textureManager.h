#ifndef ENG_TEXTURE_MANAGER_H
#define ENG_TEXTURE_MANAGER_H

#include "graphics/texture.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace Eng
{
	enum class FixedTexture
	{
		RadialGlow = 0,
		Star,
		Ring,
		Smoke,
		Bubble,
		SolidSquare
	};

	class TextureManager
	{
	public:
		static TextureManager& Get();

		std::shared_ptr<Texture2D> GetRadialGlowTexture();
		std::shared_ptr<Texture2D> GetDefaultParticleTexture();
		std::shared_ptr<Texture2D> GetStarTexture();
		std::shared_ptr<Texture2D> GetRingTexture();
		std::shared_ptr<Texture2D> GetSmokeTexture();
		std::shared_ptr<Texture2D> GetBubbleTexture();
		std::shared_ptr<Texture2D> GetWhiteTexture();

		std::shared_ptr<Texture2D> GetTexture(FixedTexture type);

		void Clear();

	private:
		TextureManager() = default;
		~TextureManager() = default;
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

	private:
		std::shared_ptr<Texture2D> m_radialGlowTexture = nullptr;
		std::shared_ptr<Texture2D> m_starTexture = nullptr;
		std::shared_ptr<Texture2D> m_ringTexture = nullptr;
		std::shared_ptr<Texture2D> m_smokeTexture = nullptr;
		std::shared_ptr<Texture2D> m_bubbleTexture = nullptr;
		std::shared_ptr<Texture2D> m_whiteTexture = nullptr;
	};
}

#endif
