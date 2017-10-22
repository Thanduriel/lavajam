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

#include "cooldowncomponent.hpp"

CooldownComponent::CooldownComponent(Actor* actor, bool isActive) :
	Component(actor, isActive)
{

}

void CooldownComponent::Process(float deltaTime)
{
	if (this->m_cooldown > 0)
	{
		this->m_cooldown -= deltaTime;
	}
}

void CooldownComponent::SetCooldown(double time)
{
	this->m_cooldown = time;
}

bool CooldownComponent::GetCooldown() const
{
	return this->m_cooldown <= 0;
}
