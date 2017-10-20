#pragma once

#include <unordered_map>
class GLFWwindow;

namespace Input {

class KeyManager
{
private:
    static GLFWwindow* m_window;   
    static KeyManager* m_instance;
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);   
    static std::unordered_map<int,int> m_pressedKeys;
public:
    KeyManager(GLFWwindow* win);
    void pollEvents();

};

}