#include <eng.h>
#include "Game.h"
#include <memory>

int main()
{
	Eng::Engine& engine = Eng::Engine::GetInstance();
	engine.SetApplication(std::make_unique<Game>());

	if (!engine.Init(1280, 720, "Particle Engine"))
	{
		return -1;
	}

	engine.Run();
	engine.Destroy();

	return 0;
}
