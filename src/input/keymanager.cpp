#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include "keymanager.hpp"

namespace Input {

    std::unordered_map<int,int> KeyManager::m_pressedKeys;
    GLFWwindow* KeyManager::m_window;   
    KeyManager* KeyManager::m_instance;

    void KeyManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
        m_pressedKeys.insert_or_assign(key,action);
    }
    KeyManager* KeyManager::instance(GLFWwindow* win){
        if(!m_instance){
            m_instance= new KeyManager();
            m_instance->m_window=win;      
        }
    }
}
