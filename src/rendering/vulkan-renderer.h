#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>


class VulkanRenderer {
public:
    VulkanRenderer(GLFWwindow * AppWindow, uint32_t height, uint32_t width);
    ~VulkanRenderer();

    // Temporary draw method, will eventually be replaced when we have the engine properly running
    void DrawFrame();

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
    void SetupSwapchainImages();
    // void DestroySwapchainImages();
    void DestroySwapchain();

    vk::Instance mInstance;
    vk::PhysicalDevice mPhysicalDevice;
    vk::Device mDevice;
    vk::Queue mGraphicsQueue;
    vk::CommandPool mCommandPool;
    std::vector<vk::CommandBuffer> mCommandBuffers;

    vk::SurfaceKHR mSurface;
    vk::SwapchainKHR mSwapchain;
    std::vector<vk::Image> mSwapchainImages;

};