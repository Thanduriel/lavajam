#include "controllercomponent.hpp"
#include "input/keymanager.hpp"
#include "glm.hpp"

ControllerComponent::ControllerComponent(Actor* actor, bool isActive, std::vector <int> keyMapping):
Component(actor, isActive),m_keyMapping(keyMapping),m_shootCallback(nullptr){};

void ControllerComponent::Process(float deltaTime)
{
    float translate = 0;
    float rotate = 0;
    if(Input::KeyManager::getKeyStates(m_keyMapping[0])!=Input::EKeyState::RELEASE) translate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[2])!=Input::EKeyState::RELEASE) translate+=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[1])!=Input::EKeyState::RELEASE) rotate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[3])!=Input::EKeyState::RELEASE) rotate+=deltaTime;
    m_actor->AddRotation(rotate * 5);
    m_actor->AddVelocity(glm::rotate(glm::vec2(0, translate / 2.f), m_actor->GetRotation()));   
    if(Input::KeyManager::getKeyStates(m_keyMapping[4])!=Input::EKeyState::RELEASE) {
		if (this->m_shootCallback != nullptr && this->GetCooldown())
		{
            this->SetCooldown(1.0);
			const glm::vec4 white(1, 1, 1, 1);
			const glm::vec2 velocity = glm::rotate(glm::vec2(0.f, 1.f + glm::length(m_actor->GetVelocity())), m_actor->GetRotation());
			const auto bullet = new BulletActor(0.01, white, 0, m_actor->GetPosition() + velocity, m_actor->GetRotation(), velocity);
			this->m_shootCallback(bullet);
		}
    }
    
    if (this->m_cooldown > 0)
    {
        this->m_cooldown -= deltaTime;
    }
}

void ControllerComponent::SetShootCallback(std::function<void(BulletActor* bullet)> callback)
{
	this->m_shootCallback = callback;
}

bool ControllerComponent::GetCooldown() const
{
    return this->m_cooldown <= 0;
}

void ControllerComponent::SetCooldown(double time)
{
    this->m_cooldown = time;
}
