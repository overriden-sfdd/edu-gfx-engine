project(debug)

add_library(${PROJECT_NAME}
    STATIC
        Debugger.h
        Debugger.cpp
)

add_library(edu::debug ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glfw
        imgui
        edu::objects
        edu::gfx
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        imgui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR/../}
)
