// Created by Matthew Bach II
// using learnopengl.com's tutorials

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <colorDef.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 vertex_color;\n"
    "out vec3 colour;\n"
    "void main()\n"
    "{\n"
    "   colour = vertex_color;\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec3 colour;\n"
    "out vec4 FragColour;\n"
    "void main() {\n"
    "   FragColour = vec4(colour.x, colour.y, colour.z, 1.0f);\n"
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
    float colors[] = {
        red[0], red[1], red[2], // top right
        green[0], green[1], green[2], // bottom right
        blue[0], blue[1], blue[2], // bottom left
        black[0], black[1], black[2] // top left
    };
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 


    // bind VAO then bind VBO, set vertex buffers, configure vertex attributes
    unsigned int posVBO, colVBO, VAO, EBO; // IDs
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &posVBO);
    glGenBuffers(1, &colVBO);
    glGenBuffers(1, &EBO);

    // copy into position vertex array
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
     // copy index array into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // copy into color vertex array
    glBindBuffer(GL_ARRAY_BUFFER, colVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

   
    
    // unbind
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
    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &colVBO);
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