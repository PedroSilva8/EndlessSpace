#include "CameraController.hpp"
#include "Core/Camera.hpp"

void CameraController::Load() {
    object->transform.localPosition = Vector3(0, 0, 0);
    Camera::FirstPersonMode = false;
    Camera::Center = object->transform.localPosition;
    Camera::Position = Vector3(0, 0, 10);
    Camera::Zoom = 10;
}

void CameraController::Update() {
    Camera::OriginalPos = object->transform.Position() + Vector3(0, 0, Camera::Zoom);
    Camera::Center = object->transform.Position();
}