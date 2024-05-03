cmake_minimum_required(VERSION 3.12)
project(SDK C)

########################################################################################################################
option(USE_TLSF "Use TLSF Memory Allocator" ON)

########################################################################################################################

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
if (USE_TLSF)
set(TLSF_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Public/TLSF-2.4.6/src)
    add_subdirectory(${TLSF_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/TLSF)
    target_include_directories(${PROJECT_NAME} PUBLIC ${TLSF_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} TLSF)
    target_compile_definitions(${PROJECT_NAME}
            PUBLIC USE_MEMORY_POOL
            PUBLIC USE_TLSF
            PUBLIC TLSF_POOL_SIZE=10485760
    )
endif ()

########################################################################################################################
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.t.c)

foreach (item IN LISTS TEST_SOURCES)
    message(STATUS "[${PROJECT_NAME}] TEST UNIT: ${item}")
    get_filename_component(TestUnit ${item} NAME_WLE)
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()

