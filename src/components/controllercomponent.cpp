#include "controllercomponent.hpp"
#include "input/keymanager.hpp"
#include "glm.hpp"

ControllerComponent::ControllerComponent(Actor* actor, bool isActive, std::vector <int> keyMapping):
Component(actor, isActive),m_keyMapping(keyMapping){};

void ControllerComponent::Process(float deltaTime)
{
    float translate =0;
    float rotate=0;
    if(Input::KeyManager::getKeyStates(m_keyMapping[0])==Input::EKeyState::PRESS) translate+=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[2])==Input::EKeyState::PRESS) translate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[1])==Input::EKeyState::PRESS) rotate-=deltaTime;
    if(Input::KeyManager::getKeyStates(m_keyMapping[3])==Input::EKeyState::PRESS) rotate+=deltaTime;
    m_actor->AddVelocity(glm::vec2(0,transform));   
    m_actor->AddRotation(rotate);
    if(Input::KeyManager::getKeyStates(m_keyMapping[4])==Input::EKeyState::PRESS) {
        //shoot!
    }
    
}
