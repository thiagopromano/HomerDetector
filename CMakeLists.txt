cmake_minimum_required(VERSION 3.10)

# set the project name
project(Homer)

# add the executable
add_executable(Homer main.cpp Image.cpp Image.h)
target_include_directories(Homer PUBLIC
        "${PROJECT_BINARY_DIR}"
        )


configure_file(config.h.in config.h)