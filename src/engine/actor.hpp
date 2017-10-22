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

#include "glm.hpp"

typedef uint64_t guid_t;

class Scene;

enum class ActorKind {
    Ai,
    Character,
	Bullet,
	Particle
};

class Actor
{
public:
    Actor(
        glm::vec2 position,
        float rotation,
        glm::vec2 velocity,
		ActorKind kind
    );
    Actor(const Actor& other);
    
    guid_t GetGUID() const;
    glm::vec2 GetPosition() const;
    float GetRotation() const;
    glm::vec2 GetVelocity() const;
    bool GetDestroy() const;
	ActorKind GetKind() const;

	void SetKind(ActorKind kind);
    void SetPosition(const glm::vec2 position);
    void SetRotation(const float rotation);
    void AddRotation(const float rotationDelta);
    void SetVelocity(const glm::vec2 velocity);
    void AddVelocity(const glm::vec2 velocityDelta);
    void SetWin();
    bool GetWin() const;
    
    void Destroy();
    void Update(float deltaTime);
    
    virtual void Register(Scene& scene) = 0;
    
private:
    guid_t m_guid;
    glm::vec2 m_position;
    float m_rotation;
    glm::vec2 m_velocity;
    bool m_destroy;
	ActorKind m_kind;
    bool m_win;
};
