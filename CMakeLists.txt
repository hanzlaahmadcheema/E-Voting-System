cmake_minimum_required(VERSION 3.16)
project(E-Voting-System)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)  # FTXUI requires C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    "D:/FTXUI/include"
)

# Automatically detect all .cpp files in src/
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Add FTXUI from sibling directory
add_subdirectory(${CMAKE_SOURCE_DIR}/../ftxui ${CMAKE_BINARY_DIR}/ftxui-build)

# Add executable
add_executable(E-Voting-System ${SRC_FILES})

# Link FTXUI components
target_link_libraries(E-Voting-System
    PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)

# Optional: Group source files in IDE (like VS)
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_FILES})

# Copy data folder to binary directory after build
add_custom_command(TARGET E-Voting-System POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/data $<TARGET_FILE_DIR:E-Voting-System>/data
)
