#ifndef ENG_APPLICATION_H
#define ENG_APPLICATION_H

namespace Eng
{
	class Application
	{
	public:
		virtual ~Application() = default;

		virtual bool OnInit() { return true; }

		virtual void OnUpdate(float deltaTime) { (void)deltaTime; }

		virtual void OnRender() {}

		virtual void OnImGui() {}

		virtual void OnDestroy() {}

		void Close() { m_shouldClose = true; }
		bool ShouldClose() const { return m_shouldClose; }

	private:
		bool m_shouldClose = false;
	};
}

#endif
