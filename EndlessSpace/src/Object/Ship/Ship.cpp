#include "Ship.hpp"
#include "Core/Camera.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#include "ShipAddon.hpp"
#include "ShipAddonDataManager.hpp"

Ship::Ship() {
    shipMovementData.OrbitDistance = 10.0f;
    ///Acceleration = Velocity^2/Radius there for pre set speed multiplyed by it self to avoid unnecessary calculations
    shipMovementData.Speed = 160.0f;
    shipMovementData.Acceleration = shipMovementData.Speed / shipMovementData.OrbitDistance;
    shipMovementData.Target = nullptr;
    shipBattleData.Fighting = false;
}

void Ship::Load() {
    SpaceObject::Load();
}

void Ship::Update() {
    SpaceObject::Update();

    for (ShipAddon* addon : Addons)
        addon->Update(transform->Position(), transform->Rotation());
}

void Ship::PreRender() {
    SpaceObject::PreRender();

    for (ShipAddon* addon: Addons)
        addon->PreRender();
}

void Ship::Render() {
    SpaceObject::Render();

    for (ShipAddon* addon : Addons)
        addon->Render();
}

void Ship::Clean() {
    SpaceObject::Clean();

    for (ShipAddon* addon : Addons)
        addon->Clean();
}

void Ship::AddAddon(string Name, Vector3 Position, Quaternion Rotation) {
    ShipAddon* addon = new ShipAddon();
    addon->Load(ShipAddonDataManager::GetAddonData(Name));
    addon->RelativePosition = Position;
    addon->LocalRotation = Rotation;
    Addons.push_back(addon);
}

void Ship::ProcessOrbit() {
    if (shipMovementData.IsOrbiting && shipMovementData.Target != nullptr) {
        /*
        * MEDIUMPRIORITY TODO: Fix Orbit
        * After Sometime It Starts Orbiting Backwards
        * We Need To Make The Orbit More Realistic 
        */
        Vector3 TargetPosition = shipMovementData.Target->transform->Position();
        Vector3 StartPosition = object->transform.localPosition;
        Vector3 OrbitPoint = TargetPosition + (shipMovementData.OrbitDistance + shipMovementData.Target->Size + Size) * ((StartPosition- TargetPosition)/(Distance(StartPosition, TargetPosition)));

        float distance = Distance(OrbitPoint, StartPosition);
        float DeltaSpeed = TimeHelper::GetDeltaTime() * shipMovementData.Acceleration;

        object->transform.localPosition = Vector3::MoveTowards(StartPosition, (distance > 1 ? OrbitPoint : Vector3::RotateAround(StartPosition, TargetPosition, transform->Up(), DeltaSpeed)), DeltaSpeed);

        Quaternion lookRotation = Quaternion::LookRotation(StartPosition, (distance > 1 ? TargetPosition : object->transform.localPosition));
        
        //If Source And Target Are Equal lookRotation Becomes Broken (nan, nan, nan, nan) this way we can test if it is broken because comparing a nan value with it self always returns false
        if (lookRotation.Quat.X == lookRotation.Quat.X)
            object->transform.localRotation = Quaternion::Slerp(object->transform.localRotation, lookRotation, TimeHelper::GetDeltaTime());
    }
}