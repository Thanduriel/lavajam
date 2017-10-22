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

#include "components/physicscomponent.hpp"
#include "actors/bulletactor.hpp"

PhysicsComponent::PhysicsComponent(
    Actor* actor,
    PhysicsShape shape,
    float size,
    bool isActive
) : Component(actor, isActive), m_shape(shape), m_size(size)
{}

float sign (glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

void PhysicsComponent::Process(float deltaTime)
{
    // nothing to do here
}

bool PhysicsComponent::Collide(const PhysicsComponent& component, glm::vec2 ownVelocityDelta, glm::vec2 otherVelocityDelta) const
{
    switch(this->m_shape)
    {
        case PhysicsShape::Triangle:
        {
            if (component.GetShape() != PhysicsShape::Triangle)
            {
                // we don't know
                return false;
            }

	        const glm::vec2 center = component.GetActor()->GetPosition();
	        const glm::vec2 my_center = this->m_actor->GetPosition();

	        const bool collides = glm::distance2(center, my_center) <= this->m_size * m_size * 1.5f + component.GetSize()*component.GetSize() * 1.5f;
            if (collides)
            {
				ownVelocityDelta = { 0,0 };
	            const auto direction = my_center - center;
				if (direction.x != 0 || direction.y != 0)
				{
					ownVelocityDelta = glm::normalize(direction) / glm::length(direction);
				}

				otherVelocityDelta = -ownVelocityDelta;
	            const float ratio = this->GetMass() / component.GetMass();
				ownVelocityDelta /= ratio;
				otherVelocityDelta *= ratio;

				auto registerCollision = [](Actor* actor, const PhysicsComponent* other)
				{
					BulletActor* bullet = static_cast<BulletActor*>(actor);
					bullet->GetBulletComponent().Collided(other);
					if (other->GetActor()->GetKind() == ActorKind::Ai)
					{
						other->GetActor()->Destroy();
					}
				};

				if (this->GetActor()->GetKind() == ActorKind::Bullet)
				{
					registerCollision(this->GetActor(), &component);
				}
				else if (component.GetActor()->GetKind() == ActorKind::Bullet)
				{
					registerCollision(component.GetActor(), this);
				}

                return true;
            }
            else
            {
                return false;
            }
        }
        
        default:
        {
            // we don't know
            return false;
        }
    }
}

PhysicsShape PhysicsComponent::GetShape() const
{
    return this->m_shape;
}

float PhysicsComponent::GetSize() const
{
    return this->m_size;
}

float PhysicsComponent::GetMass() const
{
	switch (this->GetActor()->GetKind()) 
	{ 
		case ActorKind::Ai: return 1.f;
		case ActorKind::Character: return 1.5f;
		case ActorKind::Bullet: return 0.02f;
		default: ;
	}
}


void PhysicsComponent::SetShape(PhysicsShape shape)
{
    this->m_shape = shape;
}

void PhysicsComponent::SetSize(float size)
{
    this->m_size = size;
}
