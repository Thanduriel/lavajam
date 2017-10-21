#include "engine/game.hpp"
#include "graphic/device.hpp"
#include "graphic/effect.hpp"
#include "input/keymanager.hpp"
#include "engine/camera.hpp"
#include "glm/glm.hpp"

using namespace Graphic;

Game::Game() : defaultScene(Camera(
    glm::radians(90.0f),
    16.0f / 9.0f,
    glm::vec3(0, 0, 0),
    glm::quat(glm::vec3(0, 0, 0))
))
{
	Device::Init(1366, 768);
	Input::KeyManager::Init(Device::GetWindow());
}

Game::~Game()
{
	Device::Exit();
}

void Game::Run()
{
	Effect effect("shaders/vert.spv", "shaders/frag.spv");
	Device::SetEffect(effect);
    
    double lastTime = 0;

	while (!glfwWindowShouldClose(Graphic::Device::GetWindow())) 
	{
        double currentTime = glfwGetTime();
        float deltaTime = (float) (currentTime - lastTime);
        lastTime = currentTime;
        this->defaultScene.Update(deltaTime);
		Device::Draw();
		glfwPollEvents();
	}
}
