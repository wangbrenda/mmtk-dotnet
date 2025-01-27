#include "mmtkhandlemanager.h"
#include <stdio.h>

#define UNIMPLEMENTED() printf("%s unimplemented\n", __func__);

bool MMTkHandleManager::Initialize() {
    return true;
}

void MMTkHandleManager::Shutdown() { UNIMPLEMENTED(); }

IGCHandleStore* MMTkHandleManager::GetGlobalHandleStore() { 
    return &handleStore;
}

IGCHandleStore* MMTkHandleManager::CreateHandleStore() { UNIMPLEMENTED();  return nullptr; }
void MMTkHandleManager::DestroyHandleStore(IGCHandleStore* store) { UNIMPLEMENTED();  }
OBJECTHANDLE MMTkHandleManager::CreateGlobalHandleOfType(Object* object, HandleType type)
{
    return handleStore.CreateHandleOfType(object, type);
}
OBJECTHANDLE MMTkHandleManager::CreateDuplicateHandle(OBJECTHANDLE handle) { UNIMPLEMENTED();  return nullptr; }
void MMTkHandleManager::DestroyHandleOfType(OBJECTHANDLE handle, HandleType type) {
    handleStore.DestroyHandleOfType(handle, type);
}
void MMTkHandleManager::DestroyHandleOfUnknownType(OBJECTHANDLE handle) { UNIMPLEMENTED(); }
void MMTkHandleManager::SetExtraInfoForHandle(OBJECTHANDLE handle, HandleType type, void* pExtraInfo) { UNIMPLEMENTED(); }
void* MMTkHandleManager::GetExtraInfoFromHandle(OBJECTHANDLE handle) { UNIMPLEMENTED();  return nullptr; }
void MMTkHandleManager::StoreObjectInHandle(OBJECTHANDLE handle, Object* object)
{
    Object** handleObj = (Object**)handle;
    *handleObj = object;
}
bool MMTkHandleManager::StoreObjectInHandleIfNull(OBJECTHANDLE handle, Object* object) { UNIMPLEMENTED();  return true; }
void MMTkHandleManager::SetDependentHandleSecondary(OBJECTHANDLE handle, Object* object) { UNIMPLEMENTED(); }
Object* MMTkHandleManager::GetDependentHandleSecondary(OBJECTHANDLE handle) { UNIMPLEMENTED();  return nullptr; }
Object* MMTkHandleManager::InterlockedCompareExchangeObjectInHandle(OBJECTHANDLE handle, Object* object, Object* comparandObject)
{
    Object** handleObject = (Object**)handle;
    if (*handleObject == comparandObject)
    {
        *handleObject = object;
    }
    return *handleObject;
}
HandleType MMTkHandleManager::HandleFetchType(OBJECTHANDLE handle) { UNIMPLEMENTED();  return HNDTYPE_WEAK_SHORT; }
void MMTkHandleManager::TraceRefCountedHandles(HANDLESCANPROC callback, uintptr_t param1, uintptr_t param2) { UNIMPLEMENTED(); }
