cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME asm_evm)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

set(src_dirs)
set(exe_src)
list(APPEND src_dirs ./src)

foreach( src_dir ${src_dirs} )
    aux_source_directory(${src_dir} exe_src)
    file(GLOB headers ${src_dir}/*.h)
    list(APPEND exe_src ${headers})
endforeach( )

add_executable( ${PROJECT_NAME} ${exe_src} )


