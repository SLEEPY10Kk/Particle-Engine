#ifndef ENG_PARTICLE_TYPE_H
#define ENG_PARTICLE_TYPE_H

#include <string>
#include <glm/vec4.hpp>

namespace Eng
{
	enum class ParticleType
	{
		Solid,
		Liquid,
		Gas
	};

	struct ParticleTypeProperties
	{
		float Mass = 1.0f;
		float Buoyancy = 0.0f;
		float Drag = 0.05f;
		float ExpansionRate = 0.0f;
		glm::vec4 DefaultColor = glm::vec4(1.0f);
	};

	inline ParticleTypeProperties GetDefaultProperties(ParticleType type)
	{
		ParticleTypeProperties props;
		switch (type)
		{
		case ParticleType::Solid:
			props.Mass = 1.0f;
			props.Buoyancy = 0.0f;
			props.Drag = 0.02f;
			props.ExpansionRate = 0.0f;
			props.DefaultColor = glm::vec4(0.95f, 0.65f, 0.25f, 1.0f);
			break;

		case ParticleType::Liquid:
			props.Mass = 1.2f;
			props.Buoyancy = 0.0f;
			props.Drag = 0.25f;
			props.ExpansionRate = -0.1f;
			props.DefaultColor = glm::vec4(0.25f, 0.65f, 0.95f, 0.85f);
			break;

		case ParticleType::Gas:
			props.Mass = 0.3f;
			props.Buoyancy = 1.5f;
			props.Drag = 0.35f;
			props.ExpansionRate = 1.5f;
			props.DefaultColor = glm::vec4(0.85f, 0.85f, 0.90f, 0.45f);
			break;
		}
		return props;
	}
}

#endif
