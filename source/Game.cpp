#include "Game.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool Game::OnInit()
{
	m_camera.SetViewportSize(1280.0f, 720.0f);
	m_camera.SetPosition(glm::vec2(0.0f, 0.0f));
	m_camera.SetZoom(1.0f);

	m_activeParticleTexture = Eng::TextureManager::Get().GetDefaultParticleTexture();

	m_pointEmitter = std::make_shared<Eng::PointEmitter>(glm::vec2(0.0f, 150.0f));
	m_pointEmitter->EmissionRate = 50.0f;
	m_pointEmitter->BaseAngle = 1.570796f;
	m_pointEmitter->AngleSpread = 0.8f;
	m_pointEmitter->SpeedMin = 100.0f;
	m_pointEmitter->SpeedMax = 300.0f;
	m_pointEmitter->SetTexture(m_activeParticleTexture);

	m_lineEmitter = std::make_shared<Eng::LineEmitter>(glm::vec2(-300.0f, 250.0f), glm::vec2(300.0f, 250.0f));
	m_lineEmitter->EmissionRate = 60.0f;
	m_lineEmitter->SpeedMin = 40.0f;
	m_lineEmitter->SpeedMax = 120.0f;
	m_lineEmitter->Active = false;
	m_lineEmitter->SetTexture(m_activeParticleTexture);

	m_areaEmitter = std::make_shared<Eng::AreaEmitter>(glm::vec2(0.0f, 0.0f), glm::vec2(150.0f, 80.0f));
	m_areaEmitter->EmissionRate = 60.0f;
	m_areaEmitter->OutwardVelocity = true;
	m_areaEmitter->SpeedMin = 60.0f;
	m_areaEmitter->SpeedMax = 180.0f;
	m_areaEmitter->Active = false;
	m_areaEmitter->SetTexture(m_activeParticleTexture);

	m_particleSystem.AddEmitter(m_pointEmitter);
	m_particleSystem.AddEmitter(m_lineEmitter);
	m_particleSystem.AddEmitter(m_areaEmitter);

	m_gravityForce = std::make_shared<Eng::GravityForce>(glm::vec2(0.0f, -350.0f));
	m_cursorForce = std::make_shared<Eng::CursorForce>();
	m_cursorForce->Strength = 800.0f;
	m_cursorForce->Radius = 300.0f;
	m_cursorForce->Mode = Eng::CursorMode::Attract;

	m_particleSystem.AddForce(m_gravityForce);
	m_particleSystem.AddForce(m_cursorForce);

	return true;
}

void Game::OnUpdate(float deltaTime)
{
	auto& input = Eng::Engine::GetInstance().GetInputHandler();

	glm::vec2 scroll = input.GetMouseScroll();
	if (scroll.y != 0.0f)
	{
		m_camera.SetZoom(m_camera.GetZoom() * (1.0f + scroll.y * 0.1f));
	}

	if (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		glm::vec2 delta = input.GetMouseDelta();
		glm::vec2 camPos = m_camera.GetPosition();
		camPos.x -= delta.x / m_camera.GetZoom();
		camPos.y += delta.y / m_camera.GetZoom();
		m_camera.SetPosition(camPos);
	}

	glm::vec2 mouseScreen = input.GetMousePosition();
	glm::vec2 mouseWorld = m_camera.ScreenToWorld(mouseScreen);
	m_cursorForce->CursorPosition = mouseWorld;

	if (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		m_cursorForce->Mode = Eng::CursorMode::Repel;
		m_cursorForce->Enabled = true;
	}
	else if (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		m_cursorForce->Mode = Eng::CursorMode::Attract;
		m_cursorForce->Enabled = true;
	}

	Eng::ParticleType currentType = static_cast<Eng::ParticleType>(m_selectedParticleType);
	m_pointEmitter->TypeToEmit = currentType;
	m_lineEmitter->TypeToEmit = currentType;
	m_areaEmitter->TypeToEmit = currentType;

	m_pointEmitter->Active = (m_selectedEmitterIndex == 0);
	m_lineEmitter->Active = (m_selectedEmitterIndex == 1);
	m_areaEmitter->Active = (m_selectedEmitterIndex == 2);

	m_pointEmitter->SetTexture(m_activeParticleTexture);
	m_lineEmitter->SetTexture(m_activeParticleTexture);
	m_areaEmitter->SetTexture(m_activeParticleTexture);

	m_particleSystem.Update(deltaTime);
}

