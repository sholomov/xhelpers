cmake_minimum_required(VERSION 3.1)

project(xhelpers)

# Set global CMake properties
# -----------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake targets")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)

# Build options
# -----------------------------------------------------------------------
option(BUILD_WITH_PCH "Specifies whether to use precompiled headers" OFF)

if(NOT WIN32)
  option(BUILD_32BIT "Force to build x32" OFF)
endif()

if (UNIX)
  option(UNIX_WITH_SOUND "Specifies whether ubuntu sound libs used in ar13 should be built"  OFF)
endif()

# Determine the platform
# -----------------------------------------------------------------------
if(WIN32)
  set(PLATFORM_NAME "win")
elseif(UNIX)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l")
    set(PLATFORM_NAME "armv7l-")
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(PLATFORM_NAME "aarch64-")
  else()
    set(PLATFORM_NAME "unix")
  endif()
else()
  message(fatal_error "Unsupported platform")
endif()

# Determine platform bits
# -----------------------------------------------------------------------
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(PLATFORM_BITNESS "32")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PLATFORM_BITNESS "64")
else()
  message(fatal_error "Unsupported bitness")
endif()

if(BUILD_32BIT)
  set(PLATFORM_BITNESS "32")
endif()

# Platform is the pair of system and its bitness
# -----------------------------------------------------------------------
set(PLATFORM "${PLATFORM_NAME}${PLATFORM_BITNESS}")
message( "Platform is ${PLATFORM}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(COMPILER_ID "icl")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(COMPILER_ID "gcc")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(COMPILER_ID "gcc")
elseif(MSVC_VERSION MATCHES "^150[0-9]$")
  set(COMPILER_ID "vc2008")
elseif(MSVC_VERSION MATCHES "^160[0-9]$")
  set(COMPILER_ID "vc2010")
elseif(MSVC_VERSION MATCHES "^170[0-9]$")
  set(COMPILER_ID "vc2012")
elseif(MSVC_VERSION MATCHES "^180[0-9]$")
  set(COMPILER_ID "vc2013")
elseif(MSVC_VERSION MATCHES "^190[0-9]$")
  set(COMPILER_ID "vc2015")
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} legacy_stdio_definitions.lib")
elseif(MSVC_VERSION MATCHES "^191[0-9]$")
  set(COMPILER_ID "vc2017")
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} legacy_stdio_definitions.lib")
elseif(MSVC_VERSION MATCHES "^192[0-9]$")
  set(COMPILER_ID "vc2019" )
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} legacy_stdio_definitions.lib")
else()
  set(COMPILER_ID "unknown")
endif()

message("Compiler id is ${COMPILER_ID}")

set(PLATFORM "${PLATFORM}.${COMPILER_ID}")
message("Build suffix is ${PLATFORM}")

# Configuration
# -----------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "release")
endif()
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  set( CONFIGURATION "debug" )
else()
  set( CONFIGURATION "release" )
endif()
MESSAGE( "Configuration is ${CONFIGURATION}" )


# C Runtime Environment.
# -----------------------------------------------------------------------
if(MSVC)
  set(CRT "vc")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CRT "gnu")
elseif(MAC_OS OR APPLE)
  set(CRT "gnu")
else()
  message(fatal_error "Unsupported CRT")
endif()


# Setting output directories
# -----------------------------------------------------------------------
# Runtime output directory
# -----------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.${CONFIGURATION}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.relwithdeb")
if( BUILD_SUFFIX )
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}.${BUILD_SUFFIX}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}.${BUILD_SUFFIX}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}.${BUILD_SUFFIX}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}.${BUILD_SUFFIX}")
endif()
# Library output directory
# -----------------------------------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY         "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.${CONFIGURATION}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.relwithdeb")
if( BUILD_SUFFIX )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY         "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}.${BUILD_SUFFIX}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}.${BUILD_SUFFIX}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}.${BUILD_SUFFIX}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO}.${BUILD_SUFFIX}")
endif()

# Archive output directory
# -----------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY         "${CMAKE_CURRENT_BINARY_DIR}/lib.${PLATFORM}.${CRT}.${CONFIGURATION}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_BINARY_DIR}/lib.${PLATFORM}.${CRT}.debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/lib.${PLATFORM}.${CRT}.release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/lib.${PLATFORM}.${CRT}.relwithdeb")
if( BUILD_SUFFIX )
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY         "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}.${BUILD_SUFFIX}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}.${BUILD_SUFFIX}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}.${BUILD_SUFFIX}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO}.${BUILD_SUFFIX}")
endif()
set(CMAKE_3RDPARTY_LIBRARY_DIRECTORY       "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}")

# Find OpenCV
# -----------------------------------------------------------------------
# find_package(OpenCV REQUIRED)

# Find OpenMP
# -----------------------------------------------------------------------
find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

message("COMPILER_ID is ${COMPILER_ID}")

# Include directories
# -----------------------------------------------------------------------
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/xhelpers
)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
  if(MSVC90)
    add_definitions(/wd4482)
  endif(MSVC90)
  set(CMAKE_C_FLAGS "/Zp1 /Za /W3 /nologo")
  if( ${CONFIGURATION} MATCHES "debug" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /LDd /MDd" )
  ELSE()
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /LD /MD" )
  endif()
  # TODO: check hack usage
  # Directory with libraries link to.
  # Here is a hack to set right link directory in MSVC IDE.
  if( MSVC_IDE )
    LINK_DIRECTORIES( "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}.${CRT}/$(ConfigurationName)/" )
  ELSE()
    LINK_DIRECTORIES( "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}.${CRT}/${CONFIGURATION}" )
  endif()
  
  # multiprocess object compilation
  # https://blog.kitware.com/cmake-building-with-all-your-cores/
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP3")

  if( PLATFORM_BITNESS MATCHES "32")
    add_link_options("/MACHINE:X86")
    set_target_properties(PROPERTIES LINK_FLAGS "/MACHINE:X86")
  endif()

endif(MSVC)

if( CMAKE_COMPILER_IS_GNUCXX )
  set( WITH_PCH OFF )
  LINK_DIRECTORIES( ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
  ADD_DEFINITIONS(-D_REENTRANT)
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -Wextra -Wcast-align -fPIC -Wno-unused -Wno-missing-field-initializers" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wcast-align -fPIC -Wno-unused -Wno-missing-field-initializers -Weffc++" )
  set( CMAKE_INSTALL_RPATH "$ORIGIN" )
  set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )

  if( ${CONFIGURATION} MATCHES "debug" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address" )
  ELSE()
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
  endif()
  
  #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

  if( PLATFORM_BITNESS MATCHES "32")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
  endif()

endif()

message( "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
message( "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )

add_definitions(-DTIXML_USE_STL)

# Add project collections to build tree.
# -----------------------------------------------------------------------
add_subdirectory(xhelpers)
add_subdirectory(test)
