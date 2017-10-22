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

#include "engine/actor.hpp"
#include "engine/camera.hpp"
#include "engine/component.hpp"
#include "engine/scene.hpp"
#include "components/physicscomponent.hpp"
#include "graphic/vertexbuffer.hpp"
#include "graphic/effect.hpp"

#include <vector>
#include <memory>
#include "components/controllercomponent.hpp"

typedef std::vector<std::unique_ptr<Actor>> Actors;
typedef std::vector<Component*> Components;
typedef std::vector<PhysicsComponent*> PhysicsComponents;

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;
	float rotation;
	float size;
};

extern Graphic::VertexBuffer<Vertex>* VertexBuffer;
extern Graphic::Effect* Effect;

class Scene
{
public:
    Scene(Camera camera, Scene* previous = nullptr, Scene* next = nullptr);
    
    const Camera& GetCamera() const;
    const Scene* GetPrevious() const;
    const Scene* GetNext() const;
    const Actors& GetActors() const;
    
    void AddActor(Actor& actor);
    void SetCamera(Camera camera);
    void SetPrevious(Scene* scene);
    void SetNext(Scene* scene);
    void AddComponent(Component& component);
    void AddComponent(PhysicsComponent& component);
	void AddComponent(ControllerComponent& component);
    
    void Update(float deltaTime);
    void Initialize();
    void Destroy();
    
private:
    Camera m_camera;
    Scene* m_previous;
    Scene* m_next;
    Actors m_actors;
    Components m_components;
    PhysicsComponents m_physicsComponents;
	std::vector<Actor*> m_actorsQueue;
};
