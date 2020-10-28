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
class MeshRenderer;

class SpaceObject : public Component {
public:
    /* Space Object Texture */
    Texture* texture;
    /* Space Object Collider */
    Collider* collider;
    /* Space Object Mesh*/
    MeshRenderer* mesh;
    /* Space Object Type */
    string ObjectType;
    /* Space Object Size */
    float Size;
    
    /* Space Object Model Matrix Data (Position, Rotation, Scale) */
    Matrix4 model;

    /* Is Space Object Orbitable */
    bool isOrbitable;

    SpaceObject();

    /**
    * Load Space Object
    * @param Shader The Shader Location To Be Used
    * @param Model The Model Location To Be Used
    */
    void LoadObject(const char* Shader, const char* Model);
    /**
    * Load Collider From Mesh
    * @param Collider The Mesh To Be Used To Create The Collider
    */
    void LoadCollider(Mesh* Collider);
};

#endif