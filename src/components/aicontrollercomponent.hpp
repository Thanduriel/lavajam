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
#include "cooldowncomponent.hpp"

class AiControllerComponent :
    public CooldownComponent
{
public:
    AiControllerComponent(Actor* actor, Actor* target, uint32_t team, bool isActive = true);
    
    void Process(float deltaTime) override;
    
    const Actor* GetTarget() const;
    uint32_t GetTeam() const;
    
    void SetTarget(Actor* target);
    void SetTeam(uint32_t team);
    
private:
    Actor* m_target;
    uint32_t m_team;
};
