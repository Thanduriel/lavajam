#include "controllercomponent.hpp"
#include "input/keymanager.hpp"
#include "glm.hpp"

ControllerComponent::ControllerComponent(Actor* actor, bool isActive, std::vector <int> keyMapping):
Component(actor, isActive),m_keyMapping(keyMapping){};

void ControllerComponent::Process(float deltaTime)
{
    float translate = 0;
    float rotate = 0;
    if(Input::KeyManager::getKeyStates(m_keyMapping[0])!=Input::EKeyState::RELEASE) translate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[2])!=Input::EKeyState::RELEASE) translate+=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[1])!=Input::EKeyState::RELEASE) rotate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[3])!=Input::EKeyState::RELEASE) rotate+=deltaTime;
    m_actor->AddRotation(rotate * 5);
    m_actor->AddVelocity(glm::rotate(glm::vec2(0, translate), m_actor->GetRotation()));   
    if(Input::KeyManager::getKeyStates(m_keyMapping[4])!=Input::EKeyState::RELEASE) {
        //shoot!
    }
    
}
