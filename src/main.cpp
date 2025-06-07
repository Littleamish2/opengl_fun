// Created by Matthew Bach II
// using learnopengl.com's tutorials

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <string>
#include <ctime>

#include <colorDef.h>
#include <shader.h>

// change this as needed
char *filepath = "/Users/matthewbach/Desktop/Code/OpenGL/captures/";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window, const char* filepath);
void saveImage(const char* filepath, GLFWwindow* w);

int main() {
    // Create filepath based on date
    time_t now = time(0);
    tm* localTime = localtime(&now);

    int yearI = localTime->tm_year + 1900; // Years since 1900
    int monthI = localTime->tm_mon + 1;    // Months are 0-indexed
    int dayI = localTime->tm_mday;

    std::string yearS, monthS, dayS;
    yearS = std::to_string(yearI);
    monthS = std::to_string(monthI);
    dayS = std::to_string(dayI);
    
    std::string pathS(filepath);
    pathS += (yearS + "_" + monthS + "_" + dayS + ".png");
    const char* updated_filepath = pathS.c_str();


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

    
    // SHADER SETUP
    Shader ourShader("/Users/matthewbach/Desktop/Code/OpenGL/firsttime/src/shaders/vertShader.vs", "/Users/matthewbach/Desktop/Code/OpenGL/firsttime/src/shaders/fragShader.fs");   


    // SETUP VERTEX DATA / ATTRIBUTES
    // create data (triangle)
    float vertices[] = {
        // positions         
        0.5f, -0.5f, 0.0f,    // bottom right
        -0.5f, -0.5f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f     // top 
    };    


    // bind VAO then bind VBO, set vertex buffers, configure vertex attributes
    unsigned int VBO, VAO; // IDs
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);

    // copy into vertex array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

   
    
    // unbind
    glBindVertexArray(0); // not really necessary

    

    // RENDER LOOP
    while(!glfwWindowShouldClose(window)) 
    {
        // INPUT
        processInput(window, updated_filepath);

        // RENDERING
        // clear screen
        glClearColor(pink[0], pink[1], pink[2], 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT); // state-using

        // activate shader
        ourShader.use();


        // draw triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // process events, swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ourShader.deleteShader();

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
void processInput(GLFWwindow *window, const char* filepath) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        saveImage(filepath, window);
}

// Credit to Lencerf, 
// from: https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/
void saveImage(const char* filepath, GLFWwindow* w) {
    
    int width, height;
    GLint pView[4];
    glGetIntegerv(GL_VIEWPORT, pView);
    width = pView[2];
    height = pView[3];

    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;

    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
}