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

bool PhysicsComponent::CollidesWithPoint(glm::vec2 point) const
{
    switch(this->m_shape)
    {
        case PhysicsShape::Triangle:
        {
            glm::vec2 v1(-0.5f, 0);
            glm::vec2 v2(0, 1);
            glm::vec2 v3(0.5f, 0);
            bool b1, b2, b3;
            
            v1 *= this->m_size;
            v2 *= this->m_size;
            v3 *= this->m_size;

            b1 = sign(point, v1, v2) < 0.0f;
            b2 = sign(point, v2, v3) < 0.0f;
            b3 = sign(point, v3, v1) < 0.0f;

            return ((b1 == b2) && (b2 == b3));
        }   
        default:
        {
            // we don't know
            return false;
        }
    }
}

bool PhysicsComponent::CollidesWithLine(glm::vec2 start, glm::vec2 end) const
{
    switch(this->m_shape)
    {
        case PhysicsShape::Triangle:
        {
            glm::vec3 orig(start, 0);
            glm::vec3 dir(end, 0);
            glm::vec3 v1(-0.5f, 0, 0);
            glm::vec3 v2(0, 1, 0);
            glm::vec3 v3(0.5f, 0, 0);
            glm::vec3 out(0);
            
            v1 *= this->m_size;
            v2 *= this->m_size;
            v3 *= this->m_size;
            
            return glm::intersectLineTriangle(
                orig, dir,
                v1, v2, v3,
                out
            );
        }
        
        default:
        {
            // we don't know
            return false;
        }
    }
}
