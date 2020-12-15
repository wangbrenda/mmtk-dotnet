#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <cassert>
#include <math.h>
#include <stdio.h>
#include "gcenv.base.h"
#include "gcinterface.h"
#include "mmtkheap.h"
#include "mmtkhandlemanager.h"

#ifdef _MSC_VER
#define DLLEXPORT __declspec(dllexport)
#else
#ifndef DLLEXPORT
#define DLLEXPORT __attribute__ ((visibility ("default")))
#endif // DLLEXPORT
#endif // _MSC_VER

extern "C" DLLEXPORT HRESULT
GC_Initialize(
    /* In */  IGCToCLR* clrToGC,
    /* Out */ IGCHeap** gcHeap,
    /* Out */ IGCHandleManager** gcHandleManager,
    /* Out */ GcDacVars* gcDacVars
)
{
    printf("Calling MMTkGC_Initialise\n");
    MMTkHandleManager* handleManager = new MMTkHandleManager();
    MMTkHeap* heap = new MMTkHeap(clrToGC, handleManager);
    *gcHeap = heap;
    *gcHandleManager = handleManager;
    return S_OK;
}

extern "C" DLLEXPORT void
GC_VersionInfo(
    /* Out */ VersionInfo* result
)
{
    printf("Calling MMTkGC_VersionInfo\n");
    result->MajorVersion = GC_INTERFACE_MAJOR_VERSION;
    result->MinorVersion = GC_INTERFACE_MINOR_VERSION;
    result->BuildVersion = 0;
    result->Name = "MMTk DotNet";
}