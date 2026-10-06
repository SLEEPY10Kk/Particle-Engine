#ifndef ENG_PARTICLE_H
#define ENG_PARTICLE_H

#include "ParticleType.h"
#include "graphics/texture.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace Eng
{
	class Particle
	{
	public:
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(16.0f, 16.0f);
		float Rotation = 0.0f;
		float LifeTime = 2.0f;
		ParticleType Type = ParticleType::Solid;

		glm::vec2 Velocity = glm::vec2(0.0f);
		float AngularVelocity = 0.0f;
		glm::vec4 Color = glm::vec4(1.0f);
		float RemainingLife = 2.0f;
		glm::vec2 InitialScale = glm::vec2(16.0f, 16.0f);
		ParticleTypeProperties Properties;

		std::shared_ptr<Texture2D> Texture = nullptr;

	public:
		Particle();

		Particle(
			const glm::vec2& position,
			const glm::vec2& velocity,
			const glm::vec2& scale,
			float rotation,
			const glm::vec4& color,
			float lifeTime,
			ParticleType type = ParticleType::Solid,
			const std::shared_ptr<Texture2D>& texture = nullptr);

		~Particle();

		void Update(float dt);

		bool IsAlive() const { return RemainingLife > 0.0f; }
	};
}

#endif
