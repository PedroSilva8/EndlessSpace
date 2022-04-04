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

    int64_t totalTime = ObjectsHandler::UpdateTime.count() + ObjectsHandler::RenderTime.count();

    TextRenderer::AddText("Update Time - Perc " + to_string((ObjectsHandler::UpdateTime.count() * 100 / totalTime)) + "% MiliSeconds " + to_string(ObjectsHandler::UpdateTime.count() / 1000000UL) + " NanoSeconds" + to_string(ObjectsHandler::UpdateTime.count()), Vector2(RenderPosition.X, RenderPosition.Y), 2);
    TextRenderer::AddText("Render Time - Perc " + to_string((ObjectsHandler::RenderTime.count() * 100 / totalTime)) + "% MiliSeconds " + to_string(ObjectsHandler::RenderTime.count() / 1000000UL) + " NanoSeconds" + to_string(ObjectsHandler::RenderTime.count()), Vector2(RenderPosition.X, RenderPosition.Y + 13 * 2), 2);
    TextRenderer::AddText("FPS " + to_string((int)FrameRate), Vector2(RenderPosition.X, RenderPosition.Y + 13 * 3 + 5), 2);
}