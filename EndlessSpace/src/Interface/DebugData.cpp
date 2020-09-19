#include "DebugData.hpp"
#include "Graphics/TextRenderer.hpp"
#include "Core/UpdateManager.hpp"

///Avoid Unnecessary Stuff
void DebugData::Load() { }
void DebugData::PreRender() { }
void DebugData::Render() { }

void DebugData::PosRender() {

    TotalTime += TimeHelper::GetDeltaTime();
    Frames++;

    if (TotalTime > 1.0f) {
        FrameRate = Frames;
        TotalTime = 0;
        Frames = 0;
    }

    ///1000000ns = 1ms
    for (size_t Index = 0; Index < UpdateManager::Groups.size(); Index++)
        TextRenderer::AddText("Update Group " + to_string(Index) + " Time - MiliSeconds " + to_string(UpdateManager::Groups[Index]->UpdateTime.count() / 1000000UL) + " NanoSeconds" + to_string(UpdateManager::Groups[Index]->UpdateTime.count()), Vector2(RenderPosition.X, RenderPosition.Y + 13 * 2 * Index), 2);

    TextRenderer::AddText("Update Time - MiliSeconds " + to_string(ObjectsHandler::UpdateTime.count() / 1000000UL) + " NanoSeconds" + to_string(ObjectsHandler::UpdateTime.count()), Vector2(RenderPosition.X, RenderPosition.Y + 13 * 2 * UpdateManager::Groups.size()), 2);
    TextRenderer::AddText("Render Time - MiliSeconds " + to_string(ObjectsHandler::RenderTime.count() / 1000000UL) + " NanoSeconds" + to_string(ObjectsHandler::RenderTime.count()), Vector2(RenderPosition.X, RenderPosition.Y + 13 * 2 * (UpdateManager::Groups.size() + 1)), 2);
    TextRenderer::AddText("FPS - " + to_string((int)FrameRate), Vector2(RenderPosition.X, RenderPosition.Y + 13 * 2 * (UpdateManager::Groups.size() + 2)), 2);
}