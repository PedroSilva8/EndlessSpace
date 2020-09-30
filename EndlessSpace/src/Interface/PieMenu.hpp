#ifndef _CIRCULARMENU_
#define _CIRCULARMENU_

#include "InterfaceObject/InterfaceObject.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Mesh/Mesh.hpp"

#include <vector>
#include <string>

using namespace std;

///Note: Using Alignas To Fix Padding When Transfering Data To Unifrom Buffer
struct PieMenuData {
    alignas(64) Matrix4 projection;
    alignas(16) Vector3 Color;
    alignas(8) Vector2 Center;
    alignas(4) float Rotation;
    alignas(4) float Angle;
    alignas(4) float innerRadius;
};

struct MenuOption {
    string Name;
    string Text;
    Texture* Icon;
    bool UseIcon;
    PieMenuData data;
    VulkanDescriptor* uniformData;
    PointerFunctionData OnClick;

    MenuOption(string OptionName, PointerFunctionData onClick, Vector3 Color = Vector3(1.0f, 1.0f, 1.0f));
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
    bool GetOption(string OptionName, MenuOption* option);
    void ChangePosition(Vector2 NewPosition);
    void Load() override;
    void PreRender() override;
    void Update() override;
    void Render() override;
    void Clean() override;
};

#endif