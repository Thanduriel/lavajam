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

#include "engine/scene.hpp"
#include "components/physicscomponent.hpp"
#include "actors/characteractor.hpp"

CharacterActor::CharacterActor(
    uint32_t team,
	float size,
	glm::vec4 color,
	size_t layer,
	glm::vec2 position,
	float rotation,
	glm::vec2 velocity,
	std::vector <int> keymapping,
	bool isActive
) : Actor(
    position,
    rotation,
    velocity,
	ActorKind::Character
), m_physics(this, PhysicsShape::Triangle, size),
    m_draw(this, DrawShape::Triangle, size, color, layer),
    m_controller(this, keymapping, isActive),
    m_team(team)
{}

void CharacterActor::Register(Scene& scene)
{
    scene.AddComponent(this->m_physics);
    scene.AddComponent(this->m_draw);
    scene.AddComponent(this->m_controller);
}

const DrawComponent& CharacterActor::GetDrawComponent() const
{
    return this->m_draw;
}

uint32_t CharacterActor::GetTeam() const
{
    return this->m_team;
}

void CharacterActor::SetTeam(uint32_t team)
{
    this->m_team = team;
}
