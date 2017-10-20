#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include "keymanager.hpp"
#include <iostream>

namespace Input {

    std::unordered_map<int,int> KeyManager::m_pressedKeys;
    GLFWwindow* KeyManager::m_window;   
    KeyManager* KeyManager::m_instance;

    void KeyManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
        m_pressedKeys.insert_or_assign(key,action);
        #ifndef NDEBUG
            std::cout << "keycode:" <<  key << " " << action << std::endl;
        #endif
    }
    KeyManager::KeyManager(GLFWwindow* win)
    {
        m_window=win;
        glfwSetKeyCallback(win, key_callback);
    };

}
