#include "render/camera2D.h"
#include <algorithm>

namespace Eng
{
	Camera2D::Camera2D(float viewportWidth, float viewportHeight)
		: m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight)
	{
		RecalculateMatrices();
	}

	void Camera2D::SetViewportSize(float width, float height)
	{
		m_viewportWidth = std::max(1.0f, width);
		m_viewportHeight = std::max(1.0f, height);
		RecalculateMatrices();
	}

	void Camera2D::SetPosition(const glm::vec2& position)
	{
		m_position = position;
		RecalculateMatrices();
	}

	void Camera2D::SetZoom(float zoom)
	{
		m_zoom = std::max(0.001f, zoom);
		RecalculateMatrices();
	}

	void Camera2D::SetRotation(float rotationDegrees)
	{
		m_rotation = rotationDegrees;
		RecalculateMatrices();
	}

	void Camera2D::RecalculateMatrices()
	{
		float halfWidth = (m_viewportWidth * 0.5f) / m_zoom;
		float halfHeight = (m_viewportHeight * 0.5f) / m_zoom;

		m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
		m_inverseViewProjectionMatrix = glm::inverse(m_viewProjectionMatrix);
	}

	glm::vec2 Camera2D::ScreenToWorld(const glm::vec2& screenPos) const
	{
		float ndcX = (screenPos.x / m_viewportWidth) * 2.0f - 1.0f;
		float ndcY = 1.0f - (screenPos.y / m_viewportHeight) * 2.0f;

		glm::vec4 worldPos = m_inverseViewProjectionMatrix * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
		return glm::vec2(worldPos.x, worldPos.y);
	}

	glm::vec2 Camera2D::WorldToScreen(const glm::vec2& worldPos) const
	{
		glm::vec4 clip = m_viewProjectionMatrix * glm::vec4(worldPos, 0.0f, 1.0f);
		float screenX = (clip.x + 1.0f) * 0.5f * m_viewportWidth;
		float screenY = (1.0f - clip.y) * 0.5f * m_viewportHeight;
		return glm::vec2(screenX, screenY);
	}
}
