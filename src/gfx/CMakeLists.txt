project(gfx)

add_library(${PROJECT_NAME}
    SHARED
        Shader.cpp
        Shader.h
        Texture.cpp
        Texture.h
        Material.h
        Mesh.cpp
        Mesh.h
        Asset.cpp
        Asset.h
        Mapping.cpp
        Mapping.h
        AssetModel.cpp
        AssetModel.h
)

add_library(edu::gfx ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        stbimage
        assimp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GLAD_INCLUDE_DIRS}
        ${STBIMAGE_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
)
