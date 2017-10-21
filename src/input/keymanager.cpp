#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include "keymanager.hpp"
#include <iostream>

namespace Input {

    std::unordered_map<int,int> KeyManager::m_pressedKeys;
    GLFWwindow* KeyManager::m_window;   
    KeyManager* KeyManager::m_instance;
    int KeyManager::activeJoysticks[16];    

    void KeyManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        m_pressedKeys.insert_or_assign(key,action);
        #ifndef NDEBUG
            std::cout << "keycode:" <<  key << " " << action << std::endl;
        #endif
    }

	void KeyManager::Init(GLFWwindow* win)
	{
		m_window = win;
        glfwSetKeyCallback(win, key_callback);
        glfwSetKeyCallback(win, key_callback);
        glfwSetCursorPosCallback(win, cursor_pos_callback);  
        glfwSetCursorEnterCallback(win, cursor_enter_callback);
        glfwSetMouseButtonCallback(win, mouse_button_callback); 
	}

    void KeyManager::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
    {
        #ifndef NDEBUG
            std::cout << "Mouse X:" <<  xpos << " Y: " << ypos << std::endl;
        #endif
    }    
    void KeyManager::cursor_enter_callback(GLFWwindow* window, int entered)
    {
        #ifndef NDEBUG
        
            if (entered)
            {
                std::cout << "Hey hey! Welcome cursor!" << std::endl;
            }
            else
            {
                std::cout << "Oh noes! Lonely yet again!" << std::endl;
            }
        #endif
    }
    void KeyManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        #ifndef NDEBUG
            std::cout << "Mouseclick, Button: " << button << " Action: " << action << std::endl;
        #endif
    }    


    void KeyManager::pollEvents(){
        for(size_t i= 0; i<16; i++){
            activeJoysticks[i]=glfwJoystickPresent(i);            
            if(GLFW_TRUE==activeJoysticks[i] ){
                std::cout << "Found Joystick Nr. " << (i+1) << std::endl;
            }
        }
    }

}
