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
	void SpawnTeam(uint32_t team, const glm::vec4 color, std::vector <int> keyMapping);
};
