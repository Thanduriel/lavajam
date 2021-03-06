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
#include "engine/component.hpp"

class CooldownComponent :
	public Component
{
public:
	CooldownComponent(Actor* actor, bool isActive = true);

	void Process(float deltaTime) override;

	bool GetCooldown() const;

	float GetCurrentCooldown() const;

	void SetCooldown(double time);

private:
	double m_cooldown;
};
