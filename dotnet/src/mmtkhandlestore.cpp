#include <stdio.h> // printing
#include <mutex> // concurrency
#include "mmtkhandlestore.h"

#define UNIMPLEMENTED() printf("%s unimplemented\n", __func__);

std::mutex m; // for correctness
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
OBJECTHANDLE MMTkHandleStore::CreateDuplicateHandle(OBJECTHANDLE handle) { UNIMPLEMENTED(); }
void MMTkHandleStore::SetDependentHandleSecondary(OBJECTHANDLE handle, Object* secondary) { UNIMPLEMENTED(); }
Object* MMTkHandleStore::GetDependentHandleSecondary(OBJECTHANDLE handle) { UNIMPLEMENTED(); }

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
void MMTkHandleStore::StoreObjectInHandle(OBJECTHANDLE handle, Object* object)
{
    m.lock();
    Object** handleObj = (Object**)handle;
    *handleObj = object;
    m.unlock();
}
Object* MMTkHandleStore::InterlockedCompareExchangeObjectInHandle(OBJECTHANDLE handle, Object* object, Object* comparandObject)
{
    m.lock();
    Object** handleObj = (Object**)handle;
    *handleObj = object;
    if (*handleObj == comparandObject)
    {
        *handleObj = object;
    }
    m.unlock();
    return *handleObj;
}