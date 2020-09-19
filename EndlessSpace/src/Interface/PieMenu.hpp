#ifndef _CIRCULARMENU_
#define _CIRCULARMENU_

#include "InterfaceObject/InterfaceObject.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Mesh/Mesh.hpp"

#include <vector>
#include <string>

using namespace std;

struct PieMenuData {
    Matrix4 projection;
    Vector2 Center;
    float Rotation;
    float Angle;
    float innerRadius;
};

struct MenuOption {
    string Text;
    Texture* Icon;
    bool UseIcon;
    Vector4 RenderPosition;
    PieMenuData data;
    VulkanDescriptor* uniformData;
};

class PieMenu : public InterfaceObject {
public:
    vector<MenuOption*> options;
    float innerRadius;
    float outterRadius;
    float distanceBetweenOptions;
    float optionSize;
    float circleDetail;
    vector<float> vertexBuffer;
    
    PieMenu();
    void AddOption(MenuOption* option);
    void ChangePosition(Vector2 NewPosition);
    void Load() override;
    void PreRender() override;
    void Update() override;
    void Render() override;
    void Clean() override;
};

#endif