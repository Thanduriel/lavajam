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
    
    ActorKind GetKind();
    guid_t GetGUID();
    ActorShape GetShape();
    float GetSize();
    glm::vec2 GetPosition();
    float GetRotation();
    
    void SetKind(ActorKind& kind);
    void SetShape(ActorShape& shape);
    void SetSize(float& size);
    void SetPosition(glm::vec2& position);
    void SetRotation(float& rotation);
    
private:
    ActorKind kind;
    guid_t guid;
    ActorShape shape;
    float size;
    glm::vec2 position;
    float rotation;
};
