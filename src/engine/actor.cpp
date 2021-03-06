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

#include "engine/actor.hpp"
#include "glm.hpp"

Actor::Actor(
    glm::vec2 position,
    float rotation,
    glm::vec2 velocity,
	ActorKind kind
) : m_position(position), 
m_rotation(rotation), 
m_velocity(velocity), 
m_destroy(false), 
m_kind(kind), m_win(false)
{
    // generate guid
    this->m_guid = (guid_t) this;
}

guid_t Actor::GetGUID() const
{
    return this->m_guid;
}

glm::vec2 Actor::GetPosition() const
{
    return this->m_position;
}

float Actor::GetRotation() const
{
    return this->m_rotation;
}

glm::vec2 Actor::GetVelocity() const
{
    return this->m_velocity;
}

void Actor::SetPosition(const glm::vec2 position)
{
    if (!this->m_win)
    {
        this->m_position = position;
    }
}

void Actor::SetRotation(const float rotation)
{
    this->m_rotation = rotation;
}

void Actor::AddRotation(const float rotationDelta)
{
    this->m_rotation += rotationDelta;
}

void Actor::SetVelocity(const glm::vec2 velocity)
{
    if (!this->m_win)
    {
        this->m_velocity = velocity;
    }
}

void Actor::AddVelocity(const glm::vec2 velocityDelta)
{
    if (!this->m_win)
    {
        this->m_velocity += velocityDelta;
        if (glm::length(this->m_velocity) > 1)
        {
            this->m_velocity = glm::normalize(this->m_velocity);
        }
    }
}

void Actor::Destroy()
{
    this->m_destroy = true;
}

void Actor::Update(float deltaTime)
{
    this->m_position += this->m_velocity * deltaTime;
	if (this->GetKind() != ActorKind::Bullet)
		this->m_velocity -= this->m_velocity * 0.5 * deltaTime;
}

bool Actor::GetDestroy() const
{
    return this->m_destroy;
}

void Actor::SetKind(ActorKind kind)
{
	this->m_kind = kind;
}

ActorKind Actor::GetKind() const
{
	return this->m_kind;
}

void Actor::SetWin()
{
    this->m_win = true;
}

bool Actor::GetWin() const
{
    return this->m_win;
}
