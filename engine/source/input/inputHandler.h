#ifndef ENG_INPUT_HANDLER_H
#define ENG_INPUT_HANDLER_H

#include <array>
#include <glm/vec2.hpp>

namespace Eng
{
	class InputHandler
	{
	public:
		InputHandler() = default;

		bool IsKeyPressed(int key) const;
		bool IsKeyDown(int key) const { return IsKeyPressed(key); }

		bool IsMouseButtonPressed(int button) const;
		bool IsMouseButtonDown(int button) const { return IsMouseButtonPressed(button); }

		glm::vec2 GetMousePosition() const { return m_mousePosition; }
		glm::vec2 GetMouseDelta() const { return m_mouseDelta; }
		bool IsMousePositionChanged() const { return m_mousePositionChanged; }

		glm::vec2 GetMouseScroll() const { return m_mouseScroll; }

	public:
		void SetKeyPressed(int key, bool pressed);
		void SetMouseButtonPressed(int button, bool pressed);
		void SetMousePosition(const glm::vec2& position);
		void SetMouseScroll(const glm::vec2& scrollOffset);
		void EndFrame();

	private:
		std::array<bool, 512> m_keys = { false };
		std::array<bool, 16> m_mouseButtons = { false };
		glm::vec2 m_mousePosition = glm::vec2(0.0f);
		glm::vec2 m_lastMousePosition = glm::vec2(0.0f);
		glm::vec2 m_mouseDelta = glm::vec2(0.0f);
		glm::vec2 m_mouseScroll = glm::vec2(0.0f);
		bool m_mousePositionChanged = false;
	};
}

#endif
