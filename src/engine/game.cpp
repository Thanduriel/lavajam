#include "graphic/device.hpp"
#include "graphic/effect.hpp"
#include "input/keymanager.hpp"
#include "engine/camera.hpp"
#include "engine/game.hpp"
#include "actors/characteractor.hpp"
#include "actors/aiactor.hpp"
#include "components/drawcomponent.hpp"
#include "glm.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace Graphic;

Game::Game() : m_defaultScene(Camera(
    glm::radians(90.0f),
    16.0f / 9.0f,
    glm::vec3(0, 0, 0),
    glm::quat(glm::vec3(0, 0, 0))
))
{
	Device::Init(1366, 768);
	Input::KeyManager::Init(Device::GetWindow());
    
	std::vector <int> keyMapping1 = { GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE };
	std::vector <int> keyMapping2 = { GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_RIGHT, GLFW_KEY_ENTER };
	glm::vec4 blue(0.129411765f, 0.588235294f, 0.952941176f, 1);
	glm::vec4 lime(0.796078431f, 0.862745098f, 0.011764706f, 1);

	std:srand(std::time(0));
	SpawnTeam(blue, keyMapping1);
	SpawnTeam(lime, keyMapping2);
}

void Game::SpawnTeam(const glm::vec4 color, std::vector <int> keyMapping)
{
	Actor* character = new CharacterActor(0.02f, color, 0, glm::vec2(color.x, color.y), 0.0f, glm::vec2(0, 0.01f), keyMapping);
	this->m_defaultScene.AddActor(*character);

	const auto colorAi = glm::vec4(color.x/1.333f, color.y/1.333f, color.z/1.333f, 1);
	for (auto i = 0; i < 10; i++)
	{
		auto x = std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
		auto y = std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
		auto r = std::rand() / static_cast<float>(RAND_MAX) * 2 * glm::pi<float>();

		Actor* ai = new AiActor(character, 0.01f, colorAi, 0, glm::vec2(x, -y), r, glm::vec2(0, 0.01f));
		this->m_defaultScene.AddActor(*ai);
	}
}


Game::~Game()
{
    this->m_defaultScene.Destroy();
	Device::Exit();
}

void Game::Run()
{
    this->m_defaultScene.Initialize();
    
    double lastTime = 0;

	while (!glfwWindowShouldClose(Graphic::Device::GetWindow())) 
	{
        double currentTime = glfwGetTime();
        float deltaTime = (float) (currentTime - lastTime);
        lastTime = currentTime;
        this->m_defaultScene.Update(deltaTime);

		static float shift = 0.f;
		shift += deltaTime * 0.2f;
        
		Input::KeyManager::pollEvents();
	}
}
