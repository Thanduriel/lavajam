#pragma once

#include <unordered_map>

struct GLFWwindow;

namespace Input {

class KeyManager
{
private:
    static GLFWwindow* m_window;   
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);   
    static std::unordered_map<int,int> m_pressedKeys;
public:
	static void Init(GLFWwindow* win);

    void pollEvents();

};

}