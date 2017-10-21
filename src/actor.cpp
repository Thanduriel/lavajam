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

#include "actor.hpp"

#include <cassert>

Actor::Actor(
    ActorKind kind,
    ActorShape shape,
    float size,
    glm::vec2 position,
    float rotation
) : m_kind(kind), m_shape(shape), m_size(size), m_position(position), m_rotation(rotation), m_destroy(false)
{
    assert(size != 0);
    
    // generate guid
    this->m_guid = (guid_t) this;
}

Actor::Actor(const Actor& other)
{
    this->m_kind = other.m_kind;
    this->m_shape = other.m_shape;
    this->m_size = other.m_size;
    this->m_position = other.m_position;
    this->m_rotation = other.m_rotation;
}

ActorKind Actor::GetKind() const
{
    return this->m_kind;
}

guid_t Actor::GetGUID() const
{
    return this->m_guid;
}

ActorShape Actor::GetShape() const
{
    return this->m_shape;
}

float Actor::GetSize() const
{
    return this->m_size;
}

glm::vec2 Actor::GetPosition() const
{
    return this->m_position;
}

float Actor::GetRotation() const
{
    return this->m_rotation;
}

void Actor::SetKind(const ActorKind kind)
{
    this->m_kind = kind;
}

void Actor::SetShape(const ActorShape shape)
{
    this->m_shape = shape;
}

void Actor::SetSize(const float size)
{
    this->m_size = size;
}

void Actor::SetPosition(const glm::vec2 position)
{
    this->m_position = position;
}

void Actor::SetRotation(const float rotation)
{
    this->m_rotation = rotation;
}

void Actor::Destroy()
{
    this->m_destroy = true;
}

bool Actor::GetDestroy() const
{
    return this->m_destroy;
}
