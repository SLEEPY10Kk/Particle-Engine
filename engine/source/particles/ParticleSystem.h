#ifndef ENG_PARTICLE_SYSTEM_H
#define ENG_PARTICLE_SYSTEM_H

#include "particles/Particle.h"
#include "particles/Emitter.h"
#include "particles/Force.h"
#include <vector>
#include <memory>

namespace Eng
{
	class ParticleSystem
	{
	public:
		ParticleSystem(size_t maxParticles = 10000);
		~ParticleSystem() = default;

		void AddEmitter(std::shared_ptr<Emitter> emitter);

		void AddForce(std::shared_ptr<Force> force);

		void Update(float dt);

		void Render(const std::shared_ptr<Texture2D>& defaultTexture = nullptr);

		size_t GetParticleCount() const { return m_particles.size(); }
		size_t GetMaxParticles() const { return m_maxParticles; }
		void SetMaxParticles(size_t maxParticles) { m_maxParticles = maxParticles; }
		void ClearParticles();
		void Clear();

	private:
		size_t m_maxParticles = 10000;
		std::vector<Particle> m_particles;
		std::vector<std::shared_ptr<Emitter>> m_emitters;
		std::vector<std::shared_ptr<Force>> m_forces;
	};
}

#endif
