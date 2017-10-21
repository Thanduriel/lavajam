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
#include "glm/glm.hpp"

enum class PhysicsShape {
    Triangle,
};

class PhysicsComponent :
	public Component
{
public:
	PhysicsComponent(Actor* actor, PhysicsShape shape, float size, ActorKind kind, bool isActive = true);
    
    void Process(float deltaTime) override;
    
    bool CollidesWithPoint(glm::vec2 point) const;
    bool CollidesWithLine(glm::vec2 start, glm::vec2 end) const;
    
private:
    PhysicsShape m_shape;
    float m_size;
    ActorKind m_kind;
};
