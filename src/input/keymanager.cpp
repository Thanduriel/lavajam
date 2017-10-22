#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "input/keymanager.hpp"

namespace Input {

    

    std::unordered_map<int,int> KeyManager::m_pressedKeys;
    GLFWwindow* KeyManager::m_window;   
    KeyManager* KeyManager::m_instance;
    int KeyManager::activeJoysticks[16];    

    void KeyManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        m_pressedKeys.insert_or_assign(key,action);
        #ifndef NDEBUG
            //std::cout << "keycode:" <<  key << " " << action << std::endl;
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
            //std::cout << "Mouse X:" <<  xpos << " Y: " << ypos << std::endl;
        #endif
    }    
    void KeyManager::cursor_enter_callback(GLFWwindow* window, int entered)
    {
        #ifndef NDEBUG
        
            if (entered)
            {
                //std::cout << "Hey hey! Welcome cursor!" << std::endl;
            }
            else
            {
                //std::cout << "Oh noes! Lonely yet again!" << std::endl;
            }
        #endif
    }
    void KeyManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        #ifndef NDEBUG
            //std::cout << "Mouseclick, Button: " << button << " Action: " << action << std::endl;
        #endif
    }    


    void KeyManager::pollEvents(){
        glfwPollEvents();        
        for(size_t i= 0; i<16; i++){
            activeJoysticks[i]=glfwJoystickPresent(i);            
        }
    }

    EKeyState KeyManager::getKeyStates(int key){
        return EKeyState(m_pressedKeys[key]);
    }
    

}
