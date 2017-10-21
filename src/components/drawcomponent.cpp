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

#include "drawcomponent.hpp"

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
    // TBD
}
