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
#include "random.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

Graphic::VertexBuffer<Vertex, 65536>* VertexBuffer;
Graphic::Effect* Effect;

Scene::Scene(Camera camera, Scene* previous, Scene* next) :
    m_camera(camera), m_previous(previous), m_next(next), m_gameEnded(false),
	m_eventTime(0.f),
	m_currentSec(0.f)
{
    this->m_teams = new std::unordered_map<uint32_t, std::pair<glm::vec4, Actor*>>();
}

Scene::~Scene()
{
    delete this->m_teams;
}

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

void Scene::AddActor(AiActor& actor)
{
    this->m_teams->insert({actor.GetAiControllerComponent().GetTeam(), std::pair<glm::vec4, Actor*>(actor.GetDrawComponent().GetColor(), actor.GetAiControllerComponent().GetTarget())});
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

void Scene::AddComponent(PhysicsComponent& component)
{
    component.SetTeams(this->m_teams);
	component.SetSpawnCallback([this](Actor* actor)
	{
		this->m_actorsQueue.push_back(actor);
	});
    this->m_physicsComponents.push_back(&component);
}

void Scene::AddComponent(Component& component)
{
	component.SetSpawnCallback([this](Actor* actor)
	{
		this->m_actorsQueue.push_back(actor);
	});
	this->m_components.push_back(&component);
}

void Scene::AddComponent(ControllerComponent& component)
{
	AddComponent(static_cast<Component&>(component));
	m_controllerComponents.push_back(&component);
}

void Scene::SpawnAi(Actor* targetActor, glm::vec4 color, uint32_t team)
{
    float x = std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
    float y = std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
    float r = std::rand() / static_cast<float>(RAND_MAX) * 2 * glm::pi<float>();

    AiActor* new_ai = new AiActor(
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

template<typename T>
void RemoveDestroyed(T& _container)
{
	auto ind = std::remove_if(_container.begin(),_container.end(),
		[](const Component* component)
	{
		return component->GetActor()->GetDestroy();
	});
	_container.erase(ind, _container.end());
}

void Scene::Update(float deltaTime)
{
	m_currentSec += deltaTime;
	m_eventTime += deltaTime;
	static Generators::RandomGenerator rng(static_cast<uint32_t>(time(nullptr)));
	if (m_currentSec > 1.f)
	{
		m_currentSec = 0.f;
		if (rng.Uniform(0.f, m_eventTime + 25.f) < m_eventTime)
		{
			FireMode mode = (FireMode)rng.Uniform(0, static_cast<int32_t>(FireMode::COUNT)-1);
			for (auto& controller : m_controllerComponents)
				controller->SetFireMode(mode);
			m_eventTime = 0.f;
		}
	}

	RemoveDestroyed(m_components);
	RemoveDestroyed(m_physicsComponents);
	RemoveDestroyed(m_controllerComponents);
    
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
            CharacterActor* char_actor = static_cast<CharacterActor*>(actor.get());
            uint32_t team = char_actor->GetTeam();
            
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
            AiActor* ai_actor = static_cast<AiActor*>(actor.get());
            uint32_t team = ai_actor->GetAiControllerComponent().GetTeam();
            
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
                else
                {
                    auto char_actor = static_cast<CharacterActor*>(actor);
                    char_actor->SetVelocity(glm::vec2(0, 0));
                    char_actor->SetPosition(glm::vec2(0, 0));
                    char_actor->GetDrawComponent().SetSize(0.333f);
                    char_actor->GetPhysicsComponent().SetSize(0.333f);
                    char_actor->SetWin();
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
    
    if (!this->m_gameEnded)
    {
        ResolveCollisions(deltaTime);
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

void Scene::ResolveCollisions(float deltaTime)
{
	std::sort(m_physicsComponents.begin(), m_physicsComponents.end(), [](const auto& _lhs, const auto& _rhs)
	{
		return LeftBorder(*_lhs) < LeftBorder(*_rhs);
	});
    // collide every physics object
	glm::vec2 own_velocity_delta{}, other_velocity_delta{};
	for (auto it_me = this->m_physicsComponents.begin();it_me != this->m_physicsComponents.end(); it_me++)
	{
        // ... with every other physics object
		Actor* my_actor = (*it_me)->GetActor();
		for (auto it_other = it_me + 1; it_other != m_physicsComponents.end() && ((**it_me).GetActor()->GetPosition().x + (**it_me).GetSize() > LeftBorder(**it_other)); it_other++)
		{
             // if a collision is detected
			Actor* other_actor = (*it_other)->GetActor();
			if ((*it_me)->Collide(**it_other, own_velocity_delta, other_velocity_delta))
			{
                // move away from the other object
				own_velocity_delta *= deltaTime * glm::length(my_actor->GetVelocity());
				other_velocity_delta *= deltaTime * glm::length(my_actor->GetVelocity());
				my_actor->AddVelocity(own_velocity_delta);
                other_actor->AddVelocity(other_velocity_delta);
                
                // if hit by an enemy AI, spawn more AI
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
                // if hit by the enemy, spawn more AI
				else if (my_actor->GetKind() == ActorKind::Character &&
					other_actor->GetKind() == ActorKind::Bullet)
				{
					BulletActor* bullet_actor = static_cast<BulletActor*>(other_actor);
					CharacterActor* char_actor = static_cast<CharacterActor*>(my_actor);
					CooldownComponent cooldown = bullet_actor->GetCooldownComponent();

					if (cooldown.GetCooldown())
					{
						cooldown.SetCooldown(0.25f);
						SpawnAi(my_actor, char_actor->GetDrawComponent().GetColor(), char_actor->GetTeam());
					}
				}
			}
		}

        // window boundary checking
		glm::vec2 me_pos = my_actor->GetPosition();
		float me_size = (*it_me)->GetSize();
		glm::vec2 velocity = my_actor->GetVelocity();
        glm::vec2 border_vec = { 0,0 };
        bool hitTheWall = false;        
		if (me_pos.x - me_size < -1)
		{
			if (velocity.x < 0)
                border_vec.x -= velocity.x * 2;
                hitTheWall=true;
		}
		if (me_pos.x + me_size > 1)
		{
			if (velocity.x > 0)
                border_vec.x -= velocity.x * 2;
                hitTheWall=true;
		}
		if (me_pos.y - me_size < -1)
		{
			if (velocity.y < 0)
                border_vec.y -= velocity.y * 2;
                hitTheWall=true;
		}
		if (me_pos.y + me_size > 1)
		{
			if (velocity.y > 0)
                border_vec.y -= velocity.y * 2;
                hitTheWall=true;
        }
        if(hitTheWall && my_actor->GetKind()==ActorKind::Bullet){
            static_cast<BulletActor*>(my_actor)->GetBulletComponent().IncrementBounceCount();
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
