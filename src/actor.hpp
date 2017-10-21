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

#include "glm/glm.hpp"

typedef uint64_t guid_t;

class Scene;

enum class ActorKind {
    Character,
    Beam,
};

enum class ActorShape {
    Triangle,
    Rectangle,
};

class Actor
{
public:
    Actor(
        ActorKind kind,
        ActorShape shape,
        float size,
        glm::vec2 position,
        float rotation
    );
    Actor(const Actor& other);
    
    ActorKind GetKind() const;
    guid_t GetGUID() const;
    ActorShape GetShape() const;
    float GetSize() const;
    glm::vec2 GetPosition() const;
    float GetRotation() const;
    bool GetDestroy() const;
    
    void SetKind(const ActorKind kind);
    void SetShape(const ActorShape shape);
    void SetSize(const float size);
    void SetPosition(const glm::vec2 position);
    void SetRotation(const float rotation);
    
    void Destroy();
    
    virtual void Register(Scene& scene) = 0;
    
private:
    ActorKind m_kind;
    guid_t m_guid;
    ActorShape m_shape;
    float m_size;
    glm::vec2 m_position;
    float m_rotation;
    bool m_destroy;
};
