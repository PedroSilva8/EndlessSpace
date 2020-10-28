#ifndef _SHIPADDON_
#define _SHIPADDON_

#include "Math/Vector3.hpp"
#include "Math/Quaternion.hpp"
#include <string>

using namespace std;

class VulkanDescriptor;
class Mesh;
class Shader;
class Texture;
class VulkanPipeLine;
class VulkanCommandBuffer;
class ShipAddonData;

class ShipAddon {
public:
    /* Shader Uniform Data */
    VulkanDescriptor* uniformData;
    /* Mesh Data */
    Mesh* mesh;
    /* Mesh Shader */
    Shader* shader;
    /* Mesh Texture */
    Texture* texture;
    /* Vulkan PipeLine (Generated Bu Load Function) */
    VulkanPipeLine* pipeLine;
    /* Vulkan Command Buffer (Generated Bu Load Function) */
    VulkanCommandBuffer* commandBuffer;
    /* Addon Position Local (Relative To Ship) */
    Vector3 RelativePosition;
    /* Addon Rotation */
    Quaternion LocalRotation;
    /* The Name Of The Addon */
    string Name;
    /* The Type Of The Addon */
    string Type;
    
    /**
    * Load The Addon Mesh, Shader And Texture
    * @param MeshLocation The Mesh File Location
    * @param ShaderLocation The Shader File Location
    * @param TextureLocation The Texture File Location
    */
    virtual void Load(ShipAddonData* data);
    /**
    * Void Update UniformData
    * @param Location GlobalLocation (Relative To (0, 0, 0))
    */
    virtual void Update(Vector3 Position, Quaternion Rotation);
    /**
    * Pre Render Everything Into CommandBuffer
    */
    virtual void PreRender();
    /**
    * Add Command Buffer Into Render Queue
    */
    virtual void Render();
    /* Clean Object */
    virtual void Clean();
};

#endif