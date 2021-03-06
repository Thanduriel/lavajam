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

#include "components/aicontrollercomponent.hpp"

AiControllerComponent::AiControllerComponent(
    Actor* actor, Actor* target, uint32_t team, bool isActive
) : CooldownComponent(actor, isActive), m_target(target), m_team(team)
{}

void AiControllerComponent::Process(float deltaTime)
{
	CooldownComponent::Process(deltaTime);
    auto target = this->GetTarget();
    auto me = this->m_actor;
    auto target_pos = target->GetPosition();
    auto my_pos = me->GetPosition();
    auto dir = glm::normalize(target_pos - my_pos) * deltaTime / 2.0f;
    auto new_v = me->GetVelocity() + dir;
    auto angle = glm::orientedAngle(glm::vec2(0, -1), glm::normalize(new_v));
    auto old_rot = me->GetRotation();
    auto new_rot = (angle - old_rot) * deltaTime * 5;
    
    me->AddRotation(new_rot);
    me->SetVelocity(new_v);
}

Actor* AiControllerComponent::GetTarget()
{
    return this->m_target;
}

uint32_t AiControllerComponent::GetTeam() const
{
    return this->m_team;
}

void AiControllerComponent::SetTarget(Actor* target)
{
    this->m_target = target;
}

void AiControllerComponent::SetTeam(uint32_t team)
{
    this->m_team = team;
}
