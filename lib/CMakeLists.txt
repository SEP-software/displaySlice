cmake_minimum_required(VERSION 3.5)

project(displaySliceLiv)



set(SOURCES
auto_brown.cpp
autopick_2d.cpp
auto_viteribi.cpp
buffer.cpp
#buffers_buffer.cpp
#buffers_data.cpp
#byte_buffer_buffer.cpp
byte_buffer.cpp
created_data.cpp
dataset.cpp
dynamic.cpp
fastmarch.cpp
fileIO.cpp
#float_buffer_buffer.cpp
float_buffer.cpp
incore_data.cpp
io_func.cpp
lloyd_1d.cpp
lloyd.cpp
orientation_server.cpp
orient_cube.cpp
orient_map.cpp
outcore_data.cpp
pairs_new.cpp
param_func.cpp
partial_data.cpp
#part_semb_data.cpp
path.cpp
percentile.cpp
pick_bucket.cpp
pick_new.cpp
position.cpp
pqueue.cpp
trace_io.cpp
util.cpp
viterbi.cpp
)



set(INCLUDES
auto_brown.h
autopick_2d.h
auto_viteribi.h
bar_pt.h
buffer.h
#buffers_buffer.h
buffers_data.h
#byte_buffer_buffer.h
byte_buffer.h
created_data.h
dataset.h
dynamic.h
fastmarch.h
fileIO.h
#float_buffer_buffer.h
float_buffer.h
incore_data.h
io_func.h
lloyd_1d.h
lloyd.h
my_score.h
orientation_server.h
orient_cube.h
orient_map.h
outcore_data.h
pairs_new.h
param_func.h
#partial_data.h
part_semb_data.h
path.h
percentile.h
pick_bucket.h
pick_new.h
position.h
pqueue.h
trace_io.h
util.h
viterbi.h
)


add_library(displaySlice  SHARED ${SOURCES} ${INCLUDES})
target_link_libraries(displaySlice genericCpp)

target_include_directories(
  displaySlice
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         )


 install(
	 TARGETS displaySlice
  DESTINATION lib

  )
