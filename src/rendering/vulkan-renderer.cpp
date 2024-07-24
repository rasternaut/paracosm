#include "vulkan-renderer.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <string>
#include <stdexcept>

VulkanRenderer::VulkanRenderer()
{
    SetupInstance();
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::PickBestGraphicsDevice()
{
}

void VulkanRenderer::SetupInstance()
{

    uint32_t extensionCount = 0;
    const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    std::vector<const char *> enabledExtensions;
    for(int ii = 0; ii < extensionCount; ii++){
        enabledExtensions.push_back(glfwExtensions[ii]);
    }


    std::vector<const char*> enabledLayers;
#if !NDEBUG
    enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
    enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    VkApplicationInfo applicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Paracosm Core",
        .pEngineName = "Paracosm Engine",
        .engineVersion = VK_MAKE_VERSION(0,0,1),
        .apiVersion = VK_MAKE_VERSION(1,3,290)
    };

    VkInstanceCreateInfo instanceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = (uint32_t)enabledLayers.size(),
        .ppEnabledLayerNames = enabledLayers.data(),
        .enabledExtensionCount = (uint32_t)enabledExtensions.size(),
        .ppEnabledExtensionNames = enabledExtensions.data()
    };

    if(!vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) == VK_SUCCESS){
        throw std::runtime_error("failed to create vulkan instance");
    }


    return;
}

void VulkanRenderer::DestroyInstance()
{
}

void VulkanRenderer::SetupDevice()
{
}

void VulkanRenderer::DestroyDevice()
{
}

void VulkanRenderer::SetupSwapchain()
{
}

void VulkanRenderer::DestroySwapchain()
{
}
