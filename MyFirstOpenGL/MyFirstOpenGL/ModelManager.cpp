#include "ModelManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

float ModelManager::RandomFloat(float minVal, float maxVal)
{
    return minVal + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxVal - minVal);
}

Model ModelManager::LoadOBJModel(const std::string& filePath) {

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

void ModelManager::LoadAssets()
{
    trollModel = LoadOBJModel("Assets/Models/troll.obj");
    sphereModel = LoadOBJModel("Assets/Models/sphere.obj");
    rockModel = LoadOBJModel("Assets/Models/rock.obj");
    semaforoModel = LoadOBJModel("Assets/Models/Semaforo.obj");
    tercerEdificioModel = LoadOBJModel("Assets/Models/TercerEdificio.obj");
    vallaModel = LoadOBJModel("Assets/Models/Valla.obj");
    señalModel = LoadOBJModel("Assets/Models/Señal.obj");

    trollTexture = Texture("Assets/Textures/troll.png");
    sunTexture = Texture("Assets/Textures/sun.png");
    moonTexture = Texture("Assets/Textures/moon.png");
    rockTexture = Texture("Assets/Textures/rock.png");
    bolaTexture = Texture("Assets/Textures/bola.png");
    semaforoTexture = Texture("Assets/Textures/Semaforo.png");
    tercerEdificioTexture = Texture("Assets/Textures/TercerEdificio.png");
    vallaTexture = Texture("Assets/Textures/Valla.png");
    señalTexture = Texture("Assets/Textures/señal.png");
}

void ModelManager::AddFixedObjects(RenderManager& _renderManager)
{
    //Objeto fijos, el mismo edificio 3 veces para hacer como un contorno
    tercerEdificio = GameObject(&tercerEdificioModel, &tercerEdificioTexture, glm::vec3(-10.0f, 0.0f, -8.0f), glm::vec3(180.0f), glm::vec3(0.08f));
    tercerEdificio1 = GameObject(&tercerEdificioModel, &tercerEdificioTexture, glm::vec3(10.0f, 0.0f, -8.0f), glm::vec3(0.0f), glm::vec3(0.08f));
    tercerEdificio2 = GameObject(&tercerEdificioModel, &tercerEdificioTexture, glm::vec3(0.0f, 0.0f, -18.0f), glm::vec3(90.0f), glm::vec3(0.08f));

    _renderManager.AddObject(&tercerEdificio);
    _renderManager.AddObject(&tercerEdificio1);
    _renderManager.AddObject(&tercerEdificio2);
}

void ModelManager::SpawnObjects(RenderManager& _renderManager)
{
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

            spawnedObjects.emplace_back(&señalModel, &señalTexture, pos, glm::vec3(0.0f, randomRotY, 0.0f), glm::vec3(scale));
            _renderManager.AddObject(&spawnedObjects.back());
        }
    }
}