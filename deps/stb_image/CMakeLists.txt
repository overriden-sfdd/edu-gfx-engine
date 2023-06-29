project(stbimage)

set(STBIMAGE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

add_library(${PROJECT_NAME}
    SHARED
        stb_image.cpp
        stb_image.h
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
