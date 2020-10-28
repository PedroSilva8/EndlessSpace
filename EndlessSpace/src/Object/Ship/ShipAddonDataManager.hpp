#ifndef _SHIPADDONDATAMANAGER_
#define _SHIPADDONDATAMANAGER_

#include "Math/Vector3.hpp"
#include "Math/Quaternion.hpp"
#include <vector>
#include <string>

using namespace std;

/* Ship Addon Data (Files Location, Name And Type) */
struct ShipAddonData {
    
    /* Addon Name */
    string Name;
    /* Addon Type */
    string Type;

    /* Mesh File Location */
    const char* MeshLocation;
    /* Shader File Location */
    const char* ShaderLocation;
    /* Texture File Location*/
    const char* TextureLocation;
};

class ShipAddonDataManager {
public:
    /* List Of All Ship Addons Data */
    static vector<ShipAddonData*> addonsData;

    static ShipAddonData* GetAddonData(string Name);
    static void AddAddonData(ShipAddonData* data);
};

#endif