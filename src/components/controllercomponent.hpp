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

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>
#include "actors/bulletactor.hpp"
#include "cooldowncomponent.hpp"
#include "engine/actor.hpp"

enum struct FireMode{
	Simple,
	Double,
	Spread,
	COUNT
};

class ControllerComponent :
    public CooldownComponent
{
public:
    ControllerComponent(Actor* actor, std::vector <int> keyMapping = { GLFW_KEY_W,GLFW_KEY_A,GLFW_KEY_S,GLFW_KEY_D,GLFW_KEY_SPACE }, bool isActive = true);

    void Process(float deltaTime) override;

	void SetFireMode(FireMode _fireMode) { m_fireMode = _fireMode; }
private:
	void FireBullet(float _speed, glm::vec2 _offset, float _rot);
	FireMode m_fireMode;

    std::vector <int> m_keyMapping;
	float m_particleCooldown;
};
