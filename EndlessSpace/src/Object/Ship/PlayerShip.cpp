#include "PlayerShip.hpp"
#include "Core/Camera.hpp"
#include "Core/MeshManager.hpp"
#include "Core/UpdateManager.hpp"
#include "Graphics/Renderer.hpp"
#include "Core/TextureManager.hpp"
#include "InterfaceObject/InterfaceManager.hpp"
#include "../../Interface/PieMenu.hpp"
#include "Object/Debug/RayDraw.hpp"
#include "Object/Render/MeshRenderer.hpp"
#include "../SpaceManager.hpp"

void OrbitX(void* Pointer) {
    PlayerShip* ps = static_cast<PlayerShip*>(Pointer);
    if (ps->target != nullptr && ps->target->isOrbitable && ps->target->ObjectType != "PlayerShip") {
        
        Debug::Log("Start Orbit!");
        
        ps->shipMovementData.Target = ps->target;
        ps->shipMovementData.IsOrbiting = true;
    }
}

void AttackTarget(void* Pointer) {
    Debug::Log("Attacking Target");
}

void Blue(void* Pointer) {
    Debug::Log("Hit Blue");
}

PlayerShip::PlayerShip() {
    SpaceManager::AddSpaceObject(this);
    ObjectType = "PlayerShip";

    menu = new PieMenu();
    menu->Active = false;
    menu->Tag = "PieMenu";
    InterfaceManager::AddObject(menu);
    Size = 5.0f;
}

void PlayerShip::Load() {
    
    texture = TextureManager::GetTexture("Data/Textures/Test.png");
    LoadObject("Data/Shaders/Tutorial.shader", "Data/Meshes/FightingShip.obj");
    LoadCollider(mesh->mesh);

    Ship::Load();

    object->transform.localPosition = Vector3(0, 0, 0);
    Camera::FirstPersonMode = false;
    Camera::Center = object->transform.localPosition;
    Camera::Position = Vector3(0, 0, 10);
    Camera::Zoom = 10;

    menu->AddOption(new MenuOption("Orbit", { OrbitX, this }, Vector3(1.0f, 0.0f, 0.0f)));
    menu->AddOption(new MenuOption("Attack", { AttackTarget, this }, Vector3(0.0f, 1.0f, 0.0f)));
    menu->AddOption(new MenuOption("Look", { Blue, this }, Vector3(0.0f, 0.0f, 1.0f)));
    menu->ChangePosition(Vector2(0, 0));
}

bool MenuNeedsUpdate = false;

void PlayerShip::Update() {
    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
        Ray ray = Ray(Camera::Position, Screen2World(Input::MousePositionYInv), true);
        CollisionManager::ProcessRay(ray, this);
    }

    Camera::OriginalPos = object->transform.Position() + Vector3(0, 0, Camera::Zoom);
    Camera::Center = object->transform.Position();

    ProcessOrbit();

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) && !InterfaceManager::Interacted) {
        menu->Active = false;
    }

    Ship::Update();
}

void PlayerShip::OnCollision(RayTestResult result) {
    Debug::Log("Something Was Hit " + result.result[0]->Hitted->object->Tag);

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
        menu->ChangePosition(Input::MousePosition);
        menu->Active = true;
        target = result.result[0]->Hitted->object->GetComponent<SpaceObject>();
    }
}