void Game::OnRender()
{
	Eng::Renderer2D::Clear(glm::vec4(0.06f, 0.06f, 0.08f, 1.0f));

	Eng::Renderer2D::Begin(m_camera);
	Eng::Renderer2D::SetBlendMode(m_blendMode);

	m_particleSystem.Render(m_activeParticleTexture);

	if (m_cursorForce->Enabled)
	{
		glm::vec4 cursorColor = (m_cursorForce->Mode == Eng::CursorMode::Attract)
			? glm::vec4(0.2f, 0.8f, 1.0f, 0.4f)
			: glm::vec4(1.0f, 0.3f, 0.3f, 0.4f);

		Eng::Renderer2D::DrawCircle(m_cursorForce->CursorPosition, m_cursorForce->Radius, cursorColor, 32, 1.5f);
		Eng::Renderer2D::DrawCircle(m_cursorForce->CursorPosition, 6.0f, cursorColor, 12, 2.0f);
	}

	if (m_pointEmitter->Active)
	{
		Eng::Renderer2D::DrawCircle(m_pointEmitter->Position, 8.0f, glm::vec4(1.0f, 1.0f, 0.2f, 0.8f), 16, 2.0f);
	}
	else if (m_lineEmitter->Active)
	{
		Eng::Renderer2D::DrawLine(m_lineEmitter->PointA, m_lineEmitter->PointB, glm::vec4(1.0f, 1.0f, 0.2f, 0.8f), 3.0f);
	}
	else if (m_areaEmitter->Active)
	{
		if (m_areaEmitter->Shape == Eng::AreaShape::Box)
		{
			Eng::Renderer2D::DrawRect(m_areaEmitter->Center, m_areaEmitter->HalfExtents * 2.0f, glm::vec4(1.0f, 1.0f, 0.2f, 0.8f), 2.0f);
		}
		else
		{
			Eng::Renderer2D::DrawCircle(m_areaEmitter->Center, m_areaEmitter->Radius, glm::vec4(1.0f, 1.0f, 0.2f, 0.8f), 32, 2.0f);
		}
	}

	Eng::Renderer2D::End();
}

