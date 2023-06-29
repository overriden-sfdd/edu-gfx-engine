cmake_minimum_required(VERSION 3.25)
project(engine)

set(CMAKE_CXX_STANDARD 20)
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)

add_executable(${PROJECT_NAME} src/main.cpp)

# Add GLFW
add_subdirectory("${DEPS_DIR}/glfw")

# Add GLAD
add_subdirectory("${DEPS_DIR}/glad")

# Add stb_image
add_subdirectory("${DEPS_DIR}/stb_image")

# Add assimp
add_subdirectory("${DEPS_DIR}/assimp")

# Add local files
add_subdirectory(src)

# -------------- IMGUI static library creation`
#
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui)
file(GLOB IMGUI_GLOB
        ${IMGUI_PATH}/imgui.h
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imconfig.h
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_internal.h
        ${IMGUI_PATH}/imstb_rectpack.h
        ${IMGUI_PATH}/imstb_textedit.h
        ${IMGUI_PATH}/imstb_truetype.h
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp

        # specific bindings...
        ${IMGUI_PATH}/backends/imgui_impl_glfw.h
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)
add_library("imgui" STATIC ${IMGUI_GLOB})
target_include_directories("imgui" PUBLIC ${IMGUI_PATH})
target_link_libraries("imgui" PRIVATE glfw)
# ----------------------------------------------------------------------

find_package(glm REQUIRED)

# Link libraries with the project exe
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        # External
        ${GLAD_LIBRARIES}
        glfw
        stbimage
        glm::glm
        imgui
        assimp
        # Internal
        edu::api
        edu::gfx
        edu::objects
        edu::renderer
        edu::debug
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GLAD_INCLUDE_DIRS}
        ${STBIMAGE_INCLUDE_DIRS}
        imgui
        assimp
)

set(RES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/data")
set(RES_DST "${CMAKE_CURRENT_BINARY_DIR}/data")

add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E create_symlink ${RES_SRC} ${RES_DST}
        DEPENDS ${link_dst}
        COMMENT "symbolic link ${RES_SRC} => ${RES_DST}"
)
