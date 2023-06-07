cmake_minimum_required(VERSION 3.1) # 3.1 << C_STANDARD 11

project(HelloOpenCL)

file(GLOB_RECURSE OpenClPrograms src/kernels/**.cl)

message("${OpenClPrograms}")

find_package(OpenCL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)

# Copy all kernals to executable folder on build.
foreach(file IN LISTS OpenClPrograms)
get_filename_component(filename ${file} NAME)

message("hello")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${file}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/${filename})
endforeach()