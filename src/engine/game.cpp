#include "graphic/device.hpp"
#include "graphic/effect.hpp"
#include "input/keymanager.hpp"
#include "engine/camera.hpp"
#include "engine/game.hpp"
#include "actors/characteractor.hpp"
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
    
    Actor* character = new CharacterActor(glm::vec2(0, 0), 0.0f, glm::vec2(0, 0));
    this->defaultScene.AddActor(*character);
}

Game::~Game()
{
	Device::Exit();
}

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;
	float rotation;
};

const std::vector<Vertex> vertices = {
	{ { 0.0f, -0.5f },{ 1.0f, 0.0f, 0.0f },0.f },
	{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f },0.f },
	{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f },0.f },

	{ { 0.2f, -0.6f },{ 1.0f, 1.0f, 0.0f },0.f },
	{ { 0.7f, 0.4f },{ 0.0f, 1.0f, 0.0f },0.f },
	{ { -0.3f, 0.4f },{ 1.0f, 0.0f, 1.0f },0.f }
};

void Game::Run()
{
	VertexBuffer<Vertex> vertexBuffer({ VertexFormat::VEC2, VertexFormat::VEC3, VertexFormat::FLOAT });

	using namespace glm;
	vertexBuffer.Add(vertices[0]);
	vertexBuffer.Add(vertices[1]);
	vertexBuffer.Add(vertices[2]);

	vertexBuffer.Add(vertices[3]);
	vertexBuffer.Add(vertices[4]);
	vertexBuffer.Add(vertices[5]);
	vertexBuffer.Upload();
	Effect effect("shaders/vert.spv", "shaders/frag.spv", vertexBuffer);
	Device::SetEffect(effect);
    
    double lastTime = 0;

	while (!glfwWindowShouldClose(Graphic::Device::GetWindow())) 
	{
        double currentTime = glfwGetTime();
        float deltaTime = (float) (currentTime - lastTime);
        lastTime = currentTime;
        this->defaultScene.Update(deltaTime);
		Device::Draw();
		Input::KeyManager::pollEvents();
	}
}
