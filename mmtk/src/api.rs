use libc::c_void;
use libc::c_char;
use std::ffi::CStr;
use mmtk::memory_manager;
use mmtk::AllocationSemantics;
use mmtk::util::{ObjectReference, OpaquePointer, Address};
use mmtk::{Mutator, SelectedPlan};
use mmtk::scheduler::GCWorker;
use mmtk::MMTK;

use DotNET;
use UPCALLS;
use DotNET_Upcalls;

#[no_mangle]
pub extern "C" fn gc_init(heap_size: usize) -> *mut c_void {
    let mmtk: Box<MMTK<DotNET>> = Box::new(MMTK::new());
    let mmtk: *mut MMTK<DotNET> = Box::into_raw(mmtk);
    memory_manager::gc_init(unsafe { &mut *mmtk }, heap_size);
    mmtk as *mut c_void
}

#[no_mangle]
pub extern "C" fn start_control_collector(mmtk: &mut MMTK<DotNET>, tls: OpaquePointer) {
    memory_manager::start_control_collector(&*mmtk, tls);
}

#[no_mangle]
pub extern "C" fn bind_mutator(mmtk: &'static mut MMTK<DotNET>, tls: OpaquePointer) -> *mut Mutator<SelectedPlan<DotNET>> {
    Box::into_raw(memory_manager::bind_mutator(mmtk, tls))
}

#[no_mangle]
pub extern "C" fn destroy_mutator(mutator: *mut Mutator<SelectedPlan<DotNET>>) {
    memory_manager::destroy_mutator(unsafe { Box::from_raw(mutator) })
}

#[no_mangle]
pub extern "C" fn alloc(mutator: &mut Mutator<SelectedPlan<DotNET>>, size: usize,
                    align: usize, offset: isize, semantics: AllocationSemantics) -> Address {
    memory_manager::alloc::<DotNET>(mutator, size, align, offset, semantics)
}

#[no_mangle]
pub extern "C" fn post_alloc(mutator: &mut Mutator<SelectedPlan<DotNET>>, refer: ObjectReference, type_refer: ObjectReference,
                                        bytes: usize, semantics: AllocationSemantics) {
    memory_manager::post_alloc::<DotNET>(mutator, refer, type_refer, bytes, semantics)
}

#[no_mangle]
pub extern "C" fn will_never_move(object: ObjectReference) -> bool {
    !object.is_movable()
}

#[no_mangle]
pub extern "C" fn start_worker(mmtk: &'static mut MMTK<DotNET>, tls: OpaquePointer, worker: &'static mut GCWorker<DotNET>) {
    memory_manager::start_worker::<DotNET>(tls, worker, mmtk);
}

#[no_mangle]
pub extern "C" fn enable_collection(mmtk: &'static mut MMTK<DotNET>, tls: OpaquePointer) {
    memory_manager::enable_collection(mmtk, tls);
}

#[no_mangle]
pub extern "C" fn used_bytes(mmtk: &mut MMTK<DotNET>) -> usize {
    memory_manager::used_bytes(mmtk)
}

#[no_mangle]
pub extern "C" fn free_bytes(mmtk: &mut MMTK<DotNET>) -> usize {
    memory_manager::free_bytes(mmtk)
}

#[no_mangle]
pub extern "C" fn total_bytes(mmtk: &mut MMTK<DotNET>) -> usize {
    memory_manager::total_bytes(&*mmtk)
}

#[no_mangle]
#[cfg(feature = "sanity")]
pub extern "C" fn scan_region(mmtk: &mut MMTK<DotNET>) {
    memory_manager::scan_region(mmtk);
}

#[no_mangle]
pub extern "C" fn is_live_object(object: ObjectReference) -> bool{
    object.is_live()
}

#[no_mangle]
pub extern "C" fn is_mapped_object(object: ObjectReference) -> bool {
    object.is_mapped()
}

#[no_mangle]
pub extern "C" fn is_mapped_address(address: Address) -> bool {
    address.is_mapped()
}

#[no_mangle]
pub extern "C" fn modify_check(mmtk: &mut MMTK<DotNET>, object: ObjectReference) {
    memory_manager::modify_check(mmtk, object);
}

#[no_mangle]
pub extern "C" fn handle_user_collection_request(mmtk: &mut MMTK<DotNET>, tls: OpaquePointer) {
    memory_manager::handle_user_collection_request::<DotNET>(mmtk, tls);
}

#[no_mangle]
pub extern "C" fn add_weak_candidate(mmtk: &mut MMTK<DotNET>, reff: ObjectReference, referent: ObjectReference) {
    memory_manager::add_weak_candidate(mmtk, reff, referent);
}

#[no_mangle]
pub extern "C" fn add_soft_candidate(mmtk: &mut MMTK<DotNET>, reff: ObjectReference, referent: ObjectReference) {
    memory_manager::add_soft_candidate(mmtk, reff, referent);
}

#[no_mangle]
pub extern "C" fn add_phantom_candidate(mmtk: &mut MMTK<DotNET>, reff: ObjectReference, referent: ObjectReference) {
    memory_manager::add_phantom_candidate(mmtk, reff, referent);
}

#[no_mangle]
pub extern "C" fn harness_begin(mmtk: &mut MMTK<DotNET>, tls: OpaquePointer) {
    memory_manager::harness_begin(mmtk, tls);
}

#[no_mangle]
pub extern "C" fn harness_end(mmtk: &'static mut MMTK<DotNET>, _tls: OpaquePointer) {
    memory_manager::harness_end(mmtk);
}

#[no_mangle]
pub extern "C" fn process(mmtk: &'static mut MMTK<DotNET>, name: *const c_char, value: *const c_char) -> bool {
    let name_str: &CStr = unsafe { CStr::from_ptr(name) };
    let value_str: &CStr = unsafe { CStr::from_ptr(value) };
    let res = memory_manager::process(
        mmtk, 
        name_str.to_str().unwrap(), 
        value_str.to_str().unwrap());
    
    res
}

#[no_mangle]
pub extern "C" fn starting_heap_address() -> Address {
    memory_manager::starting_heap_address()
}

#[no_mangle]
pub extern "C" fn last_heap_address() -> Address {
    memory_manager::last_heap_address()
}
