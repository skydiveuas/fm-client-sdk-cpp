cmake_minimum_required(VERSION 3.2)

project(FmSdk)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(USE_BOOST "Use Boost implementations." ON)

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

include_directories(include)
include_directories(generated)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/source/IClient.cpp
    ${PROJECT_SOURCE_DIR}/source/Device.cpp
    ${PROJECT_SOURCE_DIR}/source/Pilot.cpp
    ${PROJECT_SOURCE_DIR}/source/core/CoreClient.cpp
    ${PROJECT_SOURCE_DIR}/source/core/https/HttpsClient.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/IInputEvent.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/user/UserEvent.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/user/CloseChannels.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/user/OpenChannels.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/user/Operate.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/user/ReleaseAccepted.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/user/ReleaseRejected.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/user/RequestControl.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/connection/ConnectionEvent.cpp
    ${PROJECT_SOURCE_DIR}/source/event/input/connection/Received.cpp
    ${PROJECT_SOURCE_DIR}/source/event/output/FacadeEvent.cpp
    ${PROJECT_SOURCE_DIR}/source/event/output/ChannelsClosing.cpp
    ${PROJECT_SOURCE_DIR}/source/event/output/ChannelsOpened.cpp
    ${PROJECT_SOURCE_DIR}/source/event/output/Error.cpp
    ${PROJECT_SOURCE_DIR}/source/event/output/HandoverAccepted.cpp
    ${PROJECT_SOURCE_DIR}/source/event/output/ProcedureRejected.cpp
    ${PROJECT_SOURCE_DIR}/source/event/output/ReleaseControl.cpp
    ${PROJECT_SOURCE_DIR}/source/backend/ClientBackend.cpp
    ${PROJECT_SOURCE_DIR}/source/backend/HeartbeatHandler.cpp
    ${PROJECT_SOURCE_DIR}/source/backend/ChannelsHandler.cpp
    ${PROJECT_SOURCE_DIR}/source/state/IState.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/Disconnected.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/Connecting.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/Connected.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/Disconnecting.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/connected/Ready.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/connected/Released.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/connected/Releasing.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/connected/Flying.cpp
    ${PROJECT_SOURCE_DIR}/source/state/device/connected/Recovering.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/Disconnected.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/Connecting.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/Connected.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/Disconnecting.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/Operating.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/Recovering.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/Released.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/ReleasingControl.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/RequestingControl.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/ValidatingChannels.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/OpeningChannels.cpp
    ${PROJECT_SOURCE_DIR}/source/state/pilot/connected/ClosingChannels.cpp
    ${PROJECT_SOURCE_DIR}/source/system/Timer.cpp
    ${PROJECT_SOURCE_DIR}/source/traffic/IChannel.cpp
    ${PROJECT_SOURCE_DIR}/source/traffic/ChannelImpl.cpp
    ${PROJECT_SOURCE_DIR}/source/traffic/socket/ISocket.cpp
    ${PROJECT_SOURCE_DIR}/source/traffic/socket/TcpSocket.cpp
    ${PROJECT_SOURCE_DIR}/source/traffic/socket/UdpSocket.cpp
    # genetated files
    ${PROJECT_SOURCE_DIR}/generated/common/channel_management.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/common/channel_validation.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/common/connection_state.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/common/handover_data.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/common/location.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/common/role.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/core/attach.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/core/operate.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/core/list_devices.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/facade/control/heartbeat.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/facade/control/setup.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/facade/control/facade_service.pb.cc
    ${PROJECT_SOURCE_DIR}/generated/facade/control/facade_service.grpc.pb.cc
    )

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/IClient.hpp
    ${PROJECT_SOURCE_DIR}/include/Device.hpp
    ${PROJECT_SOURCE_DIR}/include/Pilot.hpp
    ${PROJECT_SOURCE_DIR}/include/Constants.hpp
    ${PROJECT_SOURCE_DIR}/include/core/CoreClient.hpp
    ${PROJECT_SOURCE_DIR}/include/core/https/HttpsClient.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/IInputEvent.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/user/UserEvent.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/user/CloseChannels.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/user/OpenChannels.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/user/Operate.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/user/ReleaseAccepted.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/user/ReleaseRejected.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/user/RequestControl.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/connection/ConnectionEvent.hpp
    ${PROJECT_SOURCE_DIR}/include/event/input/connection/Received.hpp
    ${PROJECT_SOURCE_DIR}/include/event/output/FacadeEvent.hpp
    ${PROJECT_SOURCE_DIR}/include/event/output/ChannelsClosing.hpp
    ${PROJECT_SOURCE_DIR}/include/event/output/ChannelsOpened.hpp
    ${PROJECT_SOURCE_DIR}/include/event/output/Error.hpp
    ${PROJECT_SOURCE_DIR}/include/event/output/HandoverAccepted.hpp
    ${PROJECT_SOURCE_DIR}/include/event/output/ProcedureRejected.hpp
    ${PROJECT_SOURCE_DIR}/include/event/output/ReleaseControl.hpp
    ${PROJECT_SOURCE_DIR}/include/backend/ClientBackend.hpp
    ${PROJECT_SOURCE_DIR}/include/backend/HeartbeatHandler.hpp
    ${PROJECT_SOURCE_DIR}/include/backend/ChannelsHandler.hpp
    ${PROJECT_SOURCE_DIR}/include/state/IState.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/Disconnected.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/Connecting.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/Connected.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/Disconnecting.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/connected/Ready.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/connected/Released.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/connected/Releasing.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/connected/Flying.hpp
    ${PROJECT_SOURCE_DIR}/include/state/device/connected/Recovering.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/Disconnected.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/Connecting.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/Connected.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/Disconnecting.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/Operating.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/Recovering.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/Released.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/ReleasingControl.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/RequestingControl.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/ValidatingChannels.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/OpeningChannels.hpp
    ${PROJECT_SOURCE_DIR}/include/state/pilot/connected/ClosingChannels.hpp
    ${PROJECT_SOURCE_DIR}/include/system/Timer.hpp
    ${PROJECT_SOURCE_DIR}/include/system/IStateMachine.hpp
    ${PROJECT_SOURCE_DIR}/include/system/IState.hpp
    ${PROJECT_SOURCE_DIR}/include/traffic/IChannel.hpp
    ${PROJECT_SOURCE_DIR}/include/traffic/ChannelImpl.hpp
    ${PROJECT_SOURCE_DIR}/include/traffic/socket/ISocket.hpp
    ${PROJECT_SOURCE_DIR}/include/traffic/socket/TcpSocket.hpp
    ${PROJECT_SOURCE_DIR}/include/traffic/socket/UdpSocket.hpp
    )

set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j4")

add_library(fm-sdk STATIC ${SOURCES} ${HEADERS} ${BOOST_FILES})

target_link_libraries(fm-sdk
    ${Boost_LIBRARIES}
    protobuf::libprotobuf
    gRPC::grpc++
    OpenSSL::SSL
    OpenSSL::Crypto
    )
