add_executable(
    Demo
    demo.cpp

    engine.cpp
    engine.h
    rendering/vulkan-renderer.h
    rendering/vulkan-renderer.cpp

    ${imgui_source_files}
)

target_link_libraries( Demo
    Vulkan::Vulkan
    glfw
    imgui
    ${OpenCV_LIBS}
 )