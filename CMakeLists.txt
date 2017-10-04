cmake_minimum_required(VERSION 3.0)
project(iothubtest)

include_directories(/home/pi/azure-iot-sdk-c/iothub_client/inc /home/pi/azure-iot-sdk-c/c-utility/inc/ azure_c_shared_utility)

link_directories(/home/pi/azure-iot-sdk-c/cmake/c-utility/ /home/pi/azure-iot-sdk-c/cmake/iothub_client/ /home/pi/azure-iot-sdk-c/cmake/uamqp/)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

add_executable(device main.cpp)

target_link_libraries(device iothub_client iothub_client_amqp_transport aziotsharedutil uamqp pthread uuid)
