#include "particles/Emitter.h"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <random>
#include <cmath>
#include <algorithm>

namespace Eng
{
	static float RandomFloat(float min, float max)
	{
		if (min > max)
		{
			std::swap(min, max);
		}

		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(min, max);
		return dis(gen);
	}

	void Emitter::Update(float dt, std::vector<Particle>& pool)
	{
		if (!Active || EmissionRate <= 0.0f)
		{
			return;
		}

		m_accumulator += dt;
		float spawnInterval = 1.0f / EmissionRate;

		while (m_accumulator >= spawnInterval)
		{
			pool.push_back(CreateParticle());
			m_accumulator -= spawnInterval;
		}
	}

	PointEmitter::PointEmitter(const glm::vec2& position)
		: Position(position)
	{
	}

	Particle PointEmitter::CreateParticle()
	{
		float angle = BaseAngle + RandomFloat(-AngleSpread * 0.5f, AngleSpread * 0.5f);
		float speed = RandomFloat(SpeedMin, SpeedMax);
		glm::vec2 velocity = { std::cos(angle) * speed, std::sin(angle) * speed };

		glm::vec4 color = GetDefaultProperties(TypeToEmit).DefaultColor;
		float rotation = RandomFloat(0.0f, 6.28318f);

		Particle p(
			Position,
			velocity,
			ParticleScale,
			rotation,
			color,
			LifeTime,
			TypeToEmit,
			ParticleTexture
		);
		p.AngularVelocity = RandomFloat(-2.0f, 2.0f);
		return p;
	}

	LineEmitter::LineEmitter(const glm::vec2& p1, const glm::vec2& p2)
		: PointA(p1), PointB(p2)
	{
	}

	Particle LineEmitter::CreateParticle()
	{
		float t = RandomFloat(0.0f, 1.0f);
		glm::vec2 position = PointA + t * (PointB - PointA);

		glm::vec2 lineDir = PointB - PointA;
		float lineLength = glm::length(lineDir);

		glm::vec2 normal = (lineLength > 0.001f)
			? glm::normalize(glm::vec2(-lineDir.y, lineDir.x))
			: glm::vec2(0.0f, 1.0f);

		float speed = RandomFloat(SpeedMin, SpeedMax);
		glm::vec2 velocity;

		if (EmitAlongNormal)
		{
			float jitterAngle = RandomFloat(-0.25f, 0.25f);
			float cosJ = std::cos(jitterAngle);
			float sinJ = std::sin(jitterAngle);
			velocity = glm::vec2(normal.x * cosJ - normal.y * sinJ, normal.x * sinJ + normal.y * cosJ) * speed;
		}
		else
		{
			velocity = glm::vec2(std::cos(DirectionAngle) * speed, std::sin(DirectionAngle) * speed);
		}

		glm::vec4 color = GetDefaultProperties(TypeToEmit).DefaultColor;
		float rotation = RandomFloat(0.0f, 6.28318f);

		Particle p(
			position,
			velocity,
			ParticleScale,
			rotation,
			color,
			LifeTime,
			TypeToEmit,
			ParticleTexture
		);
		p.AngularVelocity = RandomFloat(-2.0f, 2.0f);
		return p;
	}

	AreaEmitter::AreaEmitter(const glm::vec2& center, const glm::vec2& halfExtents)
		: Center(center), HalfExtents(halfExtents)
	{
	}

	Particle AreaEmitter::CreateParticle()
	{
		glm::vec2 position = Center;

		if (Shape == AreaShape::Box)
		{
			position.x += RandomFloat(-HalfExtents.x, HalfExtents.x);
			position.y += RandomFloat(-HalfExtents.y, HalfExtents.y);
		}
		else if (Shape == AreaShape::Circle)
		{
			float r = Radius * std::sqrt(RandomFloat(0.0f, 1.0f));
			float theta = RandomFloat(0.0f, 6.283185f);
			position.x += r * std::cos(theta);
			position.y += r * std::sin(theta);
		}

		float speed = RandomFloat(SpeedMin, SpeedMax);
		glm::vec2 velocity;

		if (OutwardVelocity)
		{
			glm::vec2 diff = position - Center;
			float dist = glm::length(diff);
			glm::vec2 dir = (dist > 0.001f) ? (diff / dist) : glm::vec2(0.0f, 1.0f);
			velocity = dir * speed;
		}
		else
		{
			float angle = RandomFloat(0.0f, 6.283185f);
			velocity = glm::vec2(std::cos(angle) * speed, std::sin(angle) * speed);
		}

		glm::vec4 color = GetDefaultProperties(TypeToEmit).DefaultColor;
		float rotation = RandomFloat(0.0f, 6.28318f);

		Particle p(
			position,
			velocity,
			ParticleScale,
			rotation,
			color,
			LifeTime,
			TypeToEmit,
			ParticleTexture
		);
		p.AngularVelocity = RandomFloat(-2.0f, 2.0f);
		return p;
	}
}
