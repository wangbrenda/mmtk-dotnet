#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cassert>
#include <math.h>
#include "gcenv.base.h"
#include "gcinterface.h"

class MMTkHandleStore : public IGCHandleStore {
public:

    virtual void Uproot() override;

    virtual bool ContainsHandle(OBJECTHANDLE handle) override;

    virtual OBJECTHANDLE CreateHandleOfType(Object* object, HandleType type) override;

    virtual OBJECTHANDLE CreateHandleOfType(Object* object, HandleType type, int heapToAffinitizeTo) override;

    virtual OBJECTHANDLE CreateHandleWithExtraInfo(Object* object, HandleType type, void* pExtraInfo) override;

    virtual OBJECTHANDLE CreateDependentHandle(Object* primary, Object* secondary) override;

    Object* GetDependentHandleSecondary(OBJECTHANDLE handle);

    void SetDependentHandleSecondary(OBJECTHANDLE handle, Object* secondary);

    OBJECTHANDLE CreateDuplicateHandle(OBJECTHANDLE handle);

    void DestroyHandleOfType(OBJECTHANDLE handle, HandleType type);

    Object* InterlockedCompareExchangeObjectInHandle(OBJECTHANDLE handle, Object* object, Object* comparandObject);

    void StoreObjectInHandle(OBJECTHANDLE handle, Object* object);
};
