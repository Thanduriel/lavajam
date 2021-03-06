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
#include "components/controllercomponent.hpp"
#include "graphic/vertexbuffer.hpp"
#include "graphic/effect.hpp"
#include "actors/aiactor.hpp"

#include <vector>
#include <memory>
#include <unordered_map>
#include <utility>

typedef std::vector<std::unique_ptr<Actor>> Actors;
typedef std::vector<Component*> Components;
typedef std::vector<PhysicsComponent*> PhysicsComponents;

struct Vertex
{
	glm::vec2 position;
	glm::vec4 color;
	float rotation;
	float size;
};

extern Graphic::VertexBuffer<Vertex, 65536>* VertexBuffer;
extern Graphic::Effect* Effect;

class Scene
{
public:
    Scene(Camera camera, Scene* previous = nullptr, Scene* next = nullptr);
    ~Scene();
    
    const Camera& GetCamera() const;
    const Scene* GetPrevious() const;
    const Scene* GetNext() const;
    const Actors& GetActors() const;
    
    void AddActor(Actor& actor);
    void AddActor(AiActor& actor);
    void SetCamera(Camera camera);
    void SetPrevious(Scene* scene);
    void SetNext(Scene* scene);
    void AddComponent(Component& component);
    void AddComponent(PhysicsComponent& component);
	void AddComponent(ControllerComponent& component);
	void SpawnAi(Actor* targetActor, glm::vec4 color, uint32_t team);

	void Update(float deltaTime);
    void Initialize();
    void Destroy();
    
private:
	void ResolveCollisions(float _deltaTime);

	float m_currentSec;
	float m_eventTime;

    Camera m_camera;
    Scene* m_previous;
    Scene* m_next;
    Actors m_actors;
    Components m_components;
    PhysicsComponents m_physicsComponents;
	std::vector<ControllerComponent*> m_controllerComponents;
	std::vector<Actor*> m_actorsQueue;
    bool m_gameEnded;
    std::unordered_map<uint32_t, std::pair<glm::vec4, Actor*>>* m_teams;
};
