// Created by Matthew Bach II
// using learnopengl.com's tutorials

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <colorDef.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);

int main() {
    
    // CONFIGURATION OF GLFW 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //mac specific
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // WINDOW OBJECT CREATION WITH GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, 
        "LearnOpenGL", NULL, NULL);
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // INITIALIZE GLAD 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // CREATE OPENGL VIEWPORT
    glViewport(0, 0, 800, 600); 
    // register our callback function for resizing the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  


    // RENDER LOOP
    while(!glfwWindowShouldClose(window)) 
    {
        // input
        processInput(window);

        // rendering
        glClearColor(pink[0], pink[1], pink[2], 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT); // state-using

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        unsigned int VBO; // buffer ID of vertex buffer obj
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // process events, swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}


// update viewport when resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// handle inputs
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}