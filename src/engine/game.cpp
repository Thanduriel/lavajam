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

//test
#include "graphic/vertexbuffer.hpp"

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
    
    glm::vec4 blue(0.129411765f, 0.588235294f, 0.952941176f, 1);
    Actor* character = new CharacterActor(0.1f, blue, 0, glm::vec2(0, 0), 0.0f, glm::vec2(0, 0));
    glm::vec4 lime(0.796078431f, 0.862745098f, 0.011764706f, 1);
    Actor* ai = new AiActor(0.05f, lime, 0, glm::vec2(0.5, -0.25), 0.4f, glm::vec2(0, -0.000001f));
    this->m_defaultScene.AddActor(*character);
    this->m_defaultScene.AddActor(*ai);
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
