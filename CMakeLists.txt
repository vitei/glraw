
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)


# Project description and (meta) information

set(META_PROJECT_NAME        "glraw")
set(META_PROJECT_DESCRIPTION "Converts images to OpenGL compatible raw files")
set(META_VERSION_MAJOR       "1")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "4")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_ORGANIZATION "hpicgs group")
set(META_AUTHOR_DOMAIN       "https://github.com/hpicgs/glraw")
set(META_AUTHOR_MAINTAINER   "daniel.limberger@hpi.de")

string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)


# Limit supported configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)

# Set project name and type (C/C++)
project(${META_PROJECT_NAME} C CXX)


# Configuration options

option(OPTION_PORTABLE_INSTALL  "Install into a self-contained directory." OFF)
option(OPTION_BUILD_STATIC      "Build static libraries." OFF)
#option(OPTION_BUILD_TESTS       "Build tests (if gmock and gtest are found)." ON)


if(OPTION_BUILD_STATIC)
   set(BUILD_SHARED_LIBS OFF)
   message("Note: ${META_PROJECT_NAME_UPPER}_STATIC needs to be defined for static linking.")
else()
    set(BUILD_SHARED_LIBS ON)
endif()


# CMake configuration:

# Append the path to the custom cmake modules from this project to the CMAKE_MODULE_PATH.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Put projects in root folder by default
set(IDE_FOLDER "")  

# Include custom cmake functions
include(cmake/Custom.cmake)
include(cmake/GitRevision.cmake)


# Platform and architecture setup

# Architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

# Setup platform specifics (compile flags, etc., ...)

# This policy was introduced in 3.0.0 and does not allow for COMPILER_DEFINITIONS_<Config>,
# anymore, but instead requires generator expressions like $<CONFIG:Debug> ... 
# For now the current compile-flag, -definitions, and linker-flags setup shall remain as is.
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 OLD)
endif()

if(MSVC)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
elseif(UNIX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
else()
    message(WARNING "Unsupported platform/compiler combination")
endif()


# Deployment/installation setup

set(project ${META_PROJECT_NAME})
if(WIN32)

    # "%PROGRAMFILES%/<project>/"

    set(INSTALL_ROOT      ".")
    set(INSTALL_DATA      "bin")
    set(INSTALL_BIN       "bin")
    set(INSTALL_SHARED    ".")
    set(INSTALL_LIB       "lib")
    set(INSTALL_INCLUDE   "include")
    set(INSTALL_DOC       "doc")
    set(INSTALL_SHORTCUTS ".") # not available in windows
    set(INSTALL_ICONS     ".") # not available in windows
    set(INSTALL_INIT      ".") # not available in windows

else()

    # "/user/[local]/"  

    set(INSTALL_ROOT      "share/${project}")
    set(INSTALL_DATA      "share/${project}")
    set(INSTALL_BIN       "bin")
    set(INSTALL_SHARED    "lib")
    set(INSTALL_LIB       "lib")
    set(INSTALL_INCLUDE   "include")
    set(INSTALL_DOC       "share/doc/${project}")
    set(INSTALL_SHORTCUTS "share/applications")
    set(INSTALL_ICONS     "share/pixmaps")
    set(INSTALL_INIT      "/etc/init") # /etc/init (upstart init scripts)

    # Adjust target paths for portable installs

    # "<INSTALL_PREFIX>/"

    if(OPTION_PORTABLE_INSTALL)
        # Put binaries in root directory and keep data directory name
        set(INSTALL_ROOT      ".")
        set(INSTALL_UTILITIES ".")
        set(INSTALL_DATA      ".")
        set(INSTALL_BIN       ".")

        # We have to change the RPATH of binaries to achieve a usable local install.
        # [TODO] For binaries, "$ORIGIN/lib" is right, so that libraries are found in ./lib.
        #        However, I have not yet tested what happens when libraries use other libraries.
        #        In that case, they might need the rpath $ORIGIN instead ...
        set(CMAKE_SKIP_BUILD_RPATH            FALSE)         # Use automatic rpath for build
        set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE)         # Use specific rpath for INSTALL
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)         # NO automatic rpath for INSTALL
        set(CMAKE_INSTALL_RPATH               "$ORIGIN/lib") # Libraries are relative to binary
    endif()

endif()

# Global deployment

# Add a revision file containing the git-head tag for cpack and install
create_revision_file(${CMAKE_BINARY_DIR}/revision ${INSTALL_ROOT})

# Project meta files
install(FILES ${CMAKE_BINARY_DIR}/revision DESTINATION ${INSTALL_ROOT} COMPONENT dev)
if(NOT OPTION_BUILD_STATIC)
    install(FILES ${CMAKE_BINARY_DIR}/revision DESTINATION ${INSTALL_ROOT} COMPONENT runtime)
endif()
install(FILES AUTHORS DESTINATION               ${INSTALL_ROOT} COMPONENT dev)
install(FILES LICENSE DESTINATION               ${INSTALL_ROOT} COMPONENT dev)
install(FILES glraw-config.cmake DESTINATION    ${INSTALL_ROOT} COMPONENT dev)


# Data files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION ${INSTALL_DATA} COMPONENT runtime)

# Include projects

add_subdirectory(source)
add_subdirectory(docs)
add_subdirectory(packages)
