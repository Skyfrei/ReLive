#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

class App{
    public:
        void run(){
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private:
        void initWindow(){
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            window = glfwCreateWindow(WIDTH, HEIGHT, "Re:live", nullptr, nullptr);
        }

        void initVulkan(){
            createInstance();
        }

        void createInstance(){

            if (enableValidationLayers && !checkValidationLayerSupport()){

                throw std::runtime_error("Validation layers requested, but not available!");
            }   

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "ReLive";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;
            
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
           
            std::vector<const char*> requiredExtensions;
            for (auto i = 0; i < glfwExtensionCount; i++){
                requiredExtensions.emplace_back(glfwExtensions[i]);
            }
            
            requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
            createInfo.ppEnabledExtensionNames = requiredExtensions.data();
            createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
           
            if (enableValidationLayers){
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            }else{
                createInfo.enabledLayerCount = 0;
            }

            VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        
            if (result != VK_SUCCESS){
                std::cerr<<"Error code: "<< result<<"\n";
                throw std::runtime_error("failed to create instance!");
            }
        }

        void mainLoop(){
            while (!glfwWindowShouldClose(window)){
                glfwPollEvents();
            }
        }

        bool checkValidationLayerSupport(){
           uint32_t layerCount;
           vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

           std::vector<VkLayerProperties> availableLayers(layerCount);
           vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

           for (const char* layerName : validationLayers){
                bool layerFound = false;
                for (const auto& layerProperties : availableLayers){
                    if (strcmp(layerName, layerProperties.layerName) == 0){
                        layerFound = true;
                        break;
                     }
                }
                if (!layerFound){
                    return false;
                }
           }

           return true;
        }

        void cleanup(){
            vkDestroyInstance(instance, nullptr);
            glfwDestroyWindow(window);

            glfwTerminate();
        }

    private:
        //  Gui glfw
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        GLFWwindow* window;
        //  Vulkan objects
        VkInstance instance;
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif
};

int main(){
    App app;
    try{
        app.run();
    } catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
