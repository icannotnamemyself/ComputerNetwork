# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name project (ex1)


############################################################
# Create a library
############################################################
add_library(proto_library STATIC
    proto.c
)
target_include_directories(proto_library
    PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

# Add an executable
add_executable(TCPServer TCPserver.c)
add_executable(TCPClient TCPclient.c)
add_executable(UDPServer UDPserver.c)
add_executable(UDPClient UDPclient.c)
add_executable(IPRAW_UDPclient IPRAW_UDPclient.c)

add_executable(protoClient proto_client.c)
target_link_libraries(protoClient
    PRIVATE
        proto_library
)

add_executable(protocolServer
    proto_server.c
)
target_link_libraries(protocolServer
    PRIVATE
        proto_library
)


