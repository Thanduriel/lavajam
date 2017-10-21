#include "graphic/device.hpp"
#include "graphic/effect.hpp"
#include "input/keymanager.hpp"
#include "engine/camera.hpp"
#include "engine/game.hpp"
#include "actors/characteractor.hpp"
#include "components/drawcomponent.hpp"
#include "glm.hpp"

#include <iostream>

//test
#include "graphic/vertexbuffer.hpp"

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
    
    glm::vec4 blue(0.129411765f, 0.588235294f, 0.952941176f, 1);
    Actor* character = new CharacterActor(0.1f, blue, 0, glm::vec2(0, 0), 0.0f, glm::vec2(0, 0));
    this->defaultScene.AddActor(*character);
}

Game::~Game()
{
	Device::Exit();
}

void Game::Run()
{
    DrawComponent::InitializeContext();
    
    double lastTime = 0;

	while (!glfwWindowShouldClose(Graphic::Device::GetWindow())) 
	{
        double currentTime = glfwGetTime();
        float deltaTime = (float) (currentTime - lastTime);
        lastTime = currentTime;
        this->defaultScene.Update(deltaTime);

		static float shift = 0.f;
		shift += deltaTime * 0.2f;
        
		Input::KeyManager::pollEvents();
	}
}
