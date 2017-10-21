#pragma once

#include "engine/scene.hpp"

class Game 
{
public:
	Game();
	~Game();

	void Run();

private:
    Scene m_defaultScene;
};
