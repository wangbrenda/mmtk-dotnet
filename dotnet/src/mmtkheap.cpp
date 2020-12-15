#include "mmtkheap.h"
#include "mmtk.h"
#include <stdio.h>

#define UNIMPLEMENTED() printf("MMTkHeap::%s unimplemented\n", __func__);
// #define UNIMPLEMENTED();

class ObjHeader
{
private:
    DWORD m_SyncBlockValue;
};

int allocCount = 0;
void* handle;

/*    Hosting APIs    */
bool MMTkHeap::IsValidSegmentSize(size_t size) { UNIMPLEMENTED(); return true; }
bool MMTkHeap::IsValidGen0MaxSize(size_t size) { UNIMPLEMENTED(); return true; }
size_t MMTkHeap::GetValidSegmentSize(bool large_seg) { UNIMPLEMENTED(); return size_t(); }
void MMTkHeap::SetReservedVMLimit(size_t vmlimit) { UNIMPLEMENTED(); }

/*   Concurrent GC routines    */
// We do not support concurrency in MMTk yet
void MMTkHeap::WaitUntilConcurrentGCComplete() { UNIMPLEMENTED(); }
bool MMTkHeap::IsConcurrentGCInProgress()
{
    return false;
}
void MMTkHeap::TemporaryEnableConcurrentGC() { UNIMPLEMENTED(); }
void MMTkHeap::TemporaryDisableConcurrentGC() { UNIMPLEMENTED(); }
bool MMTkHeap::IsConcurrentGCEnabled() { UNIMPLEMENTED(); return false; }
HRESULT MMTkHeap::WaitUntilConcurrentGCCompleteAsync(int millisecondsTimeout) { UNIMPLEMENTED(); return S_OK; }    // Use in native threads. TRUE if succeed. FALSE if failed or timeout

/*    Finalization routines    */
size_t MMTkHeap::GetNumberOfFinalizable() { UNIMPLEMENTED(); return size_t(); }
Object* MMTkHeap::GetNextFinalizable() { UNIMPLEMENTED(); return nullptr; }

/*    BCL routines    */
void MMTkHeap::GetMemoryInfo(uint64_t* highMemLoadThresholdBytes,
                            uint64_t* totalAvailableMemoryBytes,
                            uint64_t* lastRecordedMemLoadBytes,
                            uint64_t* lastRecordedHeapSizeBytes,
                            uint64_t* lastRecordedFragmentationBytes,
                            uint64_t* totalCommittedBytes,
                            uint64_t* promotedBytes,
                            uint64_t* pinnedObjectCount,
                            uint64_t* finalizationPendingCount,
                            uint64_t* index,
                            uint32_t* generation,
                            uint32_t* pauseTimePct,
                            bool* isCompaction,
                            bool* isConcurrent,
                            uint64_t* genInfoRaw,
                            uint64_t* pauseInfoRaw,
                            int kind) { UNIMPLEMENTED(); }
