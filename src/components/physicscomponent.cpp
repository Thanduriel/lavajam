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

#include "components/physicscomponent.hpp"

PhysicsComponent::PhysicsComponent(
    Actor* actor,
    PhysicsShape shape,
    float size,
    ActorKind kind,
    bool isActive
) : Component(actor, isActive), m_shape(shape), m_size(size), m_kind(kind)
{}

void PhysicsComponent::Process(float deltaTime)
{
    // nothing to do here
}

bool PhysicsComponent::CollidesWithPoint(glm::vec2 point) const
{
    // TBD
    return false;
}

bool PhysicsComponent::CollidesWithLine(glm::vec2 start, glm::vec2 end) const
{
    // TBD
    return false;
}
