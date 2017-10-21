#include "controllercomponent.hpp"
#include "input/keymanager.hpp"
#include "glm.hpp"

ControllerComponent::ControllerComponent(Actor* actor, bool isActive, std::vector <int> keyMapping):
Component(actor, isActive),m_keyMapping(keyMapping){};

void ControllerComponent::Process(float deltaTime)
{
    float x=0;
    float y=0;
    if(Input::KeyManager::getKeyStates(m_keyMapping[0])==Input::EKeyState::PRESS) y+=1;
    if(Input::KeyManager::getKeyStates(m_keyMapping[1])==Input::EKeyState::PRESS) x-=1;
    if(Input::KeyManager::getKeyStates(m_keyMapping[2])==Input::EKeyState::PRESS) y-=1;
    if(Input::KeyManager::getKeyStates(m_keyMapping[3])==Input::EKeyState::PRESS) x+=1;
    m_actor->AddVelocity(glm::vec2(x*deltaTime,y*deltaTime));    
    if(Input::KeyManager::getKeyStates(m_keyMapping[4])==Input::EKeyState::PRESS) {
        //shoot!
    }
    
}
