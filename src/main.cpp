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
#include "texture.h"
#include "chunk.h"

// Settings
const unsigned int kScreenWidth = 1280;
const unsigned int kScreenHeight = 720;
const unsigned int kFps = 60;

GLFWwindow* window;

// Time
float delta_time = 0.f;
float last_frame = 0.f;

// Camera
glm::vec3 camera_pos = glm::vec3(0.f, 45.f, 45.f);
glm::vec3 camera_front = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 camera_up = glm::vec3(0.f, 1.f, 0.f);
bool first_mouse = true;
float fov = 45.f;
float yaw = -90.f;
float pitch = 0.f;
float last_mouse_x = kScreenWidth / 2.f;
float last_mouse_y = kScreenHeight / 2.f;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Avoid camera jump to the mouse position from the center
    if (first_mouse)
    {
        last_mouse_x = xpos;
        last_mouse_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_mouse_x;
    float yoffset = last_mouse_y - ypos; // reversed: y ranges from bottom to top
    last_mouse_x = xpos;
    last_mouse_y = ypos;

    float sensitivity = 0.05f; // avoid strong movements
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch = glm::clamp(pitch + yoffset, -89.f, 89.f); // constraint the vertical movement

    glm::vec3 direction;
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}

void ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float camera_speed = 10.f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    
    // Fix movement at the ground level (xz plane)
    //camera_pos.y = 0.0f;
}

glm::mat4 MakeModelMatrix(glm::vec3& pos)
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    return model;
}
glm::mat4 MakeViewMatrix(void)
{
    glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
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

    Terrain terrain = MakeTerrain();
    PrepareToRender(terrain);

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

        for (int i = 0; i < terrain.chunks.size(); i++)
        {
            glm::mat4 model = MakeModelMatrix(terrain.chunks[i]->position);
            global_shader.setMat4("model", model);

            RenderChunk(terrain.chunks[i]);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Limit frame rate
        //float msPerFrame = 1.f / kFps;
        //if (delta_time < msPerFrame) Sleep((msPerFrame - delta_time)*1000.f);
    }

    DeleteTerrain(terrain);
    global_shader.remove();

    glfwTerminate();
    return 0;
}