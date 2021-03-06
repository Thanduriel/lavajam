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

#include "engine/component.hpp"

Component::Component(Actor* actor, bool isActive) :
    m_actor(actor), m_isActive(isActive), m_spawnCallback(nullptr)
{}

Actor* Component::GetActor() const
{
    return this->m_actor;
}

bool Component::IsActive() const
{
    return this->m_isActive;
}

void Component::SetActor(Actor* actor)
{
    this->m_actor = actor;
}

void Component::SetActive(bool isActive)
{
    this->m_isActive = isActive;
}

void Component::SetSpawnCallback(std::function<void(Actor* bullet)> callback)
{
	this->m_spawnCallback = callback;
}