#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <dependency/imgui/imgui.h>
#include <dependency/imgui/imgui_impl_glfw_gl3.h>
#include <dependency/glm/glm/glm.hpp>
#include <dependency/glm/glm/gtc/matrix_transform.hpp>
#include <dependency/glm/glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <camera.h>
#include <stb_image.h>

static int winWidth = 640;
static int winHeight = 480;

orbitCamera myCamera(0.0f, 2.0f, 3.0f, winWidth, winHeight);
static double holdDownXPos;
static double holdDownYPos;
static double holdDownPitch;
static double holdDownYaw;
static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        float yawMove = 0.5f * 180.0f * (((float)(xpos - holdDownXPos)) / ((float)winWidth));
        float pitchMove = 0.5f * 180.0f * (((float)(ypos - holdDownYPos)) / ((float)winHeight));
        // std::cout << "pitch=" << pitchMove;
        // std::cout << "yaw=" << yawMove << std::endl;
        myCamera.setPitch(holdDownPitch - pitchMove);
        myCamera.setYaw(holdDownYaw + yawMove);
    }
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "in mbc" << std::endl;
        glfwGetCursorPos(window, &holdDownXPos, &holdDownYPos);
        holdDownPitch = myCamera.getPitch();
        holdDownYaw = myCamera.getYaw();
    }
}
GLFWwindow *GUIInit(void)
{
    glfwSetErrorCallback(error_callback);
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(winWidth, winHeight, "My GUI", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return nullptr;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    //  Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glEnable(GL_DEPTH_TEST);
    return window;
}

int main(int, char **)
{
    auto window = GUIInit();
    // Vertice array
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // vertice buffer
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0,                 // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                 // size
        GL_FLOAT,          // type
        GL_FALSE,          // normalized?
        5 * sizeof(float), // stride
        (void *)0          // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,                          // attribute 0. No particular reason for 0, but must match the layout in the shader.
        2,                          // size
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        5 * sizeof(float),          // stride
        (void *)(3 * sizeof(float)) // array buffer offset
    );
    glEnableVertexAttribArray(1);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("/home/zihao/Desktop/workspace/my_gui_project/data/misc/container.jpg", &width, &height, &nrChannels, 0);
    GLuint texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    data = stbi_load("/home/zihao/Desktop/workspace/my_gui_project/data/misc/awesomeface.png", &width, &height, &nrChannels, 0);
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // coord

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = myCamera.getView();
    glm::mat4 projection = myCamera.getProjection();
    Shader ourShader("/home/zihao/Desktop/workspace/my_gui_project/data/shader/coord.vert", "/home/zihao/Desktop/workspace/my_gui_project/data/shader/coord.frag");
    ourShader.use();
    ourShader.setMat4("view", view);
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setMat4("model", model);
    ourShader.setMat4("projection", projection);
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // glBindVertexArray(VAO);
        ourShader.use();
        view = myCamera.getView();
        projection = myCamera.getProjection();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while (!glfwWindowShouldClose(window));
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}