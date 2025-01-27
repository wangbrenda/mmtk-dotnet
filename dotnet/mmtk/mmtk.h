#ifndef MMTK_H
#define MMTK_H

#include <stdint.h>
#include <stddef.h>

typedef ptrdiff_t ssize_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef void* MMTk_Mutator;
typedef void* MMTk_TraceLocal;

/**
 * Allocation
 */
extern MMTk_Mutator bind_mutator(void* mmtk, void *tls);
extern void destroy_mutator(MMTk_Mutator mutator);

extern void* alloc(MMTk_Mutator mutator, size_t size,
    size_t align, ssize_t offset, int allocator);

extern void* alloc_slow(MMTk_Mutator mutator, size_t size,
    size_t align, ssize_t offset, int allocator);

extern void post_alloc(MMTk_Mutator mutator, void* refer, void* type_refer,
    int bytes, int allocator);

extern uint32_t is_live_object(void* ref);
extern uint32_t is_mapped_object(void* ref);
extern uint32_t is_mapped_address(void* addr);
extern void modify_check(void* ref);

/**
 * Tracing
 */
extern void report_delayed_root_edge(MMTk_TraceLocal trace_local,
                                     void* addr);

extern uint32_t will_not_move_in_current_collection(MMTk_TraceLocal trace_local,
                                                void* obj);

extern void process_interior_edge(MMTk_TraceLocal trace_local, void* target,
                                  void* slot, uint32_t root);

extern void* trace_get_forwarded_referent(MMTk_TraceLocal trace_local, void* obj);

extern void* trace_get_forwarded_reference(MMTk_TraceLocal trace_local, void* obj);

extern void* trace_retain_referent(MMTk_TraceLocal trace_local, void* obj);

/**
 * Misc
 */
extern void* gc_init(size_t heap_size);
extern uint32_t will_never_move(void* object);
extern uint32_t process(char* name, char* value);
extern void scan_region();
extern void handle_user_collection_request(void *tls);

extern void start_control_collector(void *tls);
extern void start_worker(void *tls, void* worker);

/**
 * VM Accounting
 */
extern size_t free_bytes();
extern size_t total_bytes();
extern size_t used_bytes();
extern void* starting_heap_address();
extern void* last_heap_address();

/**
 * Reference Processing
 */
extern void add_weak_candidate(void* ref, void* referent);
extern void add_soft_candidate(void* ref, void* referent);
extern void add_phantom_candidate(void* ref, void* referent);

extern void harness_begin(void *tls);
extern void harness_end();

#ifdef __cplusplus
}
#endif

#endif // MMTK_H