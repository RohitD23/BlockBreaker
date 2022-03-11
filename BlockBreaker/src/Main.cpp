#include "Game.h"
#include "Window/Window.h"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game BlockBreaker(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSetKeyCallback(window.GetWindow(), key_callback);

    // initialize game
    BlockBreaker.Init();

    // deltaTime variables-
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!window.isClosed()) {
        // calculate delta time
        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // manage user input
        BlockBreaker.ProcessInput(deltaTime);

        // update game state
        BlockBreaker.Update(deltaTime);

        window.Clear();

        BlockBreaker.Render();

        window.Update();
    }

    ResourceManager::Clear();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (action == GLFW_PRESS)
            BlockBreaker.SetKey(key, true);
        else if (action == GLFW_RELEASE)
        {
            BlockBreaker.SetKey(key, false);
            BlockBreaker.SetKeyProcessed(key, false);
        }
    }
}