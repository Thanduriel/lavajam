#include "game.hpp"
#include "graphic/device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using namespace Graphic;

Game::Game()
{
	Device::Init();
}

Game::~Game()
{
	Device::Exit();
}

void Game::Run()
{
	while (!glfwWindowShouldClose(Graphic::Device::GetWindow())) 
	{
		glfwPollEvents();
	}
}