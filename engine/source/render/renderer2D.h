#ifndef ENG_RENDERER2D_H
#define ENG_RENDERER2D_H

#include "render/camera2D.h"
#include "graphics/texture.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <cstdint>

namespace Eng
{
	enum class BlendMode
	{
		Alpha,
		Additive,
		Multiply,
		None
	};

	struct RenderStats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t VertexCount() const { return QuadCount * 4; }
		uint32_t IndexCount() const { return QuadCount * 6; }
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void Clear(const glm::vec4& color = glm::vec4(0.1f, 0.1f, 0.12f, 1.0f));

		static void Begin(const Camera2D& camera);
		static void End();
		static void Flush();

		static void SetBlendMode(BlendMode mode);
		static BlendMode GetBlendMode();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRadians, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotationRadians, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

		static void DrawParticle(
			const glm::vec2& position,
			const glm::vec2& size,
			float rotationRadians,
			const glm::vec4& color,
			const std::shared_ptr<Texture2D>& texture = nullptr);

		static void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color, float thickness = 2.0f);
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness = 2.0f);
		static void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, int segments = 32, float thickness = 2.0f);

		static const RenderStats& GetStats();
		static void ResetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
}

#endif
