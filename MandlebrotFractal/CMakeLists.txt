cmake_minimum_required(VERSION 3.1) # 3.1 << C_STANDARD 11

option(DO_TEST, "Sould this run the test or the normal application?")
if(DO_TEST)
option(GENERATE_TEST_DATA, "Sould this run the test or generate the test data?")
endif()

project(HelloOpenCL)

file(GLOB_RECURSE OpenClPrograms kernels/**.cl)
file(GLOB_RECURSE FractalCpp fractal/**.cpp)
file(GLOB_RECURSE FractalHpp fractal/**.hpp)
file(GLOB_RECURSE OpenCLCpp openCL/**.cpp)
file(GLOB_RECURSE OpenCLHpp openCL/**.hpp)

message("${OpenClPrograms}")

find_package(OpenCL REQUIRED)
find_package(PNG REQUIRED)

add_executable(${PROJECT_NAME} Main.cpp ${FractalCpp} ${FractalHpp} ${OpenCLCpp} ${OpenCLHpp})

if(DO_TEST)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DDO_TEST)
if(GENERATE_TEST_DATA)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DGENERATE_TEST_DATA)
endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL PNG::PNG)

# Copy all kernals to executable folder on build.
foreach(file IN LISTS OpenClPrograms)
get_filename_component(filename ${file} NAME)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${file}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/${filename})
endforeach()
