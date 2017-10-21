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

#include "engine/camera.hpp"

Camera::Camera(
    float fov,
    float aspectRatio,
    glm::vec3 translation,
    glm::quat rotation
) : m_fieldOfView(fov), m_aspectRatio(aspectRatio), m_translation(translation), m_rotation(rotation)
{
    this->m_viewProjection = glm::mat4(1.0f);
}

void Camera::SetFieldOfView(const float fov)
{
    this->m_fieldOfView = fov;
}

void Camera::SetAspectRatio(const float ar)
{
    this->m_aspectRatio = ar;
}

void Camera::SetNear(const float near)
{
    this->m_near = near;
}

void Camera::SetFar(const float far)
{
    this->m_far = far;
}

void Camera::SetTranslation(const glm::vec3 translation)
{
    this->m_translation = translation;
}

void Camera::SetRotation(const glm::quat rotation) {
    this->m_rotation = rotation;
}

float Camera::GetFieldOfView() const
{
    return this->m_fieldOfView;
}

float Camera::GetAspectRatio() const
{
    return this->m_aspectRatio;
}

float Camera::GetNear() const
{
    return this->m_near;
}

float Camera::GetFar() const
{
    return this->m_far;
}

glm::vec3 Camera::GetTranslation() const
{
    return this->m_translation;
}

glm::quat Camera::GetRotation() const
{
    return this->m_rotation;
}

const glm::mat4& Camera::GetViewProjection() const
{
    return this->m_viewProjection;
}

void Camera::Update()
{
    // calculate view-projection with glm
    
    glm::mat4 view =
        glm::translate(glm::mat4(1.0f), this->m_translation) *
        glm::mat4_cast(this->m_rotation);
    
    
    glm::mat4 projection = glm::perspective(
        this->m_fieldOfView,
        this->m_aspectRatio,
        this->m_near,
        this->m_far
    );
    
    this->m_viewProjection = view * projection;
}
