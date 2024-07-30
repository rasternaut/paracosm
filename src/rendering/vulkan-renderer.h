#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>


class VulkanRenderer {
public:
    VulkanRenderer(GLFWwindow * AppWindow, uint32_t height, uint32_t width);
    ~VulkanRenderer();

private:
    uint32_t height;
    uint32_t width;
    void PickBestGraphicsDevice();

    void SetupInstance();
    void DestroyInstance();

    void SetupDevice();
    void DestroyDevice();

    void SetupGraphicsQueue();
    void SetupSurface(GLFWwindow * AppWindow);
    void DestroySurface();

    void SetupSwapchain();
    void DestroySwapchain();

    vk::Instance mInstance;
    vk::PhysicalDevice mPhysicalDevice;
    vk::Device mDevice;
    vk::Queue mGraphicsQueue;

    vk::SurfaceKHR mSurface;
    vk::SwapchainKHR mSwapchain;
};