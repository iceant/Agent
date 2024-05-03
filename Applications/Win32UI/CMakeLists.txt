cmake_minimum_required(VERSION 3.10)
project(Agent_Win32UI C)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../SDK ${CMAKE_CURRENT_BINARY_DIR}/SDK)

file(GLOB SOURCE *.c)

foreach (item IN LISTS SOURCE)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

add_executable(${PROJECT_NAME} WIN32  Win32GUI_Application.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
