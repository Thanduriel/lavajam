#pragma once

#include <unordered_map>

struct GLFWwindow;

namespace Input {

    enum class EKeyState {
        RELEASE=0,
        PRESS=1,
        LONGPRESS=2
    };

class KeyManager
{
private:
    static GLFWwindow* m_window;   
    static KeyManager* m_instance;
    
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);   
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void cursor_enter_callback(GLFWwindow* window, int entered);    
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static std::unordered_map<int,int> m_pressedKeys;

    static int activeJoysticks[];
public:
	static void Init(GLFWwindow* win);

    static void pollEvents();
    static EKeyState getKeyStates(int key);

};

}