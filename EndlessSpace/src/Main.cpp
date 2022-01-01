#include "Main.hpp"

#include "Core/EngineWrapper.hpp"
#include "Core/UpdateManager.hpp"

#include "InterfaceObject/InterfaceManager.hpp"
#include "Interface/DebugData.hpp"

#include "Object/Object.hpp"
#include "Object/Skybox.hpp"
#include "Object/CameraController.hpp"

int main(void) {

    EngineWrapper::Init();

    //Load Game

    UpdateManager::CreateGroup("Others");

    DebugData *dbData = new DebugData();
    dbData->Position = Vector2(0, 0);
    dbData->ForegroundColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    InterfaceManager::AddObject(dbData);

    Object *skybox = new Object();
    skybox->Tag = "Skybox";
    Skybox *skyComp = new Skybox();
    skybox->AddComponent(skyComp);
    skyComp->LoadSkybox("Data/HDR/park.hdr", "Data/Meshes/Cube.obj", "Data/Shaders/Skybox.shader");
    
    UpdateManager::AddToGroup("Others", skybox);
    ObjectsHandler::AddObject(skybox);

    Object* camera = new Object();
    camera->Tag = "PlayerShip";
    CameraController* controller = new CameraController();
    camera->AddComponent(controller);

    UpdateManager::AddToGroup("Others", camera);
    ObjectsHandler::AddObject(camera);

    //Engine
    EngineWrapper::Load();
    EngineWrapper::PreRender();

    while (EngineWrapper::IsOpen()) {
        EngineWrapper::Prepare();
        EngineWrapper::Update();
        EngineWrapper::Render();
        EngineWrapper::Draw();
    }

    EngineWrapper::Clean();

    _exit(0);
}
