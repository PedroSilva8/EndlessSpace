#include "ShipAddonDataManager.hpp"
#include "Core/Debug.hpp"

vector<ShipAddonData*> ShipAddonDataManager::addonsData;

ShipAddonData* ShipAddonDataManager::GetAddonData(string Name) {
    for (ShipAddonData* addonData : addonsData)
        if (addonData->Name == Name)
            return addonData;

    return nullptr;
}

void ShipAddonDataManager::AddAddonData(ShipAddonData* data) {
    
    for (ShipAddonData* addonData : addonsData) {
        if (addonData->Name == data->Name) {
            Debug::Log("Attempted To Add A Shipp Addon Data With A Name That Already Exists: \n\tName - " + data->Name + "\n\tType - " + data->Type);
        }
    }

    addonsData.push_back(data);
}
