cmake_minimum_required(VERSION 3.25)
project(asapp)

set(CMAKE_CXX_STANDARD 23)

# Create macro to add sources & headers automatically, usually
# this isn't good practice but this is a VS project primarily so
# this makes it easier to maintain
macro (add_sources result folder)
    if (NOT DEFINED ${result})
        set(${result} "")
    endif ()

    file(GLOB_RECURSE files
            "${folder}/*.c" "${folder}/*.cc" "${folder}/*.cpp" "${folder}/*.asm"
            "${folder}/*.h" "${folder}/*.hpp")
    list(APPEND ${result} ${files})
endmacro ()

add_sources(ASAPP_SOURCES "ASAPP/source")
add_sources(ASAPP_HEADERS "ASAPP/include")

add_library(asapp STATIC ${ASAPP_SOURCES} ${ASAPP_HEADERS})

find_package(Tesseract CONFIG REQUIRED)
target_link_libraries(asapp PRIVATE Tesseract::libtesseract)

set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

target_include_directories(asapp PUBLIC ASAPP/include)
target_include_directories(asapp PRIVATE ASAPP/source)

# Copy resources to build folder
add_custom_command(
        TARGET asapp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/ASAPP/source/itemdata.json
        ${CMAKE_CURRENT_BINARY_DIR}/itemdata.json)

add_custom_command(
        TARGET asapp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/ASAPP/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)