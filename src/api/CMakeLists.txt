project(api)

add_library(${PROJECT_NAME}
    SHARED
        Camera.cpp
        Camera.h
        Mouse.cpp
        Mouse.h
)

add_library(edu::api ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GLAD_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR/../}
)
