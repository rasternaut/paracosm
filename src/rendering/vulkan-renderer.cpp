#include "vulkan-renderer.h"
#include "GLFW/glfw3.h"
#include "vulkan/vulkan_to_string.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

VulkanRenderer::VulkanRenderer(GLFWwindow * AppWindow, uint32_t height, uint32_t width)
{
    this->height = height;
    this->width = width;
    SetupInstance();
    SetupDevice();
    SetupGraphicsQueue();
    SetupSurface(AppWindow);
    SetupSwapchain();
}

VulkanRenderer::~VulkanRenderer()
{
    std::cout << "shutdown time" << std::endl;
    DestroySwapchain();
    DestroySurface();
    DestroyDevice();
    DestroyInstance();
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

    // TODO: slurp this up from a text file and populate version and app name that way
    vk::ApplicationInfo applicationInfo{
        .pApplicationName = "Paracosm Core",
        .pEngineName = "Paracosm Engine",
        .engineVersion = VK_MAKE_VERSION(0,0,1),
        .apiVersion = VK_MAKE_VERSION(1,3,290)
    };

    vk::InstanceCreateInfo instanceCreateInfo{
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = (uint32_t)enabledLayers.size(),
        .ppEnabledLayerNames = enabledLayers.data(),
        .enabledExtensionCount = (uint32_t)enabledExtensions.size(),
        .ppEnabledExtensionNames = enabledExtensions.data()
    };

    // TODO: Set up debug call back

    mInstance = vk::createInstance(instanceCreateInfo, nullptr);

    return;
}

void VulkanRenderer::DestroyInstance()
{
    mInstance.destroy();
}

void VulkanRenderer::SetupDevice()
{
    std::vector<vk::PhysicalDevice> physicalDevices = mInstance.enumeratePhysicalDevices();

    for( vk::PhysicalDevice const & device : physicalDevices){
        vk::PhysicalDeviceProperties2 prop = device.getProperties2();
        // std::cout << prop.properties.
        std::cout << std::format("{}", prop.properties.deviceName.data()) << std::endl;
        std::cout << std::format("\t {}: {}", "API Version", prop.properties.apiVersion) << std::endl;
        std::cout << std::format("\t {}: {}", "Type", vk::to_string(prop.properties.deviceType)) << std::endl;
        std::cout << std::format("\t {}: {}", "Driver Version", prop.properties.driverVersion) << std::endl;
        // std::cout << std::format("\t {}: {}", "Limits", vk::to_string(prop.properties.limits)) << std::endl;

        // do the same for features
        vk::PhysicalDeviceFeatures2 feat = device.getFeatures2();
        // TODO go through list of features and determine what we want to log/look at for selection

        // Temp hack to pick a discrete card
        if(prop.properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ){
            mPhysicalDevice = device;
        }
    }


    if(!mPhysicalDevice){
        throw std::runtime_error("No physical device selected");
    }

    std::vector<vk::QueueFamilyProperties2> QueueFamilyProperties = mPhysicalDevice.getQueueFamilyProperties2();

    uint32_t graphicsQueueIndex = 0;
    float graphicsQueuePriority = 1.0f;

    // for(vk::QueueFamilyProperties2 const & prop: QueueFamilyProperties){
    for(int ii = 0; ii < QueueFamilyProperties.size(); ii++){
        vk::QueueFamilyProperties const * properties = &QueueFamilyProperties[ii].queueFamilyProperties;
        std::cout <<
            properties->queueCount <<
            " " <<
            to_string(properties->queueFlags) <<
            std::endl;

        if(properties->queueFlags & vk::QueueFlagBits::eGraphics){
            graphicsQueueIndex = ii;
            break;
        }
    }

    vk::DeviceQueueCreateInfo graphicsQueueCreateInfo{
        .queueFamilyIndex = graphicsQueueIndex,
        .queueCount = 1,
        .pQueuePriorities = &graphicsQueuePriority
    };

    vk::PhysicalDeviceFeatures deviceFeatures{

    };

    std::vector<const char * > deviceExtensionNames = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };



    vk::DeviceCreateInfo deviceCreateInfo{
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &graphicsQueueCreateInfo,
        .enabledExtensionCount = (uint32_t)deviceExtensionNames.size(),
        .ppEnabledExtensionNames = deviceExtensionNames.data(),
        .pEnabledFeatures = &deviceFeatures,
    };

    mDevice = mPhysicalDevice.createDevice(deviceCreateInfo);



}

void VulkanRenderer::DestroyDevice()
{
    mDevice.destroy();
}

void VulkanRenderer::SetupGraphicsQueue()
{
    // this is hardcoded and possibly wrong on any other machine
    vk::DeviceQueueInfo2 graphicsQueueInfo{
        .queueFamilyIndex = 0,
        .queueIndex = 0
    };
    mGraphicsQueue = mDevice.getQueue2(graphicsQueueInfo);
}

void VulkanRenderer::SetupSwapchain()
{
    vk::SurfaceCapabilitiesKHR capabilities =  mPhysicalDevice.getSurfaceCapabilitiesKHR(mSurface);
    std::vector<vk::SurfaceFormatKHR> formats = mPhysicalDevice.getSurfaceFormatsKHR(mSurface);
    std::vector<vk::PresentModeKHR> presentModes = mPhysicalDevice.getSurfacePresentModesKHR(mSurface);

    uint32_t imageCount = capabilities.minImageCount + 1;
    if(capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount){
        imageCount = capabilities.maxImageCount;
    }

    vk::SurfaceFormatKHR surfaceFormat ;
    for(auto const & format : formats){
        if(format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
            surfaceFormat = format;
            break;
        }
    }

    vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo; //guaranteed on all systems  https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPresentModeKHR.html
    for(auto const & mode : presentModes){
        if(mode == vk::PresentModeKHR::eMailbox){
            presentMode = mode;
            break;
        }
    }

    vk::Extent2D extent{
        .width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        .height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
    };

    vk::SwapchainCreateInfoKHR swapchainCreateInfo ={
        .surface = mSurface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment
    };
    //TODO tons more stuff before this will work


    mSwapchain = mDevice.createSwapchainKHR(swapchainCreateInfo);

}

void VulkanRenderer::DestroySwapchain()
{
    mDevice.destroySwapchainKHR(mSwapchain);
}


void VulkanRenderer::SetupSurface(GLFWwindow * AppWindow){
    VkSurfaceKHR surface = {};
    VkResult result = glfwCreateWindowSurface(mInstance, AppWindow, nullptr, &surface);
    mSurface = surface;
}

void VulkanRenderer::DestroySurface(){
    mInstance.destroySurfaceKHR(mSurface);
}