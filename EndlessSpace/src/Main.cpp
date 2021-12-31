#include "Main.hpp"

#include "Core/EngineWrapper.hpp"
#include "InterfaceObject/InterfaceManager.hpp"
#include "Interface/DebugData.hpp"

int main(void) {

    EngineWrapper::Init();

    //Load Game
    DebugData *dbData = new DebugData();
    dbData->Position = Vector2(0, 0);
    dbData->ForegroundColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    InterfaceManager::AddObject(dbData);

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
