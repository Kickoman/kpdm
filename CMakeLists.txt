cmake_minimum_required(VERSION 3.15)

project(
    kpdm
    VERSION 1.0
    DESCRIPTION "Like tqdm but kpdm"
    LANGUAGES CXX
)

add_subdirectory(include)
add_subdirectory(example)

include_directories(include)


message("HEADERS: ${HEADERS}")
message("SOUECES: ${SOURCES}")

add_executable(
    kpdm
    ${HEADERS}
    ${SOURCES}
)
