#include <stdio.h>
#include <mutex>
#include "mmtkheap.h"
#include "mmtk.h"
#define ASSERT assert
#include "gcenv.interlocked.h"
#include "gcenv.object.h"
#define UNIMPLEMENTED_VERBOSITY 1
#define UNIMPLEMENTED(x) if (x > UNIMPLEMENTED_VERBOSITY) printf("MMTkHeap::%s unimplemented\n", __func__);

int allocCount = 0;
void* handle;

/*    Hosting APIs    */
bool MMTkHeap::IsValidSegmentSize(size_t size) { UNIMPLEMENTED(2); return true; }
bool MMTkHeap::IsValidGen0MaxSize(size_t size) { UNIMPLEMENTED(2); return true; }
size_t MMTkHeap::GetValidSegmentSize(bool large_seg) { UNIMPLEMENTED(2); return size_t(); }
void MMTkHeap::SetReservedVMLimit(size_t vmlimit) { UNIMPLEMENTED(2); }

/*   Concurrent GC routines    */
// We do not support concurrency in MMTk yet
void MMTkHeap::WaitUntilConcurrentGCComplete() { UNIMPLEMENTED(2); }
bool MMTkHeap::IsConcurrentGCInProgress()
{
    return false;
}
void MMTkHeap::TemporaryEnableConcurrentGC() { UNIMPLEMENTED(2); }
void MMTkHeap::TemporaryDisableConcurrentGC() { UNIMPLEMENTED(2); }
bool MMTkHeap::IsConcurrentGCEnabled() { UNIMPLEMENTED(2); return false; }
HRESULT MMTkHeap::WaitUntilConcurrentGCCompleteAsync(int millisecondsTimeout) { UNIMPLEMENTED(2); return S_OK; }    // Use in native threads. TRUE if succeed. FALSE if failed or timeout

/*    Finalization routines    */
size_t MMTkHeap::GetNumberOfFinalizable() { UNIMPLEMENTED(2); return size_t(); }
Object* MMTkHeap::GetNextFinalizable() { UNIMPLEMENTED(2); return nullptr; }

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
                            int kind) { UNIMPLEMENTED(2); }
uint32_t MMTkHeap::GetMemoryLoad() { UNIMPLEMENTED(2); return uint32_t(); }
int MMTkHeap::GetGcLatencyMode() { UNIMPLEMENTED(2); return 0; }
int MMTkHeap::SetGcLatencyMode(int newLatencyMode) { UNIMPLEMENTED(2); return 0; }
int MMTkHeap::GetLOHCompactionMode() { UNIMPLEMENTED(2); return 0; }
void MMTkHeap::SetLOHCompactionMode(int newLOHCompactionMode) { UNIMPLEMENTED(2); }
bool MMTkHeap::RegisterForFullGCNotification(uint32_t gen2Percentage, uint32_t lohPercentage) { UNIMPLEMENTED(2); return true; }
bool MMTkHeap::CancelFullGCNotification() { UNIMPLEMENTED(2); return true; }
int MMTkHeap::WaitForFullGCApproach(int millisecondsTimeout) { UNIMPLEMENTED(2); return 0;}
int MMTkHeap::WaitForFullGCComplete(int millisecondsTimeout) { UNIMPLEMENTED(2); return 0; }
unsigned MMTkHeap::WhichGeneration(Object* obj) { UNIMPLEMENTED(2); return 0; }
int MMTkHeap::CollectionCount(int generation, int get_bgc_fgc_coutn) { UNIMPLEMENTED(2); return 0; }
int MMTkHeap::StartNoGCRegion(uint64_t totalSize, bool lohSizeKnown, uint64_t lohSize, bool disallowFullBlockingGC) { UNIMPLEMENTED(2); return 0; }
int MMTkHeap::EndNoGCRegion() { UNIMPLEMENTED(2); return 0; }
size_t MMTkHeap::GetTotalBytesInUse() { UNIMPLEMENTED(2); return size_t(); }
uint64_t MMTkHeap::GetTotalAllocatedBytes() { UNIMPLEMENTED(2); return uint64_t(); }
HRESULT MMTkHeap::GarbageCollect(int generation, bool low_memory_p, int mode) { UNIMPLEMENTED(2); return S_OK; }
unsigned MMTkHeap::GetMaxGeneration() { UNIMPLEMENTED(2); return 0; }
void MMTkHeap::SetFinalizationRun(Object* obj) { UNIMPLEMENTED(2); }
bool MMTkHeap::RegisterForFinalization(int gen, Object* obj) { UNIMPLEMENTED(2); return true; }
int MMTkHeap::GetLastGCPercentTimeInGC() { UNIMPLEMENTED(2); return 0; }
size_t MMTkHeap::GetLastGCGenerationSize(int gen) { UNIMPLEMENTED(2); return size_t(); }

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

    void* mmtk = gc_init(10 * 1024 * 1024);
    printf("Initializing MMTK Heap\n");
    handle = bind_mutator(mmtk, 0);
    printf("Binding MMTK Mutator\n");

    return NOERROR;
}
bool MMTkHeap::IsPromoted(Object* object) { UNIMPLEMENTED(2); return false;}
bool MMTkHeap::IsHeapPointer(void* object, bool small_heap_only)
{
    return is_mapped_object(object);
}
unsigned MMTkHeap::GetCondemnedGeneration() { UNIMPLEMENTED(2); return 0; }
bool MMTkHeap::IsGCInProgressHelper(bool bConsiderGCStart)
{
    // Currently, a GC is never performed
    return isVMSuspended;
}
unsigned MMTkHeap::GetGcCount() { return 0; }
bool MMTkHeap::IsThreadUsingAllocationContextHeap(gc_alloc_context* acontext, int thread_number) { UNIMPLEMENTED(2); return false; }
bool MMTkHeap::IsEphemeral(Object* object) { UNIMPLEMENTED(2); return false; }
uint32_t MMTkHeap::WaitUntilGCComplete(bool bConsiderGCStart) { UNIMPLEMENTED(2); return uint32_t(); }
void MMTkHeap::FixAllocContext(gc_alloc_context* acontext, void* arg, void* heap) { UNIMPLEMENTED(2); }
size_t MMTkHeap::GetCurrentObjSize() { UNIMPLEMENTED(2); return size_t(); }
void MMTkHeap::SetGCInProgress(bool fInProgress)
{
    // The VM calls this function with true when they are suspended
    // It does not mean that it is triggering a GC
    isVMSuspended = fInProgress;
}
bool MMTkHeap::RuntimeStructuresValid() { UNIMPLEMENTED(1); return true; }
void MMTkHeap::SetSuspensionPending(bool fSuspensionPending) { UNIMPLEMENTED(2); }
void MMTkHeap::SetYieldProcessorScalingFactor(float yieldProcessorScalingFactor) { UNIMPLEMENTED(2); }
void MMTkHeap::Shutdown() { UNIMPLEMENTED(2); }

