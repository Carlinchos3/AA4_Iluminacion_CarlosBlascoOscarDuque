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
#include <fstream>
#include <sstream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define CYCLE_DURATION 20.0f
#define ORBIT_RADIUS 15.0f
#define FLASHLIGHT_INNER_CONE glm::cos(glm::radians(15.f))
#define FLASHLIGHT_OUTER_CONE glm::cos(glm::radians(25.f))
#define FLASHLIGHT_MAX_DIST 50.0f

RenderManager* renderManager = nullptr;

void Resize_Window(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glUniform2f(glGetUniformLocation(renderManager->GetProgram(), "windowSize"), (float)width, (float)height);
}

Model LoadOBJModel(const std::string& filePath) {

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line, prefix;
    std::stringstream ss;
    glm::vec3 tmpVec3;
    glm::vec2 tmpVec2;

    std::vector<float> vertexs, vertexNormal, textureCoordinates;
    std::vector<float> tmpVertexs, tmpNormals, tmpTextureCoordinates;

    while (std::getline(file, line)) {
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v") {
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;
            tmpVertexs.push_back(tmpVec3.x);
            tmpVertexs.push_back(tmpVec3.y);
            tmpVertexs.push_back(tmpVec3.z);
        }
        else if (prefix == "vt") {
            ss >> tmpVec2.x >> tmpVec2.y;
            tmpTextureCoordinates.push_back(tmpVec2.x);
            tmpTextureCoordinates.push_back(tmpVec2.y);
        }
        else if (prefix == "vn") {
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;
            tmpNormals.push_back(tmpVec3.x);
            tmpNormals.push_back(tmpVec3.y);
            tmpNormals.push_back(tmpVec3.z);
        }
        else if (prefix == "f") {
            int vertexData;
            short counter = 0;
            while (ss >> vertexData) {
                switch (counter) {
                case 0:
                    vertexs.push_back(tmpVertexs[(vertexData - 1) * 3]);
                    vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 1]);
                    vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 2]);
                    ss.ignore(1, '/');
                    counter++;
                    break;
                case 1:
                    textureCoordinates.push_back(tmpTextureCoordinates[(vertexData - 1) * 2]);
                    textureCoordinates.push_back(tmpTextureCoordinates[((vertexData - 1) * 2) + 1]);
                    ss.ignore(1, '/');
                    counter++;
                    break;
                case 2:
                    vertexNormal.push_back(tmpNormals[(vertexData - 1) * 3]);
                    vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
                    vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
                    counter = 0;
                    break;
                }
            }
        }
    }
    return Model(vertexs, textureCoordinates, vertexNormal);
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
        Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), -90.0f, 0.0f);
        InputManager inputManager(camera);
        glfwSetWindowUserPointer(window, &inputManager);
        glfwSetCursorPosCallback(window, InputManager::MouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Modelos
        Model trollModel = LoadOBJModel("Assets/Models/troll.obj");
        Model sphereModel = LoadOBJModel("Assets/Models/sphere.obj");
        Model rockModel = LoadOBJModel("Assets/Models/rock.obj");
        Model skullModel = LoadOBJModel("Assets/Models/skull.obj");

        // Texturas
        Texture trollTexture("Assets/Textures/troll.png");
        Texture sunTexture("Assets/Textures/sun.png");
        Texture moonTexture("Assets/Textures/moon.png");
        Texture rockTexture("Assets/Textures/rock.png");
        Texture skullTexture("Assets/Textures/skull.png");

        // GameObjects
        GameObject trollLeft(&trollModel, &trollTexture, glm::vec3(-1.5f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.0f));
        GameObject trollRight(&trollModel, &trollTexture, glm::vec3(1.5f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.0f));
        GameObject rock(&rockModel, &rockTexture, glm::vec3(-1.f, 0.0f, -0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
        GameObject skull(&skullModel, &skullTexture, glm::vec3(1.5f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.01f));

        // Astros
        Satelite sun(&sphereModel, &sunTexture, ORBIT_RADIUS, glm::vec3(0.5f));
        Satelite moon(&sphereModel, &moonTexture, ORBIT_RADIUS, glm::vec3(0.3f));

        // Sky
        Sky sky(&sun, &moon, CYCLE_DURATION);

        // RenderManager
        RenderManager _renderManager(&shaderManager);
        renderManager = &_renderManager;
        _renderManager.AddObject(&trollLeft);
        _renderManager.AddObject(&trollRight);
        _renderManager.AddObject(&sun);
        _renderManager.AddObject(&rock);
        _renderManager.AddObject(&skull);
        _renderManager.AddObject(&moon);

        glfwSetFramebufferSizeCallback(window, Resize_Window);

        // Uniforms fijos
        GLuint program = shaderManager.GetProgram();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform2f(glGetUniformLocation(program, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
        glUniform1i(glGetUniformLocation(program, "textureSampler"), 0);
        glUniform1f(glGetUniformLocation(program, "flashlightInnerCone"), glm::cos(glm::radians(FLASHLIGHT_INNER_CONE)));
        glUniform1f(glGetUniformLocation(program, "flashlightOuterCone"), glm::cos(glm::radians(FLASHLIGHT_OUTER_CONE)));
        glUniform1f(glGetUniformLocation(program, "flashlightMaxDist"), FLASHLIGHT_MAX_DIST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        float lastFrame = 0.0f;

        // Game loop
        while (!glfwWindowShouldClose(window)) {

            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            float currentFrame = (float)glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            inputManager.ProcessKeyboard(window, deltaTime);
            //inputManager.PassUniforms(program);

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