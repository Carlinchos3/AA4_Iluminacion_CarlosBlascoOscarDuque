#pragma once

#include <vector>
#include <string>
#include <glm.hpp>
#include "Model.h"
#include "Texture.h"
#include "GameObject.h"
#include "RenderManager.h"

//Numero de spawns
#define SPAWNPOINTS         6

//Objetos randoms que spawnear
#define SPAWN_TYPE_COUNT    5
#define SPAWN_TYPE_TROLL    0
#define SPAWN_TYPE_SEMAFORO 1
#define SPAWN_TYPE_VALLA    2
#define SPAWN_TYPE_SENAL    3
#define SPAWN_TYPE_BOLA     4

//Sus escalas base
#define TROLL_SCALE_BASE    0.8f
#define SEMAFORO_SCALE_BASE 0.05f
#define VALLA_SCALE_BASE    0.15f
#define SENAL_SCALE_BASE    0.02f
#define BOLA_SCALE_BASE     0.55f
#define SCALE_VARIANCE      0.6f

class ModelManager
{
public:
    ModelManager() = default;

    void LoadAssets();
    void AddFixedObjects(RenderManager& _renderManager);
    void SpawnObjects(RenderManager& _renderManager);

    Model& GetSphereModel() { return sphereModel; }
    Texture& GetSunTexture() { return sunTexture; }
    Texture& GetMoonTexture() { return moonTexture; }

private:
    float RandomFloat(float minVal, float maxVal);
    Model LoadOBJModel(const std::string& filePath);

    // Modelos
    Model trollModel;
    Model sphereModel;
    Model rockModel;
    Model semaforoModel;
    Model tercerEdificioModel;
    Model vallaModel;
    Model señalModel;

    // Texturas
    Texture trollTexture;
    Texture sunTexture;
    Texture moonTexture;
    Texture rockTexture;
    Texture bolaTexture;
    Texture semaforoTexture;
    Texture tercerEdificioTexture;
    Texture vallaTexture;
    Texture señalTexture;

    // Objetos fijos
    GameObject tercerEdificio;
    GameObject tercerEdificio1;
    GameObject tercerEdificio2;

    // Objetos spawneados
    std::vector<GameObject> spawnedObjects;
};