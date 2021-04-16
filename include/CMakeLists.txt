project(xhelpers)

set(xhelpers_hdr
  sx_cast.h
  sx_findfile.h
  sx_jsonstring.h
  sx_path.h
  sx_srm.h
  sx_str.h
  sx_string.h
  sx_system.h
  sx_timer.h
  sx_timestamp.h
  sx_types.h
)

set(xhelpers_src
  src/sx_system.cpp
)

set(thirdparty
  portability/filesystem_system.cpp
  portability/filesystem_fixes.cpp
)

add_library(xhelpers 
  ${xhelpers_hdr}
  ${xhelpers_src}
  ${thirdparty_src}
)

target_link_libraries(xhelpers)
