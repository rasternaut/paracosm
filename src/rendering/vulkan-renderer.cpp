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

struct QueueFamilyData{
    std::optional<uint32_t> GraphicsQueue;
    std::optional<uint32_t> ComputeQueue;
    std::optional<uint32_t> PresentQueue;

    bool isValid(){
        return GraphicsQueue.has_value() ; // && ComputeQueue.has_value() && PresentQueue.has_value()
    }
};

QueueFamilyData determineQueuesForDevice(vk::PhysicalDevice device){
    QueueFamilyData data;
    std::vector<vk::QueueFamilyProperties> queueProperties = device.getQueueFamilyProperties();

    for(int ii = 0; ii < queueProperties.size(); ii++){
        vk::QueueFamilyProperties const * properties = &queueProperties[ii];
        // std::cout <<
        //     properties->queueCount <<
        //     " " <<
        //     to_string(properties->queueFlags) <<
        //     std::endl;

        if( !data.GraphicsQueue.has_value() &&  properties->queueFlags & vk::QueueFlagBits::eGraphics){
            data.GraphicsQueue = ii;
        }
    }

    return data;
}

vk::PhysicalDevice selectBestDevice(std::vector<vk::PhysicalDevice> deviceCandidates){

    vk::PhysicalDevice bestCandidate;
    uint32_t bestScore;

    for (auto && candidate : deviceCandidates)
    {
        uint32_t score = 0;

        vk::PhysicalDeviceProperties prop = candidate.getProperties();
        // std::cout << prop.properties.
        std::cout << std::format("{}", prop.deviceName.data()) << std::endl;
        std::cout << std::format("\t {}: {}", "API Version", prop.apiVersion) << std::endl;
        std::cout << std::format("\t {}: {}", "Type", vk::to_string(prop.deviceType)) << std::endl;
        std::cout << std::format("\t {}: {}", "Driver Version", prop.driverVersion) << std::endl;

        QueueFamilyData queueData = determineQueuesForDevice(candidate);

        if(!queueData.isValid()){
            continue;
        }

        switch (prop.deviceType)
        {
            case vk::PhysicalDeviceType::eDiscreteGpu :
                score = 100;
                break;
            case vk::PhysicalDeviceType::eIntegratedGpu :
                score = 10;
                break;
            case vk::PhysicalDeviceType::eCpu :
                score = 5;
                break;
            default:
                score = 1;
                break;
        }

        if(!bestCandidate || score > bestScore){
            bestCandidate = candidate;
            bestScore = score;
        }
    }
    return bestCandidate;
}

void VulkanRenderer::SetupDevice()
{
    std::vector<vk::PhysicalDevice> physicalDevices = mInstance.enumeratePhysicalDevices();

    mPhysicalDevice = selectBestDevice(physicalDevices);

    if(!mPhysicalDevice){
        throw std::runtime_error("No physical device selected");
    }

    QueueFamilyData data = determineQueuesForDevice(mPhysicalDevice);

    float graphicsQueuePriority = 1.0f;

    vk::DeviceQueueCreateInfo graphicsQueueCreateInfo{
        .queueFamilyIndex = data.GraphicsQueue.value(),
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