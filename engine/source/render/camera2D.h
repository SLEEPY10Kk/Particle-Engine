#ifndef ENG_CAMERA2D_H
#define ENG_CAMERA2D_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Eng
{
	class Camera2D
	{
	public:
		Camera2D(float viewportWidth = 1280.0f, float viewportHeight = 720.0f);

		void SetViewportSize(float width, float height);
		float GetViewportWidth() const { return m_viewportWidth; }
		float GetViewportHeight() const { return m_viewportHeight; }

		const glm::vec2& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec2& position);

		float GetZoom() const { return m_zoom; }
		void SetZoom(float zoom);

		float GetRotation() const { return m_rotation; }
		void SetRotation(float rotationDegrees);

		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

		glm::vec2 ScreenToWorld(const glm::vec2& screenPos) const;
		glm::vec2 WorldToScreen(const glm::vec2& worldPos) const;

	private:
		void RecalculateMatrices();

	private:
		float m_viewportWidth = 1280.0f;
		float m_viewportHeight = 720.0f;

		glm::vec2 m_position = glm::vec2(0.0f);
		float m_zoom = 1.0f;
		float m_rotation = 0.0f;

		glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
		glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_inverseViewProjectionMatrix = glm::mat4(1.0f);
	};
}

#endif
