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

#include <ctime>
#include <cstdlib>
#include <cassert>

Actor::Actor(
    ActorKind kind,
    ActorShape shape,
    float size,
    glm::vec2 position,
    float rotation
) : kind(kind), shape(shape), size(size), position(position), rotation(rotation)
{
    assert(size != 0);
    
    // generate guid
    std::srand(std::time(0));
    this->guid = std::rand();
}

Actor::Actor(const Actor& other)
{
    this->kind = other.kind;
    this->shape = other.shape;
    this->size = other.size;
    this->position = other.position;
    this->rotation = other.rotation;
}

ActorKind Actor::GetKind()
{
    return this->kind;
}

guid_t Actor::GetGUID()
{
    return this->guid;
}

ActorShape Actor::GetShape()
{
    return this->shape;
}

float Actor::GetSize()
{
    return this->size;
}

glm::vec2 Actor::GetPosition()
{
    return this->position;
}

float Actor::GetRotation()
{
    return this->rotation;
}

void Actor::SetKind(ActorKind& kind)
{
    this->kind = kind;
}

void Actor::SetShape(ActorShape& shape)
{
    this->shape = shape;
}

void Actor::SetSize(float& size)
{
    this->size = size;
}

void Actor::SetPosition(glm::vec2& position)
{
    this->position = position;
}

void Actor::SetRotation(float& rotation)
{
    this->rotation = rotation;
}
