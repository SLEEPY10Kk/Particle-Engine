#include "particles/Particle.h"
#include <glm/glm.hpp>
#include <algorithm>

namespace Eng
{
	Particle::Particle()
		: Properties(GetDefaultProperties(ParticleType::Solid))
	{
		RemainingLife = LifeTime;
		InitialScale = Scale;
	}

	Particle::Particle(
		const glm::vec2& position,
		const glm::vec2& velocity,
		const glm::vec2& scale,
		float rotation,
		const glm::vec4& color,
		float lifeTime,
		ParticleType type,
		const std::shared_ptr<Texture2D>& texture)
		: Position(position),
		  Scale(scale),
		  Rotation(rotation),
		  LifeTime(lifeTime),
		  Type(type),
		  Velocity(velocity),
		  AngularVelocity(0.0f),
		  Color(color),
		  RemainingLife(lifeTime),
		  InitialScale(scale),
		  Properties(GetDefaultProperties(type)),
		  Texture(texture)
	{
	}

	Particle::~Particle()
	{
	}

	void Particle::Update(float dt)
	{
		RemainingLife -= dt;
		if (RemainingLife < 0.0f)
		{
			RemainingLife = 0.0f;
			return;
		}

		Position += Velocity * dt;
		Rotation += AngularVelocity * dt;

		float dragFactor = 1.0f - (Properties.Drag * dt);
		if (dragFactor < 0.0f) dragFactor = 0.0f;
		Velocity *= dragFactor;

		if (Type == ParticleType::Gas)
		{
			Scale += InitialScale * (Properties.ExpansionRate * dt);
		}
		else if (Type == ParticleType::Liquid)
		{
			float speed = glm::length(Velocity);
			if (speed > 50.0f)
			{
				float stretch = glm::clamp(speed * 0.002f, 0.0f, 0.5f);
				Scale.y = InitialScale.y * (1.0f + stretch);
				Scale.x = InitialScale.x * (1.0f - stretch * 0.5f);
			}
		}

		if (LifeTime > 0.0f)
		{
			float lifeRatio = RemainingLife / LifeTime;
			Color.a = glm::clamp(lifeRatio, 0.0f, 1.0f);
		}
	}
}
