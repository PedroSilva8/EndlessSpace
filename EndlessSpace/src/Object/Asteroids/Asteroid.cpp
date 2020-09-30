#include "Asteroid.hpp"
#include "Object/Render/MeshRenderer.hpp"
#include "Core/TextureManager.hpp"

void Asteroid::Load() {

    texture = TextureManager::GetTexture("Data/Textures/texture.jpg");
    LoadObject("Data/Shaders/Tutorial.shader", "Data/Meshes/Asteroids.obj");

    object->transform.localPosition = Vector3(rand() % (100 - (-100) + 1) + -100, rand() % (100 - (-100) + 1) + -100, rand() % (100 - (-100) + 1) + -100);

    LoadCollider(mesh->mesh);
}
