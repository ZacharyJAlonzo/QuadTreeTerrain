// OpenGLTemplate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "QuadTree/QuadTree.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Camera camera = Camera(glm::vec3(0, 0, 0));
bool firstMouse = true;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.CameraPos -= camera.CameraFront * camera.speed;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.CameraPos += camera.CameraFront * camera.speed;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.CameraPos -= camera.CameraRight * camera.speed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.CameraPos += camera.CameraRight * camera.speed;

    camera.UpdateCamera(camera.CameraFront, camera.CameraPos);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        camera.lastX = xpos;
        camera.lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos;
    camera.lastX = xpos;
    camera.lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw -= xoffset;
    camera.pitch -= yoffset;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    
    camera.UpdateCamera(direction, camera.CameraPos);
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        return -1;
    }

    QuadTree tree = QuadTree(0.f, 100.f, 100.f, 0.f, 10);


    float points[] = { -0.5, 0.0, -0.5,
                        0.5, 0.0, -0.5,
                        0.5, 0.0, 0.5,
                        -0.5, 0.0, 0.5 };

    unsigned int elem[] = { 0, 1, 2,
                    0, 3, 2 };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elem), elem, GL_STATIC_DRAW);

    Shader shader = Shader("res/Basic.shader");
    shader.Use();

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/img/97295-mountain2-height-map-merged.png", &width, &height, &nrChannels, STBI_rgb);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);






    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);


        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0, 0, -1.0));
        glm::mat4 view = glm::mat4(1.0);
        glm::mat4 project = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

        glm::mat4 mvp;
        mvp = camera.projection * camera.view * model;

        shader.SetMat4("mvp", false, mvp);
      
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        tree.Render(camera, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}