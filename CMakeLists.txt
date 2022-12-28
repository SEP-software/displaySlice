cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
option(BUILD_SEP "Build SEP version of qtcube" ON)


if(NOT SUBPACKAGE)
  option(BUILD_PYTHON "Build python interfaces" ON)
  option(INSTALL_CMAKE_ENV "Install cmake build environment" ON)
  option(BUILD_DOC "Build documentation" OFF)
  option(BUILD_TEST "Build with test support" OFF)
else(NOT SUBPACKAGE)

endif(NOT SUBPACKAGE)

project(hypercube LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE True)
# configure a header file to pass some of the CMake settings

set(displaySlice_MAJOR_VERSION 0)
set(displaySlice_MINOR_VERSION 2)
set(hypercube_VERSION_STRING
    ${displaySlice_MAJOR_VERSION}.$displaySlice_MINOR_VERSION})

include_directories("${PROJECT_BINARY_DIR}")

set(displaySlice_DIRNAME_cmake "lib/cmake")
set(displaySlice_DIRNAME_lib "lib")
set(displaySlice_DIRNAME_include "include")

include(CMakePackageConfigHelpers)

# first we can indicate the documentation build as an option and set it to ON by
# default

if(BUILD_DOC)
  # check if Doxygen is installed
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # note the option ALL which allows to build the docs together with the
    # application
    add_custom_target(doc_doxygen ALL
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)
  else(DOXYGEN_FOUND)
    message(
      "Doxygen need to be installed to generate the doxygen documentation")
  endif(DOXYGEN_FOUND)
endif(BUILD_DOC)

if(NOT SUBPACKAGE)


  execute_process(COMMAND git submodule update --init --recursive --
                          external/sep-ioLibs
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


endif(NOT SUBPACKAGE)

set(SUBPACKAGE true)

if(BUILD_TEST)
  enable_testing()
endif(BUILD_TEST)

if(BUILD_SEP)
add_subdirectory(external/sep-iolibs)
else(BUILD_SEP)
add_subdirectory(external/sep-iolibs/external/genericIO)
endif(BUILD_SEP)
add_subdirectory(lib)
add_subdirectory(python)
include(CMakePackageConfigHelpers)
configure_package_config_file(
  displaySliceConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/hypercubeConfig.cmake"
  INSTALL_DESTINATION
  cmake/SEP/IO)



include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${displaySlice_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${displaySlice_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_NAME "displaySlice")
set(CPACK_PACKAGE_CONTACT "Robert Clapp")
include(CPack)
