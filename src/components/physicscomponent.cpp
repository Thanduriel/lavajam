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

#include "glm.hpp"
#include "components/physicscomponent.hpp"

#include <cassert>

PhysicsComponent::PhysicsComponent(
    Actor* actor,
    PhysicsShape shape,
    float size,
    ActorKind kind,
    bool isActive
) : Component(actor, isActive), m_shape(shape), m_size(size), m_kind(kind)
{}

float sign (glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

void PhysicsComponent::Process(float deltaTime)
{
    // nothing to do here
}

bool PhysicsComponent::Collide(PhysicsComponent& component, glm::vec2& velocityDelta) const
{
    switch(this->m_shape)
    {
        case PhysicsShape::Triangle:
        {
            if (component.GetShape() != PhysicsShape::Triangle)
            {
                // we don't know
                return false;
            }
            
            glm::vec2 center = component.GetActor()->GetPosition();
            float radius = component.GetSize();
            glm::vec2 my_center = this->m_actor->GetPosition();
            float my_radius = this->m_size * 1.5f;
            
            bool collides = glm::distance(center, my_center) <= my_radius + radius;
            if (collides)
            {
				auto direction = my_center - center;
				if (direction.x != 0 || direction.y != 0) velocityDelta = glm::normalize(direction) / glm::length(direction);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        default:
        {
            // we don't know
            return false;
        }
    }
}

PhysicsShape PhysicsComponent::GetShape() const
{
    return this->m_shape;
}

float PhysicsComponent::GetSize() const
{
    return this->m_size;
}

ActorKind PhysicsComponent::GetKind() const
{
    return this->m_kind;
}

void PhysicsComponent::SetShape(PhysicsShape shape)
{
    this->m_shape = shape;
}

void PhysicsComponent::SetSize(float size)
{
    this->m_size = size;
}

void PhysicsComponent::SetKind(ActorKind kind)
{
    this->m_kind = kind;
}
