#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

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
    VkInstance instance;

    void initVulkan() {
        createInstance(); // just a custom function from the tutorial to group all the vk configuration
    }

    void mainLoop() {
        // just like raylib, there is a similar function to prevent the program end
        // until clicking the close button

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        // we must explicitly destory the windows because this is C++, and it doesn't have 
        // garbage collection like java or go
        // In zig we also need to do the same, perhaps writing a deinit() function
        vkDestroyInstance(instance, nullptr);

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

    void createInstance() {
        // 10
        // basically all the vulkan configuration locates here
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // usually 0
        appInfo.pApplicationName = "Hello Triangle";
        
        // VK_MAKE_VERSION has been deprecated, so I will use the suggested VK_MAKE_API_VERSION
        appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // 20
        // createInfo is used for determine the type of extensions and validation used for the creation process
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // 30
        // initialize global extensions
        // but since we have no specific extention to be used in this example,
        // all we need is a dummy extension by setting them as 0

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions; // use undefined for zig

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // 40 
        // load all the defined extension back to the createInfo
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        // 50
        // This is for defining the number of layer for the validation,
        // nothing is needed for this example
        createInfo.enabledLayerCount = 0;

        // 60
        // we all the configuration above, we can now construct a vk instance
        // and the parameters for the function call is the fullowing:
        // - borrowing createInfo (VkInstanceCreateInfo)
        // - custom allocator callback, not useful for the triangle example
        // - the return VK object (pass by reference)
        // 
        // Thus, this function works in a C/zig way, which the function returns
        // a state rather the actual object which is passed by a parameter
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        // just like when I was using SunVox Lib, the result can be used to
        // determine whether the instance or operation is created successfully
        if (result != VK_SUCCESS) {
            throw std::runtime_error("<!-- failed to create VK instance -->");
        }

        // 70 
        // To check the extionsion support, we can use vkEnumerateInstanceExtensionProperties
        uint32_t extensionCount = 0;
        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        // 80
        // iterate extension name;
        std::cout << "available extensions:" << std::endl;;

        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << std::endl;
        }
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