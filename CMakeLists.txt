 cmake_minimum_required(VERSION 3.12)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  /opt/dependency/package/include/
)
set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")
set(CMAKE_LD_FLAGS "-g")

add_executable(clashcmd main.cc clashcmd.cc)

target_link_libraries(clashcmd  jsoncpp pthread)

INSTALL(TARGETS clashcmd RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})