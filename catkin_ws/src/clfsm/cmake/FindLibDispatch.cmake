# Components: private_headers, compiler_support

include (FindPackageHandleStandardArgs)
include (CheckFunctionExists)
include (CheckCCompilerFlag)

find_path(LIBDISPATCH_PUBLIC_INCLUDE_DIR dispatch/dispatch.h
  DOC "Path to dispatch/dispatch.h"
)

if (LIBDISPATCH_PUBLIC_INCLUDE_DIR)
  list (APPEND LIBDISPATCH_INCLUDE_DIRS ${LIBDISPATCH_PUBLIC_INCLUDE_DIR})
endif ()

check_function_exists(LIBDISPATCH_RUNTIME_IN_LIBC _dispatch_async_f)

if (LIBDISPATCH_RUNTIME_IN_LIBC)
  set (LIBDISPATCH_LIBRARIES "c")
else ()
  if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set (LIBDISPATCH_LIBRARIES "c")
    set (LIBDISPATCH_LINKER_FLAGS "-framework System")
  else ()
    find_library(LIBDISPATCH_LIBRARIES "dispatch" HINTS "${CMAKE_CURRENT_LIST_DIR}" /usr/lib /usr/local/lib)
  endif ()
endif ()

check_c_compiler_flag("-fblocks" LIBDISPATCH_COMPILER_BLOCKS_SUPPORT_FOUND)
if (LIBDISPATCH_COMPILER_BLOCKS_SUPPORT_FOUND)
  set (LIBDISPATCH_COMPILE_FLAGS "-fblocks")
  set (CBlocks_compiler_support_FOUND TRUE)  # for FPHSA
endif ()

find_package_handle_standard_args(LibDispatch
  REQUIRED_VARS LIBDISPATCH_LIBRARIES LIBDISPATCH_PUBLIC_INCLUDE_DIR
  HANDLE_COMPONENTS
)