void Game::OnImGui()
{
	ImGui::Begin("Emitters & Particles");
	{
		ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Emitter Selection");
		const char* emitterTypes[] = { "Point Emitter", "Line Emitter", "Area Emitter" };
		ImGui::Combo("Emitter Type", &m_selectedEmitterIndex, emitterTypes, 3);

		const char* particleTypes[] = { "Solid", "Liquid", "Gas" };
		ImGui::Combo("Particle Type", &m_selectedParticleType, particleTypes, 3);

		if (m_selectedEmitterIndex == 0)
		{
			ImGui::DragFloat2("Position", &m_pointEmitter->Position.x, 1.0f);
			ImGui::SliderFloat("Emission Rate", &m_pointEmitter->EmissionRate, 1.0f, 300.0f);
			ImGui::SliderFloat("LifeTime", &m_pointEmitter->LifeTime, 0.5f, 10.0f);
			ImGui::SliderFloat("Cone Angle", &m_pointEmitter->BaseAngle, 0.0f, 6.283f);
			ImGui::SliderFloat("Angle Spread", &m_pointEmitter->AngleSpread, 0.0f, 3.141f);
			ImGui::SliderFloat("Speed Min", &m_pointEmitter->SpeedMin, 10.0f, 500.0f);
			ImGui::SliderFloat("Speed Max", &m_pointEmitter->SpeedMax, 10.0f, 500.0f);
		}
		else if (m_selectedEmitterIndex == 1)
		{
			ImGui::DragFloat2("Point A", &m_lineEmitter->PointA.x, 1.0f);
			ImGui::DragFloat2("Point B", &m_lineEmitter->PointB.x, 1.0f);
			ImGui::SliderFloat("Emission Rate", &m_lineEmitter->EmissionRate, 1.0f, 300.0f);
			ImGui::SliderFloat("LifeTime", &m_lineEmitter->LifeTime, 0.5f, 10.0f);
			ImGui::Checkbox("Emit Along Normal", &m_lineEmitter->EmitAlongNormal);
		}
		else if (m_selectedEmitterIndex == 2)
		{
			ImGui::DragFloat2("Center", &m_areaEmitter->Center.x, 1.0f);
			const char* shapes[] = { "Box", "Circle" };
			int shapeIndex = static_cast<int>(m_areaEmitter->Shape);
			if (ImGui::Combo("Area Shape", &shapeIndex, shapes, 2))
			{
				m_areaEmitter->Shape = static_cast<Eng::AreaShape>(shapeIndex);
			}
			if (m_areaEmitter->Shape == Eng::AreaShape::Box)
			{
				ImGui::DragFloat2("Box Half Size", &m_areaEmitter->HalfExtents.x, 1.0f, 10.0f, 500.0f);
			}
			else
			{
				ImGui::DragFloat("Circle Radius", &m_areaEmitter->Radius, 1.0f, 10.0f, 500.0f);
			}
			ImGui::Checkbox("Outward Velocity", &m_areaEmitter->OutwardVelocity);
			ImGui::SliderFloat("Emission Rate", &m_areaEmitter->EmissionRate, 1.0f, 300.0f);
			ImGui::SliderFloat("LifeTime", &m_areaEmitter->LifeTime, 0.5f, 10.0f);
		}
	}
	ImGui::End();

	ImGui::Begin("Particle Textures");
	{
		ImGui::TextColored(ImVec4(0.9f, 0.5f, 1.0f, 1.0f), "Fixed Procedural Textures");
		const char* textures[] = {
			"Radial Glow",
			"Star / Sparkle",
			"Ring / Halo",
			"Smoke / Cloud",
			"Bubble / Droplet",
			"Solid Square"
		};

		if (ImGui::Combo("Particle Texture", &m_selectedTextureIndex, textures, 6))
		{
			switch (m_selectedTextureIndex)
			{
			case 0:
				m_activeParticleTexture = Eng::TextureManager::Get().GetRadialGlowTexture();
				break;
			case 1:
				m_activeParticleTexture = Eng::TextureManager::Get().GetStarTexture();
				break;
			case 2:
				m_activeParticleTexture = Eng::TextureManager::Get().GetRingTexture();
				break;
			case 3:
				m_activeParticleTexture = Eng::TextureManager::Get().GetSmokeTexture();
				break;
			case 4:
				m_activeParticleTexture = Eng::TextureManager::Get().GetBubbleTexture();
				break;
			case 5:
				m_activeParticleTexture = Eng::TextureManager::Get().GetWhiteTexture();
				break;
			}
		}

		if (m_activeParticleTexture)
		{
			ImGui::Separator();
			ImGui::Text("Current Texture Preview (%dx%d):", m_activeParticleTexture->GetWidth(), m_activeParticleTexture->GetHeight());
			ImGui::Image((ImTextureID)(intptr_t)m_activeParticleTexture->GetID(), ImVec2(80, 80));
		}
	}
	ImGui::End();

	ImGui::Begin("Forces");
	{
		ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "Simulation Forces");
		ImGui::Checkbox("Enable Gravity", &m_gravityForce->Enabled);
		if (m_gravityForce->Enabled)
		{
			ImGui::SliderFloat2("Gravity Vector", &m_gravityForce->Gravity.x, -1000.0f, 1000.0f);
		}

		ImGui::Separator();
		ImGui::Checkbox("Enable Cursor Force", &m_cursorForce->Enabled);
		if (m_cursorForce->Enabled)
		{
			const char* cursorModes[] = { "Attract", "Repel" };
			int modeIdx = static_cast<int>(m_cursorForce->Mode);
			if (ImGui::Combo("Cursor Mode", &modeIdx, cursorModes, 2))
			{
				m_cursorForce->Mode = static_cast<Eng::CursorMode>(modeIdx);
			}
			ImGui::SliderFloat("Force Strength", &m_cursorForce->Strength, 100.0f, 3000.0f);
			ImGui::SliderFloat("Effect Radius", &m_cursorForce->Radius, 50.0f, 800.0f);
		}

		ImGui::Separator();
		const char* blendModes[] = { "Alpha Blending", "Additive (Glow)", "Multiply", "None" };
		int blendIdx = static_cast<int>(m_blendMode);
		if (ImGui::Combo("Blend Mode", &blendIdx, blendModes, 4))
		{
			m_blendMode = static_cast<Eng::BlendMode>(blendIdx);
		}
	}
	ImGui::End();

	ImGui::Begin("Controls & Stats");
	{
		ImGui::TextColored(ImVec4(0.2f, 0.9f, 1.0f, 1.0f), "Mouse & Camera Controls");
		ImGui::BulletText("Left Mouse: Attract particles towards cursor");
		ImGui::BulletText("Right Mouse: Repel particles away from cursor");
		ImGui::BulletText("Middle Mouse + Drag: Pan camera view");
		ImGui::BulletText("Mouse Scroll: Zoom camera in / out");

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Quick Actions");
		if (ImGui::Button("Clear All Particles"))
		{
			m_particleSystem.ClearParticles();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset Camera"))
		{
			m_camera.SetPosition(glm::vec2(0.0f, 0.0f));
			m_camera.SetZoom(1.0f);
		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.6f, 1.0f), "Performance Metrics");
		ImGui::Text("FPS: %.1f  (%.2f ms)", Eng::Engine::GetInstance().GetFPS(), Eng::Engine::GetInstance().GetDeltaTime() * 1000.0f);
		ImGui::Text("Active Particles: %zu / %zu", m_particleSystem.GetParticleCount(), m_particleSystem.GetMaxParticles());

		auto stats = Eng::Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %u  |  Quads: %u", stats.DrawCalls, stats.QuadCount);
		Eng::Renderer2D::ResetStats();
	}
	ImGui::End();
}

void Game::OnDestroy()
{
	m_particleSystem.Clear();
}
