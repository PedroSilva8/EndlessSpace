#include "SpaceObject.hpp"
#include "Collisions/CollisionManager.hpp"
#include "Object/Render/MeshRenderer.hpp"

SpaceObject::SpaceObject() {
    isOrbitable = true;
    texture = nullptr;
    mesh = nullptr;
}

void SpaceObject::LoadCollider(Mesh* colliderMesh) {

    collider = new Collider();
    object->AddComponent(collider);
    collider->LoadCollider(colliderMesh);
    CollisionManager::AddCollider(collider);
}

void SpaceObject::LoadObject(const char* Shader, const char* Model) {

    ///Check If mesh is Set If Not Get Component
    if (mesh == nullptr)
        mesh = object->GetComponent<MeshRenderer>();
    ///Second Check To See If There Is No Component of Type Mesh In That Case Create And Add
    if (mesh == nullptr) {
        mesh = new MeshRenderer();
        object->AddComponent(mesh);
    }

    mesh->PrepareObject(Shader, Model, texture);
    mesh->uniformData->GetUniform("MVP")->Generate(sizeof(Matrix4));
    if (texture != nullptr)
        mesh->uniformData->GetUniform("texSampler")->SetTexture(texture);
    mesh->LoadObject("SpaceObjectUniform");

    if (collider == nullptr)
        LoadCollider(mesh->mesh);
}