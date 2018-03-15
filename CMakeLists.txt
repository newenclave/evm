cmake_minimum_required( VERSION 2.8 )

set( PROJECT_NAME evm )

project( ${PROJECT_NAME} )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

if(MAKE_TESTS)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/catch)

    execute_process( COMMAND "git" "submodule" "update" "catch"
                     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif(MAKE_TESTS)

list( APPEND src_dirs ./inc/evm
                      ./src 
    )

foreach(src_dir ${src_dirs})
    aux_source_directory(${src_dir} exe_src)
    file(GLOB headers ${src_dir}/*.h)
    list(APPEND exe_src ${headers})
endforeach()

add_executable(${PROJECT_NAME} ${exe_src})

if(MAKE_TESTS)
    add_subdirectory(tests)
endif(MAKE_TESTS)

add_subdirectory(asm_evm)
