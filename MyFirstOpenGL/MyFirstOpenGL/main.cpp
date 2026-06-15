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

//Ventana
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//Ciclo del sol y la luna
#define CYCLE_DURATION 20.0f
#define ORBIT_RADIUS 15.0f

//Linterna
#define FLASHLIGHT_INNER_CONE glm::cos(glm::radians(15.f))
#define FLASHLIGHT_OUTER_CONE glm::cos(glm::radians(25.f))
#define FLASHLIGHT_MAX_DIST 10.0f

//Spawnpoints
#define SPAWNPOINTS 6
#define SPAWN_TYPE_COUNT        5
#define SPAWN_TYPE_TROLL        0
#define SPAWN_TYPE_SEMAFORO     1
#define SPAWN_TYPE_VALLA        2
#define SPAWN_TYPE_SENAL        3
#define SPAWN_TYPE_BOLA         4
#define TROLL_SCALE_BASE        0.8f
#define SEMAFORO_SCALE_BASE     0.05f
#define VALLA_SCALE_BASE        0.15f
#define SENAL_SCALE_BASE        0.02f
#define BOLA_SCALE_BASE         0.55f
#define SCALE_VARIANCE          0.6f

RenderManager* renderManager = nullptr;

void Resize_Window(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glUniform2f(glGetUniformLocation(renderManager->GetProgram(), "windowSize"), (float)width, (float)height);
}

