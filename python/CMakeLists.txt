project(displaySlice-python)
set(PYTHON_DIR "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")

cmake_minimum_required(VERSION 3.9)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# find_package(NumPy REQUIRED)

set(PYTHON_DIR "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../lib")

find_package(Boost 1.40 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(PYREQ numpy)
foreach (PACKAGE IN LISTS ${PYREQ})
 execute_process(
    COMMAND Python3:Interpreter -c "import ${PACKAGE}"
    RESULT_VARIABLE EXIT_CODE
    OUTPUT_QUIET
)
if (NOT ${EXIT_CODE} EQUAL 0)
  message(FATAL_ERROR " Required ${PACKAGE} python3 package not installed")
  endif()

endforeach()
pybind11_add_module(
  pyDisplaySlice
  MODULE
  pyDisplay.cpp
  pyDataset.cpp
  pyPosition.cpp
 )

target_include_directories(
  pyDisplaySlice
   PUBLIC ${PYTHON_INCLUDE_DIR}
  ${pybind11_INCLUDE_DIR}
  ${PYTHON_INCLUDE_PATH}
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}../src>)



  target_link_libraries(pyDisplaySlice PRIVATE displaySlice)
  install(FILES Dataset.py Position.py DESTINATION ${PYTHON_DIR})


