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
#include <functional>

class Component
{
public:
    Component(Actor* actor, bool isActive = true);
    
    Actor* GetActor() const;
    
    bool IsActive() const;
    
    void SetActor(Actor* actor);
    void SetActive(bool isActive);
    
    virtual void Process(float deltaTime) = 0;
	void SetSpawnCallback(std::function<void(Actor* bullet)> callback);

private:
    bool m_isActive;
protected:
    Actor* m_actor;
	std::function<void(Actor* actor)> m_spawnCallback;
    
};
