#include "graphics/textureManager.h"

namespace Eng
{
	TextureManager& TextureManager::Get()
	{
		static TextureManager instance;
		return instance;
	}

	std::shared_ptr<Texture2D> TextureManager::GetRadialGlowTexture()
	{
		if (!m_radialGlowTexture)
		{
			m_radialGlowTexture = Texture2D::CreateRadialGradient(64);
		}
		return m_radialGlowTexture;
	}

	std::shared_ptr<Texture2D> TextureManager::GetDefaultParticleTexture()
	{
		return GetRadialGlowTexture();
	}

	std::shared_ptr<Texture2D> TextureManager::GetStarTexture()
	{
		if (!m_starTexture)
		{
			m_starTexture = Texture2D::CreateStar(64);
		}
		return m_starTexture;
	}

	std::shared_ptr<Texture2D> TextureManager::GetRingTexture()
	{
		if (!m_ringTexture)
		{
			m_ringTexture = Texture2D::CreateRing(64);
		}
		return m_ringTexture;
	}

	std::shared_ptr<Texture2D> TextureManager::GetSmokeTexture()
	{
		if (!m_smokeTexture)
		{
			m_smokeTexture = Texture2D::CreateSmoke(64);
		}
		return m_smokeTexture;
	}

	std::shared_ptr<Texture2D> TextureManager::GetBubbleTexture()
	{
		if (!m_bubbleTexture)
		{
			m_bubbleTexture = Texture2D::CreateBubble(64);
		}
		return m_bubbleTexture;
	}

	std::shared_ptr<Texture2D> TextureManager::GetWhiteTexture()
	{
		if (!m_whiteTexture)
		{
			m_whiteTexture = Texture2D::CreateWhiteTexture();
		}
		return m_whiteTexture;
	}

	std::shared_ptr<Texture2D> TextureManager::GetTexture(FixedTexture type)
	{
		switch (type)
		{
		case FixedTexture::RadialGlow:
			return GetRadialGlowTexture();
		case FixedTexture::Star:
			return GetStarTexture();
		case FixedTexture::Ring:
			return GetRingTexture();
		case FixedTexture::Smoke:
			return GetSmokeTexture();
		case FixedTexture::Bubble:
			return GetBubbleTexture();
		case FixedTexture::SolidSquare:
			return GetWhiteTexture();
		default:
			return GetRadialGlowTexture();
		}
	}

	void TextureManager::Clear()
	{
		m_radialGlowTexture.reset();
		m_starTexture.reset();
		m_ringTexture.reset();
		m_smokeTexture.reset();
		m_bubbleTexture.reset();
		m_whiteTexture.reset();
	}
}
