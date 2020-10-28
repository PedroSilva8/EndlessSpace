#include "SpaceManager.hpp"
#include "SpaceObject.hpp"

vector<SpaceObject*> SpaceManager::spaceObjects;

void SpaceManager::AddSpaceObject(SpaceObject* spaceObject) {
    spaceObjects.push_back(spaceObject);
}

void SpaceManager::RemoveSpaceObject(SpaceObject* spaceObject) {
    spaceObjects.erase(remove(spaceObjects.begin(), spaceObjects.end(), spaceObject), spaceObjects.end());
}

bool SpaceManager::GetSpaceObjectsOfType(string Type, vector<SpaceObject*>& Objects) {
    bool ObjectFound = false;

    for (SpaceObject* object : spaceObjects) {
        if (object->ObjectType == Type) {
            ObjectFound = true;
            Objects.push_back(object);
        }
    }

    return ObjectFound;
}