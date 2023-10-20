#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <windows.h>

#include "shader.hpp"
#include "chunk.hpp"

// Settings
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;
const unsigned int FPS = 60;

// Time
float deltaTime = 0.f;
float lastFrame = 0.f;

// Camera
glm::vec3 cameraPos = glm::vec3(0.f, 10.f, 45.f);
glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
bool firstMouse = true;
float fov = 45.f;
float yaw = -90.f;
float pitch = 0.f;
float lastMouseX = SCREEN_WIDTH / 2.f;
float lastMouseY = SCREEN_HEIGHT / 2.f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Avoid camera jump to the mouse position from the center
    if (firstMouse)
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // reversed: y ranges from bottom to top
    lastMouseX = xpos;
    lastMouseY = ypos;

    float sensitivity = 0.05f; // avoid strong movements
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch = glm::clamp(pitch + yoffset, -89.f, 89.f); // constraint the vertical movement

    glm::vec3 direction;
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 5.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    // Fix movement at the ground level (xz plane)
    //cameraPos.y = 0.0f;
}

glm::mat4 makeModelMatrix(glm::vec3& pos)
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    return model;
}
glm::mat4 makeViewMatrix()
{
    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));
    return view;
}
glm::mat4 makeProjectionMatrix()
{
    glm::mat4 projection;
    float fov = glm::radians(45.f);
    float aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
    float nearPlane = 0.1f;
    float farPlane = 100.f;
    projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    return projection;
}

void applyNoise(bool*** chunk)
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            /*
                Generate simplex noise, clamp to [0,1] and multiply by chunk size.
            */
            float noise = glm::simplex(glm::vec2(x / 16.f, z / 16.f));
            float height = ((noise + 1) / 2) * CHUNK_SIZE;
            std::cout << height << std::endl;
            for (int y = 0; y < height; y++)
            {
                chunk[x][y][z] = true;
            }

        }
    }
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

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    Shader shaderProgram("shaders/shader.vert", "shaders/shader.frag");

    bool*** chunk = makeChunk();
    applyNoise(chunk);
    chunkMesh chunkMesh = makeChunkMesh(chunk);
    deleteChunk(chunk);

    // Load texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("assets/dirt.jpg", &width, &height, &nrChannels, 0);

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

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Show frame rate
        //std::cout << 1.f / deltaTime << std::endl;

        processInput(window);

        glClearColor(0.529f, 0.808f, 0.922f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        /* Transformation matrices */
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shaderProgram.setMat4("view", view);
        glm::mat4 projection = makeProjectionMatrix();
        shaderProgram.setMat4("projection", projection);

        for (int x = 0; x < 5*CHUNK_SIZE; x += CHUNK_SIZE)
        {
            for (int z = 0; z < 5*CHUNK_SIZE; z += CHUNK_SIZE)
            {
                glm::vec3 pos(x, 0, z);
                glm::mat4 model = makeModelMatrix(pos);
                shaderProgram.setMat4("model", model);

                renderChunk(chunkMesh, texture);
            }
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Limit frame rate
        //float msPerFrame = 1.f / FPS;
        //if (deltaTime < msPerFrame) Sleep((msPerFrame - deltaTime)*1000.f);
    }

    deleteChunkMesh(chunkMesh);
    shaderProgram.remove();

    glfwTerminate();
    return 0;
}