#ifndef ENG_EMITTER_H
#define ENG_EMITTER_H

#include "Particle.h"
#include "graphics/texture.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>
#include <memory>

namespace Eng
{
	class Emitter
	{
	public:
		virtual ~Emitter() = default;
		virtual Particle CreateParticle() = 0;
		virtual void Update(float dt, std::vector<Particle>& pool);
		void SetTexture(const std::shared_ptr<Texture2D>& texture) { ParticleTexture = texture; }

	public:
		bool Active = true;

		ParticleType TypeToEmit = ParticleType::Solid;
		float EmissionRate = 50.0f;
		float LifeTime = 2.5f;
		glm::vec2 ParticleScale = glm::vec2(14.0f, 14.0f);

		float SpeedMin = 80.0f;
		float SpeedMax = 200.0f;

		std::shared_ptr<Texture2D> ParticleTexture = nullptr;

	protected:
		float m_accumulator = 0.0f;
	};

	class PointEmitter : public Emitter
	{
	public:
		PointEmitter(const glm::vec2& position = glm::vec2(0.0f, 0.0f));

		Particle CreateParticle() override;

		glm::vec2 Position = glm::vec2(0.0f, 0.0f);
		float BaseAngle = 1.570796f;
		float AngleSpread = 0.5f;
	};

	class LineEmitter : public Emitter
	{
	public:
		LineEmitter(const glm::vec2& p1 = glm::vec2(-200.0f, 0.0f), const glm::vec2& p2 = glm::vec2(200.0f, 0.0f));

		Particle CreateParticle() override;

		glm::vec2 PointA = glm::vec2(-200.0f, 0.0f);
		glm::vec2 PointB = glm::vec2(200.0f, 0.0f);
		bool EmitAlongNormal = true;
		float DirectionAngle = 1.570796f;
	};

	enum class AreaShape
	{
		Box,
		Circle
	};

	class AreaEmitter : public Emitter
	{
	public:
		AreaEmitter(const glm::vec2& center = glm::vec2(0.0f, 0.0f), const glm::vec2& halfExtents = glm::vec2(100.0f, 50.0f));

		Particle CreateParticle() override;

		AreaShape Shape = AreaShape::Box;
		glm::vec2 Center = glm::vec2(0.0f, 0.0f);
		glm::vec2 HalfExtents = glm::vec2(100.0f, 50.0f);
		float Radius = 80.0f;
		bool OutwardVelocity = false;
	};
}

#endif
