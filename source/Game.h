#ifndef GAME_H
#define GAME_H

#include <eng.h>
#include <memory>

class Game : public Eng::Application
{
public:
	Game() = default;
	~Game() override = default;

	bool OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGui() override;
	void OnDestroy() override;

private:
	Eng::Camera2D m_camera;

	Eng::ParticleSystem m_particleSystem;

	std::shared_ptr<Eng::PointEmitter> m_pointEmitter;
	std::shared_ptr<Eng::LineEmitter> m_lineEmitter;
	std::shared_ptr<Eng::AreaEmitter> m_areaEmitter;

	std::shared_ptr<Eng::GravityForce> m_gravityForce;
	std::shared_ptr<Eng::CursorForce> m_cursorForce;

	std::shared_ptr<Eng::Texture2D> m_activeParticleTexture;
	int m_selectedTextureIndex = 0;

	int m_selectedEmitterIndex = 0;
	int m_selectedParticleType = 0;
	Eng::BlendMode m_blendMode = Eng::BlendMode::Additive;
};

#endif
