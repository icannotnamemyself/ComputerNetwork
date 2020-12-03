# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (ex1)

# Add an executable
add_executable(TCPServer TCPserver.c)
add_executable(TCPClient TCPclient.c)
add_executable(UDPServer UDPserver.c)
add_executable(UDPClient UDPclient.c)
#add_executable(IPServer IPserver.c) deleted
#add_executable(IPClient IPclient.c) deleted
add_executable(IPRAW_UDPclient IPRAW_UDPclient.c)
