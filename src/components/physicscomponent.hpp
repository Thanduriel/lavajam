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

#include "glm.hpp"
#include "engine/actor.hpp"
#include "engine/component.hpp"

enum class PhysicsShape {
    Triangle,
};

class PhysicsComponent :
	public Component
{
public:
	PhysicsComponent(Actor* actor, PhysicsShape shape, float size, ActorKind kind, bool isActive = true);
    
    void Process(float deltaTime) override;
    
    bool Collide(PhysicsComponent& component, glm::vec2& velocityDelta) const;
    
    PhysicsShape GetShape() const;
    float GetSize() const;
    ActorKind GetKind() const;
    
    void SetShape(PhysicsShape shape);
    void SetSize(float size);
    void SetKind(ActorKind kind);
    
private:
    PhysicsShape m_shape;
    float m_size;
    ActorKind m_kind;
};
