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

#include "engine/scene.hpp"
#include "components/drawcomponent.hpp"

#include <algorithm>
#include <iostream>

Scene::Scene(Camera camera, Scene* previous, Scene* next) :
    camera(camera), previous(previous), next(next)
{}

const Camera& Scene::GetCamera() const
{
    return this->camera;
}

const Scene* Scene::GetPrevious() const
{
    return this->previous;
}

const Scene* Scene::GetNext() const
{
    return this->next;
}

const Actors& Scene::GetActors() const
{
    return this->actors;
}

void Scene::AddActor(Actor& actor)
{
    std::unique_ptr<Actor> actor_ptr(&actor);
    this->actors.push_back(std::move(actor_ptr));
    actor.Register(*this);
}

void Scene::SetCamera(Camera camera)
{
    this->camera = camera;
}

void Scene::SetPrevious(Scene* scene)
{
    this->previous = scene;
}

void Scene::SetNext(Scene* scene)
{
    this->next = scene;
}

void Scene::AddComponent(Component& component)
{
    this->components.push_back(&component);
}

void Scene::AddComponent(PhysicsComponent& component)
{
    this->physicsComponents.push_back(&component);
}

void Scene::Update(float deltaTime)
{
    //std::cout << "Updating scene [delta-time: " << deltaTime << "]" << std::endl;
    
    auto cidx = std::remove_if(
        this->components.begin(),
        this->components.end(),
        [](const Component* component)
        {
            return component->GetActor()->GetDestroy();
        });
    this->components.erase(cidx, this->components.end());
    
    auto cpidx = std::remove_if(
        this->physicsComponents.begin(),
        this->physicsComponents.end(),
        [](const PhysicsComponent* component)
        {
            return component->GetActor()->GetDestroy();
        });
    this->physicsComponents.erase(cpidx, this->physicsComponents.end());
    
    auto idx = std::remove_if(
        this->actors.begin(),
        this->actors.end(),
        [](const std::unique_ptr<Actor>& actor)
        {
           return actor->GetDestroy(); 
        });
    this->actors.erase(idx, this->actors.end());
    
    this->camera.Update();
    
    for (const auto& actor: this->actors)
    {
        actor->Update();
    }
    
    GraphicContext& ctx = DrawComponent::GetContext();
    ctx.VertexBuffer.Clear();
    
    glm::vec2 v;
    for (auto it_me = this->physicsComponents.begin(); it_me != this->physicsComponents.end(); it_me++)
    {
        for (auto it_other = it_me + 1; it_other != this->physicsComponents.end(); it_other++)
        {
            if ((*it_me)->Collide(**it_other, v))
            {
                (*it_me)->GetActor()->AddVelocity(v);
                (*it_other)->GetActor()->AddVelocity(-v);
            }
        }
    }
    
    for (auto const& component : this->components)
    {
        component->Process(deltaTime);
    }
    
    DrawComponent::DrawContext();
}
