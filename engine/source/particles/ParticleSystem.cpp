#include "particles/ParticleSystem.h"
#include "render/renderer2D.h"
#include <algorithm>

namespace Eng
{
	ParticleSystem::ParticleSystem(size_t maxParticles)
		: m_maxParticles(maxParticles)
	{
		m_particles.reserve(maxParticles);
	}

	void ParticleSystem::AddEmitter(std::shared_ptr<Emitter> emitter)
	{
		if (emitter)
		{
			m_emitters.push_back(emitter);
		}
	}

	void ParticleSystem::AddForce(std::shared_ptr<Force> force)
	{
		if (force)
		{
			m_forces.push_back(force);
		}
	}

	void ParticleSystem::ClearParticles()
	{
		m_particles.clear();
	}

	void ParticleSystem::Clear()
	{
		m_particles.clear();
		m_emitters.clear();
		m_forces.clear();
	}

	void ParticleSystem::Update(float dt)
	{
		if (m_particles.size() < m_maxParticles)
		{
			for (auto& emitter : m_emitters)
			{
				if (emitter && emitter->Active)
				{
					emitter->Update(dt, m_particles);
				}
			}
		}

		if (m_particles.size() > m_maxParticles)
		{
			m_particles.resize(m_maxParticles);
		}

		for (auto& particle : m_particles)
		{
			for (auto& force : m_forces)
			{
				if (force && force->Enabled)
				{
					force->Apply(particle, dt);
				}
			}

			particle.Update(dt);
		}

		m_particles.erase(
			std::remove_if(
				m_particles.begin(),
				m_particles.end(),
				[](const Particle& p) { return !p.IsAlive(); }
			),
			m_particles.end()
		);
	}

	void ParticleSystem::Render(const std::shared_ptr<Texture2D>& defaultTexture)
	{
		for (const auto& particle : m_particles)
		{
			auto tex = particle.Texture ? particle.Texture : defaultTexture;

			Renderer2D::DrawParticle(
				particle.Position,
				particle.Scale,
				particle.Rotation,
				particle.Color,
				tex
			);
		}
	}
}
