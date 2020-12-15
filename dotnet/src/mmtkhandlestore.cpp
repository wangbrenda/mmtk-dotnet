#include "mmtkhandlestore.h"
#include <stdio.h>

#define UNIMPLEMENTED() printf("%s unimplemented\n", __func__);

int handlesCount = 0;
OBJECTHANDLE handles[65535];

void MMTkHandleStore::Uproot() {}
bool MMTkHandleStore::ContainsHandle(OBJECTHANDLE handle) { UNIMPLEMENTED(); return false; }

OBJECTHANDLE MMTkHandleStore::CreateHandleOfType(Object* object, HandleType type)
{
    handles[handlesCount] = (OBJECTHANDLE__*) object;
    return (OBJECTHANDLE) &handles[handlesCount++];
}

OBJECTHANDLE MMTkHandleStore::CreateHandleOfType(Object* object, HandleType type, int heapToAffinitizeTo) { UNIMPLEMENTED(); return nullptr; }
OBJECTHANDLE MMTkHandleStore::CreateHandleWithExtraInfo(Object* object, HandleType type, void* pExtraInfo) { UNIMPLEMENTED(); return nullptr; }
OBJECTHANDLE MMTkHandleStore::CreateDependentHandle(Object* primary, Object* secondary) { UNIMPLEMENTED(); return nullptr; }
void MMTkHandleStore::DestroyHandleOfType(OBJECTHANDLE handle, HandleType type)
{
    for (int i = 0; i < handlesCount; ++i)
    {
        if (handles[i] == handle->unused)
        {
            handles[i] == nullptr;
            break;
        }
    }
}