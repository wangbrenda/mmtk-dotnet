add_definitions(-DHOST_64BIT -DHOST_X86 -DTARGET_UNIX)

include_directories(
    ${ABSOLUTE_CORECLR_FOR_CUSTOM_GC}/gc/env
    ${ABSOLUTE_CORECLR_FOR_CUSTOM_GC}/gc
    ${ABSOLUTE_CORECLR_FOR_CUSTOM_GC}/inc
    ${ABSOLUTE_CORECLR_FOR_CUSTOM_GC}/pal/inc
    ${ABSOLUTE_CORECLR_FOR_CUSTOM_GC}/pal/src
    ${ABSOLUTE_MMTK_HEADER}
)

add_library(mmtk_dotnet SHARED
    mmtkgc.cpp
    mmtkheap.cpp
    mmtkhandlemanager.cpp
    mmtkhandlestore.cpp
)

#find_library(MMTK_LIB_PATH
#    NAMES mmtk_dotnet libmmtk_dotnet.so
#    PATHS ${ABSOLUTE_MMTK_LIBRARY_PATH}
#    NO_DEFAULT_PATH
#)
add_library(mmtk_lib SHARED IMPORTED)
set_target_properties(mmtk_lib PROPERTIES BUILD_WITH_INSTALL_RPATH 1 INSTALL_NAME_DIR "@rpath" LINKER_LANGUAGE C IMPORTED_LOCATION ${ABSOLUTE_MMTK_LIBRARY_PATH}/libmmtk_dotnet.so)
target_link_libraries(mmtk_dotnet mmtk_lib)