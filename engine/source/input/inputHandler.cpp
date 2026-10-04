#include "input/inputHandler.h"

namespace Eng
{
	void InputHandler::SetKeyPressed(int key, bool pressed)
	{
		if (key < 0 || key >= static_cast<int>(m_keys.size()))
		{
			return;
		}
		m_keys[key] = pressed;
	}

	bool InputHandler::IsKeyPressed(int key) const
	{
		if (key < 0 || key >= static_cast<int>(m_keys.size()))
		{
			return false;
		}
		return m_keys[key];
	}

	void InputHandler::SetMouseButtonPressed(int button, bool pressed)
	{
		if (button < 0 || button >= static_cast<int>(m_mouseButtons.size()))
		{
			return;
		}
		m_mouseButtons[button] = pressed;
	}

	bool InputHandler::IsMouseButtonPressed(int button) const
	{
		if (button < 0 || button >= static_cast<int>(m_mouseButtons.size()))
		{
			return false;
		}
		return m_mouseButtons[button];
	}

	void InputHandler::SetMousePosition(const glm::vec2& position)
	{
		m_mouseDelta = position - m_mousePosition;
		m_mousePosition = position;
		m_mousePositionChanged = true;
	}

	void InputHandler::SetMouseScroll(const glm::vec2& scrollOffset)
	{
		m_mouseScroll = scrollOffset;
	}

	void InputHandler::EndFrame()
	{
		m_mouseDelta = glm::vec2(0.0f);
		m_mouseScroll = glm::vec2(0.0f);
		m_mousePositionChanged = false;
	}
}
