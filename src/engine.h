#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "rendering/vulkan-renderer.h"

namespace paracosm {

class Engine{
    public:

        Engine();
        ~Engine();
        void Run();

    private:

        GLFWwindow * app_window;
        uint32_t height  = 720;
        uint32_t width = 1280;

        VulkanRenderer * mRenderingEngine = nullptr;

};


}

