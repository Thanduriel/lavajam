#include "game.hpp"
#include "graphic/device.hpp"
#include "graphic/effect.hpp"
#include "input/keymanager.hpp"

using namespace Graphic;

Game::Game()
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

	while (!glfwWindowShouldClose(Graphic::Device::GetWindow())) 
	{
		Device::Draw();
		glfwPollEvents();
	}
}