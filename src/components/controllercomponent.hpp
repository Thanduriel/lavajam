/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2017  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#pragma once

#include "engine/actor.hpp"
#include "engine/component.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class ControllerComponent :
    public Component
{
public:
    ControllerComponent(Actor* actor,bool isActive = true, std::vector <int> keyMapping = {GLFW_KEY_W,GLFW_KEY_A,GLFW_KEY_S,GLFW_KEY_D,GLFW_KEY_SPACE});
    
    void Process(float deltaTime) override;

private:
    std::vector <int> m_keyMapping;
};
