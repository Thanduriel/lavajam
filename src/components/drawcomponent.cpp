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

GraphicContext& DrawComponent::GetContext()
{
    using namespace Graphic;
    static VertexBuffer<Vertex> vertexBuffer({ VertexFormat::VEC2, VertexFormat::VEC3, VertexFormat::FLOAT });
    static Effect effect("shaders/vert.spv", "shaders/frag.spv", vertexBuffer);
    static GraphicContext ctx({ vertexBuffer, effect });
    return ctx;
}

void DrawComponent::InitializeContext()
{
    GraphicContext& ctx = DrawComponent::GetContext();
    Graphic::Device::SetEffect(ctx.Effect);
}

void DrawComponent::DrawContext()
{
    GraphicContext& ctx = DrawComponent::GetContext();
    ctx.VertexBuffer.Upload();
    Graphic::Device::Draw(ctx.VertexBuffer);
}

void DrawComponent::Process(float deltaTime)
{
    GraphicContext& ctx = DrawComponent::GetContext();
    
	Vertex v {
        this->m_actor->GetPosition() + glm::vec2(0.f, -this->m_size),
        this->m_color,
        this->m_actor->GetRotation()
    };
    
	ctx.VertexBuffer.Add(v);
	v.position = this->m_actor->GetPosition() + glm::vec2(this->m_size/2, this->m_size);
	ctx.VertexBuffer.Add(v);
	v.position = this->m_actor->GetPosition() + glm::vec2(-this->m_size/2, this->m_size);
	ctx.VertexBuffer.Add(v);
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
