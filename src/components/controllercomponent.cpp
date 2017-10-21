#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "controllercomponent.hpp"
#include "input/keymanager.hpp"
#include "glm/glm.hpp"

ControllerComponent::ControllerComponent(Actor* actor,bool isActive, std::vector <int> keyMapping = {GLFW_KEY_W,GLFW_KEY_A,GLFW_KEY_S,GLFW_KEY_D,GLFW_KEY_SPACE}):
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