uint32_t MMTkHeap::GetMemoryLoad() { UNIMPLEMENTED(); return uint32_t(); }
int MMTkHeap::GetGcLatencyMode() { UNIMPLEMENTED(); return 0; }
int MMTkHeap::SetGcLatencyMode(int newLatencyMode) { UNIMPLEMENTED(); return 0; }
int MMTkHeap::GetLOHCompactionMode() { UNIMPLEMENTED(); return 0; }
void MMTkHeap::SetLOHCompactionMode(int newLOHCompactionMode) { UNIMPLEMENTED(); }
bool MMTkHeap::RegisterForFullGCNotification(uint32_t gen2Percentage, uint32_t lohPercentage) { UNIMPLEMENTED(); return true; }
bool MMTkHeap::CancelFullGCNotification() { UNIMPLEMENTED(); return true; }
int MMTkHeap::WaitForFullGCApproach(int millisecondsTimeout) { UNIMPLEMENTED(); return 0;}
int MMTkHeap::WaitForFullGCComplete(int millisecondsTimeout) { UNIMPLEMENTED(); return 0; }
unsigned MMTkHeap::WhichGeneration(Object* obj) { UNIMPLEMENTED(); return 0; }
int MMTkHeap::CollectionCount(int generation, int get_bgc_fgc_coutn) { UNIMPLEMENTED(); return 0; }
int MMTkHeap::StartNoGCRegion(uint64_t totalSize, bool lohSizeKnown, uint64_t lohSize, bool disallowFullBlockingGC) { UNIMPLEMENTED(); return 0; }
int MMTkHeap::EndNoGCRegion() { UNIMPLEMENTED(); return 0; }
size_t MMTkHeap::GetTotalBytesInUse() { UNIMPLEMENTED(); return size_t(); }
uint64_t MMTkHeap::GetTotalAllocatedBytes() { UNIMPLEMENTED(); return uint64_t(); }
HRESULT MMTkHeap::GarbageCollect(int generation, bool low_memory_p, int mode) { UNIMPLEMENTED(); return S_OK; }
unsigned MMTkHeap::GetMaxGeneration() { UNIMPLEMENTED(); return 0; }
void MMTkHeap::SetFinalizationRun(Object* obj) { UNIMPLEMENTED(); }
bool MMTkHeap::RegisterForFinalization(int gen, Object* obj) { UNIMPLEMENTED(); return true; }
int MMTkHeap::GetLastGCPercentTimeInGC() { UNIMPLEMENTED(); return 0; }
size_t MMTkHeap::GetLastGCGenerationSize(int gen) { UNIMPLEMENTED(); return size_t(); }

/*    Miscellaneous routines    */
HRESULT MMTkHeap::Initialize()
{
    MethodTable* freeObjectMethodTable = gcToCLR->GetFreeObjectMethodTable();

    WriteBarrierParameters args = {};
    args.operation = WriteBarrierOp::Initialize;
    args.is_runtime_suspended = true;
    args.requires_upper_bounds_check = false;
    args.card_table = new uint32_t[1];
    args.lowest_address = reinterpret_cast<uint8_t*>(~0);;
    args.highest_address = reinterpret_cast<uint8_t*>(1);
    args.ephemeral_low = reinterpret_cast<uint8_t*>(~0);
    args.ephemeral_high = reinterpret_cast<uint8_t*>(1);
    gcToCLR->StompWriteBarrier(&args);
    printf("Stomping Write Barrier\n");

    void* mmtk = gc_init(1024 * 1024);
    printf("Initializing MMTK Heap\n");
    handle = bind_mutator(mmtk, 0);
    printf("Binding MMTK Mutator\n");

    return NOERROR;
}
bool MMTkHeap::IsPromoted(Object* object) { UNIMPLEMENTED(); return false;}
bool MMTkHeap::IsHeapPointer(void* object, bool small_heap_only)
{
    return is_mapped_object(object);
}
unsigned MMTkHeap::GetCondemnedGeneration() { UNIMPLEMENTED(); return 0; }
bool MMTkHeap::IsGCInProgressHelper(bool bConsiderGCStart) { UNIMPLEMENTED(); return false;}
unsigned MMTkHeap::GetGcCount() { return 0; }
bool MMTkHeap::IsThreadUsingAllocationContextHeap(gc_alloc_context* acontext, int thread_number) { UNIMPLEMENTED(); return false; }
bool MMTkHeap::IsEphemeral(Object* object) { UNIMPLEMENTED(); return false; }
uint32_t MMTkHeap::WaitUntilGCComplete(bool bConsiderGCStart) { UNIMPLEMENTED(); return uint32_t(); }
void MMTkHeap::FixAllocContext(gc_alloc_context* acontext, void* arg, void* heap) { UNIMPLEMENTED(); }
size_t MMTkHeap::GetCurrentObjSize() { UNIMPLEMENTED(); return size_t(); }
void MMTkHeap::SetGCInProgress(bool fInProgress) { UNIMPLEMENTED(); }
bool MMTkHeap::RuntimeStructuresValid() { UNIMPLEMENTED(); return true; }
void MMTkHeap::SetSuspensionPending(bool fSuspensionPending) { UNIMPLEMENTED(); }
void MMTkHeap::SetYieldProcessorScalingFactor(float yieldProcessorScalingFactor) { UNIMPLEMENTED(); }
void MMTkHeap::Shutdown() { UNIMPLEMENTED(); }