/*    Add/RemoveMemoryPressure support routines    */
size_t MMTkHeap::GetLastGCStartTime(int generation) { UNIMPLEMENTED(2); return size_t(); }
size_t MMTkHeap::GetLastGCDuration(int generation) { UNIMPLEMENTED(2); return size_t(); }
size_t MMTkHeap::GetNow() { UNIMPLEMENTED(2); return size_t(); }

/*    Allocation routines    */
Object* MMTkHeap::Alloc(gc_alloc_context* acontext, size_t size, uint32_t flags)
{
    size_t sizeWithHeader = size + sizeof(ObjHeader) * 8;
    ObjHeader* address = (ObjHeader*)alloc(handle, sizeWithHeader, 16, 16, 0);
    if (address == nullptr) printf("Out of memory exception occurred in MMTk\n");
    Object* returnAddr = (Object*)(address + 4);
    allocCount++;
    if (allocCount % 100 == 0) {
        printf("Allocated %d objects\n", allocCount);
    }
    return returnAddr;
}
void MMTkHeap::PublishObject(uint8_t* obj) { UNIMPLEMENTED(2); }
void MMTkHeap::SetWaitForGCEvent() { UNIMPLEMENTED(2); }
void MMTkHeap::ResetWaitForGCEvent() { UNIMPLEMENTED(2); }

/*    Heap verification routines    */
bool MMTkHeap::IsLargeObject(Object* pObj) { UNIMPLEMENTED(1); return false; }
void MMTkHeap::ValidateObjectMember(Object* obj) { UNIMPLEMENTED(1); }
Object* MMTkHeap::NextObj(Object* object) { UNIMPLEMENTED(1); return nullptr; }
Object* MMTkHeap::GetContainingObject(void* pInteriorPtr, bool fCollectedGenOnly) { UNIMPLEMENTED(1); return nullptr; }

/*    Profiling routines    */
void MMTkHeap::DiagWalkObject(Object* obj, walk_fn fn, void* context) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagWalkObject2(Object* obj, walk_fn2 fn, void* context) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagWalkHeap(walk_fn fn, void* context, int gen_number, bool walk_large_object_heap_p) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagWalkSurvivorsWithType(void* gc_context, record_surv_fn fn, void* diag_context, walk_surv_type type, int gen_number) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagWalkFinalizeQueue(void* gc_context, fq_walk_fn fn) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagScanFinalizeQueue(fq_scan_fn fn, ScanContext* context) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagScanHandles(handle_scan_fn fn, int gen_number, ScanContext* context) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagScanDependentHandles(handle_scan_fn fn, int gen_number, ScanContext* context) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagDescrGenerations(gen_walk_fn fn, void* context) { UNIMPLEMENTED(2); }
void MMTkHeap::DiagTraceGCSegments() { UNIMPLEMENTED(2); }

/*    GC Stress routines    */
bool MMTkHeap::StressHeap(gc_alloc_context* acontext) { UNIMPLEMENTED(2); return true; }
segment_handle MMTkHeap::RegisterFrozenSegment(segment_info *pseginfo) { UNIMPLEMENTED(2); return nullptr; }
void MMTkHeap::UnregisterFrozenSegment(segment_handle seg) { UNIMPLEMENTED(2); }
bool MMTkHeap::IsInFrozenSegment(Object *object) { UNIMPLEMENTED(2); return false; }

/*    Routines for informing the GC about which events are enabled.    */
void MMTkHeap::ControlEvents(GCEventKeyword keyword, GCEventLevel level) { UNIMPLEMENTED(2); }
void MMTkHeap::ControlPrivateEvents(GCEventKeyword keyword, GCEventLevel level) { UNIMPLEMENTED(2); }