project(renderer)

add_library(${PROJECT_NAME}
    SHARED
        Renderer.cpp
        Renderer.h
)

add_library(edu::renderer ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        edu::gfx
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GLAD_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR/../}
)
