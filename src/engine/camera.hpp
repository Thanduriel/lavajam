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

class Camera
{
public:
    Camera(
        float fov,
        float aspectRatio,
        glm::vec3 translation,
        glm::quat rotation
    );
    
    void SetFieldOfView(const float fov);
    void SetAspectRatio(const float ar);
    void SetNear(const float near);
    void SetFar(const float far);
    void SetTranslation(const glm::vec3 translation);
    void SetRotation(const glm::quat rotation);
    
    float GetFieldOfView() const;
    float GetAspectRatio() const;
    float GetNear() const;
    float GetFar() const;
    glm::vec3 GetTranslation() const;
    glm::quat GetRotation() const;
    
    const glm::mat4& GetViewProjection() const;
    
    void Update();
    
private:
    float m_aspectRatio;
    float m_fieldOfView;
    float m_near;
    float m_far;
    glm::vec3 m_translation;
    glm::quat m_rotation;
    glm::mat4 m_viewProjection;
};
