#include "Asteroid.hpp"
#include "Object/Render/MeshRenderer.hpp"
#include "Core/TextureManager.hpp"
#include "../SpaceManager.hpp"

void Asteroid::Load() {

    SpaceManager::AddSpaceObject(this);

    ObjectType = "Asteroid";
    Size = 8.0f;
    
    texture = TextureManager::GetTexture("Data/Textures/texture.jpg");
    LoadObject("Data/Shaders/Tutorial.shader", "Data/Meshes/Asteroid.obj");

    object->transform.localPosition = Vector3(rand() % (100 - (-100) + 1) + -100, rand() % (100 - (-100) + 1) + -100, rand() % (100 - (-100) + 1) + -100);

    LoadCollider(mesh->mesh);
}
