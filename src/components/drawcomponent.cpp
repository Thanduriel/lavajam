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

#include "components/drawcomponent.hpp"
#include "engine/scene.hpp"
#include "glm.hpp"

DrawComponent::DrawComponent(
    Actor* actor,
    DrawShape shape,
    float size,
    glm::vec4 color,
    size_t layer,
    bool isActive
) : Component(actor, isActive), m_shape(shape), m_size(size), m_color(color), m_layer(layer)
{}

void DrawComponent::Process(float deltaTime)
{
	Vertex v {
        this->m_actor->GetPosition(),
        this->m_color,
        this->m_actor->GetRotation(),
		m_size
    };
    
	VertexBuffer->Add(v);
}

DrawShape DrawComponent::GetShape() const
{
    return this->m_shape;
}

float DrawComponent::GetSize() const
{
    return this->m_size;
}

glm::vec4 DrawComponent::GetColor() const
{
    return this->m_color;
}

size_t DrawComponent::GetLayer() const
{
    return this->m_layer;
}

void DrawComponent::SetShape(DrawShape shape)
{
    this->m_shape = shape;
}

void DrawComponent::SetSize(float size)
{
    this->m_size = size;
}

void DrawComponent::SetColor(glm::vec4 color)
{
    this->m_color = color;
}

void DrawComponent::SetLayer(size_t layer)
{
    this->m_layer = layer;
}
