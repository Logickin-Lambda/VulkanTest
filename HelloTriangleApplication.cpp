#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

    GLFWwindow* window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    void initVulkan() {

    }

    void mainLoop() {
        // just like raylib, there is a similar function to prevent the program end
        // until clicking the close button

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        // we must explicitly destory the windows because this is C++
        // In zig we also need to do the same, perhaps writing a deinit() function
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void initWindow() {

        // just like zig, c++ also require some form of initializations
        glfwInit();

        // Windows requires a bunch hint to be correctly created
        // and to simplify the tutorial

        // Since we are using vulkan, not opengl, we have to disable the original opengl context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // the "Vulkan" here is nothing related to the api, and it is just a name for the window
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }
};

int runTriangleApp() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}