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
		if (this->m_shootCallback != nullptr)
		{
			glm::vec4 brown(141/255, 110/255, 99/255, 1);
			auto bullet = new BulletActor(0.005, brown, 0, m_actor->GetPosition(), m_actor->GetRotation(), glm::rotate(glm::vec2(0.f,1.f), m_actor->GetRotation()));
			this->m_shootCallback(bullet);
		}
    }
    
}

void ControllerComponent::SetShootCallback(std::function<void(BulletActor* bullet)> callback)
{
	this->m_shootCallback = callback;
}
