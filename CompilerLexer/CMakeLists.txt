cmake_minimum_required(VERSION 3.1) # 3.1 << C_STANDARD 11

project(HelloOpenCL)

file(GLOB_RECURSE OpenClPrograms src/kernels/**.cl)
file(GLOB_RECURSE prj_hpp_files src/**.hpp)
file(GLOB_RECURSE prj_cpp_files src/**.cpp)

message("${OpenClPrograms}")

find_package(OpenCL REQUIRED)

add_executable(${PROJECT_NAME} ${prj_hpp_files} ${prj_cpp_files})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)

#Allow to include from root
include_directories(src)

# Copy all kernals to executable folder on build.
foreach(file IN LISTS OpenClPrograms)
get_filename_component(filename ${file} NAME)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${file}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/${filename})
endforeach()