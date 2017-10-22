#include "controllercomponent.hpp"
#include "input/keymanager.hpp"
#include "glm.hpp"
#include "actors/particleactor.hpp"
#include "actors/characteractor.hpp"

ControllerComponent::ControllerComponent(Actor* actor, std::vector <int> keyMapping, bool isActive):
CooldownComponent(actor, isActive),m_keyMapping(keyMapping),m_spawnCallback(nullptr){};

void ControllerComponent::Process(float deltaTime)
{
	CooldownComponent::Process(deltaTime);
    float translate = 0;
    float rotate = 0;
    if(Input::KeyManager::getKeyStates(m_keyMapping[0])!=Input::EKeyState::RELEASE) translate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[2])!=Input::EKeyState::RELEASE) translate+=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[1])!=Input::EKeyState::RELEASE) rotate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[3])!=Input::EKeyState::RELEASE) rotate+=deltaTime;
    m_actor->AddRotation(rotate * 5);
    m_actor->AddVelocity(glm::rotate(glm::vec2(0, translate / 1.f), m_actor->GetRotation()));   
    if(Input::KeyManager::getKeyStates(m_keyMapping[4])!=Input::EKeyState::RELEASE) {
		if (this->m_spawnCallback != nullptr && this->GetCooldown())
		{
            this->SetCooldown(0.25f);
			const glm::vec4 brown(141/255.f, 110/255.f, 99/255.f, 1);
			const glm::vec2 velocity = glm::rotate(glm::vec2(0.f, 1.f + glm::length(m_actor->GetVelocity())), m_actor->GetRotation());
			const auto bullet = new BulletActor(0.006f, brown, 0, m_actor->GetPosition() - normalize(velocity)/15, m_actor->GetRotation(), -velocity);
			this->m_spawnCallback(bullet);
		}
    }

	if (this->m_spawnCallback != nullptr && this->m_particleCooldown <= 0)
	{
		this->m_particleCooldown = 0.1f;
		auto actor = static_cast<CharacterActor*>(this->GetActor());
		auto particle = new ParticleActor(0.001f, actor->GetDrawComponent().GetColor(), 0, actor->GetPosition(), actor->GetRotation(), glm::vec2(0,0), 1);
		this->m_spawnCallback(particle);
	}

	this->m_particleCooldown -= deltaTime;
}

void ControllerComponent::SetSpawnCallback(std::function<void(Actor* bullet)> callback)
{
	this->m_spawnCallback = callback;
}
