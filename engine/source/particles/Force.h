#ifndef ENG_FORCE_H
#define ENG_FORCE_H

#include "Particle.h"
#include <glm/vec2.hpp>
#include <string>

namespace Eng
{
	class Force
	{
	public:
		virtual ~Force() = default;
		virtual void Apply(Particle& particle, float dt) = 0;

		bool Enabled = true;
	};

	class GravityForce : public Force
	{
	public:
		GravityForce(const glm::vec2& gravity = glm::vec2(0.0f, -400.0f));

		void Apply(Particle& particle, float dt) override;

		glm::vec2 Gravity = glm::vec2(0.0f, -400.0f);
	};

	enum class CursorMode
	{
		Attract,
		Repel
	};

	class CursorForce : public Force
	{
	public:
		CursorForce();

		void Apply(Particle& particle, float dt) override;

		glm::vec2 CursorPosition = glm::vec2(0.0f);
		CursorMode Mode = CursorMode::Attract;
		float Strength = 800.0f;
		float Radius = 300.0f;
	};
}

#endif
