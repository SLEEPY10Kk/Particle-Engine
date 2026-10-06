#include "particles/Force.h"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <algorithm>

namespace Eng
{
	GravityForce::GravityForce(const glm::vec2& gravity)
		: Gravity(gravity)
	{
	}

	void GravityForce::Apply(Particle& particle, float dt)
	{
		if (!Enabled) return;

		glm::vec2 effectiveGravity = Gravity;
		if (particle.Type == ParticleType::Gas)
		{
			effectiveGravity = Gravity * (1.0f - particle.Properties.Buoyancy);
		}
		else
		{
			effectiveGravity *= particle.Properties.Mass;
		}

		particle.Velocity += effectiveGravity * dt;
	}

	CursorForce::CursorForce()
	{
	}

	void CursorForce::Apply(Particle& particle, float dt)
	{
		if (!Enabled) return;

		glm::vec2 diff = CursorPosition - particle.Position;
		float dist = glm::length(diff);

		if (dist > 1.0f && dist < Radius)
		{
			glm::vec2 dir = diff / dist;

			float normalizedDist = dist / Radius;
			float falloff = (1.0f - normalizedDist) * (1.0f - normalizedDist);

			float forceMagnitude = Strength * falloff;

			glm::vec2 forceVec = (Mode == CursorMode::Attract)
				? (dir * forceMagnitude)
				: (-dir * forceMagnitude);

			particle.Velocity += forceVec * dt;
		}
	}
}
