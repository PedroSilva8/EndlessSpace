#include "DebugData.hpp"
#include "Graphics/Renderer.hpp"
#include "Core/UpdateManager.hpp"

void DebugData::Render() {
    for (size_t Index = 0; Index < UpdateManager::Groups.size(); Index++)
        Renderer::DrawText("Arial", "Update Group " + to_string(Index) + " Time - " + to_string(UpdateManager::Groups[Index]->UpdateTime.count()), Vector2(RenderPosition.X, RenderPosition.Y + 20 * Index + 5), 0.5, ForegroundColor.Xyz());
}