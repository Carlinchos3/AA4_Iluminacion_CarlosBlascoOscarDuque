#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include "Model.h"
#include "Camera.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "GameObject.h"
#include "Satelite.h"
#include "Sky.h"
#include "RenderManager.h"
#include "ModelManager.h"
#include <fstream>
#include <sstream>

//Ventana
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

//Ciclo del sol y la luna
#define CYCLE_DURATION 20.0f
#define ORBIT_RADIUS 15.0f

//Linterna
#define FLASHLIGHT_INNER_CONE glm::cos(glm::radians(15.f))
#define FLASHLIGHT_OUTER_CONE glm::cos(glm::radians(25.f))
#define FLASHLIGHT_MAX_DIST 10.0f

RenderManager* renderManager = nullptr;

void Resize_Window(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glUniform2f(glGetUniformLocation(renderManager->GetProgram(), "windowSize"), (float)width, (float)height);
}

void main() {

    srand(static_cast<unsigned int>(time(NULL)));

    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    if (glewInit() == GLEW_OK) 
    {
        // Shaders
        ShaderManager shaderManager("MyFirstVertexShader.glsl", "MyFirstGeometryShader.glsl", "MyFirstFragmentShader.glsl");
        shaderManager.Use();

        // Camara e input
        Camera camera(glm::vec3(0.0f, 3.0f, 8.0f), -90.0f, 0.0f);
        InputManager inputManager(camera);
        glfwSetWindowUserPointer(window, &inputManager);
        glfwSetCursorPosCallback(window, InputManager::MouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // ModelManager
        ModelManager modelManager;
        modelManager.LoadAssets();

        // Astros
        Satelite sun(&modelManager.GetSphereModel(), &modelManager.GetSunTexture(), ORBIT_RADIUS, glm::vec3(0.5f));
        Satelite moon(&modelManager.GetSphereModel(), &modelManager.GetMoonTexture(), ORBIT_RADIUS, glm::vec3(0.3f));

        // Sky
        Sky sky(&sun, &moon, CYCLE_DURATION);

        // RenderManager
        RenderManager _renderManager(&shaderManager);
        renderManager = &_renderManager;
        _renderManager.AddObject(&moon);
        _renderManager.AddObject(&sun);

        //Objeto fijos
        modelManager.AddFixedObjects(_renderManager);
        modelManager.SpawnObjects(_renderManager);
        
        glfwSetFramebufferSizeCallback(window, Resize_Window);

        // Uniforms fijos
        GLuint program = shaderManager.GetProgram();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, camera.GetNear(), camera.GetFar());
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform2f(glGetUniformLocation(program, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
        glUniform1i(glGetUniformLocation(program, "textureSampler"), 0);
        glUniform1f(glGetUniformLocation(program, "flashlightInnerCone"), FLASHLIGHT_INNER_CONE);
        glUniform1f(glGetUniformLocation(program, "flashlightOuterCone"), FLASHLIGHT_OUTER_CONE);
        glUniform1f(glGetUniformLocation(program, "flashlightMaxDist"), FLASHLIGHT_MAX_DIST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        float lastFrame = 0.0f;

        // Game loop
        while (!glfwWindowShouldClose(window)) 
        {

            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            float currentFrame = (float)glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            inputManager.ProcessKeyboard(window, deltaTime);
            _renderManager.Render(camera, sky, inputManager, currentFrame);

            glFlush();
            glfwSwapBuffers(window);
        }

        glUseProgram(0);
        glDeleteProgram(program);
    }
    else {
        std::cerr << "Error al inicializar GLEW." << std::endl;
    }

    glfwTerminate();
}