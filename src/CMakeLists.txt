cmake_minimum_required(VERSION 3.1) # 3.1 << C_STANDARD 11

project(HelloOpenCL)

file(GLOB_RECURSE OpenClPrograms kernels/**.cl)

message("${OpenClPrograms}")

find_package(OpenCL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)

# Copy all kernals to executable folder on build.
foreach(file IN LISTS OpenClPrograms)
get_filename_component(filename ${file} NAME)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${file}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/${filename})
endforeach()
