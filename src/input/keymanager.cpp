#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include "keymanager.hpp"

namespace Input {
    KeyManager::KeyManager(GLFWwindow* win){
        m_window=win;
        glfwSetKeyCallback(m_window, key_callback);
    }
    void KeyManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
        m_pressedKeys.insert_or_assign({key,action});
    }
    KeyManager::KeyManager* instance(GLFWwindow* win){
        if(!m_instance){
            m_instance= new KeyManager();
            m_instance->m_window=win;      
        }
    }
}
