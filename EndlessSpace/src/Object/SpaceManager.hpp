#ifndef _SPACEMANAGER_
#define _SPACEMANAGER_

#include <string>
#include <vector>

using namespace std;

class SpaceObject;

///Keeps Track Of Space Objects To Make AI Programing Easier
class SpaceManager {
public:
    static vector<SpaceObject*> spaceObjects;

    static void AddSpaceObject(SpaceObject* spaceObject);
    static void RemoveSpaceObject(SpaceObject* spaceObject);
    static bool GetSpaceObjectsOfType(string Type, vector<SpaceObject*>& Objects);
};

#endif