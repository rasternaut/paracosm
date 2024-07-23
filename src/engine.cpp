#include "engine.h"

#include <iostream>
#include <format>
namespace{

    // Eventually we need a central logger
    void glfw_error_callback(int error, const char* description){
        std::cerr << std::format("GLFW ERROR {}: {}", error, description) << std::endl;
    }

    // Eventually we need a central input event handler
    void glfw_input_callback(GLFWwindow * window, int key, int scancode, int action, int mods){

        const char* key_name = glfwGetKeyName(GLFW_KEY_UNKNOWN, scancode);
        if(key_name){
            std::cerr << std::format("Key Event '{}' : {}", key_name, action) << std::endl;
        }

        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
            // https://www.glfw.org/docs/3.3/window_guide.html#window_close
            glfwSetWindowShouldClose(window, true);
        }
    }

}


namespace paracosm{

Engine::Engine(){

    if(!glfwInit()){
        // shut it all down
        exit(-1);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwSetErrorCallback(glfw_error_callback);
    app_window = glfwCreateWindow(width, height, "Paracosm Demo", nullptr, nullptr);
    glfwSetKeyCallback(app_window, glfw_input_callback);

}

Engine::~Engine(){
    glfwDestroyWindow(app_window);
    glfwTerminate();

}

void Engine::Run(){

    while(!glfwWindowShouldClose(app_window)){
        glfwPollEvents();
    }

}



}