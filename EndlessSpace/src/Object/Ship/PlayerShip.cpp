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

SpaceObject* obj;
Vector3 PossibleTarget;
PieMenu* menu;

void OrbitX(void* Pointer) {
    PlayerShip* ps = static_cast<PlayerShip*>(Pointer);
    Debug::Alert("Start Orbit!");
    if (obj != nullptr && obj->isOrbitable) {
        ps->MovementTarget = PossibleTarget;
        ps->isOrbiting = true;
        ps->inOrbit = false;
        ps->orbitDistance = 10.0f;
        ///Acceleration = Velocity^2/Radius there for pre set speed multiplyed by it self to avoid unnecessary calculations
        ps->speed = 40.0f;
        ps->acceleration = ps->speed / ps->orbitDistance;
    }
}

void Green(void* Pointer) {
    Debug::Log("Hit Green");
}

void Blue(void* Pointer) {
    Debug::Log("Hit Blue");
}

PlayerShip::PlayerShip() {
    menu = new PieMenu();
    menu->Active = false;
    menu->Tag = "PieMenu";
    InterfaceManager::AddObject(menu);
}

void PlayerShip::Load() {
    
    texture = TextureManager::GetTexture("Data/Textures/Test.png");
    LoadObject("Data/Shaders/Tutorial.shader", "Data/Meshes/Main Ship.obj");
    LoadCollider(mesh->mesh);

    Ship::Load();

    movementMesh = MeshManager::GetMesh("/Data/Meshes/Sphere.obj");

    object->transform.localPosition = Vector3(0, 0, 0);
    Camera::FirstPersonMode = false;
    Camera::Center = object->transform.localPosition;
    Camera::Position = Vector3(0, 0, 10);
    Camera::Zoom = 10;

    menu->AddOption(new MenuOption("Orbit", { OrbitX, this }, Vector3(1.0f, 0.0f, 0.0f)));
    menu->AddOption(new MenuOption("Attack", { Green, this }, Vector3(0.0f, 1.0f, 0.0f)));
    menu->AddOption(new MenuOption("Look", { Blue, this }, Vector3(0.0f, 0.0f, 1.0f)));
    menu->ChangePosition(Vector2(0, 0));
}

bool MenuNeedsUpdate = false;

void PlayerShip::Update() {
    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
        Ray ray = Ray(Camera::Position, Screen2World(Input::MousePositionYInv), true);
        CollisionManager::ProcessRay(ray, this);
    }

    if (isOrbiting) {

        Vector3 StartPosition = object->transform.localPosition;
        Vector3 OrbitPoint = MovementTarget + orbitDistance * ((StartPosition- MovementTarget)/(Distance(StartPosition, MovementTarget)));

        float distance = Distance(OrbitPoint, StartPosition);
        float DeltaSpeed = TimeHelper::GetDeltaTime() * acceleration;

        object->transform.localPosition = Vector3::MoveTowards(StartPosition, (distance > 1 ? OrbitPoint : Vector3::RotateAround(StartPosition, MovementTarget, Vector3(0, 1, 0), DeltaSpeed)), DeltaSpeed);
        
        Camera::OriginalPos = StartPosition + Vector3(0, 0, Camera::Zoom);
        Camera::Center = StartPosition;

        Quaternion lookRotation = Quaternion::LookRotation(StartPosition, (distance > 1 ? MovementTarget : object->transform.localPosition));
        
        //If Source And Target Are Equal lookRotation Becomes Broken (nan, nan, nan, nan) this way we can test if it is broken because comparing a nan value with it self always returns false
        if (lookRotation.Quat.X == lookRotation.Quat.X)
            object->transform.localRotation = Quaternion::Slerp(object->transform.localRotation, lookRotation, TimeHelper::GetDeltaTime());
    }

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) && !InterfaceManager::Interacted) {
        menu->Active = false;
    }
}

void PlayerShip::OnCollision(RayTestResult result) {
    Debug::Log("Something Was Hit " + result.result[0]->Hitted->object->Tag);

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
        menu->ChangePosition(Input::MousePosition);
        menu->Active = true;
        obj = result.result[0]->Hitted->object->GetComponent<SpaceObject>();
        PossibleTarget = result.result[0]->Target;
    }
}