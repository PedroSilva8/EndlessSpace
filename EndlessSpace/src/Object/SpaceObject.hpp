#ifndef _SPACEOBJECT_
#define _SPACEOBJECT_

#include "Object/Component.hpp"
#include "Mesh/Mesh.hpp"
#include "Collisions/Collider.hpp"
#include "Graphics/Vulkan/VulkanPipeLine.hpp"
#include "Graphics/Vulkan/Vulkan.hpp"
#include "Graphics/Vulkan/VulkanDescriptor.hpp"

#include <string>

using namespace std;

class Texture;

class SpaceObject : public Component {
public:
    Mesh* mesh;
    Shader* shader;
    Texture* texture;
    Collider* collider;
    VulkanPipeLine* pipeLine;
    VulkanCommandBuffer* commandBuffer;
    VulkanDescriptor uniformData;
    
    Matrix4 model;

    bool isOrbitable;

    SpaceObject();

    void LoadObject(const char* Shader, const char* Model);
    void LoadCollider(Mesh* Collider);
    virtual void Load() override;
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Clean() override;
};

#endif