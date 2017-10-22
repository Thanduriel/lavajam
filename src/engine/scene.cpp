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
#include "actors/aiactor.hpp"
#include "components/drawcomponent.hpp"

#include <algorithm>
#include <iostream>

Graphic::VertexBuffer<Vertex>* VertexBuffer;
Graphic::Effect* Effect;

Scene::Scene(Camera camera, Scene* previous, Scene* next) :
    m_camera(camera), m_previous(previous), m_next(next)
{}

const Camera& Scene::GetCamera() const
{
    return this->m_camera;
}

const Scene* Scene::GetPrevious() const
{
    return this->m_previous;
}

const Scene* Scene::GetNext() const
{
    return this->m_next;
}

const Actors& Scene::GetActors() const
{
    return this->m_actors;
}

void Scene::AddActor(Actor& actor)
{
    std::unique_ptr<Actor> actor_ptr(&actor);
    this->m_actors.push_back(std::move(actor_ptr));
    actor.Register(*this);
}

void Scene::SetCamera(Camera camera)
{
    this->m_camera = camera;
}

void Scene::SetPrevious(Scene* scene)
{
    this->m_previous = scene;
}

void Scene::SetNext(Scene* scene)
{
    this->m_next = scene;
}

void Scene::AddComponent(Component& component)
{
    this->m_components.push_back(&component);
}

void Scene::AddComponent(PhysicsComponent& component)
{
    this->m_physicsComponents.push_back(&component);
}

void Scene::AddComponent(ControllerComponent& component)
{
	component.SetShootCallback([this](BulletActor* bullet)
	{
		this->m_actorsQueue.push_back(bullet);
	});
	this->m_components.push_back(&component);
}

void Scene::Update(float deltaTime)
{    
    auto cidx = std::remove_if(
        this->m_components.begin(),
        this->m_components.end(),
        [](const Component* component)
        {
            return component->GetActor()->GetDestroy();
        });
    this->m_components.erase(cidx, this->m_components.end());
    
    auto cpidx = std::remove_if(
        this->m_physicsComponents.begin(),
        this->m_physicsComponents.end(),
        [](const PhysicsComponent* component)
        {
            return component->GetActor()->GetDestroy();
        });
    this->m_physicsComponents.erase(cpidx, this->m_physicsComponents.end());
    
    auto idx = std::remove_if(
        this->m_actors.begin(),
        this->m_actors.end(),
        [](const std::unique_ptr<Actor>& actor)
        {
           return actor->GetDestroy(); 
        });
    this->m_actors.erase(idx, this->m_actors.end());
    
    this->m_camera.Update();
    
    for (const auto& actor: this->m_actors)
    {
        actor->Update(deltaTime);
    }
    
    VertexBuffer->Clear();
    
    glm::vec2 own_velocity_delta, other_velocity_delta;
    for (auto it_me = this->m_physicsComponents.begin(); it_me != this->m_physicsComponents.end(); it_me++)
    {
        for (auto it_other = it_me + 1; it_other != this->m_physicsComponents.end(); it_other++)
        {
            if ((*it_me)->Collide(**it_other, own_velocity_delta, other_velocity_delta))
            {
				own_velocity_delta *= deltaTime * glm::length((*it_me)->GetActor()->GetVelocity());
				other_velocity_delta *= deltaTime * glm::length((*it_me)->GetActor()->GetVelocity());
                (*it_me)->GetActor()->AddVelocity(own_velocity_delta);
                (*it_other)->GetActor()->AddVelocity(other_velocity_delta);
                
                auto my_actor = (*it_me)->GetActor();
                auto other_actor = (*it_other)->GetActor();
                if ((*it_other)->GetActor()->GetKind() == ActorKind::Ai)
                {
                    auto ai_actor = (AiActor*) other_actor;
                    auto& ai = ai_actor->GetAiControllerComponent();
                                        
                    if (ai.GetTarget()->GetGUID() == my_actor->GetGUID() && ai.GetCooldown())
                    {
                        ai.SetCooldown(1.0);
                        float x = std::rand() / (float) RAND_MAX * 2.0f - 1.0f;
                        float y = std::rand() / (float) RAND_MAX * 2.0f - 1.0f;
                        float r = std::rand() / (float) RAND_MAX * 2 * glm::pi<float>();

                        Actor* new_ai = new AiActor(
                            my_actor,
                            0.01f,
                            ai_actor->GetDrawComponent().GetColor(),
                            0,
                            glm::vec2(x, -y),
                            r,
                            glm::vec2(0, 0)
                        );
                        this->m_actorsQueue.push_back(new_ai);
                    }
                }
            }
        }

		auto me_pos = (*it_me)->GetActor()->GetPosition();
		auto me_size = (*it_me)->GetSize();
		auto velocity = (*it_me)->GetActor()->GetVelocity();
		glm::vec2 border_vec = {0,0};
		if (me_pos.x - me_size < -1)
		{
			if (velocity.x < 0)
				border_vec.x -= velocity.x * 2;
		}
		if (me_pos.x + me_size > 1)
		{
			if (velocity.x > 0)
				border_vec.x -= velocity.x * 2;
		}
		if (me_pos.y - me_size < -1)
		{
			if (velocity.y < 0)
				border_vec.y -= velocity.y * 2;
		}
		if (me_pos.y + me_size > 1)
		{
			if (velocity.y > 0)
				border_vec.y -= velocity.y * 2;
		}

		(*it_me)->GetActor()->AddVelocity(border_vec);
    }
    
    for (auto const& component : this->m_components)
    {
        component->Process(deltaTime);
    }
	    
    VertexBuffer->Upload();
    Graphic::Device::Draw(*VertexBuffer);
    
    for (auto const& actor : this->m_actorsQueue)
    {
        this->AddActor(*actor);
    }
	this->m_actorsQueue.clear();
}

void Scene::Initialize()
{
    VertexBuffer = new Graphic::VertexBuffer<Vertex>({
        Graphic::VertexFormat::VEC2,
        Graphic::VertexFormat::VEC3,
        Graphic::VertexFormat::FLOAT,
		Graphic::VertexFormat::FLOAT
    });
    Effect = new Graphic::Effect("shaders/vert.spv", "shaders/frag.spv", *VertexBuffer, "shaders/geom.spv");

    Graphic::Device::SetEffect(*Effect);
}

void Scene::Destroy()
{
    delete Effect;
    delete VertexBuffer;
}
