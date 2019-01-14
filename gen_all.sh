mkdir -p generated
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/common/channel_management.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/common/channel_validation.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/common/connection_state.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/common/handover_data.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/common/location.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/common/role.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/core/attach.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/core/list_devices.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/core/operate.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/facade/control/heartbeat.proto
protoc -I fm-client-proto --cpp_out=generated fm-client-proto/facade/control/setup.proto
protoc -I fm-client-proto --grpc_out=generated --cpp_out=generated --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin fm-client-proto/facade/control/facade_service.proto
