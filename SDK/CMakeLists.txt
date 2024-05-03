cmake_minimum_required(VERSION 3.12)
project(SDK C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
list(FILTER SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_definitions(${PROJECT_NAME}
        PUBLIC CPU_64BIT)


########################################################################################################################
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.t.c)

foreach (item IN LISTS TEST_SOURCES)
    message(STATUS "[${PROJECT_NAME}] TEST UNIT: ${item}")
    get_filename_component(TestUnit ${item} NAME_WLE)
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()

