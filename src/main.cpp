#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <windows.h>

#include <iostream>
#include <sstream>

#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "chunk.h"
#include "chunk-management.h"

// Settings
const unsigned int kScreenWidth = 1280;
const unsigned int kScreenHeight = 720;
const unsigned int kFps = 60;

// Time
float delta_time = 0.f;
float last_frame = 0.f;

// Camera
Camera camera(glm::vec3(0.f, 45.f, 0.f));
bool first_mouse = true;
float last_mouse_x = kScreenWidth / 2.f;
float last_mouse_y = kScreenHeight / 2.f;

GLFWwindow* window;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (first_mouse)
    {
        last_mouse_x = xpos;
        last_mouse_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_mouse_x;
    float yoffset = last_mouse_y - ypos;
    last_mouse_x = xpos;
    last_mouse_y = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
}

glm::mat4 MakeModelMatrix(glm::vec3& pos)
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    return model;
}
glm::mat4 MakeViewMatrix(void)
{
    glm::mat4 view = camera.GetViewMatrix();
    return view;
}
glm::mat4 MakeProjectionMatrix(void)
{
    glm::mat4 projection;
    float fov = glm::radians(45.f);
    float aspect_ratio = (float)kScreenWidth / kScreenHeight;
    float near_plane = 0.1f;
    float far_plane = 400.f;
    projection = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
    return projection;
}

void ShowFps()
{
    float fps = 1.f / delta_time;
    std::stringstream ss;
    ss << "Minecraft Clone" << " " << "[" << fps << " FPS]";
    glfwSetWindowTitle(window, ss.str().c_str());
}

int main(void)
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(kScreenWidth, kScreenHeight, "Minecraft Clone", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    Shader global_shader("shaders/shader.vert", "shaders/shader.frag");

    UpdateLoadList();
    UpdateSetupList();
    UpdateVisibilityList();
    UpdateRenderList();

    LoadTextures(global_shader);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        ShowFps();
        ProcessInput();

        glClearColor(0.529f, 0.808f, 0.922f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        global_shader.use();
        /* Transformation matrices */
        glm::mat4 view = MakeViewMatrix();
        global_shader.setMat4("view", view);
        glm::mat4 projection = MakeProjectionMatrix();
        global_shader.setMat4("projection", projection);

        BindTextures();

        for (Chunk* chunk : render_list)
        {
            glm::mat4 model = MakeModelMatrix(chunk->position);
            global_shader.setMat4("model", model);

            RenderChunk(chunk);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Limit frame rate
        //float msPerFrame = 1.f / kFps;
        //if (delta_time < msPerFrame) Sleep((msPerFrame - delta_time)*1000.f);
    }

    for (Chunk* chunk : render_list) DeleteChunk(chunk);
    global_shader.remove();

    glfwTerminate();
    return 0;
}