/*    Add/RemoveMemoryPressure support routines    */
size_t MMTkHeap::GetLastGCStartTime(int generation) { UNIMPLEMENTED(); return size_t(); }
size_t MMTkHeap::GetLastGCDuration(int generation) { UNIMPLEMENTED(); return size_t(); }
size_t MMTkHeap::GetNow() { UNIMPLEMENTED(); return size_t(); }

/*    Allocation routines    */
Object* MMTkHeap::Alloc(gc_alloc_context* acontext, size_t size, uint32_t flags)
{
    //todo: this allocation function is broken ; v ;
    int sizeWithHeader = size + sizeof(ObjHeader) + sizeof(ObjHeader);
    printf("Alloc: size=%d,header=%d\n", size, sizeof(ObjHeader));

    ObjHeader* address = (ObjHeader*)alloc(handle, sizeWithHeader, 32, 0, 0);

    printf("MMTk Alloc %d %p\n", allocCount, address);
    allocCount++;
    return (Object*)(address + 4);
}
void MMTkHeap::PublishObject(uint8_t* obj) { UNIMPLEMENTED(); }
void MMTkHeap::SetWaitForGCEvent() { UNIMPLEMENTED(); }
void MMTkHeap::ResetWaitForGCEvent() { UNIMPLEMENTED(); }

/*    Heap verification routines    */
bool MMTkHeap::IsLargeObject(Object* pObj) { UNIMPLEMENTED(); return false; }
void MMTkHeap::ValidateObjectMember(Object* obj) { UNIMPLEMENTED(); }
Object* MMTkHeap::NextObj(Object* object) { UNIMPLEMENTED(); return nullptr; }
Object* MMTkHeap::GetContainingObject(void* pInteriorPtr, bool fCollectedGenOnly) { UNIMPLEMENTED(); return nullptr; }

/*    Profiling routines    */
void MMTkHeap::DiagWalkObject(Object* obj, walk_fn fn, void* context) { UNIMPLEMENTED(); }
void MMTkHeap::DiagWalkObject2(Object* obj, walk_fn2 fn, void* context) { UNIMPLEMENTED(); }
void MMTkHeap::DiagWalkHeap(walk_fn fn, void* context, int gen_number, bool walk_large_object_heap_p) { UNIMPLEMENTED(); }
void MMTkHeap::DiagWalkSurvivorsWithType(void* gc_context, record_surv_fn fn, void* diag_context, walk_surv_type type, int gen_number) { UNIMPLEMENTED(); }
void MMTkHeap::DiagWalkFinalizeQueue(void* gc_context, fq_walk_fn fn) { UNIMPLEMENTED(); }
void MMTkHeap::DiagScanFinalizeQueue(fq_scan_fn fn, ScanContext* context) { UNIMPLEMENTED(); }
void MMTkHeap::DiagScanHandles(handle_scan_fn fn, int gen_number, ScanContext* context) { UNIMPLEMENTED(); }
void MMTkHeap::DiagScanDependentHandles(handle_scan_fn fn, int gen_number, ScanContext* context) { UNIMPLEMENTED(); }
void MMTkHeap::DiagDescrGenerations(gen_walk_fn fn, void* context) { UNIMPLEMENTED(); }
void MMTkHeap::DiagTraceGCSegments() { UNIMPLEMENTED(); }

/*    GC Stress routines    */
bool MMTkHeap::StressHeap(gc_alloc_context* acontext) { UNIMPLEMENTED(); return true; }
segment_handle MMTkHeap::RegisterFrozenSegment(segment_info *pseginfo) { UNIMPLEMENTED(); return nullptr; }
void MMTkHeap::UnregisterFrozenSegment(segment_handle seg) { UNIMPLEMENTED(); }
bool MMTkHeap::IsInFrozenSegment(Object *object) { UNIMPLEMENTED(); return false; }

/*    Routines for informing the GC about which events are enabled.    */
void MMTkHeap::ControlEvents(GCEventKeyword keyword, GCEventLevel level) { UNIMPLEMENTED(); }
void MMTkHeap::ControlPrivateEvents(GCEventKeyword keyword, GCEventLevel level) { UNIMPLEMENTED(); }