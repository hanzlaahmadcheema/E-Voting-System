cmake_minimum_required(VERSION 3.16)
project(E-Voting-System)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)

# Automatically detect all .cpp files in src/
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Add executable
add_executable(E-Voting-System ${SRC_FILES})

# If using json.hpp (nlohmann/json single-header library), no linking needed
# But if linking sqlite3 statically/dynamically, you may need:
# find_package(SQLite3 REQUIRED)
# target_link_libraries(E-Voting-System PRIVATE SQLite::SQLite3)

# Optional: Group source files in IDE (like VS)
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_FILES})

# Copy data folder to binary directory after build
add_custom_command(TARGET E-Voting-System POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/data $<TARGET_FILE_DIR:E-Voting-System>/data
)
