#include <vulkan/vulkan.h>

class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

private:
    void PickBestGraphicsDevice();

    void SetupInstance();
    void DestroyInstance();

    void SetupDevice();
    void DestroyDevice();

    void SetupSwapchain();
    void DestroySwapchain();

    VkInstance mInstance;
    VkDevice mDevice;

};