float RandomFloat(float minVal, float maxVal)
{
    return minVal + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxVal - minVal);
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
        Camera camera(glm::vec3(0.0f, 3.0f, 8.0f), -90.0f, 0.0f);
        InputManager inputManager(camera);
        glfwSetWindowUserPointer(window, &inputManager);
        glfwSetCursorPosCallback(window, InputManager::MouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Modelos
        Model trollModel = LoadOBJModel("Assets/Models/troll.obj");
        Model sphereModel = LoadOBJModel("Assets/Models/sphere.obj");
        Model rockModel = LoadOBJModel("Assets/Models/rock.obj");
        Model semaforoModel = LoadOBJModel("Assets/Models/Semaforo.obj");
        Model tercerEdificioModel = LoadOBJModel("Assets/Models/TercerEdificio.obj");
        Model vallaModel = LoadOBJModel("Assets/Models/Valla.obj");
        Model señalModel = LoadOBJModel("Assets/Models/Señal.obj");
        //Model sueloModel = LoadOBJModel("Assets/Models/Suelo.obj");

        // Texturas
        Texture trollTexture("Assets/Textures/troll.png");
        Texture sunTexture("Assets/Textures/sun.png");
        Texture moonTexture("Assets/Textures/moon.png");
        Texture rockTexture("Assets/Textures/rock.png");
        Texture bolaTexture("Assets/Textures/bola.png");
        Texture semaforoTexture("Assets/Textures/Semaforo.png");
        Texture tercerEdificioTexture("Assets/Textures/TercerEdificio.png");
        Texture vallaTexture("Assets/Textures/Valla.png");
        Texture señalTexture("Assets/Textures/señal.png");
        //Texture sueloTexture("Assets/Textures/Suelo.png");

        // Astros
        Satelite sun(&sphereModel, &sunTexture, ORBIT_RADIUS, glm::vec3(0.5f));
        Satelite moon(&sphereModel, &moonTexture, ORBIT_RADIUS, glm::vec3(0.3f));

        // Sky
        Sky sky(&sun, &moon, CYCLE_DURATION);

        // RenderManager
        RenderManager _renderManager(&shaderManager);
        renderManager = &_renderManager;
        _renderManager.AddObject(&moon);
        _renderManager.AddObject(&sun);

        //Objeto fijos, el mismo edificio 3 veces para hacer como un contorno

        GameObject tercerEdificio(&tercerEdificioModel, &tercerEdificioTexture, glm::vec3(-10.0f, 0.0f, -8.0f), glm::vec3(180.0f), glm::vec3(0.08f));
        _renderManager.AddObject(&tercerEdificio);

        GameObject tercerEdificio1(&tercerEdificioModel, &tercerEdificioTexture, glm::vec3(10.0f, 0.0f, -8.0f), glm::vec3(0.0f),glm::vec3(0.08f));
        _renderManager.AddObject(&tercerEdificio1);

        GameObject tercerEdificio2(&tercerEdificioModel, &tercerEdificioTexture, glm::vec3(0.0f, 0.0f, -18.0f), glm::vec3(90.0f), glm::vec3(0.08f));
        _renderManager.AddObject(&tercerEdificio2);

        //Suelo comentado ya que no se adapta bien la textura al modelo y queda peor
        //GameObject suelo(&sueloModel, &sueloTexture, glm::vec3(-3.1f, -0.0f, -15.0f), glm::vec3(0.0f), glm::vec3(0.17f));
        //_renderManager.AddObject(&suelo);

        //Sistema de spawnpoints
        // 6 posiciones distribuidas en la acera de la escena urbana

        const glm::vec3 spawnPoints[SPAWNPOINTS] =
        {
            glm::vec3(-4.0f, 0.0f,  -8.0f),
            glm::vec3(-1.5f, 0.0f,  -5.0f),
            glm::vec3(1.0f, 0.0f,  -2.0f),
            glm::vec3(3.5f, 0.0f,  -5.0f),
            glm::vec3(3.0f, 0.0f,  -10.0f),
            glm::vec3(0.5f, 0.0f,  -10.0f)
        };

        // Vector que mantiene vivos todos los GameObjects generados en los spawnpoints

        std::vector<GameObject> spawnedObjects; 
        spawnedObjects.reserve(SPAWNPOINTS * 2);

        for (int i = 0; i < SPAWNPOINTS; i++)
        {
            // Elegir aleatoriamente qué conjunto aparece en este spawnpoint
            int spawnType = rand() % SPAWN_TYPE_COUNT;

            // Rotación Y aleatoria para que no todos miren en la misma dirección
            float randomRotY = RandomFloat(0.0f, 360.0f);

            const glm::vec3& pos = spawnPoints[i];

            //spawneamos los objetos
            if (spawnType == SPAWN_TYPE_TROLL)
            {
                float trollScale = TROLL_SCALE_BASE * (1.0f + RandomFloat(0.0f, SCALE_VARIANCE));

                spawnedObjects.emplace_back(&trollModel, &trollTexture, pos, glm::vec3(0.0f, randomRotY, 0.0f), glm::vec3(trollScale));
                _renderManager.AddObject(&spawnedObjects.back());
            }
            else if (spawnType == SPAWN_TYPE_SEMAFORO)
            {
                float scale = SEMAFORO_SCALE_BASE * (1.0f + RandomFloat(0.0f, SCALE_VARIANCE));

                spawnedObjects.emplace_back(&semaforoModel, &semaforoTexture, pos, glm::vec3(0.0f, randomRotY, 0.0f), glm::vec3(scale));
                _renderManager.AddObject(&spawnedObjects.back());
            }
            else if (spawnType == SPAWN_TYPE_VALLA)
            {
                float scale = VALLA_SCALE_BASE * (1.0f + RandomFloat(0.0f, SCALE_VARIANCE));

                spawnedObjects.emplace_back(&vallaModel, &vallaTexture, pos, glm::vec3(0.0f, randomRotY, 0.0f), glm::vec3(scale));
                _renderManager.AddObject(&spawnedObjects.back());
            }
            else if (spawnType == SPAWN_TYPE_BOLA)
            {
                float bolaScale = BOLA_SCALE_BASE * (1.0f + RandomFloat(0.0f, SCALE_VARIANCE));

                spawnedObjects.emplace_back(&sphereModel, &bolaTexture, glm::vec3(pos.x, pos.y, pos.z), glm::vec3(0.0f), glm::vec3(bolaScale * 0.5f));
                _renderManager.AddObject(&spawnedObjects.back());
            }
            else //la señal
            {
                float scale = SENAL_SCALE_BASE * (1.0f + RandomFloat(0.0f, SCALE_VARIANCE));

                spawnedObjects.emplace_back(&señalModel,&señalTexture, pos,glm::vec3(0.0f, randomRotY, 0.0f), glm::vec3(scale));
                _renderManager.AddObject(&spawnedObjects.back());
            }
        }

        glfwSetFramebufferSizeCallback(window, Resize_Window);

        // Uniforms fijos
        GLuint program = shaderManager.GetProgram();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, camera.GetFar(), camera.GetNear());
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