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
    "}\n\0";


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
    // register our callback function for resizing the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // INITIALIZE GLAD 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    
    
    // SHADER PROGRAM
    // --------------
    // compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check if shader compiled properly
    int successV;
    char infoLogV[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successV);
    if(!successV)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogV);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogV << std::endl;
    }

    // compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check if fragment compiled properly
    int successF;
    char infoLogF[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successF);
    if(!successF)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogF);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogF << std::endl;
    }

    // Shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check if shader program compiled properly
    int successS;
    char infoLogS[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successS);
    if(!successS)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogS);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLogS << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // SETUP VERTEX DATA / ATTRIBUTES
    // create data (rectangle)
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 


    // bind VAO then bind VBO, set vertex buffers, configure vertex attributes
    unsigned int VBO, VAO, EBO; // IDs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // copy into vertex array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // copy index array into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // not really necessary


    // RENDER LOOP
    while(!glfwWindowShouldClose(window)) 
    {
        // INPUT
        processInput(window);

        // RENDERING
        // clear screen
        glClearColor(pink[0], pink[1], pink[2], 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT); // state-using

        // draw triangles (in rectangle)
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // process events, swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}


// update viewport when resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // create openGlViewport
    glViewport(0, 0, width, height);
}

// handle inputs
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}