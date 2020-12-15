use mmtk::vm::ReferenceGlue;
use mmtk::util::ObjectReference;
use mmtk::TraceLocal;
use mmtk::util::OpaquePointer;
use DotNET;

pub struct VMReferenceGlue {}

impl ReferenceGlue<DotNET> for VMReferenceGlue {
    fn set_referent(reff: ObjectReference, referent: ObjectReference) {
        unimplemented!()
    }
    fn get_referent(object: ObjectReference) -> ObjectReference {
        unimplemented!()
    }
    fn process_reference<T: TraceLocal>(trace: &mut T, reference: ObjectReference, tls: OpaquePointer) -> ObjectReference {
        unimplemented!()
    }
}