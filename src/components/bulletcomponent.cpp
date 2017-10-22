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

#include "components/bulletcomponent.hpp"
#include "actors/bulletactor.hpp"

BulletComponent::BulletComponent(Actor* actor, bool isActive) :
	Component(actor, isActive), m_collidedComponent(nullptr)
{}

void BulletComponent::Collided(const PhysicsComponent* component)
{
	if (this->m_collidedComponent == nullptr)
		this->m_collidedComponent = component;
	else if (this->m_collidedComponent != component)
		this->GetActor()->Destroy();
}

void BulletComponent::Process(float deltaTime)
{
    this->GetActor()->AddRotation(0.01f);
}
