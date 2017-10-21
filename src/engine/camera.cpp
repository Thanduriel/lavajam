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
) : fieldOfView(fov), aspectRatio(aspectRatio), translation(translation), rotation(rotation)
{
    this->viewProjection = glm::mat4(1.0f);
}

void Camera::SetFieldOfView(const float fov)
{
    this->fieldOfView = fov;
}

void Camera::SetAspectRatio(const float ar)
{
    this->aspectRatio = ar;
}

void Camera::SetNear(const float near)
{
    this->near = near;
}

void Camera::SetFar(const float far)
{
    this->far = far;
}

void Camera::SetTranslation(const glm::vec3 translation)
{
    this->translation = translation;
}

void Camera::SetRotation(const glm::quat rotation) {
    this->rotation = rotation;
}

float Camera::GetFieldOfView() const
{
    return this->fieldOfView;
}

float Camera::GetAspectRatio() const
{
    return this->aspectRatio;
}

float Camera::GetNear() const
{
    return this->near;
}

float Camera::GetFar() const
{
    return this->far;
}

glm::vec3 Camera::GetTranslation() const
{
    return this->translation;
}

glm::quat Camera::GetRotation() const
{
    return this->rotation;
}

const glm::mat4& Camera::GetViewProjection() const
{
    return this->viewProjection;
}

void Camera::Update()
{
    // calculate view-projection with glm
    
    glm::mat4 view =
        glm::translate(glm::mat4(1.0f), this->translation) *
        glm::mat4_cast(this->rotation);
    
    
    glm::mat4 projection = glm::perspective(
        this->fieldOfView,
        this->aspectRatio,
        this->near,
        this->far
    );
    
    this->viewProjection = view * projection;
}
