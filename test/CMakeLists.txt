project (xhelpers_test)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/xhelpers
)

add_executable(xhelpers_test
  xhelpers_test.cpp
)

target_link_libraries(xhelpers_test
  xhelpers)

