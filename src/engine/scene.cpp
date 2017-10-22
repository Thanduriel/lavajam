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
#include "actors/characteractor.hpp"
#include "components/drawcomponent.hpp"
#include "actors/particleactor.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

Graphic::VertexBuffer<Vertex, 65536>* VertexBuffer;
Graphic::Effect* Effect;

Scene::Scene(Camera camera, Scene* previous, Scene* next) :
    m_camera(camera), m_previous(previous), m_next(next), m_gameEnded(false)
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
	component.SetSpawnCallback([this](Actor* actor)
	{
		this->m_actorsQueue.push_back(actor);
	});
	this->m_components.push_back(&component);
}

void Scene::SpawnAi(Actor* targetActor, glm::vec4 color, uint32_t team)
{
    float x = std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
    float y = std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
    float r = std::rand() / static_cast<float>(RAND_MAX) * 2 * glm::pi<float>();

    Actor* new_ai = new AiActor(
        targetActor,
        team,
        0.01f,
        color,
        0,
        glm::vec2(x, -y),
        r,
        glm::vec2(0, 0)
    );
    this->m_actorsQueue.push_back(new_ai);
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
    
    std::unordered_map<uint32_t, std::vector<Actor*>> teamCounter;
    
    for (const auto& actor : this->m_actors)
    {
        actor->Update(deltaTime);
        
        if (actor->GetKind() == ActorKind::Character)
        {
            auto char_actor = static_cast<CharacterActor*>(actor.get());
            auto team = char_actor->GetTeam();
            
            if (teamCounter.count(team))
            {
                teamCounter[team].push_back(char_actor);
            }
            else
            {
                teamCounter.insert({team, {char_actor}});
            }
        }
        else if (actor->GetKind() == ActorKind::Ai)
        {
            auto ai_actor = static_cast<AiActor*>(actor.get());
            auto team = ai_actor->GetAiControllerComponent().GetTeam();
            
            if (teamCounter.count(team))
            {
                teamCounter[team].push_back(ai_actor);
            }
            else
            {
                teamCounter.insert({team, {ai_actor}});
            }
        }
    }
    
    for (auto it : teamCounter)
    {
        if (it.second.size() > 300)
        {
            for (const auto& actor : it.second)
            {
                actor->Destroy();
            }
        }
    }
    
    if (teamCounter.size() == 1 && !this->m_gameEnded)
    {
        // we don't know which team won but teamCounter only contains one item
        // so we can iterate over it to access the first item of the map
        for (auto it : teamCounter)
        {
            std::cout << "Player " << it.first << " won!" << std::endl;
            
            for (const auto& actor : it.second)
            {
                if (actor->GetKind() != ActorKind::Character)
                {
                    actor->Destroy();
                }
            }
            
            for (const auto& actor : this->m_actors)
            {
                if (actor->GetKind() == ActorKind::Bullet)
                {
                    actor->Destroy();
                }
            }
        }
        this->m_gameEnded = true;
    }
    else if (teamCounter.size() < 1)
    {
        this->m_gameEnded = true;
    }
    
    VertexBuffer->Clear();
    
    for (auto const& component : this->m_components)
    {
        component->Process(deltaTime);
    }
	    
	ResolveCollisionns(deltaTime);

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
    VertexBuffer = new Graphic::VertexBuffer<Vertex, 65536>({
        Graphic::VertexFormat::VEC2,
        Graphic::VertexFormat::VEC4,
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

float LeftBorder(const PhysicsComponent& _comp)
{
	return _comp.GetActor()->GetPosition().x - _comp.GetSize();
}

void Scene::ResolveCollisionns(float deltaTime)
{
	std::sort(m_physicsComponents.begin(), m_physicsComponents.end(), [](const auto& _lhs, const auto& _rhs)
	{
		return LeftBorder(*_lhs) < LeftBorder(*_rhs);
	});

	glm::vec2 own_velocity_delta{}, other_velocity_delta{};
	for (auto it_me = this->m_physicsComponents.begin();it_me != this->m_physicsComponents.end(); it_me++)
	{
		auto my_actor = (*it_me)->GetActor();
		for (auto it_other = it_me + 1; it_other != m_physicsComponents.end() && ((**it_me).GetActor()->GetPosition().x + (**it_me).GetSize() > LeftBorder(**it_other)); it_other++)
		{
			auto other_actor = (*it_other)->GetActor();
			if ((*it_me)->Collide(**it_other, own_velocity_delta, other_velocity_delta))
			{
				own_velocity_delta *= deltaTime * glm::length(my_actor->GetVelocity());
				other_velocity_delta *= deltaTime * glm::length(my_actor->GetVelocity());
				my_actor->AddVelocity(own_velocity_delta);
				other_actor->AddVelocity(other_velocity_delta);

				if (other_actor->GetKind() == ActorKind::Ai)
				{
					auto ai_actor = static_cast<AiActor*>(other_actor);
					auto& ai = ai_actor->GetAiControllerComponent();

					if (ai.GetTarget()->GetGUID() == my_actor->GetGUID() && ai.GetCooldown())
					{
						ai.SetCooldown(1.0);
						SpawnAi(my_actor, ai_actor->GetDrawComponent().GetColor(), ai.GetTeam());
					}
				}
				else if (my_actor->GetKind() == ActorKind::Character &&
					other_actor->GetKind() == ActorKind::Bullet)
				{
					auto bullet_actor = static_cast<BulletActor*>(other_actor);
					auto char_actor = static_cast<CharacterActor*>(my_actor);
					auto& cooldown = bullet_actor->GetCooldownComponent();

					if (cooldown.GetCooldown())
					{
						cooldown.SetCooldown(0.25f);
						SpawnAi(my_actor, char_actor->GetDrawComponent().GetColor(), char_actor->GetTeam());
					}
				}
			}
		}

		auto me_pos = my_actor->GetPosition();
		auto me_size = (*it_me)->GetSize();
		auto velocity = my_actor->GetVelocity();
		glm::vec2 border_vec = { 0,0 };
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
		/*
		if (my_actor->GetKind() == ActorKind::Bullet && border_vec.x == 0 && border_vec.y == 0)
		{
		BulletActor* bullet = static_cast<BulletActor*>(my_actor);
		bullet->GetBulletComponent().Collided(&bullet->GetPhysicsComponent());
		}
		*/
		my_actor->AddVelocity(border_vec);
	}
}
