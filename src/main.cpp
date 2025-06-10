// Created by Matthew Bach II
// using learnopengl.com's tutorials

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <string>
#include <ctime>

#include <colorDef.h>
#include <shader.h>


// change this as needed
char *filepath = "/Users/matthewbach/Desktop/Code/OpenGL/captures/";



void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window, const char* filepath, float* mix_add);
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
    Shader ourShader("src/shaders/vertShader.vs", "src/shaders/fragShader.fs");   

    // SETUP VERTEX DATA / ATTRIBUTES
    // create data (rectangle)
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };  

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
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
    // element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);



    // TEXTURES
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    glBindTexture(GL_TEXTURE_2D, texture1);
    // options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { green[0], green[1], green[2], 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture2);
    // options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate
    int width1, width2, height1, height2, nrChannels1, nrChannels2;
    unsigned char *data1 = stbi_load("src/resources/container.jpg", &width1, &height1, &nrChannels1, 0);
    
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data2 = stbi_load("src/resources/awesomeface.png", &width2, &height2, &nrChannels2, 0);
    
    if (data1 && data2)
    {
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load textures" << std::endl;
    }
    // free

    stbi_image_free(data1);
    stbi_image_free(data2);

    
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    

    float mix_add = 0.0;


    // RENDER LOOP
    while(!glfwWindowShouldClose(window)) 
    {
        // INPUT
        processInput(window, updated_filepath, &mix_add);
        ourShader.setFloat("mixing", 0.2 + mix_add);

        // RENDERING
        // clear screen
        glClearColor(black[0], black[1], black[2], 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT); // state-using
    
        
        glActiveTexture(GL_TEXTURE0);  // defined in order! (0, 1, 2...)
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);  
        glBindTexture(GL_TEXTURE_2D, texture2);



        // draw rectangle with texture
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // process events, swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
void processInput(GLFWwindow *window, const char* filepath, float* mix_add) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        saveImage(filepath, window);
    else {
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            *mix_add += 0.001f;
            if (*mix_add > 0.8)
                *mix_add = 0.8;
        }
        else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            *mix_add -= 0.001f;
            if(*mix_add < -0.2) 
                *mix_add = -0.2;
        }
    }
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