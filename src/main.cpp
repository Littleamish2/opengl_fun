// Created by Matthew Bach II
// using learnopengl.com's tutorials

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <colorDef.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";


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
        // INPUT
        processInput(window);

        // RENDERING
        // clear screen
        glClearColor(pink[0], pink[1], pink[2], 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT); // state-using

        // create VBO
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        
        unsigned int VBO; // buffer ID of vertex buffer obj
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // compile vertex shader
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // check if shader compiled properly
        int successV;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successV);
        if(!successV)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // compile fragment shader
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // check if fragment compiled properly
        int successF;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successF);
        if(!successF)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


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