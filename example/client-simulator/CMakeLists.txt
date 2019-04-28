project(ClientSimulator)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

find_package(Boost REQUIRED COMPONENTS system)

# set FM SDK directory
set(_FM_SDK_DIR ${PROJECT_SOURCE_DIR}/../..)
add_subdirectory(${_FM_SDK_DIR} ${_FM_SDK_DIR}/lib)

include_directories(${_FM_SDK_DIR}/include)
include_directories(${_FM_SDK_DIR}/generated)

set(FILES
    ${PROJECT_SOURCE_DIR}/ISimulator.cpp
    ${PROJECT_SOURCE_DIR}/ISimulator.hpp
    ${PROJECT_SOURCE_DIR}/DeviceSimulator.cpp
    ${PROJECT_SOURCE_DIR}/DeviceSimulator.hpp
    ${PROJECT_SOURCE_DIR}/PilotSimulator.cpp
    ${PROJECT_SOURCE_DIR}/PilotSimulator.hpp
    )

set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j4")

add_library(client-simulator STATIC ${FILES})
target_link_libraries(client-simulator
    fm-sdk
    )

foreach (_target deviceSim pilotSim)

    add_executable("1-${_target}" "${_target}.cpp" "${_target}-config.ini")
    target_link_libraries("1-${_target}"
        ${Boost_LIBRARIES}
        client-simulator
        )

endforeach()
